// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPGAttributeSet.generated.h"

// Макрос для создания аксессоров атрибутов
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class RPGFRAMEWORK_API URPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	URPGAttributeSet();
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override; // Обработка эффектов после их применения
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override; // Обработка изменений атрибутов перед их применением
	
	// Здоровье
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Health);

	// Максимальное здоровье
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxHealth);

	// Выносливость
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Stamina);

	// Максимальная выносливость
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxStamina);

	// Адреналин
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData Adrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, Adrenaline);

	// Максимальный адреналин
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData MaxAdrenaline;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxAdrenaline);

	// Опыт
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData ExperiencePoints;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, ExperiencePoints);

	// Максимальный опыт
	UPROPERTY(BlueprintReadOnly, Category = "RPG Attributes")
	FGameplayAttributeData MaxExperiencePoints;
	ATTRIBUTE_ACCESSORS(URPGAttributeSet, MaxExperiencePoints);

	protected:
	// Функция для корректировки зависимых атрибутов при изменении максимального значения
	UFUNCTION()
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty); 
	
	
	
};
