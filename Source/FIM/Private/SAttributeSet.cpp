// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

USAttributeSet::USAttributeSet()
{
}

void USAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == Getn_BASIC_HPAttribute())
	{
		Setn_BASIC_HP(FMath::Clamp(Getn_BASIC_HP(), 0.0f, GetMaxn_BASIC_HP()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BASIC_MPAttribute())
	{
		Setn_BASIC_MP(FMath::Clamp(Getn_BASIC_MP(), 0.0f, GetMaxn_BASIC_MP()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BASIC_SPAttribute())
	{
		Setn_BASIC_SP(FMath::Clamp(Getn_BASIC_SP(), 0.0f, GetMaxn_BASIC_SP()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BAISC_DPAttribute())
	{
		Setn_BAISC_DP(FMath::Clamp(Getn_BAISC_DP(), 0.0f, GetMaxn_BAISC_DP()));
	}
	if (Data.EvaluatedData.Attribute == Getn_LVAttribute())
	{
		Setn_LV(FMath::Clamp(Getn_LV(), 0.0f, GetMaxn_LV()));
	}
	if (Data.EvaluatedData.Attribute == Getn_EXPAttribute())
	{
		Setn_EXP(FMath::Clamp(Getn_EXP(), 0.0f, GetMaxn_EXP()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BAISC_DATKAttribute())
	{
		Setn_BAISC_DATK(FMath::Clamp(Getn_BAISC_DATK(), 0.0f, GetMaxn_BAISC_DATK()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BAISC_DDEFAttribute())
	{
		Setn_BAISC_DDEF(FMath::Clamp(Getn_BAISC_DDEF(), 0.0f, GetMaxn_BAISC_DDEF()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BAISC_MOVE_SPEEDAttribute())
	{
		Setn_BAISC_MOVE_SPEED(FMath::Clamp(Getn_BAISC_MOVE_SPEED(), 0.0f, GetMaxn_BAISC_MOVE_SPEED()));
	}
	if (Data.EvaluatedData.Attribute == Getn_BAISC_ATTACK_SPEEDAttribute())
	{
		Setn_BAISC_ATTACK_SPEED(FMath::Clamp(Getn_BAISC_ATTACK_SPEED(), 0.0f, GetMaxn_BAISC_ATTACK_SPEED()));
	}
	if (Data.EvaluatedData.Attribute == Getn_CRIAttribute())
	{
		Setn_CRI(FMath::Clamp(Getn_CRI(), 0.0f, GetMaxn_CRI()));
	}
	if (Data.EvaluatedData.Attribute == Getn_CRI_PROPORTIONAttribute())
	{
		Setn_CRI_PROPORTION(FMath::Clamp(Getn_CRI_PROPORTION(), 0.0f, GetMaxn_CRI_PROPORTION()));
	}
}

void USAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BASIC_HP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BASIC_MP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BASIC_SP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BAISC_DP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_LV, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_EXP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BAISC_DATK, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BAISC_DDEF, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BAISC_MOVE_SPEED, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_BAISC_ATTACK_SPEED, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_CRI, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, n_CRI_PROPORTION, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BASIC_HP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BASIC_MP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BASIC_SP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BAISC_DP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_LV, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_EXP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BAISC_DATK, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BAISC_DDEF, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BAISC_MOVE_SPEED, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_BAISC_ATTACK_SPEED, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_CRI, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USAttributeSet, Maxn_CRI_PROPORTION, COND_None, REPNOTIFY_Always);
}

void USAttributeSet::OnRep_n_BASIC_HP(const FGameplayAttributeData& Oldn_BASIC_HP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BASIC_HP, Oldn_BASIC_HP);
}

void USAttributeSet::OnRep_n_BASIC_MP(const FGameplayAttributeData& Oldn_BASIC_MP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BASIC_MP, Oldn_BASIC_MP);
}

void USAttributeSet::OnRep_n_BASIC_SP(const FGameplayAttributeData& Oldn_BASIC_SP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BASIC_SP, Oldn_BASIC_SP);
}

void USAttributeSet::OnRep_n_BAISC_DP(const FGameplayAttributeData& Oldn_BAISC_DP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BAISC_DP, Oldn_BAISC_DP);
}

void USAttributeSet::OnRep_n_LV(const FGameplayAttributeData& Oldn_LV)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_LV, Oldn_LV);
}

void USAttributeSet::OnRep_n_EXP(const FGameplayAttributeData& Oldn_EXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_EXP, Oldn_EXP);
}

void USAttributeSet::OnRep_n_BAISC_DATK(const FGameplayAttributeData& Oldn_BAISC_DATK)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BAISC_DATK, Oldn_BAISC_DATK);
}

void USAttributeSet::OnRep_n_BAISC_DDEF(const FGameplayAttributeData& Oldn_BAISC_DDEF)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BAISC_DDEF, Oldn_BAISC_DDEF);
}

void USAttributeSet::OnRep_n_BAISC_MOVE_SPEED(const FGameplayAttributeData& Oldn_BAISC_MOVE_SPEED)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BAISC_MOVE_SPEED, Oldn_BAISC_MOVE_SPEED);
}

void USAttributeSet::OnRep_n_BAISC_ATTACK_SPEED(const FGameplayAttributeData& Oldn_BAISC_ATTACK_SPEED)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_BAISC_ATTACK_SPEED, Oldn_BAISC_ATTACK_SPEED);
}

void USAttributeSet::OnRep_n_CRI(const FGameplayAttributeData& Oldn_CRI)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_CRI, Oldn_CRI);
}

void USAttributeSet::OnRep_n_CRI_PROPORTION(const FGameplayAttributeData& Oldn_CRI_PROPORTION)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USAttributeSet, n_CRI_PROPORTION, Oldn_CRI_PROPORTION);
}
