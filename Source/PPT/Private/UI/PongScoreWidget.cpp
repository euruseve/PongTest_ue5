// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PongScoreWidget.h"
#include "Components/TextBlock.h"
#include "Core/PongPlayerState.h"

UPongScoreWidget::UPongScoreWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UPongScoreWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UpdateScoreDisplay();
        
    if (GameStatusText)
    {
        GameStatusText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UPongScoreWidget::UpdateScore(int32 Player1Score, int32 Player2Score)
{
    CurrentPlayer1Score = Player1Score;
    CurrentPlayer2Score = Player2Score;
    
    UpdateScoreDisplay();
}

void UPongScoreWidget::UpdateScoreDisplay()
{
    if (!ScoreText) return;
        
    APongPlayerState* LocalPlayerState = nullptr;
    if (UWorld* World = GetWorld())
    {
        if (APlayerController* PC = World->GetFirstPlayerController())
        {
            LocalPlayerState = Cast<APongPlayerState>(PC->PlayerState);
        }
    }
        
    FString ScoreString;
    FLinearColor TextColor = NeutralColor;
        
    if (LocalPlayerState)
    {
        int32 MyScore = LocalPlayerState->IsPlayer1() ? CurrentPlayer1Score : CurrentPlayer2Score;
        int32 OpponentScore = LocalPlayerState->IsPlayer1() ? CurrentPlayer2Score : CurrentPlayer1Score;
            
        ScoreString = FString::Printf(TEXT("%d : %d"), MyScore, OpponentScore);
        
        if (MyScore > OpponentScore)
        {
            TextColor = LocalPlayerState->IsPlayer1() ? Player2Color : Player1Color;  
        }
        else if (OpponentScore > MyScore)
        {
            TextColor = LocalPlayerState->IsPlayer1() ? Player1Color : Player2Color;  
        }
    }
    else
    {
        ScoreString = FString::Printf(TEXT("%d : %d"), CurrentPlayer1Score, CurrentPlayer2Score);
        
        if (CurrentPlayer1Score > CurrentPlayer2Score)
        {
            TextColor = Player2Color;  
        }
        else if (CurrentPlayer2Score > CurrentPlayer1Score)
        {
            TextColor = Player1Color;  
        }
    }
        
    ScoreText->SetText(FText::FromString(ScoreString));
    ScoreText->SetColorAndOpacity(TextColor);
}

void UPongScoreWidget::SetGameStatus(const FString& Status)
{
    if (GameStatusText)
    {
        GameStatusText->SetText(FText::FromString(Status));
        GameStatusText->SetVisibility(Status.IsEmpty() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
}