// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Character.h"

// Sets default values
AFPS_Character::AFPS_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Character Movement Tweaks
	GetCharacterMovement()->GravityScale = 2.8f;
	GetCharacterMovement()->JumpZVelocity = 588;
	GetCharacterMovement()->BrakingDecelerationFalling = 7.0f;
	GetCharacterMovement()->FallingLateralFriction = 7.0f;
	GetCharacterMovement()->Mass = 140.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// Called when the game starts or when spawned
void AFPS_Character::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugLine(GetWorld(), CameraSocket->GetComponentTransform().GetLocation(), CameraSocket->GetComponentLocation() + AimOffset.GetLocation(), FColor::Red, false, 0.8f);
}

void AFPS_Character::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FRotator &&CharacterRotation = this->GetActorRotation();
	CharacterRotation.Pitch = 0;
	FVector Direction = UKismetMathLibrary::GetForwardVector(CharacterRotation);
	AddMovementInput(Direction, Value);
}

void AFPS_Character::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AFPS_Character::CustomJump()
{
	if (!IsSprinting && !IsReloading && !IsInADS)
	{
		Jump();
	}
}

void AFPS_Character::StartSprinting()
{
	if (!IsFiring && !IsReloading)
	{
		IsSprinting = true;
		MovementComponent->MaxWalkSpeed = 500;
	}
}

void AFPS_Character::StopSprinting()
{
	IsSprinting = false;
	MovementComponent->MaxWalkSpeed = 300;
}

void AFPS_Character::EnterADS()
{
	if (!IsReloading && !IsSprinting)
	{
		IsInADS = true;
		AimOffset = UKismetMathLibrary::MakeRelativeTransform(GunMesh->GetSocketTransform("AimSocket"), ArmsMesh->GetSocketTransform("hand_r"));
		R_Hand_Location = RightHandSocket->GetRelativeLocation() - FVector(AimOffset.GetLocation().X, AimOffset.GetLocation().Y * -1, AimOffset.GetLocation().Z);
		RightHandSocket->SetRelativeLocation(R_Hand_Location);
		RightHandSocket->SetRelativeRotation(AimOffset.GetRotation());

		UE_LOG(LogTemp, Display, TEXT("%s"), *R_Hand_Location.ToString());
		BP_EnterADS.Broadcast();
	}
}

void AFPS_Character::ExitADS()
{
	if (!IsReloading && !IsSprinting)
	{
		IsInADS = false;
		BP_ExitADS.Broadcast();
	}
}

void AFPS_Character::FireBullet()
{
	if (AmmoInMag > 0)
	{
		//Raycast
		FVector StartB = GunMesh->GetSocketLocation("FireSocket");
		FVector EndB = UKismetMathLibrary::GetForwardVector(GunMesh->GetSocketRotation("FireSocket")) * 100000;
		FCollisionQueryParams QueryParams;
		FHitResult OutHit;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, StartB, EndB, ECC_Visibility, QueryParams))
		{
			//DrawDebugLine(GetWorld(), StartB, OutHit.ImpactPoint, FColor::Red, true);
		}

		//Recoil
		float Pitch = (FMath::RandRange(-0.08F, -0.1F)) * rPitch;
		float Yaw = (FMath::RandRange(-0.1F, 0.1F)) * rYaw;
		AddControllerPitchInput(Pitch);
		AddControllerYawInput(Yaw);

		//Animation
		ArmsAnimInstance->Montage_Play(ArmsFiringMontage);
		GunAnimInstance->Montage_Play(GunFiringMontage);

		//Ammunition
		AmmoInMag--;
		UE_LOG(LogTemp, Log, TEXT("% d"), AmmoInMag);
	}
}

void AFPS_Character::FireWeapon()
{
	if (!IsSprinting && !IsReloading)
	{
		IsFiring = true;
		if (isAutomatic)  //Checks Fire Mode
		{
			//Automatic Fire
			GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AFPS_Character::FireBullet, 0.12F, true, 0.0F);
		}
		else
		{
			//Single Fire
			FireBullet();
		}
	}
}

void AFPS_Character::StopAutoFire()
{
	//Halts Auto Firing
	GetWorldTimerManager().ClearTimer(AutoFireHandle);
	IsFiring = false;
}

void AFPS_Character::ChangeFireMode()
{
	//Changes Fire Mode
	isAutomatic = !isAutomatic;
}

void AFPS_Character::ReloadWeapon()
{
	if (!IsSprinting && !IsInADS)
	{
		UE_LOG(LogTemp, Log, TEXT("%d"), AmmoInMag);
		if (AmmoInMag < MagazineAmmoCapacity)
		{
			int AmmoRequired = MagazineAmmoCapacity - AmmoInMag;

			if (AmmoInMag == 0)
			{
				ArmsAnimInstance->Montage_Play(ArmsReloadingMontageEmpty);
				GunAnimInstance->Montage_Play(GunReloadingMontageEmpty);
				GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFPS_Character::ReloadBoolSwitch, 3.17, true, 0.0F);
			}
			else
			{
				ArmsAnimInstance->Montage_Play(ArmsReloadingMontage);
				GunAnimInstance->Montage_Play(GunReloadingMontage);
				GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AFPS_Character::ReloadBoolSwitch, 2.67, true, 0.0F);
			}

			AmmoInMag += AmmoRequired;
			StoredAmmo -= AmmoRequired;

			UE_LOG(LogTemp, Log, TEXT("Reloaded"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Mag Full"));
		}
		UE_LOG(LogTemp, Log, TEXT("%d"), AmmoInMag);
	}
}

void AFPS_Character::ReloadBoolSwitch()
{
	IsReloading = !IsReloading;
}

void AFPS_Character::Interaction()
{
	/*
	FVector StartI = PlayerCamera->GetComponentLocation();
	FVector EndI =
	GetWorld()->LineTraceSingleByChannel();
	*/
}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &AFPS_Character::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPS_Character::CustomJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPS_Character::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPS_Character::StopSprinting);

	PlayerInputComponent->BindAction("FireWeapon", IE_Pressed, this, &AFPS_Character::FireWeapon);
	PlayerInputComponent->BindAction("FireWeapon", IE_Released, this, &AFPS_Character::StopAutoFire);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AFPS_Character::EnterADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AFPS_Character::ExitADS);

	PlayerInputComponent->BindAction("ChangeFireMode", IE_Pressed, this, &AFPS_Character::ChangeFireMode);
	PlayerInputComponent->BindAction("ReloadWeapon", IE_Pressed, this, &AFPS_Character::ReloadWeapon);

	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AFPS_Character::Interaction);
}