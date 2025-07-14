// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PongPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

APongPawn::APongPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(25.0f, 100.0f, 50.0f));
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 800.0f;
    SpringArm->bUsePawnControlRotation = false;
    SpringArm->bDoCollisionTest = false;
    SpringArm->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
    
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void APongPawn::BeginPlay()
{
    Super::BeginPlay();
    
    ReplicatedLocation = GetActorLocation();
    TargetLocation = ReplicatedLocation;
}

void APongPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!HasAuthority() && bShouldInterpolate)
    {
        FVector CurrentLocation = GetActorLocation();
        FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);
        SetActorLocation(NewLocation);
        
        if (FVector::Dist(CurrentLocation, TargetLocation) < 1.0f)
        {
            bShouldInterpolate = false;
        }
    }
}

void APongPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(APongPawn, ReplicatedLocation);
}

void APongPawn::OnRep_ReplicatedLocation()
{
    if (!HasAuthority())
    {
        TargetLocation = ReplicatedLocation;
        bShouldInterpolate = true;
    }
}

void APongPawn::HorizontalMove(float AxisValue)
{
    if (FMath::Abs(AxisValue) > 0.01f)
    {
        FVector CameraRight = Camera->GetRightVector();
        
        FVector MovementDirection = CameraRight;
        
        FVector CurrentLocation = GetActorLocation();
        
        float DeltaTime = GetWorld()->GetDeltaSeconds();
        FVector Movement = MovementDirection * AxisValue * MovementSpeed * DeltaTime;
        FVector NewLocation = CurrentLocation + Movement;
        
        SetActorLocation(NewLocation, true);
        
        if (HasAuthority())
        {
            UpdateReplicatedLocation();
        }
    }
}

void APongPawn::UpdateReplicatedLocation()
{
    if (HasAuthority())
    {
        ReplicatedLocation = GetActorLocation();
    }
}