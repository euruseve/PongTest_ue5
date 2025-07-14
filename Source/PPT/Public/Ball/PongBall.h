// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

UCLASS()
class PPT_API APongBall : public AActor
{
    GENERATED_BODY()
	
public:
    APongBall();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<class USphereComponent> SphereComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> BallMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    float InitialSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    float MaxSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    float SpeedIncrease = 50.0f;

    UPROPERTY(ReplicatedUsing = OnRep_BallVelocity)
    FVector ReplicatedVelocity;

    UFUNCTION()
    void OnRep_BallVelocity();

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
        FVector NormalImpulse, const FHitResult& Hit);

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category = "Ball")
    void LaunchBall();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    void ResetBall();

private:
    void SetBallVelocity(const FVector& NewVelocity);
    
    FVector GetRandomDirection();
    void IncreaseSpeed();
};