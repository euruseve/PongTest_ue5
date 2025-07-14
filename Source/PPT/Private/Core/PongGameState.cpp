// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PongGameState.h"
#include "Net/UnrealNetwork.h"

APongGameState::APongGameState()
{
	bReplicates = true;
}

void APongGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
	DOREPLIFETIME(APongGameState, Player1Score);
	DOREPLIFETIME(APongGameState, Player2Score);
}

void APongGameState::OnRep_Player1Score()
{
	OnScoreChanged.Broadcast(Player1Score, Player2Score);
}

void APongGameState::OnRep_Player2Score()
{
	OnScoreChanged.Broadcast(Player1Score, Player2Score);
}

void APongGameState::AddScore(int32 PlayerID)
{
	if (HasAuthority())
	{
		if (PlayerID == 1)
		{
			Player1Score++;
		}
		else if (PlayerID == 2)
		{
			Player2Score++;
		}
        
		OnScoreChanged.Broadcast(Player1Score, Player2Score);
	}
}

void APongGameState::ResetScore()
{
	if (HasAuthority())
	{
		Player1Score = 0;
		Player2Score = 0;
		OnScoreChanged.Broadcast(Player1Score, Player2Score);
	}
}