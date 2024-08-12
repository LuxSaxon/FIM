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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	bool IsInvincible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Devour")
	bool IsDevour;

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

	/*
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ServerMode")
			void Server_Equip();*/

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ServerMode")
	void Server_Attack();

	UFUNCTION(BlueprintCallable, Category = "NormalAttack")
	void NormalAttack(const TArray<UAnimMontage*>& NormalAttacks);

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
	bool forBackup;

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

	virtual void BeginPlay() override;

	UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadOnly, Category = "CharacterMovement")
	UCharacterMovementComponent* CMComp;

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

	UFUNCTION(BlueprintNativeEvent, Category = "Melee")
	void OnInvicinble();

	/*Devour System*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevourSystem")
	void DevourEvent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevourSystem")
	void Devour();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DevourSystem")
	float CurrentMaximonn_BASIC_HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DevourSystem")
	int DevourTest;

	/*Possessed*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DevourSystem")
	void PossessedEvent();

	UPROPERTY(BlueprintReadWrite, Category = "CharacterInput")
	bool ProcessPossess;
	
	UPROPERTY(BlueprintReadWrite, Category = "CharacterInput")
	bool OnPossess;


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
	void Getn_BASIC_HPValues(float& n_BASIC_HP, float& Maxn_BASIC_HP);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Gethn_BASIC_MPValues(float& n_BASIC_MP, float& Maxn_BASIC_MP);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_BASIC_SPValues(float& n_BASIC_SP, float& Maxn_BASIC_SP);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_BAISC_DPValues(float& n_BAISC_DP, float& Maxn_BAISC_DP);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_LVValues(float& n_LV, float& Maxn_LV);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_EXPValues(float& n_EXP, float& Maxn_EXP);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_BAISC_DATKValues(float& n_BAISC_DATK, float& Maxn_BAISC_DATK);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_BAISC_DDEFValues(float& n_BAISC_DDEF, float& Maxn_BAISC_DDEF);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_BAISC_MOVE_SPEEDValues(float& n_BAISC_MOVE_SPEED, float& Maxn_BAISC_MOVE_SPEED);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_BAISC_ATTACK_SPEEDValues(float& n_BAISC_ATTACK_SPEED, float& Maxn_BAISC_ATTACK_SPEED);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_CRIValues(float& n_CRI, float& Maxn_CRI);
	UFUNCTION(BlueprintPure, Category = "GAS")
	void Getn_CRI_PROPORTIONValues(float& n_CRI_PROPORTION, float& Maxn_CRI_PROPORTION);

	void Onn_BASIC_HPChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BASIC_MPChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BASIC_SPChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BAISC_DPChangedNative(const FOnAttributeChangeData& Data);
	void Onn_LVChangedNative(const FOnAttributeChangeData& Data);
	void Onn_EXPChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BAISC_DATKChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BAISC_DDEFChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BAISC_MOVE_SPEEDChangedNative(const FOnAttributeChangeData& Data);
	void Onn_BAISC_ATTACK_SPEEDChangedNative(const FOnAttributeChangeData& Data);
	void Onn_CRIChangedNative(const FOnAttributeChangeData& Data);
	void Onn_CRI_PROPORTIONChangedNative(const FOnAttributeChangeData& Data);

	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BASIC_HPChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GAS")
	void BP_Die();
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BASIC_MPChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BASIC_SPChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BAISC_DPChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_LVChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_EXPChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BAISC_DATKChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BAISC_DDEFChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BAISC_MOVE_SPEEDChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_BAISC_ATTACK_SPEEDChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_CRIChanged(float OldValue, float NewValue);
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Onn_CRI_PROPORTIONChanged(float OldValue, float NewValue);

	/*Setup Revering Time function*/
	UFUNCTION(BlueprintCallable, Category = "ReverseTime")
	void StartReversingTime();

	UFUNCTION(BlueprintCallable, Category = "ReverseTime")
	void EndReversingTime();

	FReverseTimeDelegate ReverseTimeDelegate;
};
