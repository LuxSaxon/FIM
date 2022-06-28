// Fill out your copyright notice in the Description page of Project Settings.


#include "SReverseTimeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "FIMCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "SFramePackage.h"

// Sets default values for this component's properties
USReverseTimeComponent::USReverseTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bReversingTime = false;
}

// Called when the game starts
void USReverseTimeComponent::BeginPlay()
{
	Super::BeginPlay();

	RevTimeCharacter = Cast<AFIMCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	RevTimeCharacter->ReverseTimeDelegate.AddDynamic(this, &USReverseTimeComponent::SetReversingTime);

// 	OwnerCharacter = Cast<ACharacter>(RevTimeCharacter);
}

// Called every frame
void USReverseTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bReversingTime)// storing data
	{
		RunningTime = 0.f;
		LeftReverseRunningTime = 0.f;
		RightReverseRunningTime = 0.f;

		AActor* Owner = GetOwner();
		TArray<UActorComponent*> Components = Owner->GetComponentsByClass(UStaticMeshComponent::StaticClass());

		if (Components.Num() > 0)
		{
			UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Components[0]);

			if (SMC)
			{
				FSFramePackage Package(Owner->GetActorLocation(), Owner->GetActorRotation(), SMC->GetPhysicsLinearVelocity(), SMC->GetPhysicsAngularVelocityInDegrees(), DeltaTime);

				if (RecordedTime < 15.0f)
				{
					StoredFrames.AddTail(Package);
					// 					StoredAnim.AddTail(AnimPackage);
					RecordedTime += DeltaTime;
					bOutOfData = false;
				}
				else
				{
					while (RunningTime >= 15.0f)
					{
						auto FrameHead = StoredFrames.GetHead();
						float FrameHeadDT = FrameHead->GetValue().DeltaTime;
						StoredFrames.RemoveNode(FrameHead);
						RecordedTime -= FrameHeadDT;
					}
					StoredFrames.AddTail(Package);
					RecordedTime += Package.DeltaTime;
					bOutOfData = false;
				}
			}
		}
	}
	else if (!bOutOfData) // Reversing Time
	{
		RunningTime += DeltaTime * ReverseTimeSpeed;
		auto Right = StoredFrames.GetTail();
		auto Left = Right->GetPrevNode();
		LeftReverseRunningTime = RightReverseRunningTime + Right->GetValue().DeltaTime;

		while (RunningTime > LeftReverseRunningTime)
		{
			RightReverseRunningTime += Right->GetValue().DeltaTime;
			Right = Left;
			LeftReverseRunningTime += Left->GetValue().DeltaTime;
			Left = Left->GetPrevNode();

			auto Tail = StoredFrames.GetTail();
			RecordedTime -= Tail->GetValue().DeltaTime;
			StoredFrames.RemoveNode(Tail);
			if (Left == StoredFrames.GetHead())
			{
				bOutOfData = true;
			}
		}

		//now left and right surround RunningTime
		if (RunningTime <= LeftReverseRunningTime && RunningTime >= RightReverseRunningTime)
		{
			float DT = RunningTime - RightReverseRunningTime;
			float Interval = LeftReverseRunningTime = RightReverseRunningTime;
			float Fraction = DT / Interval;

			FVector InterpLoc = FMath::VInterpTo(Right->GetValue().Location, Left->GetValue().Location, Fraction, 1.f);
			FRotator InterpRot = FMath::RInterpTo(Right->GetValue().Rotation, Left->GetValue().Rotation, Fraction, 1.f);
			FVector InterpLinVel = FMath::VInterpTo(Right->GetValue().LinerVelocity, Left->GetValue().LinerVelocity, Fraction, 1.f);
			FVector InterpAngVel = FMath::VInterpTo(Right->GetValue().AngularVelocity, Left->GetValue().AngularVelocity, Fraction, 1.f);

			SetActorVairable(InterpLoc, InterpRot, InterpLinVel, InterpAngVel);
		}
	}
}

void USReverseTimeComponent::SetReversingTime(bool InReversingTime)
{
	bReversingTime = InReversingTime;

	if (InReversingTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reversing Time!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop Reversing Time!"));
	}
}

void USReverseTimeComponent::SetActorVairable(FVector Location, FRotator Rotation, FVector LinerVel, FVector AngularVel)
{
	AActor* Owner = GetOwner();

	Owner->SetActorLocation(Location);
	Owner->SetActorRotation(Rotation);

	TArray<UActorComponent*> Components = Owner->GetComponentsByClass(UStaticMeshComponent::StaticClass());

	if (Components.Num() > 0)
	{
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Components[0]);
		if (SMC)
		{
			SMC->SetPhysicsLinearVelocity(LinerVel);
			SMC->SetPhysicsAngularVelocityInDegrees(AngularVel);
		}
	}
}

/*
void USReverseTimeComponent::SetActorAnimBool(bool CombatMode, bool Falling, bool Moveing, bool Attacking, bool Dashing, bool Hitted, bool Sprinting, bool Dead, float MoveInputX, float MoveInputZ)
{
	RevTimeCharacter->IsCombatMode = CombatMode;
	RevTimeCharacter->IsFalling = Falling;
	RevTimeCharacter->IsMoving = Moveing;
	RevTimeCharacter->IsAttacking = Attacking;
	RevTimeCharacter->bIsDashing = Dashing;
	RevTimeCharacter->bIsHitted = Hitted;
	RevTimeCharacter->IsSprinting = Sprinting;
	RevTimeCharacter->bIsDead = Dead;
	RevTimeCharacter->InputX = MoveInputX;
	RevTimeCharacter->InputZ = MoveInputZ;

	if (Attacking == true)
	{
		RevTimeCharacter->Server_Attack();
	}

	if (Dashing == true)
	{
		RevTimeCharacter->Dashing();
	}

	if (CombatMode == true)
	{
		RevTimeCharacter->Server_CombatMode();
	}
}

float USReverseTimeComponent::ReverseInputValue(float InputValue)
{
	return InputValue * -1.0f;
}

bool USReverseTimeComponent::BoolState(bool SetStateBool)
{
	if (SetStateBool == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}*/

