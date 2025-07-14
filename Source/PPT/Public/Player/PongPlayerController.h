// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PongPlayerController.generated.h"

UCLASS()
class PPT_API APongPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APongPlayerController();

	UFUNCTION(Client, Reliable)
	void Client_UpdateGameStatus(const FString& Status);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;
    
	void HorizontalMove(const struct FInputActionValue& Value);
    
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveHorizontal(float AxisValue);

private:
	void SetupHUD();
    
	UPROPERTY()
	TObjectPtr<class APongHUD> PongHUD;
};