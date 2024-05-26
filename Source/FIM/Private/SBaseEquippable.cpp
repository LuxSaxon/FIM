// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseEquippable.h"
#include "GameFramework/Actor.h"
#include "FIMCharacter.h"

// Sets default values
ASBaseEquippable::ASBaseEquippable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));

	ItemStatticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));

	bIsEquipped = false;

}

void ASBaseEquippable::OnEquipped(USkeletalMeshComponent* TargetComp)
{
	bIsEquipped = true;
}

void ASBaseEquippable::OnUnEquipped()
{
	if (bIsEquipped == true)
	{
		bIsEquipped = false;
	}
}

void ASBaseEquippable::SetIsEquipped(bool bEquipped)
{
	bEquipped = bIsEquipped;
}

bool ASBaseEquippable::IsEquipped()
{
	return bIsEquipped;
}

