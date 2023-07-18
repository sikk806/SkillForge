// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 100.f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input.
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	BaseLookUpRate = 55.f;
	BaseTurnRate = 55.f;

	WalkSpeed = 300.f;
	RunSpeed = 450.f;

}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	// Camera Start Rotation
	FRotator SetCameraRotation = GetControlRotation();
	SetCameraRotation.Pitch = 335.f;

	Controller->SetControlRotation(SetCameraRotation);

	// Movement Init
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	SetMovementStatus(EMovementStatus::EMS_IdleWalk);
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Identity_1", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Identity_1", IE_Released, this, &AMain::ShiftKeyUp);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);
	
}

void AMain::MoveForward(float Value)
{
	ForwardValue = Value;
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// Setting WalkFront & WalkBack

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMain::MoveRight(float Value)
{
	RightValue = Value;
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AMain::LookUpRate(float rate)
{
	float CameraPitch = GetControlRotation().Pitch;
	float NewCameraPitch = CameraPitch + rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();
	float DeltaPitch = rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds();

	// 최소값과 최댓값을 지정
	float MinPitch = 325.f;
	float MaxPitch = 350.f;


	// NewCameraPitch 값을 MinPitch와 MaxPitch 사이로 동기화
	NewCameraPitch = FMath::Clamp(NewCameraPitch, MinPitch, MaxPitch);

	// SetControlRotation을 사용하여 Pitch 값을 설정
	FRotator NewControlRotation = GetControlRotation();
	NewControlRotation.Pitch = NewCameraPitch;
	Controller->SetControlRotation(NewControlRotation);
}

void AMain::TurnRate(float rate)
{
	AddControllerYawInput(rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if(MovementStatus == EMovementStatus::EMS_IdleWalk)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else if(MovementStatus == EMovementStatus::EMS_Run)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
	SetMovementStatus(EMovementStatus::EMS_Run);
}

void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
	SetMovementStatus(EMovementStatus::EMS_IdleWalk);
}

void AMain::SpaceKeyDown()
{
	bSpaceKeyDown = true;
	SetMovementStatus(EMovementStatus::EMS_Roll);
	
}

void AMain::SpaceKeyUp()
{
	bSpaceKeyDown = false;
}