// Fill out your copyright notice in the Description page of Project Settings.

#include "BallLauncher.h"

#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include <InputAction.h>
#include "InputMappingContext.h"
#include <Kismet/GameplayStatics.h>

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
		LauncherMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -56.0f));
		LauncherMesh->SetRelativeRotation(FRotator( 0.0f, 90.0f, 0.0f));
	}

	ArrowBallPos = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchPosition"));
	ArrowBallPos->SetupAttachment(LauncherMesh);
	ArrowBallPos->SetRelativeLocation(FVector(0.0f, 0.0f, 316.0f));
	ArrowBallPos->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(LauncherMesh);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 260.0f));
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 80.0f);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
		
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingAsset(TEXT("/Game/Inputs/IMC_Launcher.IMC_Launcher"));
	if (InputMappingAsset.Succeeded())
	{
		InputMapping = InputMappingAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionAsset(TEXT("/Game/Inputs/Actions/IA_Look.IA_Look"));
	if (LookActionAsset.Succeeded())
	{
		LookAction = LookActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PullActionAsset(TEXT("/Game/Inputs/Actions/IA_Pull.IA_Pull"));
	if (PullActionAsset.Succeeded())
	{
		PullAction = PullActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EngageShootActionAsset(TEXT("/Game/Inputs/Actions/IA_EngageShoot.IA_EngageShoot"));
	if (PullActionAsset.Succeeded())
	{
		EngageShootAction = EngageShootActionAsset.Object;
	}
}

// Called when the game starts or when spawned
void ABallLauncher::BeginPlay()
{
	Super::BeginPlay();

	AddNewMappingContext(InputMapping);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;
		if (CameraManager)
		{
			CameraManager->ViewYawMin = -70.0f;
			CameraManager->ViewYawMax = 70.0f;
		}
	}

	FVector ArrowBallPosWorld = ArrowBallPos->GetComponentLocation();

	SpawnedBall = GetWorld()->SpawnActor<AFlyinCatCharacter>(ArrowBallPosWorld, GetActorRotation());
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
	// predict the path of the ball
	FVector Start = SpawnedBall->GetActorLocation();

	FPredictProjectilePathResult PredictedPath;
	bool bHit = UGameplayStatics::PredictProjectilePath(GetWorld(), FPredictProjectilePathParams(10, Start, GetControlRotation().Vector() * StoredImpulseStrength, 5.f), PredictedPath);

	for (FPredictProjectilePathPointData PointData : PredictedPath.PathData)
	{
		DrawDebugSphere(GetWorld(), PointData.Location, 10.0f, 12, FColor::Red, false, 0.1f);
	}
}

void ABallLauncher::Look(const FInputActionValue& Value)
{
	FVector2D axisValue = Value.Get<FVector2D>();

	AddControllerYawInput(axisValue.X);
	AddControllerPitchInput(-axisValue.Y);
}

void ABallLauncher::PullBall(const FInputActionValue& Value)
{
	bShouldPredictPath = true;

	FVector2D TriggerValue = Value.Get<FVector2D>();

	float MinImpulse = 3000.0f;
	float MaxImpulse = 10000.0f;

	float JoystickStrength = TriggerValue.Size();
	StoredImpulseStrength = FMath::Lerp(MinImpulse, MaxImpulse, JoystickStrength);	
}

void ABallLauncher::ShootBall()
{
	if (bCanShoot && SpawnedBall)
	{
		if (StoredImpulseStrength > 0.0f)
		{
			FVector LaunchDirection = GetControlRotation().Vector();
			FVector Impulse = LaunchDirection * StoredImpulseStrength;

			SpawnedBall->GetCapsuleComponent()->SetSimulatePhysics(true);
			SpawnedBall->GetCapsuleComponent()->AddImpulse(Impulse, NAME_None, true);

			StoredImpulseStrength = 0.0f;
		}
	}

	DisablePredictPath();
}

void ABallLauncher::DisablePredictPath()
{
	bShouldPredictPath = false;
}

void ABallLauncher::EngageShoot()
{
	bCanShoot = true;
}

void ABallLauncher::DisengageShoot()
{
	ShootBall();
	bCanShoot = false;
}

// Called to bind functionality to input
void ABallLauncher::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABallLauncher::Look);
	Input->BindAction(PullAction, ETriggerEvent::Triggered, this, &ABallLauncher::PullBall);
	Input->BindAction(PullAction, ETriggerEvent::Canceled, this, &ABallLauncher::DisablePredictPath);
	Input->BindAction(PullAction, ETriggerEvent::Completed, this, &ABallLauncher::DisablePredictPath);
	Input->BindAction(EngageShootAction, ETriggerEvent::Triggered, this, &ABallLauncher::EngageShoot);
	Input->BindAction(EngageShootAction, ETriggerEvent::Canceled, this, &ABallLauncher::DisengageShoot);
	Input->BindAction(EngageShootAction, ETriggerEvent::Completed, this, &ABallLauncher::DisengageShoot);

}

