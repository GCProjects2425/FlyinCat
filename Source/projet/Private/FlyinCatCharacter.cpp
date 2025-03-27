// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyinCatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <Misc/OutputDeviceNull.h>

// Sets default values
AFlyinCatCharacter::AFlyinCatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->InitCapsuleSize(34.f, 34.f);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SetRootComponent(CapsuleComponent);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
    SkeletalMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleComponent->GetUnscaledCapsuleHalfHeight()));
    SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AFlyinCatCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFlyinCatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFlyinCatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

