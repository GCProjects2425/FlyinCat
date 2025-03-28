// Fill out your copyright notice in the Description page of Project Settings.

#include "BallLauncher.h"

#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include <InputAction.h>
#include "InputMappingContext.h"
#include <Kismet/GameplayStatics.h>

class UPawnMovementComponent;

// Sets default values
ABallLauncher::ABallLauncher()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	LauncherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	LauncherMesh->SetupAttachment(CapsuleComponent);

	LauncherMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -56.0f));
	LauncherMesh->SetRelativeRotation(FRotator( 0.0f, 90.0f, 0.0f));

	ArrowBallPos = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchPosition"));
	ArrowBallPos->SetupAttachment(LauncherMesh);
	ArrowBallPos->SetRelativeLocation(FVector(0.0f, 0.0f, 316.0f));
	ArrowBallPos->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	ArrowLaunchPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchPoint"));
	ArrowLaunchPoint->SetupAttachment(ArrowBallPos);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(LauncherMesh);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 320.0f));
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 100.0f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 5.f;

	bUseControllerRotationYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
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
			CameraManager->ViewYawMin = GetActorRotation().Yaw - 70.0f;
			CameraManager->ViewYawMax = GetActorRotation().Yaw + 70.0f;
			CameraManager->ViewPitchMin = GetActorRotation().Pitch - 50.0f;
			CameraManager->ViewPitchMax = GetActorRotation().Pitch + 50.0f;
		}
	}

	SpawnNewBall();	
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
	if (SpawnedBall == nullptr)
		return;

	// predict the path of the ball
	FVector Start = SpawnedBall->GetActorLocation();

	FPredictProjectilePathResult PredictedPath;
	FPredictProjectilePathParams Params(10, Start, GetControlRotation().Vector() * StoredImpulseStrength, 5.f);
	bool bHit = UGameplayStatics::PredictProjectilePath(GetWorld(), Params, PredictedPath);

	FPredictProjectilePathPointData& LastPoint = PredictedPath.PathData.Last();

	for (const FPredictProjectilePathPointData& PointData : PredictedPath.PathData)
	{
		//DrawDebugSphere(GetWorld(), PointData.Location, 10.0f, 12, FColor::Red, false, 0.f);

		// Line trace between the precedent point and the current point

		if (PointData.Location != Start)
		{
			FHitResult HitResult;
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(SpawnedBall);
			DrawDebugLine(GetWorld(), LastPoint.Location, PointData.Location, FColor::White, false, 0.f, 0, 4.f);

		}
		LastPoint = PointData;
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
	if (SpawnedBall == nullptr)
		return;

	bShouldPredictPath = true;

	FVector2D TriggerValue = Value.Get<FVector2D>();

	float MinImpulse = 300.0f;
	float MaxImpulse = 3000.0f;

	float JoystickStrength = TriggerValue.Size();
	StoredImpulseStrength = FMath::Lerp(MinImpulse, MaxImpulse, JoystickStrength);

	GetWorld()->GetFirstPlayerController()->PlayDynamicForceFeedback(JoystickStrength, 0.1f, false, true, false, true, EDynamicForceFeedbackAction::Start);

	FVector Xoffset = FMath::Lerp(FVector(0.f, 0.f, 0.f), FVector(-100.f, 0.f, 0.f ), JoystickStrength);
	
	float Pitch = GetControlRotation().Pitch;
	if (Pitch <= 70.0f)
	{
		Pitch += 360.0f; // Convertir 70° -> 430° pour garder l'ordre croissant
	}
	
	// Normalisation entre [290, 430] vers [0,1]
	float NormalizedValue = FMath::Clamp((Pitch - 290.0f) / (430.0f - 290.0f), 0.0f, 1.0f);
	
	FVector Zoffset = FMath::Lerp(FVector(0.f, 0.f, 30.f), FVector(0.f, 0.f, -30.f), NormalizedValue);
	
	FVector offset = Xoffset + Zoffset;
	ArrowLaunchPoint->SetRelativeLocation(offset);

	FRotator NewRotation = ArrowBallPos->GetRelativeRotation();
	NewRotation.Pitch = GetControlRotation().Pitch;

	ArrowBallPos->SetRelativeRotation(NewRotation);
}

void ABallLauncher::ShootBall()
{
	if (bCanShoot && SpawnedBall != nullptr)
	{
		if (StoredImpulseStrength > 0.0f)
		{
			SpawnedBall->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			FVector LaunchDirection = GetControlRotation().Vector();
			FVector Impulse = LaunchDirection * StoredImpulseStrength;

			SpawnedBall->OnShooted();
			SpawnedBall->CapsuleComponent->SetSimulatePhysics(true);
			SpawnedBall->CapsuleComponent->AddImpulse(Impulse, NAME_None, true);
			GetWorld()->GetFirstPlayerController()->PlayDynamicForceFeedback(1.f, 0.4f, true, true, true, true, EDynamicForceFeedbackAction::Start);

			StoredImpulseStrength = 0.0f;

			SpawnedBall = nullptr;

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABallLauncher::SpawnNewBall, 5.f, false);
		}
	}

	DisablePredictPath();
}

void ABallLauncher::SpawnNewBall()
{
	FVector ArrowBallPosWorld = ArrowBallPos->GetComponentLocation();

	SpawnedBall = GetWorld()->SpawnActor<AFlyinCatCharacter>(BallClass, ArrowBallPosWorld, GetActorRotation());
	if (SpawnedBall)
	{
		SpawnedBall->CapsuleComponent->SetSimulatePhysics(false);
		SpawnedBall->AttachToComponent(ArrowLaunchPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);

		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 1.f);
	}
}

void ABallLauncher::DisablePredictPath()
{
	bShouldPredictPath = false;
	ArrowLaunchPoint->SetRelativeLocation({ 0.f, 0.f, 0.f });
	ArrowLaunchPoint->SetRelativeRotation({ 0.f, 0.f, 0.f });
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

void ABallLauncher::SwitchCamera()
{
	if(SpawnedBall)
		SpawnedBall->SwitchCamera();
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
	Input->BindAction(CameraAction, ETriggerEvent::Started, this, &ABallLauncher::SwitchCamera);
}

