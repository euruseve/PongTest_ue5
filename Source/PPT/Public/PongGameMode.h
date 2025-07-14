// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameMode.generated.h"

UCLASS()
class PPT_API APongGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	APongGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf<class APongBall> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FVector Player1SpawnLocation = FVector(-500.0f, 0.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FVector Player2SpawnLocation = FVector(500.0f, 0.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	FVector BallSpawnLocation = FVector(0.0f, 0.0f, 50.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	int32 MaxScore = 5;

private:
	UPROPERTY()
	TObjectPtr<class APongBall> GameBall;

	int32 ConnectedPlayers = 0;
	bool bGameStarted = false;

	void StartGame();
	void SpawnBall();
	void ResetGame();
	
	// Для відрахунку
	int32 CountdownSeconds = 3;
	FTimerHandle CountdownTimerHandle;
    
	// Методи для відрахунку
	void StartCountdown();
	void UpdateCountdown();
	void OnCountdownFinished();

public:
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnGoalScored(int32 ScoringPlayer);

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartRound();

	UFUNCTION()
	void CheckGameStart();
	
	UFUNCTION()
	FVector GetBallTargetLocation();
};

