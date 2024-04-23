// Silent Evil Game, All Rights Reserved.

#include "Player/SECharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SEInventoryComponent.h"
#include "Components/SEHealthComponent.h"
#include "Components/SECharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "InventorySystem/SEWeaponData.h"

#include "InteractableObjects/SEInteractableObject.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/CollisionProfile.h"

DEFINE_LOG_CATEGORY_STATIC(LogSECharacter, All, All);

ASECharacter::ASECharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<USECharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Inventory
	InventoryComponent = CreateDefaultSubobject<USEInventoryComponent>(TEXT("CharacterInventory"));

	// Health
	HealthComponent = CreateDefaultSubobject<USEHealthComponent>(TEXT("CharacterHealth"));

	// SpringArmComponet
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 0.0f, 0.0f);

	SpringArmComponent->bUsePawnControlRotation = true;

	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 40.0f;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 30.0f;

	// Camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// FPSMesh
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSMesh"));
	FirstPersonMeshComponent->SetupAttachment(CameraComponent);
	FirstPersonMeshComponent->SetRelativeLocation(FVector(-30.0f, 0.0f, -150.0f));
	FirstPersonMeshComponent->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	// CharacterMovement
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanWalk = true;
	GetCharacterMovement()->NavAgentProps.bCanSwim = false;
	GetCharacterMovement()->NavAgentProps.bCanJump = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->AirControl = 0.35f;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 200.0f;
}

void ASECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchInteractableObject();
}

void ASECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &ASECharacter::InputStartMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASECharacter::InputStopMove);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASECharacter::InputMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASECharacter::InputLook);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASECharacter::InputSprint);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASECharacter::InputCrouch);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ASECharacter::InputReload);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ASECharacter::InputStartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ASECharacter::InputStopAim);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASECharacter::InputStartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ASECharacter::InputStopFire);

		EnhancedInputComponent->BindAction(FastAccessAction, ETriggerEvent::Started, this, &ASECharacter::InputFastAccessItem);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASECharacter::InputInteract);
	}
}

bool ASECharacter::GetShouldSprint() const
{
	return ShouldSprint;
}

float ASECharacter::GetDistanceForInteraction() const
{
	return CloseDistanceInteraction;
}

UCameraComponent* ASECharacter::GetCameraComponent() const
{
	return CameraComponent;
}

void ASECharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	FVector SpringArmLocation = SpringArmComponent->GetRelativeLocation();
	SpringArmLocation.Z = CrouchCameraHeight;

	SpringArmComponent->SetRelativeLocation(SpringArmLocation);
}

void ASECharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	FVector SpringArmLocation = SpringArmComponent->GetRelativeLocation();
	SpringArmLocation.Z = StandCameraHeight;

	SpringArmComponent->SetRelativeLocation(SpringArmLocation);
}

void ASECharacter::BeginPlay()
{
	Super::BeginPlay();

	FVector SpringArmLocation = SpringArmComponent->GetRelativeLocation();
	SpringArmLocation.Z = GetCharacterMovement()->IsCrouching() ? CrouchCameraHeight : StandCameraHeight;

	SpringArmComponent->SetRelativeLocation(SpringArmLocation);
}

void ASECharacter::SearchInteractableObject()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr)
	{
		return;
	}

	FVector CameraPosition = PlayerController->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		CameraPosition,
		CameraPosition + CameraForward * DistanceForSearchInteraction,
		CapsuleRadiusForSearchInteraction,
		UCollisionProfile::Get()->ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		{},
		EDrawDebugTrace::None,
		HitResult,
		true);
	
	if (!HitResult.bBlockingHit)
	{
		if (CurrentInteractableObject != nullptr)
		{
			CurrentInteractableObject->StopCanInteract(this);
			CurrentInteractableObject = nullptr;
		}

		return;
	}

	ASEInteractableObject* InteractObject = Cast<ASEInteractableObject>(HitResult.GetActor());
	if (InteractObject == nullptr || !InteractObject->GetEnabled())
	{
		if (CurrentInteractableObject != nullptr)
		{
			CurrentInteractableObject->StopCanInteract(this);
			CurrentInteractableObject = nullptr;
		}

		return;
	}

	CurrentDustanceToInteractionObject = FVector::Dist(CameraPosition, InteractObject->GetTargetLocation());
	if (CurrentInteractableObject == nullptr)
	{
		CurrentInteractableObject = InteractObject;
		CurrentInteractableObject->StartCanInteract(this);

		return;
	}

	if (CurrentInteractableObject != InteractObject)
	{
		CurrentInteractableObject->StopCanInteract(this);

		CurrentInteractableObject = InteractObject;
		CurrentInteractableObject->StartCanInteract(this);

		return;
	}
}

