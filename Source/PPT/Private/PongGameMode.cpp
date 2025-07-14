// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameMode.h"
#include "Ball/PongBall.h"
#include "Core/PongGameState.h"
#include "Core/PongPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PongPawn.h"
#include "Player/PongPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(LogPongGameMode, All, All);

APongGameMode::APongGameMode()
{
    DefaultPawnClass = APongPawn::StaticClass();
    PlayerControllerClass = APongPlayerController::StaticClass();
    GameStateClass = APongGameState::StaticClass();
    PlayerStateClass = APongPlayerState::StaticClass();
    
    bUseSeamlessTravel = false;
}

void APongGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void APongGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    ConnectedPlayers++;
    
    UE_LOG(LogPongGameMode, Warning, TEXT("Player %d connected. Total players: %d"), ConnectedPlayers, ConnectedPlayers);
    
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
    
    if (PlayerStarts.Num() >= 2)
    {
        if (APongPawn* PongPawn = Cast<APongPawn>(NewPlayer->GetPawn()))
        {
            int32 StartIndex = ConnectedPlayers - 1;
            if (StartIndex < PlayerStarts.Num())
            {
                FVector SpawnLocation = PlayerStarts[StartIndex]->GetActorLocation();
                FRotator SpawnRotation = PlayerStarts[StartIndex]->GetActorRotation();
                
                PongPawn->SetActorLocation(SpawnLocation);
                PongPawn->SetActorRotation(SpawnRotation);
                
                UE_LOG(LogPongGameMode, Warning, TEXT("Player %d spawned at PlayerStart %d: %s"), 
                       ConnectedPlayers, StartIndex, *SpawnLocation.ToString());
            }
        }
    }
    else
    {
        UE_LOG(LogPongGameMode, Error, TEXT("Not enough PlayerStart actors found! Need at least 2, found %d"), PlayerStarts.Num());
        
        if (APongPawn* PongPawn = Cast<APongPawn>(NewPlayer->GetPawn()))
        {
            FVector SpawnLocation = (ConnectedPlayers == 1) ? Player1SpawnLocation : Player2SpawnLocation;
            PongPawn->SetActorLocation(SpawnLocation);
        }
    }
    
    if (APongPlayerState* PongPlayerState = Cast<APongPlayerState>(NewPlayer->PlayerState))
    {
        PongPlayerState->SetPlayerID(ConnectedPlayers);
    }
    
    CheckGameStart();
}

void APongGameMode::Logout(AController* Exiting)
{
    if (bGameStarted) return;
    
    bGameStarted = true;
    
    if (APongGameState* PongGameState = Cast<APongGameState>(GameState))
    {
        PongGameState->ResetScore();
    }
    
    SpawnBall();
}

void APongGameMode::StartGame()
{
    if (bGameStarted) return;
    
    bGameStarted = true;
    
    UE_LOG(LogPongGameMode, Warning, TEXT("Game Started!"));
    
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        if (APongPlayerController* PC = Cast<APongPlayerController>(*Iterator))
        {
            PC->Client_UpdateGameStatus(TEXT("Game Started!"));
        }
    }
    
    if (APongGameState* PongGameState = Cast<APongGameState>(GameState))
    {
        PongGameState->ResetScore();
    }
    
    FTimerHandle StartedDelayHandle;
    GetWorld()->GetTimerManager().SetTimer(StartedDelayHandle, this, &ThisClass::StartCountdown, 1.0f, false);
}

