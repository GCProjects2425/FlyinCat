// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include <Components/CapsuleComponent.h>
#include "FlyinCatCharacter.h"

#include <EnhancedInputLibrary.h>
#include <InputAction.h>

#include "BallLauncher.generated.h"


UCLASS()
class PROJET_API ABallLauncher : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LauncherMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowBallPos;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AFlyinCatCharacter> BallClass;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookAction;

	void AddNewMappingContext(UInputMappingContext* newMapping);

	void PretictBallPath();

	void Look(const FInputActionValue& Value);

	bool bShouldPredictPath;

	bool bCanShoot;

private:
	AFlyinCatCharacter* SpawnedBall;
};
