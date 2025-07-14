// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int32, Player1Score, int32, Player2Score);

UCLASS()
class PPT_API APongGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	APongGameState();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Player1Score, BlueprintReadOnly, Category = "Score")
	int32 Player1Score = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Player2Score, BlueprintReadOnly, Category = "Score")
	int32 Player2Score = 0;

	UFUNCTION()
	void OnRep_Player1Score();

	UFUNCTION()
	void OnRep_Player2Score();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetPlayer1Score() const { return Player1Score; }

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 GetPlayer2Score() const { return Player2Score; }

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 PlayerID);

	UFUNCTION(BlueprintCallable, Category = "Score")
	void ResetScore();

	UPROPERTY(BlueprintAssignable, Category = "Score")
	FOnScoreChanged OnScoreChanged;
};
