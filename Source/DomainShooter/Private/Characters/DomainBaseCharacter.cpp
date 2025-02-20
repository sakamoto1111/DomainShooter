// Domain Shooter By Genifinity


#include "Characters/DomainBaseCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ADomainBaseCharacter::ADomainBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	BaseSpringArm->SetupAttachment(GetRootComponent());

	BaseCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	BaseCamera->SetupAttachment(BaseSpringArm);
}

void ADomainBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(BaseCharacterIMC);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		check(Subsystem);
		Subsystem->AddMappingContext(BaseCharacterIMC, 0);
	}

}

void ADomainBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADomainBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(IA_BaseCharacterMovement, ETriggerEvent::Triggered, this, &ADomainBaseCharacter::BaseCharacterMovement);
	EnhancedInputComponent->BindAction(IA_BaseCharacterLookAround, ETriggerEvent::Triggered, this, &ADomainBaseCharacter::BaseCharacterLookAround);
	EnhancedInputComponent->BindAction(IA_BaseCharacterJump, ETriggerEvent::Started, this, &ADomainBaseCharacter::BaseCharacterJump);
}


void ADomainBaseCharacter::BaseCharacterMovement(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(Rotation);
		const FVector RightDirection = UKismetMathLibrary::GetRightVector(Rotation);

		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void ADomainBaseCharacter::BaseCharacterLookAround(const FInputActionValue& InputActionValue)
{
	FVector2D LookAroundValue = InputActionValue.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAroundValue.X);
		AddControllerPitchInput(-LookAroundValue.Y);
	}
}

void ADomainBaseCharacter::BaseCharacterJump(const FInputActionValue& InputActionValue)
{
	Jump();
}

