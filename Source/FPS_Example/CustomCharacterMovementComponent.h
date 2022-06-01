// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class FPS_EXAMPLE_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	//Curves
	UPROPERTY(EditAnywhere)
		UCurveFloat* JumpCurve;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick tick_type, FActorComponentTickFunction* tickFunction) override;
	virtual bool DoJump(bool bReplayingMoves) override;

private:
	bool isJumping;
	float jumpTime;
	float prevJumpCurveValue;

	float jumpMinTime;
	float jumpMaxTime;
};