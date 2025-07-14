// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PongPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Player/PongPawn.h"
#include "UI/PongHUD.h"

APongPlayerController::APongPlayerController()
{
    bReplicates = true;
}

void APongPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
       ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
       Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
    
    SetupHUD();
}

void APongPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
    {
       Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HorizontalMove);
    }
}

void APongPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
}

void APongPlayerController::HorizontalMove(const FInputActionValue& Value)
{
    float MovementValue = Value.Get<float>();

    if (IsLocalController())
    {
        if (APongPawn* PongPawn = Cast<APongPawn>(GetPawn()))
        {
            PongPawn->HorizontalMove(MovementValue);
        }
        
        Server_MoveHorizontal(MovementValue);
    }
}

void APongPlayerController::Server_MoveHorizontal_Implementation(float AxisValue)
{
    if (APongPawn* PongPawn = Cast<APongPawn>(GetPawn()))
    {
        PongPawn->HorizontalMove(AxisValue);
    }
}

bool APongPlayerController::Server_MoveHorizontal_Validate(float AxisValue)
{
    return FMath::Abs(AxisValue) <= 1.1f; 
}

void APongPlayerController::Client_UpdateGameStatus_Implementation(const FString& Status)
{
    if (PongHUD)
    {
        PongHUD->ShowGameStatus(Status);
    }
}

void APongPlayerController::SetupHUD()
{
    if (IsLocalController())
    {
        PongHUD = Cast<APongHUD>(GetHUD());
        
        if (!PongHUD)
        {
            PongHUD = GetWorld()->SpawnActor<APongHUD>();
            
            if (PongHUD)
            {
                PongHUD->SetOwner(this);
            }
        }
    }
}