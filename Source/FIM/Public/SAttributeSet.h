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

	//Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_Health*/)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_Health*/)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USAttributeSet, MaxHealth);

	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	//Mana
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_Mana*/)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Mana);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_Mana*/)
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(USAttributeSet, MaxMana);

	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	//Stamina
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_Stamina*/)
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USAttributeSet, Stamina);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes"/*, ReplicatedUsing = OnRep_Stamina*/)
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USAttributeSet, MaxStamina);

	UFUNCTION()
		virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	//Attack Power
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackPower)
		FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USAttributeSet, AttackPower);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_AttackPower)
		FGameplayAttributeData MaxAttackPower;
	ATTRIBUTE_ACCESSORS(USAttributeSet, MaxAttackPower);

	UFUNCTION()
		virtual void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower);

};
