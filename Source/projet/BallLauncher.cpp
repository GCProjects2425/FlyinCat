// Fill out your copyright notice in the Description page of Project Settings.

#include "BallLauncher.h"

#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include <InputAction.h>
#include "InputMappingContext.h"

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

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingAsset(TEXT("/Game/Inputs/IMC_Cat.IMC_Cat"));
	if (InputMappingAsset.Succeeded())
	{
		InputMapping = InputMappingAsset.Object;
	}
}

// Called when the game starts or when spawned
void ABallLauncher::BeginPlay()
{
	Super::BeginPlay();

	AddNewMappingContext(InputMapping);


	FVector ArrowBallPosWorld = ArrowBallPos->GetComponentLocation();

	//SpawnedBall = GetWorld()->SpawnActor<AFlyinCatCharacter>(BallClass, ArrowBallPosWorld, FRotator::ZeroRotator);
	//SpawnedBall->GetCapsuleComponent()->SetSimulatePhysics(false);
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

void ABallLauncher::Look(const FInputActionValue& Value)
{
	FVector2D axisValue = Value.Get<FVector2D>();

	AddControllerYawInput(axisValue.X);
	AddControllerPitchInput(-axisValue.Y);
}

// Called to bind functionality to input
void ABallLauncher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABallLauncher::Look);

}

