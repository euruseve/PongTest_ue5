// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PongScoreWidget.generated.h"

UCLASS()
class PPT_API UPongScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPongScoreWidget(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> ScoreText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> GameStatusText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Colors")
    FLinearColor Player1Color = FLinearColor::Blue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Colors")
    FLinearColor Player2Color = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Colors")
    FLinearColor NeutralColor = FLinearColor::White;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Text")
    float ScoreTextSize = 48.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Text")
    bool bUseCustomTextSize = false;

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateScore(int32 Player1Score, int32 Player2Score);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetGameStatus(const FString& Status);

private:
    void UpdateScoreDisplay();
    
    int32 CurrentPlayer1Score = 0;
    int32 CurrentPlayer2Score = 0;
};