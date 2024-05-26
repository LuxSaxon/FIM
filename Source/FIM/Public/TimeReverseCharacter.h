/*// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeReverseCharacter.generated.h"

class UCapsuleComponent;
class UPoseableMeshComponent;
class UMaterialInstanceDynamic;
class UPoseableMeshComponent;

UCLASS()
class FIM_API ATimeReverseCharacter : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeReverseCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere)
		UPoseableMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere)
		UPoseableMeshComponent* L_Weapon;

	UPROPERTY(EditAnywhere)
		UPoseableMeshComponent* R_Weapon;

	UPROPERTY(BlueprintReadWrite, Category = "GhostMaterial")
		TArray<UMaterialInstanceDynamic*> Array_BodyMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "GhostMaterial")
		UMaterialInterface* GhostMaterial;

	UPROPERTY(BlueprintReadWrite, Category = "BodySkelet")
		USkeletalMeshComponent* BodySkeletMeshComp;

	UPROPERTY(BlueprintReadWrite, Category = "ObjectComp")
		UObject* ObjectComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	ACharacter* CharacterComp;

	UPROPERTY(EditAnywhere, Category = "WeaponName")
	FName L_WeaponName;

	UPROPERTY(EditAnywhere, Category = "WeaponName")
	FName R_WeaponName;

// 	AFIMCharacter* FIMChar;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
// 		UBlueprint* BP_Character;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GhostMode")
		void GhostCharacter();

	void AssignMesh();

};*/
