// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

#include "GameFramework/Character.h"


void UCustomCharacterMovementComponent::BeginPlay()
{
	if (JumpCurve)
	{
		JumpCurve->GetTimeRange(jumpMinTime, jumpMaxTime);
	}
}

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);

	if (isJumping && JumpCurve)
	{
		jumpTime += DeltaTime;

		if (jumpTime <= jumpMaxTime)
		{
			float jumpCurveValue = JumpCurve->GetFloatValue(jumpTime);
			float jumpCurveValueDelta = jumpCurveValue - prevJumpCurveValue;

			Velocity.Z = jumpCurveValueDelta / DeltaTime;

			FVector actorLocation = GetActorLocation();
			FVector destinatioLocation = actorLocation + FVector(0.0F, 0.0F, jumpCurveValueDelta);
		}
	}
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


