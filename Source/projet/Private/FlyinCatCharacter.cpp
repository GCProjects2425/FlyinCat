// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyinCatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AFlyinCatCharacter::AFlyinCatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Charger un Skeletal Mesh depuis le Content Browser
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Graph/CatBall/Meshes/MSH_cat.MSH_cat"));
    if (MeshAsset.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(MeshAsset.Object);
    }

	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleRadius());
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AFlyinCatCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetMassOverrideInKg(NAME_None, 200.f, true);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionObjectType(ECC_PhysicsBody);
    UPhysicalMaterial* PhysMatBoule = NewObject<UPhysicalMaterial>(this);
    if (PhysMatBoule)
    {
        PhysMatBoule->Friction = 2.5f;
        PhysMatBoule->FrictionCombineMode = EFrictionCombineMode::Multiply;
        PhysMatBoule->Restitution = 0.0f;

        GetCapsuleComponent()->SetPhysMaterialOverride(PhysMatBoule);
    }
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

