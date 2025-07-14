// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PongHUD.generated.h"


UCLASS()
class PPT_API APongHUD : public AHUD
{
	GENERATED_BODY()
public:
	APongHUD();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UPongScoreWidget> ScoreWidgetClass;

    UPROPERTY()
    TObjectPtr<UPongScoreWidget> ScoreWidget;
private:

    UFUNCTION()
    void OnScoreChanged(int32 Player1Score, int32 Player2Score);

    int32 CurrentPlayer1Score = 0;
    int32 CurrentPlayer2Score = 0;
public:
	UFUNCTION(BlueprintCallable)
	void ShowGameStatus(const FString& Status);
    
	UFUNCTION(BlueprintCallable)
	void HideGameStatus();
};