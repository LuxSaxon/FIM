// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseEquippable.generated.h"

class USkeletalMeshComponent;

UCLASS()
class FIM_API ASBaseEquippable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBaseEquippable();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		UStaticMeshComponent* ItemStatticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
		USkeletalMeshComponent* ItemSkeletalMesh;

	//Variables

	bool bIsEquipped;

public:

	//Variables

	UPROPERTY(BlueprintReadOnly)
		FName AttachSocketName;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "Equip")
		void OnEquipped(USkeletalMeshComponent* TargetComp);

	UFUNCTION(BlueprintCallable, Category = "Equip")
		void OnUnEquipped();

	UFUNCTION(BlueprintCallable, Category = "Equip")
		void SetIsEquipped(bool bEquipped);

	UFUNCTION(BlueprintCallable, Category = "Equip")
		bool IsEquipped();
};
