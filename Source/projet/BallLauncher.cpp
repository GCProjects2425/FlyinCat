// Fill out your copyright notice in the Description page of Project Settings.

#include "BallLauncher.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"

// Sets default values
ABallLauncher::ABallLauncher()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	LauncherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	LauncherMesh->SetupAttachment(CapsuleComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/SlingShot.SlingShot"));
	if (MeshAsset.Succeeded())
	{
		LauncherMesh->SetStaticMesh(MeshAsset.Object);
		LauncherMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
		LauncherMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	}

	ArrowBallPos = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchPosition"));
	ArrowBallPos->SetupAttachment(LauncherMesh);
	ArrowBallPos->SetRelativeLocation(FVector(0.0f, 0.0f, 316.0f));
	ArrowBallPos->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void ABallLauncher::BeginPlay()
{
	Super::BeginPlay();

	AddNewMappingContext(MappingContext);

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	FVector ArrowBallPosWorld = ArrowBallPos->GetComponentLocation();

	SpawnedBall = GetWorld()->SpawnActor<AFlyinCatCharacter>(BallClass, ArrowBallPosWorld, FRotator::ZeroRotator);
	SpawnedBall->GetCapsuleComponent()->SetSimulatePhysics(false);
}

void ABallLauncher::AddNewMappingContext(UInputMappingContext* newMapping)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem 
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			LocalPlayerSubsystem->AddMappingContext(newMapping, 0);
		}		
	}
}

// Called every frame
void ABallLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldPredictPath)	
		PretictBallPath();

}

void ABallLauncher::PretictBallPath()
{
}

// Called to bind functionality to input
void ABallLauncher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

