#include "MyPawn.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "InputActionValue.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	// 포인터 타입 변수에 할당
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	// 루트 컴포넌트로 지정
	SetRootComponent(SphereComp);

	SphereComp->SetSimulatePhysics(false);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAll"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticObj(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (StaticObj.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticObj.Object);
	}

	// Script / Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// 스프링암 길이 초기화
	SpringArm->TargetArmLength = 300.f;
	MoveSpeed = 10.0f;
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D Inputvalue = value.Get<FVector2D>();
	FVector currentLocation = GetActorLocation();

	if (!FMath::IsNearlyZero(Inputvalue.X))
	{
		currentLocation += GetActorForwardVector() * (Inputvalue.X * MoveSpeed);
		SetActorLocation(currentLocation);
	}

	if (!FMath::IsNearlyZero(Inputvalue.Y))
	{
		currentLocation += GetActorRightVector() * (Inputvalue.Y * MoveSpeed);
		SetActorLocation(currentLocation);
	}
}

void AMyPawn::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += LookInput.X;
		SetActorRotation(NewRotation);
	}
	{
		FRotator NewRotation = SpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookInput.Y, -89.9f, 89.9f);
		SpringArm->SetWorldRotation(NewRotation);
	}
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}
}

