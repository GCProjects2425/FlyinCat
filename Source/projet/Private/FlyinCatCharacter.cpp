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

    // Charger un Skeletal Mesh depuis le Content Browser
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Graph/CatBall/Meshes/MSH_cat.MSH_cat"));
    if (MeshAsset.Succeeded())
    {
        SkeletalMeshComponent->SetSkeletalMesh(MeshAsset.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Graph/CatBall/Anims/ABP_Cat.ABP_Cat_C"));
    if (AnimBPClass.Succeeded())
    {
        SkeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        SkeletalMeshComponent->SetAnimClass(AnimBPClass.Class);
    }
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

void AFlyinCatCharacter::PlayFlyinAnimation()
{
    PlayFlyinMontage();
}

void AFlyinCatCharacter::PlayFlyinMontage()
{
    UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Pas d'AnimInstance trouvé !"));
        return;
    }

    FOutputDeviceNull ar;
    AnimInstance->CallFunctionByNameWithArguments(TEXT("PlayFlyinMontage"), ar, NULL, true);
}

