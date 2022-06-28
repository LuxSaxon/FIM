// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include <GameplayEffectTypes.h>
#include "Components/TimelineComponent.h"
#include "FIMCharacter.generated.h"

class USAttributeSet;
class USActionComponent;
class UCurveFloat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReverseTimeDelegate, bool, bReverseTime);

UCLASS(config = Game)
class AFIMCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComp;
public:
	AFIMCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CameraComp subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return CameraComp; }

	UPROPERTY(BlueprintReadWrite, Category = "CharacterInput")
		bool IsMoving;

	UPROPERTY(BlueprintReadWrite, Category = "CharacterInput")
		float InputX;

	UPROPERTY(BlueprintReadWrite, Category = "CharacterInput")
		float InputZ;

	UPROPERTY(BlueprintReadWrite, Category = "ServerMode")
		bool IsCombatMode;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterInput")
		bool IsSprinting;

	UPROPERTY(BlueprintReadWrite, Category = "Dash")
		bool bIsDashing;

	UPROPERTY(BlueprintReadOnly, Category = "Dead")
		bool bIsDead;

	UPROPERTY(BlueprintReadWrite, Category = "Blocking")
		bool bIsBlock;

	UPROPERTY(BlueprintReadWrite, Category = "Hit")
		bool bIsHitted;

	UPROPERTY(BlueprintReadWrite, Category = "ReverseTime")
		bool IsReverseTime;

	UPROPERTY(BlueprintReadWrite, Category = "Falling")
		bool IsFalling;

	UPROPERTY(BlueprintReadWrite, Category = "Attack")
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MontagerRate")
		float PlayMontageRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float RotationSpeed;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/*Set up attack*/

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ServerMode")
		void Server_CombatMode();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ServerMode")
		void Server_Equip(FName SkeletalName_L, FName SkeletalName_R);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ServerMode")
		void Server_Attack();

	UFUNCTION(BlueprintCallable, Category = "NormalAttack")
		void NormalAttack(UAnimMontage* NormalAttack_1, UAnimMontage* NormalAttack_2, UAnimMontage* NormalAttack_3, UAnimMontage* NormalAttack_4);

	UPROPERTY(BlueprintReadWrite, Category = "Normal_Attack")
		const UAnimMontage* AM_Attack;

	FTimerHandle TH_EndAttack;

	UPROPERTY(BlueprintReadWrite, Category = "Normal_Attack")
		int32 Attack_Num;

	UPROPERTY(EditDefaultsOnly, Category = "Normal_Attack")
		float DelayAttackEnd;

	UFUNCTION()
		void EndNormalAttack();

	UFUNCTION(BlueprintNativeEvent, Category = "ServerMode")
		void CombatModeEvent(bool OnCombatStart);

	FVector LastInput;

	UFUNCTION(BlueprintCallable, Category = "ServerMode")
		void ConculateLastInput();

	UFUNCTION(BlueprintCallable, Category = "ServerMode")
		void InterToRotation(bool IsInter);

	bool IsInput;

	FRotator LastRotation;

	/*initialize character moving speed*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
		float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
		float BlockSpeed;

	UFUNCTION(BlueprintCallable, Category = "Character")
		bool IsOtherHostile(AFIMCharacter* Other);

	uint8 GetTeamID() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dead")
		void Dead();

	USkeletalMeshComponent* CharacterMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UStaticMeshComponent* WeaponComp_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UStaticMeshComponent* WeaponComp_L;

	UFUNCTION(BlueprintCallable, Category = "Dashing")
		void Dashing();

	/*Set up Sprint*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprint")
		float SprintSpeed;
	float StopMovementSpeed;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
		void SprintStart();

	UFUNCTION(BlueprintCallable, Category = "Sprint")
		void SprintEnd();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Effects")
		FName TimeToHitParamName;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay()override;

	UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterMovement")
		UCharacterMovementComponent* CM;

	/*Set up Dashing*/
	UFUNCTION(BlueprintCallable, Category = "Dashing")
		void DashingMovement(UCharacterMovementComponent* MovementComp);

	UFUNCTION(BlueprintCallable, Category = "Dashing")
		void PlayDashingMontage();

	void DashingEnd();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		float DashDistance;

	FVector CharacterLocation;
	FVector ActorDirection;
	FVector ForwardDir;

	UFUNCTION(BlueprintCallable, Category = "Combat")
		void CombatStart(bool DoCombatMode, bool IsOrientRotationToMovement, bool IsUseControllerDesiredRotation);
	void DisableRotationInWalking();

	FVector DirectionX;
	FVector DirectionZ;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterInput")
		float PlayerDirection;

	FTimerHandle EquipTimeDelay;

	FTimerHandle TH_Dashing;

	UPROPERTY(EditAnywhere, Category = "Dash")
		float DashDelayTime;

	UFUNCTION(BlueprintCallable, Category = "CharacterInput")
		int32 InputValue();

	/*setup block event*/
	void BlockingStart();
	void BlockingEnd();

	/*Team ID*/
	UPROPERTY(BlueprintReadOnly, Category = "Hit")
		uint8 TeamID;
	void AutoDeterminTeamIDbyControllerType();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USActionComponent* ActionComp;

	class UInputComponent* PlayerInputComp;

	/*setup dash animation montage*/
	UPROPERTY(BlueprintReadWrite, Category = "AnimMontage")
		UAnimMontage* Montage_GetAttack;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Montage_FDodge;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Montage_RDodge;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Montage_LDodge;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Montage_BDodge;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Montage_Equip;

	UPROPERTY(EditAnywhere, Category = "AnimMontage")
		UAnimMontage* Montage_Unequip;

public:

	/*Ability System setup*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
		class USAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(EditAnywhere, Category = "GAS")
		TArray<TSubclassOf<UGameplayAbility>> GA_Ability;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "GAS")
		virtual void InitializeAttributes(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	int32 AbilityLevelSetup;

	/*Attribute setup*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GAS")
		const class USAttributeSet* AttributeSettings;

	UFUNCTION(BlueprintPure, Category = "GAS")
		void GetHealthValues(float& Health, float& MaxHealth);
	UFUNCTION(BlueprintPure, Category = "GAS")
		void GethManaValues(float& Mana, float& MaxMana);
	UFUNCTION(BlueprintPure, Category = "GAS")
		void GetStaminaValues(float& Stamina, float& MaxStamina);
	UFUNCTION(BlueprintPure, Category = "GAS")
		void GetAttackPowerValues(float& AttackPower, float& MaxAttackPower);

	void OnHealthChangedNative(const FOnAttributeChangeData& Data);
	void OnManaChangedNative(const FOnAttributeChangeData& Data);
	void OnStaminaChangedNative(const FOnAttributeChangeData& Data);
	void OnAttackPowerChangedNative(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
		void OnHealthChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
		void BP_Die();
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
		void OnManaChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
		void OnStaminaChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
		void OnAttackPowerChanged(float OldValue, float NewValue);

	/*Setup Revering Time function*/
	UFUNCTION(BlueprintCallable, Category = "ReverseTime")
		void StartReversingTime();

	UFUNCTION(BlueprintCallable, Category = "ReverseTime")
		void EndReversingTime();

	FReverseTimeDelegate ReverseTimeDelegate;
};

