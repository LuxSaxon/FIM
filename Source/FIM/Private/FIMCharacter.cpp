// Copyright Epic Games, Inc. All Rights Reserved.

#include "FIMCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Logging/LogMacros.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/RotationMatrix.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SAbilitySystemComponent.h"
#include "SAttributeSet.h"
#include "GameplayAbilitySpec.h"
#include <GameplayEffectTypes.h>
#include "SGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Components/PrimitiveComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "FIM/FIM.h"
#include "SActionComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

//////////////////////////////////////////////////////////////////////////
// AFIMCharacter

AFIMCharacter::AFIMCharacter()
{
	// Set size for collision capsule
	CapsuleComp = GetCapsuleComponent();
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//get skeletal mesh & set up weapon for it.
// 	CharacterMeshComp = GetMesh();
	CharacterMeshComp = GetMesh();
	CharacterMeshComp->SetSimulatePhysics(true);
	CharacterMeshComp->SetAllBodiesSimulatePhysics(true);
	
	WeaponComp_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_R"));
   	WeaponComp_R->SetupAttachment(CharacterMeshComp);

	WeaponComp_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon_L"));
  	WeaponComp_L->SetupAttachment(CharacterMeshComp);

	// set our turn rate for input
	TurnRateGamepad = 50.f;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	CMComp = GetCharacterMovement();
	CMComp->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	CMComp->bAllowPhysicsRotationDuringAnimRootMotion = false;
	CMComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	CMComp->JumpZVelocity = 600.0f;
	CMComp->AirControl = 0.2f;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	//make camera more realistic and heavy
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 50.0f;
	CameraBoom->CameraRotationLagSpeed = 50.0f;
	CameraBoom->CameraLagMaxDistance = 10.0f;

	// Create a follow camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComp->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	WalkingSpeed = 200;
	SprintSpeed = 800;
	DashDistance = 5000.0f;
	DashDelayTime = 1.0f;
	DelayAttackEnd = 2.0f;

	BlockSpeed = 200;
	IsCombatMode = false;

	TimeToHitParamName = "TimeToHit";

	// Ability System Components
	AbilitySystemComp = CreateDefaultSubobject<USAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSettings = CreateDefaultSubobject<USAttributeSet>("AttributeSettings");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	bIsDead = false;

	bIsBlock = false;

	bIsHitted = false;

	TeamID = 255;

	IsMoving = false;

	PlayMontageRate = 1.0f;

	bReplicates = true;

	StopMovementSpeed = 0;
}

void AFIMCharacter::BeginPlay()
{
	Super::BeginPlay();
	Server_Equip("S_Unequip_L", "S_Unequip_R");

	if (AbilitySystemComp)
	{
		AttributeSettings = AbilitySystemComp->GetSet<USAttributeSet>();

		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSettings->GetHealthAttribute()).AddUObject(this, &AFIMCharacter::OnHealthChangedNative);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSettings->GetManaAttribute()).AddUObject(this, &AFIMCharacter::OnManaChangedNative);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSettings->GetStaminaAttribute()).AddUObject(this, &AFIMCharacter::OnStaminaChangedNative);
		AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSettings->GetAttackPowerAttribute()).AddUObject(this, &AFIMCharacter::OnAttackPowerChangedNative);
		AbilityLevelSetup = 1;
	}

	AutoDeterminTeamIDbyControllerType();
}

void AFIMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Player InputZ %f"), InputZ));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Player InputX %f"), InputX));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("ActorDirection %s"), *ActorDirection.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("ForwardDir %s"), *ForwardDir.ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Player Speed %f"), CM->MaxWalkSpeed));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("Player Direction %f"), PlayerDirection));
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("AttackNumber %i"), Attack_Num));

		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, FString::Printf(TEXT("IsDashing %s"), (bIsDashing ? TEXT("true") : TEXT("false"))));
	*/

	InputZ = InputZ * DeltaTime;
	InputX = InputX * DeltaTime;

	DisableRotationInWalking();
	ConculateLastInput();
}

void AFIMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("CombatMode", IE_Pressed, this, &AFIMCharacter::Server_CombatMode);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFIMCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFIMCharacter::SprintEnd);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AFIMCharacter::Dashing);

	PlayerInputComponent->BindAction("NormlaAttack", IE_Pressed, this, &AFIMCharacter::Server_Attack);

	PlayerInputComponent->BindAction("Blocking", IE_Pressed, this, &AFIMCharacter::BlockingStart);
	PlayerInputComponent->BindAction("Blocking", IE_Released, this, &AFIMCharacter::BlockingEnd);

	PlayerInputComponent->BindAction("ReverseTime", IE_Pressed, this, &AFIMCharacter::StartReversingTime);
	PlayerInputComponent->BindAction("ReverseTime", IE_Released, this, &AFIMCharacter::EndReversingTime);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFIMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFIMCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFIMCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFIMCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFIMCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFIMCharacter::TouchStopped);

	if (AbilitySystemComp && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}

void AFIMCharacter::Server_CombatMode_Implementation()
{
	if (!IsCombatMode)
	{
		CombatStart(true, false, true);
		CombatModeEvent(true);
	}
	else
	{
		CombatStart(false, true, false);
		CombatModeEvent(false);
	}
}

void AFIMCharacter::CombatModeEvent_Implementation(bool OnCombatStart)
{

}

void AFIMCharacter::Server_Equip_Implementation(FName SkeletalName_L, FName SkeletalName_R)
{
	WeaponComp_L->AttachToComponent(CharacterMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SkeletalName_L);
	WeaponComp_R->AttachToComponent(CharacterMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SkeletalName_R);
}


/*
void AFIMCharacter::Server_Equip_Implementation()
{
}*/

void AFIMCharacter::Server_Attack_Implementation()
{
	SetReplicates(true);

	if (GA_Ability.Num() > 0 && IsCombatMode == true)
	{
		AbilitySystemComp->TryActivateAbilityByClass(GA_Ability[0], true);
	}
}

void AFIMCharacter::NormalAttack(UAnimMontage* NormalAttack_1, UAnimMontage* NormalAttack_2, UAnimMontage* NormalAttack_3, UAnimMontage* NormalAttack_4)
{
	if (bIsHitted == false)
	{
		switch (Attack_Num)
		{
		case 0:
			PlayAnimMontage(NormalAttack_1, 1);
			AM_Attack = NormalAttack_1;
			Attack_Num++;
			IsAttacking = true;
			break;

		case 1:
			PlayAnimMontage(NormalAttack_2, 1);
			AM_Attack = NormalAttack_2;
			Attack_Num++;
			IsAttacking = true;
			break;

		case 2:
			PlayAnimMontage(NormalAttack_3, 1);
			AM_Attack = NormalAttack_3;
			Attack_Num++;
			IsAttacking = true;
			break;

		case 3:
			PlayAnimMontage(NormalAttack_4, 1);
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			AM_Attack = NormalAttack_4;
			Attack_Num = 0;
			IsAttacking = true;
			break;

		default:
			Attack_Num = 0;
			AM_Attack = nullptr;
		}

		GetWorldTimerManager().SetTimer(TH_EndAttack, this, &AFIMCharacter::EndNormalAttack, DelayAttackEnd, false);
	}

}

void AFIMCharacter::EndNormalAttack()
{
	Attack_Num = 0;
	AM_Attack = nullptr;
	IsAttacking = false;
}

