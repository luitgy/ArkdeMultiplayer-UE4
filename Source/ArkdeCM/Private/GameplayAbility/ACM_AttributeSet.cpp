// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility/ACM_AttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

//=========================================================================================================================================================
UACM_AttributeSet::UACM_AttributeSet()
{

	MaxHealth = 100.0f;
	Health = MaxHealth;
	HealthRegen = 0.5f;
	
	MaxMana = 100.0f;
	Mana = MaxMana;
	ManaRegen = 2.0f;

	MaxStamina = 100.0f;
	Stamina = MaxStamina;
	StaminaRegen = 1.0f;

}

//=========================================================================================================================================================
void UACM_AttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float & NewValue)
{

	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute()) 
	{
		AdjustAttributeForMaxChange(Mana, MaxMana, NewValue, GetManaAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}

}

//=========================================================================================================================================================
void UACM_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{

	if(Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UACM_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UACM_AttributeSet, Health)) )
	{
	
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));

	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UACM_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UACM_AttributeSet, Mana)))
	{

		Mana.SetCurrentValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f, MaxMana.GetCurrentValue()));
		Mana.SetBaseValue(FMath::Clamp(Mana.GetBaseValue(), 0.0f, MaxMana.GetCurrentValue()));

	}
	else if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<FProperty>(UACM_AttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UACM_AttributeSet, Stamina)))
	{

		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.0f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.0f, MaxStamina.GetCurrentValue()));

	}

}

//=========================================================================================================================================================
void UACM_AttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData & AffectedAttribute, const FGameplayAttributeData & MaxAttribute, float NewMaxValue, const FGameplayAttribute & AffectedAttributeProperty)
{

	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && IsValid(AbilityComponent))
	{
		
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = CurrentMaxValue > 0.0f ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);

	}

}
