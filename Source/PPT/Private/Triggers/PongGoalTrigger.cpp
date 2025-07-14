// Fill out your copyright notice in the Description page of Project Settings.

#include "Triggers/PongGoalTrigger.h"
#include "PongGameMode.h"
#include "Ball/PongBall.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

class APongGameMode;
class APongBall;

APongGoalTrigger::APongGoalTrigger()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
    
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetBoxExtent(FVector(50.0f, 200.0f, 200.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    
	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoalMesh"));
	GoalMesh->SetupAttachment(RootComponent);
	GoalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void APongGoalTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void APongGoalTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	
	if (APongBall* Ball = Cast<APongBall>(OtherActor))
	{
		Ball->Destroy();
		
		if (APongGameMode* GameMode = Cast<APongGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->OnGoalScored(ScoringPlayerID);
		}
	}
}