void ASECharacter::InputStartMove(const FInputActionValue& Value)
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (CameraShakeInstance == nullptr)
	{
		PlayerController->PlayerCameraManager->StopCameraShake(CameraShakeInstance);
	}

	if (ShouldSprint)
	{
		CameraShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(CameraSprintShake);
	}
	else
	{
		CameraShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(CameraWalkShake);
	}

	//UE_LOG(LogSECharacter, Display, TEXT("StartMove"));
}

void ASECharacter::InputStopMove(const FInputActionValue& Value)
{
	if (ShouldSprint)
	{
		StopSprint();
	}

	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (CameraShakeInstance != nullptr)
	{
		PlayerController->PlayerCameraManager->StopCameraShake(CameraShakeInstance);
		CameraShakeInstance = nullptr;
	}

	//UE_LOG(LogSECharacter, Display, TEXT("StopMove"));
}

void ASECharacter::InputMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

	if (Controller != nullptr)
	{
		FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

	if (ShouldSprint && !(MovementVector.Y > 0))
	{
		StopSprint();
	}
}

void ASECharacter::InputLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASECharacter::InputCrouch(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsCrouching())
	{
		StopCrouch();
	}
	else
	{
		StartCrouch();
	}
}

void ASECharacter::StartCrouch()
{
	UE_LOG(LogSECharacter, Display, TEXT("StartCrouch %s"), GetCharacterMovement()->IsCrouching() ? TEXT("true") :  TEXT("false"));

	Crouch();

	if (ShouldSprint)
	{
		StopSprint();
	}
}

void ASECharacter::StopCrouch()
{
	UE_LOG(LogSECharacter, Display, TEXT("StopCrouch %s"), GetCharacterMovement()->IsCrouching() ? TEXT("true") : TEXT("false"));

	UnCrouch();
}

void ASECharacter::InputSprint(const FInputActionValue& Value)
{
	if (!ShouldSprint && GetVelocity().Length() > 0 && !GetCharacterMovement()->IsCrouching())
	{
		StartSprint();
	}
}

void ASECharacter::StartSprint()
{
	ShouldSprint = true;
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (CameraShakeInstance != nullptr)
	{
		PlayerController->PlayerCameraManager->StopCameraShake(CameraShakeInstance);
	}
	CameraShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(CameraSprintShake);
}

void ASECharacter::StopSprint()
{
	ShouldSprint = false;
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController == nullptr)
	{
		return;
	}

	if (CameraShakeInstance != nullptr)
	{
		PlayerController->PlayerCameraManager->StopCameraShake(CameraShakeInstance);
	}
	CameraShakeInstance = PlayerController->PlayerCameraManager->StartCameraShake(CameraWalkShake);
}

void ASECharacter::InputReload(const FInputActionValue& Value)
{
	InventoryComponent->ReloadEquipWeapon();

	UE_LOG(LogSECharacter, Display, TEXT("Reload"));
}

void ASECharacter::InputStartFire(const FInputActionValue& Value)
{
	USEWeaponData* WeaponData = InventoryComponent->GetEquipWeapon();
	if (IsAim && WeaponData != nullptr)
	{
		WeaponData->MakeShot();
	}
}

void ASECharacter::InputStopFire(const FInputActionValue& Value)
{

}

void ASECharacter::InputStartAim(const FInputActionValue& Value)
{
	IsAim = true;
}

void ASECharacter::InputStopAim(const FInputActionValue& Value)
{
	IsAim = false;
}

void ASECharacter::InputFastAccessItem(const FInputActionValue& Value)
{
	UE_LOG(LogSECharacter, Display, TEXT("%i"), (int32)Value.Get<float>() - 1);
	InventoryComponent->FastAccessItem((int32)Value.Get<float>() - 1);
}

void ASECharacter::InputInteract(const FInputActionValue& Value)
{
	if (CurrentInteractableObject != nullptr)
	{
		if (CurrentDustanceToInteractionObject <= CloseDistanceInteraction)
		{
			CurrentInteractableObject->Interact(this);
			CurrentInteractableObject = nullptr;
		}
	}
}
