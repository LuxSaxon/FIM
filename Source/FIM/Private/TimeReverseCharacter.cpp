// Fill out your copyright notice in the Description page of Project Settings.

/*
#include "TimeReverseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FIMCharacter.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
ATimeReverseCharacter::ATimeReverseCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CharacterCapsule"));
	CapsuleComp->SetupAttachment(RootComponent);

	BodyMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(CapsuleComp);

	L_Weapon = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("L_SwordMesh"));
	L_Weapon->SetupAttachment(BodyMesh);
	R_Weapon = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("R_SwordMesh"));
	R_Weapon->SetupAttachment(BodyMesh);
}

// Called when the game starts or when spawned
void ATimeReverseCharacter::BeginPlay()
{
	Super::BeginPlay();
// 	AssignMesh();
	GhostCharacter();
}

/*
void ATimeReverseCharacter::AssignMesh()
{
	//Get Actor by class
	//CharacterComp = Cast<ACharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AFIMCharacter::StaticClass()));
	// 	CharacterComp = Cast<ACharacter>(BP_Character);

		//Set Body Skeletal Mesh
	BodyMesh->SetSkeletalMesh(Cast<USkeletalMesh>(CharacterComp->GetMesh()));

	//Get Equipment Static and assign it
	TArray<UStaticMeshComponent*> Components;
	CharacterComp->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++)
	{
		if (Components[i]->GetName() == L_WeaponName.ToString())
		{
			L_Weapon->SetSkeletalMesh(Cast<USkeletalMesh>(Components[i]));
		}
		else if (Components[i]->GetName() == R_WeaponName.ToString())
		{
			R_Weapon->SetSkeletalMesh(Cast<USkeletalMesh>(Components[i]));
		}
	}

	//Assign combat mode bool
// 	IsActorCombatmode = BP_Charac;
}

void ATimeReverseCharacter::GhostCharacter_Implementation()
{
// 	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(GhostMaterial, this);

	Array_BodyMaterial.SetNum(BodyMesh->GetMaterials().Num() + L_Weapon->GetMaterials().Num() + R_Weapon->GetMaterials().Num());

	if (BodyMesh != nullptr)
	{
		for (int32 i = 0; i < BodyMesh->GetMaterials().Num(); i++)
		{
			Array_BodyMaterial[i]->Create(GhostMaterial, BodyMesh->GetMaterial(i));
			BodyMesh->SetMaterial(i, GhostMaterial);
		}

		BodyMesh->CopyPoseFromSkeletalComponent(BodySkeletMeshComp);
	}

/*
	if (L_Weapon != nullptr && L_Weapon != nullptr)
	{
		for (int32 i = 0; i < L_Weapon->GetMaterials().Num(); i++)
		{
			L_Weapon->SetMaterial(i, DynMaterial);
			Array_BodyMaterial[i + BodyMesh->GetMaterials().Num()]->Create(L_Weapon->GetMaterial(i), L_Weapon);
		}

		for (int32 i = 0; i < R_Weapon->GetMaterials().Num(); i++)
		{
			R_Weapon->SetMaterial(i, DynMaterial);
			Array_BodyMaterial[i + BodyMesh->GetMaterials().Num() + L_Weapon->GetMaterials().Num()]->Create(R_Weapon->GetMaterial(i), R_Weapon);
		}
	}*/
//}

