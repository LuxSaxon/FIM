// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Math/Vector2D.h"
#include "CoreMinimal.h"

/**
 Storage Information
 */
struct FSFramePackage
{
	FORCEINLINE FSFramePackage();//FORCEINLINE means is "force the Unreal Engine connect to it that allow to jump around in code when call the function".
	FORCEINLINE FSFramePackage(FVector InLocation, FRotator InRotation, FVector InLinerVelocity, FVector InAngularVelocity, float InDeltaTime);

	FVector Location;
	FRotator Rotation;
	FVector LinerVelocity;
	FVector AngularVelocity;

	bool bIsPlayerPawn;
	FVector2D MouseXY;

	float DeltaTime;
};

FSFramePackage::FSFramePackage()
{

}

FSFramePackage::FSFramePackage(FVector InLocation, FRotator InRotation, FVector InLinerVelocity, FVector InAngularVelocity, float InDeltaTime) :
	Location(InLocation), Rotation(InRotation), LinerVelocity(InLinerVelocity), AngularVelocity(InAngularVelocity), DeltaTime(InDeltaTime)// it's equal to "Location = InLocation;"
{

}
