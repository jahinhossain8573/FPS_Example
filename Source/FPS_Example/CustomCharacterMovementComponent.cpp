// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"

#include "GameFramework/Character.h"


void UCustomCharacterMovementComponent::BeginPlay()
{
	if (JumpCurve)
	{
		JumpCurve->GetTimeRange(jumpMinTime, jumpMaxTime);
	}
}

void UCustomCharacterMovementComponent::Tick(float DeltaTime, ELevelTick tick_type, FActorComponentTickFunction* tickFunction)
{
	Super::TickComponent(DeltaTime, tick_type, tickFunction);
}

bool UCustomCharacterMovementComponent::DoJump(bool bReplayingMoves)
{
	if (CharacterOwner && CharacterOwner->CanJump())
	{
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.0f)
		{
			if (JumpCurve)
			{
				Velocity.Z = FMath::Max(Velocity.Z, JumpZVelocity);

				SetMovementMode(MOVE_Flying);
				isJumping = true;
				jumpTime = jumpMinTime;
				prevJumpCurveValue = JumpCurve->GetFloatValue(jumpMinTime);

				return true;
			}
			else
			{
				return Super::DoJump(bReplayingMoves);
			}
		}
	}

	return false;
}


