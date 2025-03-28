// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "FlyinCatCharacter.generated.h"

UCLASS()
class PROJET_API AFlyinCatCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlyinCatCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SwitchCamera();
	void OnShooted();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComponent;

	// Skeletal Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* ThirdPersonSpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

private:
	UCameraComponent* ActiveCamera;

};
