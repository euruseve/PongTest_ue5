// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PongPlayerState.generated.h"

UCLASS()
class PPT_API APongPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	APongPlayerState();

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	int32 PongPlayerId = 0;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	int32 GetPongPlayerId() const { return PongPlayerId; }

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayerID(int32 NewPlayerID);

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool IsPlayer1() const { return PongPlayerId == 1; }

	UFUNCTION(BlueprintCallable, Category = "Player")
	bool IsPlayer2() const { return PongPlayerId == 2; }
};
