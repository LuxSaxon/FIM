// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFramePackage.h"
#include "AnimationPackage.h"
#include "FIMCharacter.h"
#include "SReverseTimeComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class FIM_API USReverseTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USReverseTimeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "ReversTime")
		void SetReversingTime(bool InReversingTime);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// reversing time when true, collecting data when false
	bool bReversingTime;

	// out of time data, cannot keep reversing
	bool bOutOfData;

	// Actual time since we started reversing time
	float RunningTime;

	// Running count of the frame package data times
	float LeftReverseRunningTime;
	float RightReverseRunningTime;

	// the total amount of time recorded in FramePackage
	float RecordedTime;

	TDoubleLinkedList<FSFramePackage> StoredFrames;
// 	TDoubleLinkedList<FAnimationPackage> StoredAnim;

	void SetActorVairable(FVector Location, FRotator Rotation, FVector LinerVel, FVector AngularVel);
// 	void SetActorAnimBool(bool CombatMode, bool Falling, bool Moveing, bool Attacking, bool Dashing, bool Hitted, bool Sprinting, bool Dead, float MoveInputX, float MoveInputZ);

	UPROPERTY(EditAnywhere, Category = "ReverseTime")
		float ReverseTimeSpeed;

/*
	float ReverseInputValue(float InputValue);
	bool BoolState(bool SetStateBool);*/

	AFIMCharacter* RevTimeCharacter;
// 	ACharacter* OwnerCharacter;

};
