// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/PongHUD.h"
#include "UI/PongScoreWidget.h"
#include "Core/PongGameState.h"
#include "Blueprint/UserWidget.h"

APongHUD::APongHUD()
{
}

void APongHUD::BeginPlay()
{
    Super::BeginPlay();
    
    if (ScoreWidgetClass)
    {
        ScoreWidget = CreateWidget<UPongScoreWidget>(GetWorld(), ScoreWidgetClass);
        if (ScoreWidget)
        {
            ScoreWidget->AddToViewport();
        }
    }
    
    if (APongGameState* GameState = Cast<APongGameState>(GetWorld()->GetGameState()))
    {
        GameState->OnScoreChanged.AddDynamic(this, &ThisClass::OnScoreChanged);
    }
}

void APongHUD::OnScoreChanged(int32 Player1Score, int32 Player2Score)
{
    CurrentPlayer1Score = Player1Score;
    CurrentPlayer2Score = Player2Score;
    
    if (ScoreWidget)
    {
        ScoreWidget->UpdateScore(Player1Score, Player2Score);
    }
}
void APongHUD::ShowGameStatus(const FString& Status)
{
    if (ScoreWidget)
    {
        ScoreWidget->SetGameStatus(Status);
    }
}

void APongHUD::HideGameStatus()
{
    if (ScoreWidget)
    {
        ScoreWidget->SetGameStatus(TEXT(""));
    }
}
