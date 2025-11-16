// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPGAttributeSet.h"
#include  "GameplayEffect.h"
#include  "GameplayEffectExtension.h"
#include  "RPGCharacter.h"

// Конструктор класса URPGAttributeSet
URPGAttributeSet::URPGAttributeSet() 
{
	
}
// Обработка эффектов после их применения
void URPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) 
{
	Super::PostGameplayEffectExecute(Data);

	float DeltaValue = 0; // Инициализация переменной для хранения изменения значения атрибута
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive) // Проверка если модификатор был аддитивным
	{
		DeltaValue = Data.EvaluatedData.Magnitude; // Получение величины изменения
	}

	AActor* TargetActor = nullptr; // Инициализация указателя на целевой актор
	ARPGCharacter* RPGCharacter = nullptr; // Инициализация указателя на персонажа RPG
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid()) // Проверка валидности информации об актере и аватаре
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get(); // Получение целевого актера
		RPGCharacter = Cast<ARPGCharacter>(TargetActor); // Приведение к типу ARPGCharacter
	}

	
	// Если изменился атрибут здоровья
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) 
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth())); // Ограничение значения здоровья между 0 и максимальным здоровьем
		if (RPGCharacter) // Если персонаж RPG валиден
		{
			RPGCharacter->HandleHealthChanged(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Вызов обработчика изменения здоровья персонажа 
		}
	}
	// Если изменился атрибут стамины
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute()) 
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina())); // Ограничение значения стамины между 0 и максимальным стамины
		if (RPGCharacter) // Если персонаж RPG валиден
		{
			RPGCharacter->HandleStaminaChanged(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Вызов обработчика изменения здоровья персонажа 
		}
	}
	// Если изменился атрибут адреналина
	if (Data.EvaluatedData.Attribute == GetAdrenalineAttribute()) 
	{
		SetAdrenaline(FMath::Clamp(GetAdrenaline(), 0.0f, GetMaxAdrenaline())); // Ограничение значения адреналина между 0 и максимальным адреналином
		if (RPGCharacter) // Если персонаж RPG валиден
		{
			RPGCharacter->HandleAdrenalineChanged(DeltaValue, Data.EffectSpec.GetContext().GetInstigator()); // Вызов обработчика изменения здоровья персонажа 
		}
	}
	// Если изменился атрибут очков опыта
	if (Data.EvaluatedData.Attribute == GetExperiencePointsAttribute()) 
	{
		float Difference = GetExperiencePoints() - GetMaxExperiencePoints(); // Вычисление разницы между текущими и максимальными очками опыта
		if (RPGCharacter)  // Если персонаж RPG валиден и достигнут максимум опыта 
		{
			RPGCharacter->HandleExperiencePointsChanged(DeltaValue); // Вызов обработчика изменения очков опыта персонажа
			if (Difference >= 0) // Если достигнут максимум опыта
			{
				RPGCharacter->HandleCharacterLevelUp(); // Вызов обработчика повышения уровня персонажа
			}
		}
	}
}
// Обработка изменений атрибутов перед их применением
void URPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) 
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())// Если изменился атрибут максимального здоровья
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute()); // Корректировка здоровья при изменении максимального здоровья
	}
	if (Attribute == GetMaxStaminaAttribute()) // Если изменился атрибут максимальной стамины
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute()); // Корректировка стамины при изменении максимальной стамины
	}
	if (Attribute == GetMaxAdrenalineAttribute()) // Если изменился атрибут максимального адреналина
	{
		AdjustAttributeForMaxChange(Adrenaline, MaxAdrenaline, NewValue, GetAdrenalineAttribute()); // Корректировка адреналина при изменении максимального адреналина
	}
	if (Attribute == GetMaxExperiencePointsAttribute()) // Если изменился атрибут максимальных очков опыта
	{
		SetExperiencePoints(0.0f); // Сброс очков опыта при изменении максимальных очков опыта
	}


}
// Корректировка зависимых атрибутов при изменении максимального значения
void URPGAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent(); // Получение компонента системы способностей
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue(); // Текущее максимальное значение атрибута

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent) // Если максимальное значение изменилось
	{
		// Вычисление нового значения зависимого атрибута с учетом изменения максимума
		AbilitySystemComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Override, NewMaxValue); // Применение изменения к зависимому атрибуту
	}
	
}
