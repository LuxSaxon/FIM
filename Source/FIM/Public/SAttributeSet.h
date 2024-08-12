// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SAttributeSet.generated.h"

//Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 Basic setting for player attributes.
 */
UCLASS()
class FIM_API USAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USAttributeSet();

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//n_BASIC_HP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_n_BASIC_HP*/)
	FGameplayAttributeData n_BASIC_HP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BASIC_HP);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_n_BASIC_HP*/)
	FGameplayAttributeData Maxn_BASIC_HP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BASIC_HP);

	UFUNCTION()
	virtual void OnRep_n_BASIC_HP(const FGameplayAttributeData& Oldn_BASIC_HP);

	//n_BASIC_MP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_n_BASIC_MP*/)
	FGameplayAttributeData n_BASIC_MP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BASIC_MP);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_n_BASIC_MP*/)
	FGameplayAttributeData Maxn_BASIC_MP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BASIC_MP);

	UFUNCTION()
	virtual void OnRep_n_BASIC_MP(const FGameplayAttributeData& Oldn_BASIC_MP);

	//n_BASIC_SP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_n_BASIC_SP*/)
	FGameplayAttributeData n_BASIC_SP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BASIC_SP);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_n_BASIC_SP*/)
	FGameplayAttributeData Maxn_BASIC_SP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BASIC_SP);

	UFUNCTION()
	virtual void OnRep_n_BASIC_SP(const FGameplayAttributeData& Oldn_BASIC_SP);

	//n_BAISC_DP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_DP)
	FGameplayAttributeData n_BAISC_DP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BAISC_DP);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_DP)
	FGameplayAttributeData Maxn_BAISC_DP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BAISC_DP);

	UFUNCTION()
	virtual void OnRep_n_BAISC_DP(const FGameplayAttributeData& Oldn_BAISC_DP);

	//n_LV
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_LV)
	FGameplayAttributeData n_LV;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_LV);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_LV)
	FGameplayAttributeData Maxn_LV;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_LV);

	UFUNCTION()
	virtual void OnRep_n_LV(const FGameplayAttributeData& Oldn_LV);

	//n_EXP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_EXP)
	FGameplayAttributeData n_EXP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_EXP);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_EXP)
	FGameplayAttributeData Maxn_EXP;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_EXP);

	UFUNCTION()
	virtual void OnRep_n_EXP(const FGameplayAttributeData& Oldn_EXP);

	//n_BAISC_DATK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_DATK)
	FGameplayAttributeData n_BAISC_DATK;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BAISC_DATK);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_DATK)
	FGameplayAttributeData Maxn_BAISC_DATK;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BAISC_DATK);

	UFUNCTION()
	virtual void OnRep_n_BAISC_DATK(const FGameplayAttributeData& Oldn_BAISC_DATK);
	
	//n_BAISC_DDEF
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_DDEF)
	FGameplayAttributeData n_BAISC_DDEF;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BAISC_DDEF);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_DDEF)
	FGameplayAttributeData Maxn_BAISC_DDEF;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BAISC_DDEF);

	UFUNCTION()
	virtual void OnRep_n_BAISC_DDEF(const FGameplayAttributeData& Oldn_BAISC_DDEF);

	//n_BAISC_MOVE_SPEED
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_MOVE_SPEED)
	FGameplayAttributeData n_BAISC_MOVE_SPEED;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BAISC_MOVE_SPEED);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_MOVE_SPEED)
	FGameplayAttributeData Maxn_BAISC_MOVE_SPEED;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BAISC_MOVE_SPEED);

	UFUNCTION()
	virtual void OnRep_n_BAISC_MOVE_SPEED(const FGameplayAttributeData& Oldn_BAISC_MOVE_SPEED);

	//n_BAISC_ATTACK_SPEED
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_ATTACK_SPEED)
	FGameplayAttributeData n_BAISC_ATTACK_SPEED;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_BAISC_ATTACK_SPEED);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_BAISC_ATTACK_SPEED)
	FGameplayAttributeData Maxn_BAISC_ATTACK_SPEED;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_BAISC_ATTACK_SPEED);

	UFUNCTION()
	virtual void OnRep_n_BAISC_ATTACK_SPEED(const FGameplayAttributeData& Oldn_BAISC_ATTACK_SPEED);

	//n_CRI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_CRI)
	FGameplayAttributeData n_CRI;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_CRI);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_CRI)
	FGameplayAttributeData Maxn_CRI;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_CRI);

	UFUNCTION()
	virtual void OnRep_n_CRI(const FGameplayAttributeData& Oldn_CRI);

	//n_CRI_PROPORTION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_CRI_PROPORTION)
	FGameplayAttributeData n_CRI_PROPORTION;
	ATTRIBUTE_ACCESSORS(USAttributeSet, n_CRI_PROPORTION);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_n_CRI_PROPORTION)
	FGameplayAttributeData Maxn_CRI_PROPORTION;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Maxn_CRI_PROPORTION);

	UFUNCTION()
	virtual void OnRep_n_CRI_PROPORTION(const FGameplayAttributeData& Oldn_CRI_PROPORTION);
};