void AFIMCharacter::CombatStart(bool DoCombatMode, bool IsOrientRotationToMovement, bool IsUseControllerDesiredRotation)
{
	IsCombatMode = DoCombatMode;
	GetCharacterMovement()->bOrientRotationToMovement = IsOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = IsUseControllerDesiredRotation;

	if (IsCombatMode == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;
		PlayAnimMontage(Montage_Equip, 1, NAME_None);
		GetWorld()->GetTimerManager().SetTimer(EquipTimeDelay, [&]()
			{
				Server_Equip("S_Equip_L", "S_Equip_R");
			}, 0.2f, false);
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		PlayAnimMontage(Montage_Unequip, 1, NAME_None);
		GetWorld()->GetTimerManager().SetTimer(EquipTimeDelay, [&]()
			{
				Server_Equip("S_Unequip_L", "S_Unequip_R");
			}, 0.2f, false);
	}

}

void AFIMCharacter::DisableRotationInWalking()
{
	if (InputX == 0 && InputZ == 0)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AFIMCharacter::SprintStart()
{
	IsSprinting = true;

	if (IsCombatMode == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

}

void AFIMCharacter::SprintEnd()
{
	IsSprinting = false;

	if (IsCombatMode == false)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;
	}

}

int32 AFIMCharacter::InputValue()
{
	if (InputX > 0)
		return 1;
	else if (InputX < 0)
		return 2;
	else if (InputZ > 0)
		return 3;
	else if (InputZ < 0)
		return 4;
	else
		return 0;
}

void AFIMCharacter::Dashing()
{
	if (IsCombatMode == true)
	{
		if (!bIsDashing)
		{
			bIsDashing = true;
			switch (InputValue())
			{
			case 1:
				DashingMovement(CMComp);
				PlayAnimMontage(Montage_RDodge, PlayMontageRate, NAME_None);
				break;

			case 2:
				DashingMovement(CMComp);
				PlayAnimMontage(Montage_LDodge, PlayMontageRate, NAME_None);
				break;

			case 3:
				DashingMovement(CMComp);
				PlayAnimMontage(Montage_FDodge, PlayMontageRate, NAME_None);
				break;

			case 4:
				DashingMovement(CMComp);
				PlayAnimMontage(Montage_BDodge, PlayMontageRate, NAME_None);
				break;
			}
			GetWorld()->GetTimerManager().SetTimer(TH_Dashing, this, &AFIMCharacter::DashingEnd, DashDelayTime, false);
		}
	}
}

void AFIMCharacter::DashingMovement(UCharacterMovementComponent* MovementComp)
{
	if (IsReverseTime == false)
	{
		const FVector Dash = MovementComp->GetLastInputVector();
		if (GetCharacterMovement()->Velocity != FVector::ZeroVector)
		{
			LaunchCharacter(Dash * DashDistance, true, true);
		}
	}
}

void AFIMCharacter::PlayDashingMontage()
{
	if (!bIsDashing)
	{
		bIsDashing = true;
		switch (InputValue())
		{
		case 1:
			PlayAnimMontage(Montage_RDodge, 1, NAME_None);
			break;

		case 2:
			PlayAnimMontage(Montage_LDodge, 1, NAME_None);
			break;

		case 3:
			PlayAnimMontage(Montage_FDodge, 1, NAME_None);
			break;

		case 4:
			PlayAnimMontage(Montage_BDodge, 1, NAME_None);
			break;
		}
		GetWorld()->GetTimerManager().SetTimer(TH_Dashing, this, &AFIMCharacter::DashingEnd, DashDelayTime, false);
	}
}

void AFIMCharacter::DashingEnd()
{
	bIsDashing = false;
}

void AFIMCharacter::ConculateLastInput()
{
	LastInput = CMComp->GetLastInputVector();
	FRotator CameraRotation = CameraComp->GetComponentRotation();

	if (!LastInput.IsZero() || IsAttacking)
	{
		IsInput = true;

		if (IsInput == true)
		{
			LastRotation.Yaw = CameraRotation.Yaw;
		}
	}
	else
	{
		IsInput = false;
		LastInput.Zero();
	}
}

void AFIMCharacter::InterToRotation(bool IsInter)
{
	if (IsInter == true)
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), LastRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed));
	}
}

