// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GAS/RPGAttributeSet.h"
#include "GameFramework/Character.h"
#include "RPGCharacter.generated.h"

UCLASS()
class RPGFRAMEWORK_API ARPGCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacter();

	// Получение уровня персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual int32 GetCharacterLevel() const;
	// Получение текущего здоровья персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetHealth() const;
	// Получение максимального здоровья персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxHealth() const;
	// Получение текущей выносливости персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetStamina() const;
	// Получение максимальной выносливости персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxStamina() const;
	// Получение текущего адреналина персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetAdrenaline() const;
	// Получение максимального адреналина персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxAdrenaline() const;
	// Получение текущих очков опыта персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetExperiencePoints() const;
	// Получение максимальных очков опыта персонажа
	UFUNCTION(BlueprintCallable, Category = "RPG Attributes")
	virtual float GetMaxExperiencePoints() const;


	
	// Активация способностей с заданными тегами
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	bool ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation = true);

	
	/* Вызов этого события позволяет реализовать логику в Blueprints, которая реагирует на изменения здоровья персонажа.
	 * @param DeltaValue - Изменение значения здоровья (положительное или отрицательное).
	 * @param Causer - Актор, который вызвал изменение здоровья (например, источник урона или лечение).
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaValue, AActor* Causer);
	// Вызывается когда изменяется выносливость персонажа
	UFUNCTION(BlueprintImplementableEvent)
	void OnStaminaChanged(float DeltaValue, AActor* Causer);
	// Вызывается когда изменяется адреналин персонажа
	UFUNCTION(BlueprintImplementableEvent)
	void OnAdrenalineChanged(float DeltaValue, AActor* Causer);
	// Вызывается когда изменяются очки опыта персонажа
	UFUNCTION(BlueprintImplementableEvent)
	void OnXPChanged(float DeltaValue);
	// Вызывается когда персонаж повышает уровень
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterLeveledUp();

	
	// Вызывается когда персонаж умирает 
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();

	// Активация способности ближнего боя 
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities|Melee")
	bool ActivateMeleeAbility(bool AllowRemoteActivation = true);
	// Получение активных способностей с заданными тегами
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag);
	// Применение игрового эффекта к персонажу
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual  void ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);
	// Проверка возможности применения игрового эффекта к персонажу
	UFUNCTION(BlueprintCallable, Category = "RPG Abilities")
	virtual bool CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Компонент системы способностей
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;
	// Набор атрибутов персонажа
	UPROPERTY()
	class URPGAttributeSet* AttributeSet;
	// Уровень персонажа
	UPROPERTY(EditAnywhere, Category = "RPG Attributes")
	int32 CharacterLevel;
	// Атрибуты эффектов по умолчанию (здоровье, стамина, адреналин и т.д.)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RPG Attributes")
	TArray<TSubclassOf<class UGameplayEffect>> DefaultAttributeEffect;



	
	
	// Флаг для включения тестовых способностей
	UPROPERTY(EditAnywhere, Category = "RPG Attributes|Debug")
	bool EnableTestAbilities;
	// Тестовые способности для отладки
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RPG Attributes|Debug")
	TArray<TSubclassOf<class UGameplayAbility>> TestAbilities;
	// Функция для установки тестовых способностей
	virtual void SetTestAbilities();


	
	// Способность ближнего боя выбирается по классу в редакторе 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RPG Abilities|Melee")
	TSubclassOf<class UGameplayAbility> MeleeAbility; 
	// Хэндл спецификации способности ближнего боя
	UPROPERTY()
	FGameplayAbilitySpecHandle MeleeAbilitySpecHandle;
	// Функция для установки способности ближнего боя
	virtual void SetMeleeAbility();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Вызов для привязки функционала к вводу
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Переопределение функции PossessedBy для инициализации системы способностей при получении контроля
	virtual void PossessedBy(AController* NewController) override;
	
	// Реализация интерфейса IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	
	// Обработчик изменения здоровья персонажа вызывается при изменении атрибута здоровья
	virtual void HandleHealthChanged(float DeltaValue, AActor* Causer);
	// Обработчик изменения выносливости персонажа вызывается при изменении атрибута выносливости
	virtual void HandleStaminaChanged(float DeltaValue, AActor* Causer);
	// Обработчик изменения адреналина персонажа вызывается при изменении атрибута адреналина
	virtual void HandleAdrenalineChanged(float DeltaValue, AActor* Causer);
	
	// Обработчик изменения очков опыта персонажа вызывается при изменении атрибута очков опыта
	virtual void HandleExperiencePointsChanged(float DeltaValue);
	// Обработчик повышения уровня персонажа вызывается при достижении порога опыта
	virtual void HandleCharacterLevelUp();


	
	// Функция для применения атрибутов по умолчанию (здоровье, стамина, адреналин и т.д.)
	virtual void ApplyDefaultAttributesEffects();
	// Функция для удаления атрибутов по умолчанию (здоровье, стамина, адреналин и т.д.)
	virtual void RemoveDefaultAttributesEffects();

	
	
};
