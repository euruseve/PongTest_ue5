// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/PongPlayerState.h"
#include "Net/UnrealNetwork.h"

APongPlayerState::APongPlayerState()
{
	bReplicates = true;
}

void APongPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
	DOREPLIFETIME(APongPlayerState, PongPlayerId);
}

void APongPlayerState::SetPlayerID(int32 NewPlayerID)
{
	if (HasAuthority())
	{
		PongPlayerId = NewPlayerID;
	}
}