void AFIMCharacter::BlockingStart()
{
	if (IsCombatMode)
	{
		bIsBlock = true;
		GetCharacterMovement()->StopMovementImmediately();
		OnInvicinble();
	}
	else
	{
		bIsBlock = false;
	}
}

void AFIMCharacter::BlockingEnd()
{
	bIsBlock = false;
}

void AFIMCharacter::OnInvicinble_Implementation()
{

}

void AFIMCharacter::AutoDeterminTeamIDbyControllerType()
{
	if (GetController() && GetController()->IsPlayerController())
	{
		TeamID = 0;
	}
}

bool AFIMCharacter::IsOtherHostile(AFIMCharacter* Other)
{
	return TeamID != Other->GetTeamID();
}

uint8 AFIMCharacter::GetTeamID() const
{
	return TeamID;
}

void AFIMCharacter::Dead_Implementation()
{
	if (!bIsDead)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			PC->DisableInput(PC);
		}

		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			AIC->GetBrainComponent()->StopLogic("Dead");
		}
	}
}

void AFIMCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AFIMCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AFIMCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFIMCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFIMCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	if (Value > 0 || Value < 0)
	{
		IsMoving = true;
	}
	else
	{
		IsMoving = false;
	}

	InputZ = Value;
}

void AFIMCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}

	if (Value > 0 || Value < 0)
	{
		IsMoving = true;
	}
	else
	{
		IsMoving = false;
	}

	InputX = Value;
}

void AFIMCharacter::InitializeAttributes(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	if (AbilitySystemComp)
	{
		if (HasAuthority() && AbilityToGet)
		{
			AbilityLevel = AbilityLevelSetup;
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel, 0));
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);//holds information about who we are acting on and who controls us.
	}
}

class UAbilitySystemComponent* AFIMCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AFIMCharacter::GetHealthValues(float& Health, float& MaxHealth)
{
	Health = AttributeSettings->GetHealth();
	MaxHealth = AttributeSettings->GetMaxHealth();
}

void AFIMCharacter::GethManaValues(float& Mana, float& MaxMana)
{
	Mana = AttributeSettings->GetMana();
	MaxMana = AttributeSettings->GetMaxMana();
}

void AFIMCharacter::GetStaminaValues(float& Stamina, float& MaxStamina)
{
	Stamina = AttributeSettings->GetStamina();
	MaxStamina = AttributeSettings->GetMaxStamina();
}

void AFIMCharacter::GetAttackPowerValues(float& AttackPower, float& MaxAttackPower)
{
	AttackPower = AttributeSettings->GetAttackPower();
	AttackPower = AttributeSettings->GetMaxAttackPower();
}

void AFIMCharacter::OnHealthChangedNative(const FOnAttributeChangeData& Data)
{
	OnHealthChanged(Data.OldValue, Data.NewValue);
	if (Data.OldValue <= 0.0f && !bIsDead)
	{
		Dead();
		bIsDead = true;
		BP_Die();
	}
}

void AFIMCharacter::OnManaChangedNative(const FOnAttributeChangeData& Data)
{
	OnManaChanged(Data.OldValue, Data.NewValue);
}

void AFIMCharacter::OnStaminaChangedNative(const FOnAttributeChangeData& Data)
{
	OnStaminaChanged(Data.OldValue, Data.NewValue);
}

void AFIMCharacter::OnAttackPowerChangedNative(const FOnAttributeChangeData& Data)
{
	OnAttackPowerChanged(Data.OldValue, Data.NewValue);
}

void AFIMCharacter::StartReversingTime()
{
	ReverseTimeDelegate.Broadcast(true);
	IsReverseTime = true;
	PlayMontageRate = -1.0f;
}

void AFIMCharacter::EndReversingTime()
{
	ReverseTimeDelegate.Broadcast(false);
	IsReverseTime = false;
	PlayMontageRate = 1.0f;
}

