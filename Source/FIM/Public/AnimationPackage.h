// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Math/Vector2D.h"
#include "CoreMinimal.h"

struct FAnimationPackage
{
	FORCEINLINE FAnimationPackage();//FORCEINLINE means is "force the Unreal Engine connect to it that allow to jump around in code when call the function".
	FORCEINLINE FAnimationPackage(bool InCombatMode, bool InFalling, bool InMoveing, bool InAttacking, bool InDashing, bool InHitted, bool InSprinting, bool InDead, float MoveInputXVal, float MoveInputZVal, float InDeltaTime);

	bool bIsPlayerPawn;

	bool CombatMode;
	bool Falling;
	bool Moveing;
	bool Attacking;
	bool Dashing;
	bool Hitted;
	bool Sprinting;
	bool Dead;
	float MoveInputX;
	float MoveInputZ;

	float DeltaTime;
};

FAnimationPackage::FAnimationPackage()
{

}

FAnimationPackage::FAnimationPackage(bool InCombatMode, bool InFalling, bool InMoveing, bool InAttacking, bool InDashing, bool InHitted, bool InSprinting, bool InDead, float MoveInputXVal, float MoveInputZVal, float InDeltaTime):
	CombatMode(InCombatMode), Falling(InFalling), Moveing(InMoveing), Attacking(InAttacking), Dashing(InDashing), Hitted(InHitted), Sprinting(InSprinting), Dead(InDead), MoveInputX(MoveInputXVal), MoveInputZ(MoveInputZVal), DeltaTime(InDeltaTime)
{

}
