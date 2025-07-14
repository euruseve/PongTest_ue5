// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongGoalTrigger.generated.h"

UCLASS()
class PPT_API APongGoalTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	APongGoalTrigger();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> GoalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal")
	int32 ScoringPlayerID = 1;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
