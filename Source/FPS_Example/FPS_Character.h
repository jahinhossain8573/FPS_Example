// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"


#include "FPS_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequest);

UCLASS()
class FPS_EXAMPLE_API AFPS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Variables
	//Integers
	// FVectors
	UPROPERTY(BlueprintReadWrite)
		FVector AimOffset;

	//Ammo
	UPROPERTY(BlueprintReadWrite)
		int AmmoInMag = 30;
	UPROPERTY(BlueprintReadWrite)
		int StoredAmmo = 210;
	UPROPERTY(BlueprintReadWrite)
		int MagazineAmmoCapacity = 31;
	//Recoil
	UPROPERTY(BlueprintReadWrite)
		int rPitch = 4;
	UPROPERTY(BlueprintReadWrite)
		int rYaw = 3;

	//Booleans
	UPROPERTY(BlueprintReadWrite)
		bool IsInADS = false;
	UPROPERTY(BlueprintReadWrite)
		bool IsSprinting = false;
	UPROPERTY(BlueprintReadWrite)
		bool IsFiring = false;
	UPROPERTY(BlueprintReadWrite)
		bool IsReloading = false;
	UPROPERTY(BlueprintReadWrite)
		bool isAutomatic = false;

	//Components
	UPROPERTY(BlueprintReadWrite)
		UCharacterMovementComponent* MovementComponent = nullptr;

	//Camera
	UPROPERTY(BlueprintReadWrite)
		UCameraComponent* PlayerCamera = nullptr;

	//Anim Instances
	UPROPERTY(BlueprintReadWrite)
		UAnimInstance* ArmsAnimInstance = nullptr;
	UPROPERTY(BlueprintReadWrite)
		UAnimInstance* GunAnimInstance = nullptr;

	//Skeletal Meshes
	UPROPERTY(BlueprintReadWrite)
		USkeletalMeshComponent* GunMesh = nullptr;
	UPROPERTY(BlueprintReadWrite)
		USkeletalMeshComponent* ArmsMesh = nullptr;

	//Static Meshes
	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* AimSocket = nullptr;
	UPROPERTY(BlueprintReadWrite)
		UStaticMeshComponent* CameraSocket = nullptr;

	//Animation Montages
	//ArmsMontages
	UPROPERTY(EditAnywhere)
		UAnimMontage* ArmsFiringMontage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* ArmsReloadingMontage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* ArmsReloadingMontageEmpty;

	//GunMontages
	UPROPERTY(EditAnywhere)
		UAnimMontage* GunFiringMontage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* GunReloadingMontage;
	UPROPERTY(EditAnywhere)
		UAnimMontage* GunReloadingMontageEmpty;


	//Timers
	//Firing
	FTimerHandle AutoFireHandle;

	//Reloading
	FTimerHandle ReloadTimerHandle;

	// UFUNCTIONS
	//Basic Movement
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void CustomJump();

	UFUNCTION()
		void StartSprinting();

	UFUNCTION()
		void StopSprinting();


	//Gameplay Mechanics
	UFUNCTION()
		void EnterADS();

	UFUNCTION()
		void ExitADS();

	UFUNCTION()
		void FireBullet();

	UFUNCTION()
		void FireWeapon();

	UFUNCTION()
		void StopAutoFire();

	UFUNCTION()
		void ChangeFireMode();

	UFUNCTION()
		void ReloadWeapon();

	UFUNCTION()
		void ReloadBoolSwitch();

	UFUNCTION()
		void Interaction();

	//BlueprintAssignables
	UPROPERTY(BlueprintAssignable)
		FOnRequest BP_EnterADS;
	UPROPERTY(BlueprintAssignable)
		FOnRequest BP_ExitADS;
};