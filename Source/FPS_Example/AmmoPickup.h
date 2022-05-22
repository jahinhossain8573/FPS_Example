// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class FPS_EXAMPLE_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

	UPROPERTY(BlueprintReadWrite)
		int PickableAmmo;

	//1 = 5.56 mm, 2 = 7.62 mm
	UPROPERTY(BlueprintReadWrite)
		int AmmoTypeIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
