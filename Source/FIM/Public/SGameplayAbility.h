// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FIM/FIM.h"
#include "SGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class FIM_API USGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	USGameplayAbility();

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

};