void APongGameMode::SpawnBall()
{
    if (!BallClass || GameBall) return;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
    FVector SpawnLocation = GetBallTargetLocation(); 
    
    GameBall = GetWorld()->SpawnActor<APongBall>(BallClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
    
    if (GameBall)
    {
        GameBall->LaunchBall();
    }
}

void APongGameMode::OnGoalScored(int32 ScoringPlayer)
{
    if (APongGameState* PongGameState = Cast<APongGameState>(GameState))
    {
        PongGameState->AddScore(ScoringPlayer);
        
        int32 Player1Score = PongGameState->GetPlayer1Score();
        int32 Player2Score = PongGameState->GetPlayer2Score();
        
        UE_LOG(LogTemp, Warning, TEXT("Goal! Player %d scored. Score: %d - %d"), 
               ScoringPlayer, Player1Score, Player2Score);
        
        if (Player1Score >= MaxScore || Player2Score >= MaxScore)
        {
            UE_LOG(LogPongGameMode, Warning, TEXT("Game Over! Winner: Player %d"), 
                   (Player1Score >= MaxScore) ? 1 : 2);
            
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::ResetGame, 5.0f, false);
        }
        else
        {
            RestartRound();
        }
    }
}

void APongGameMode::RestartRound()
{
    if (GameBall)
    {
        GameBall->Destroy();
        GameBall = nullptr;
    }
    
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::SpawnBall, 2.0f, false);
}

void APongGameMode::ResetGame()
{
    if (APongGameState* PongGameState = Cast<APongGameState>(GameState))
    {
        PongGameState->ResetScore();
    }
    
    RestartRound();
}

FVector APongGameMode::GetBallTargetLocation()
{
    if (AActor* SpawnPoint = UGameplayStatics::GetActorOfClass(GetWorld(), AActor::StaticClass()))
    {
        if (SpawnPoint->Tags.Contains(TEXT("BallSpawnPoint")))
        {
            return SpawnPoint->GetActorLocation();
        }
    }
    
    return BallSpawnLocation;
}


void APongGameMode::StartCountdown()
{
    CountdownSeconds = 3;
    UpdateCountdown();
}

void APongGameMode::UpdateCountdown()
{
    UE_LOG(LogPongGameMode, Warning, TEXT("Countdown: %d"), CountdownSeconds);
    
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        if (APongPlayerController* PC = Cast<APongPlayerController>(*Iterator))
        {
            if (CountdownSeconds > 0)
            {
                PC->Client_UpdateGameStatus(FString::Printf(TEXT("%d"), CountdownSeconds));
            }
            else
            {
                PC->Client_UpdateGameStatus(TEXT("GO!"));
            }
        }
    }
    
    CountdownSeconds--;
    
    if (CountdownSeconds >= 0)
    {
        GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ThisClass::UpdateCountdown, 1.0f, false);
    }
    else
    {
        FTimerHandle HideGoHandle;
        GetWorld()->GetTimerManager().SetTimer(HideGoHandle, this, &ThisClass::OnCountdownFinished, 1.0f, false);
    }
}

void APongGameMode::OnCountdownFinished()
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        if (APongPlayerController* PC = Cast<APongPlayerController>(*Iterator))
        {
            PC->Client_UpdateGameStatus(TEXT(""));
        }
    }
    
    SpawnBall();
}

void APongGameMode::CheckGameStart()
{
    UE_LOG(LogPongGameMode, Warning, TEXT("CheckGameStart called. Connected players: %d"), ConnectedPlayers);
    
    if (ConnectedPlayers < 2)
    {
        /*
         * To whom it may concern,
         * So, I tried to add the text below
         * into the GameStatusText as UI-element
         * but HUD loads only after two players
         * ready for the game.
         */
        
        UE_LOG(LogPongGameMode, Warning, TEXT("Waiting for more players... Current: %d/2"), ConnectedPlayers);
    }
    
    if (ConnectedPlayers == 2 && !bGameStarted)
    {
        UE_LOG(LogPongGameMode, Warning, TEXT("Both players connected! Starting game..."));
        
        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            if (APongPlayerController* PC = Cast<APongPlayerController>(*Iterator))
            {
                PC->Client_UpdateGameStatus(TEXT("Both players connected!"));
            }
        }
        
        FTimerHandle StartDelayHandle;
        GetWorld()->GetTimerManager().SetTimer(StartDelayHandle, this, &ThisClass::StartGame, 2.0f, false);
    }
}