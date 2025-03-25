// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyinCatCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFlyinCatCharacter::AFlyinCatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkeletalMeshComponent->SetupAttachment(RootComponent);

    // Charger un Skeletal Mesh depuis le Content Browser
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/Graph/CatBall/Meshes/MSH_cat.MSH_cat"));
    if (MeshAsset.Succeeded())
    {
        SkeletalMeshComponent->SetSkeletalMesh(MeshAsset.Object);
    }

	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleRadius());
}

// Called when the game starts or when spawned
void AFlyinCatCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetSimulatePhysics(true);
	
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

