// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball/PongBall.h"

#include "PongGameMode.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PongPawn.h"

APongBall::APongBall()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
    
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;
    SphereComponent->SetSphereRadius(25.0f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);
    BallMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = SphereComponent;
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 1.0f;
    ProjectileMovement->Friction = 0.0f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    
    SphereComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

void APongBall::BeginPlay()
{
    Super::BeginPlay();
}

void APongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(APongBall, ReplicatedVelocity);
}

void APongBall::OnRep_BallVelocity()
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = ReplicatedVelocity;
    }
}

void APongBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority()) return;
    
    if (APongPawn* Paddle = Cast<APongPawn>(OtherActor))
    {
        FVector BallLocation = GetActorLocation();
        FVector PaddleLocation = Paddle->GetActorLocation();
        
        float HitOffset = (BallLocation.Y - PaddleLocation.Y) / 100.0f; 
        HitOffset = FMath::Clamp(HitOffset, -1.0f, 1.0f);
        
        FVector NewDirection = ProjectileMovement->Velocity.GetSafeNormal();
        NewDirection.X = -NewDirection.X; 
        NewDirection.Y += HitOffset * 0.5f; 
        NewDirection.Z = 0.0f;
        NewDirection.Normalize();
        
        IncreaseSpeed();
        
        FVector NewVelocity = NewDirection * ProjectileMovement->InitialSpeed;
        SetBallVelocity(NewVelocity);
        
    }
}

void APongBall::LaunchBall()
{
    if (!HasAuthority()) return;
    
    FVector LaunchDirection = GetRandomDirection();
    FVector LaunchVelocity = LaunchDirection * InitialSpeed;
    
    SetBallVelocity(LaunchVelocity);
}

void APongBall::ResetBall()
{
    if (!HasAuthority()) return;
    
    if (APongGameMode* GameMode = Cast<APongGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        FVector ResetLocation = GameMode->GetBallTargetLocation();
        SetActorLocation(ResetLocation);
    }
    else
    {
        SetActorLocation(FVector(0.0f, 0.0f, 50.0f));
    }
    
    SetBallVelocity(FVector::ZeroVector);
    ProjectileMovement->InitialSpeed = InitialSpeed;
}

void APongBall::SetBallVelocity(const FVector& NewVelocity)
{
    if (HasAuthority())
    {
        ReplicatedVelocity = NewVelocity;
        if (ProjectileMovement)
        {
            ProjectileMovement->Velocity = NewVelocity;
        }
    }
}

FVector APongBall::GetRandomDirection()
{
    float RandomX = FMath::RandBool() ? 1.0f : -1.0f;
    float RandomY = FMath::RandRange(-0.5f, 0.5f);
    
    FVector Direction = FVector(RandomX, RandomY, 0.0f);
    Direction.Normalize();
    
    return Direction;
}

void APongBall::IncreaseSpeed()
{
    if (HasAuthority())
    {
        ProjectileMovement->InitialSpeed = FMath::Min(ProjectileMovement->InitialSpeed + SpeedIncrease, MaxSpeed);
    }
}