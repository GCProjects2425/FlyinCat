// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyinCatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include <Misc/OutputDeviceNull.h>
#include <Kismet/GameplayStatics.h>

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

    // third-person spring arm
    ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdPersonSpringArm"));
    ThirdPersonSpringArm->SetupAttachment(RootComponent);
    ThirdPersonSpringArm->TargetArmLength = 350.f;
    ThirdPersonSpringArm->bUsePawnControlRotation = false;
	// third-person camera
    ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
    ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);

	// first-person camera
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(RootComponent);
    FirstPersonCamera->SetRelativeLocation(FVector(0, 0, 50));
    FirstPersonCamera->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;
    ActiveCamera = ThirdPersonCamera;
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

void AFlyinCatCharacter::SwitchCamera()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
        return;

    ActiveCamera = (ActiveCamera == ThirdPersonCamera) ? FirstPersonCamera : ThirdPersonCamera;

    PlayerController->SetViewTargetWithBlend(this, 0.5f);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Switching camera to %s"), ActiveCamera == ThirdPersonCamera ? TEXT("ThirdPersonCamera") : TEXT("FirstPersonCamera")));
}

void AFlyinCatCharacter::OnShooted()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->SetViewTargetWithBlend(this, 0.2f);
    }
}

