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

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ArrowLaunchPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AFlyinCatCharacter> BallClass;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* PullAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* EngageShootAction;

	void AddNewMappingContext(UInputMappingContext* newMapping);

	void PretictBallPath();

	void Look(const FInputActionValue& Value);

	void PullBall(const FInputActionValue& Value);

	void ShootBall();

	void SpawnNewBall();

	void DisablePredictPath();

	void EngageShoot();

	void DisengageShoot();


	float StoredImpulseStrength = 0.0f;
	FVector2D LastJoystickValue = FVector2D::ZeroVector;
	FVector2D JoystickVelocity = FVector2D::ZeroVector;

	bool bShouldPredictPath;

	bool bCanShoot;

private:
	class AFlyinCatCharacter* SpawnedBall;
};
