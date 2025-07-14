// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PongPawn.generated.h"

UCLASS()
class PPT_API APongPawn : public APawn
{
	GENERATED_BODY()

public:
	APongPawn();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<class UCameraComponent> Camera;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementBounds = 400.0f;
	
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedLocation)
	FVector ReplicatedLocation;

	UFUNCTION()
	void OnRep_ReplicatedLocation();
    
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
    
public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void HorizontalMove(float AxisValue);
    
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void UpdateReplicatedLocation();
    
	FVector TargetLocation;
	bool bShouldInterpolate = false;
};