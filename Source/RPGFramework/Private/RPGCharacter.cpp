// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacter.h"
#include "GAS/RPGAbilitySystemComponent.h"

// Sets default values
ARPGCharacter::ARPGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Инициализация компонента системы способностей
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	// Инициализация набора атрибутов
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));
 	// Установка начального уровня персонажа
	CharacterLevel = 1;
	
}
// Получение уровня персонажа
int32 ARPGCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

// Получение текущего здоровья персонажа
float ARPGCharacter::GetHealth() const
{	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения здоровья если набор атрибутов существует
	return AttributeSet->GetHealth();
}

float ARPGCharacter::GetMaxHealth() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения максимального здоровья если набор атрибутов существует
	return AttributeSet->GetMaxHealth();
}

float ARPGCharacter::GetStamina() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения стамины если набор атрибутов существует
	return AttributeSet->GetStamina();
}

float ARPGCharacter::GetMaxStamina() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения максимального стамины если набор атрибутов существует
	return AttributeSet->GetMaxStamina();
}

float ARPGCharacter::GetAdrenaline() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения Адреналина если набор атрибутов существует
	return AttributeSet->GetAdrenaline();
}

float ARPGCharacter::GetMaxAdrenaline() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения максимального Адреналина если набор атрибутов существует
	return AttributeSet->GetMaxAdrenaline();
}

float ARPGCharacter::GetExperiencePoints() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения очков опыта если набор атрибутов существует
	return AttributeSet->GetExperiencePoints();
}

float ARPGCharacter::GetMaxExperiencePoints() const
{
	// Проверка наличия набора атрибутов
	if (!AttributeSet)
	{
		return 0.0f;
	}
	// Возврат текущего значения максимального очков опыта если набор атрибутов существует
	return AttributeSet->GetMaxExperiencePoints();
}
// Активация способностей с заданными тегами
bool ARPGCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent) // Проверка наличия компонента системы способностей
	{
		return false;
	}
	return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation); // Попытка активации способностей с заданными тегами
}

// Активация способности ближнего боя
bool ARPGCharacter::ActivateMeleeAbility(bool AllowRemoteActivation)
{
	if (!AbilitySystemComponent || !MeleeAbilitySpecHandle.IsValid()) // Проверка наличия компонента системы способностей и валидности хэндла способности ближнего боя
	{
		return false;
	}
	return AbilitySystemComponent->TryActivateAbility(MeleeAbilitySpecHandle); // Попытка активации способности ближнего боя
}

void ARPGCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags,
	TArray<UGameplayAbility*>& ActiveAbilities, bool MatchExactTag)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	TArray<FGameplayAbilitySpec*> MatchingAbilities; // Массив для хранения соответствующих способностей
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTags, MatchingAbilities, MatchExactTag); // Получение всех активируемых способностей, соответствующих заданным тегам
	for (FGameplayAbilitySpec* Spec : MatchingAbilities) // Итерация по соответствующим способностям
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances(); // Получение экземпляров способности из спецификации
		for (UGameplayAbility* ActiveAbility : AbilityInstances) // Итерация по экземплярам способности
		{
			ActiveAbilities.Add(ActiveAbility); // Добавление активной способности в выходной массив
		}
	}
}
// Применение игрового эффекта к персонажу
void ARPGCharacter::ApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return;
	}
	// Создание контекста эффекта
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // Установка источника эффекта
	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, CharacterLevel, EffectContext); // Создание спецификации эффекта
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent); // Применение эффекта к цели
	}
	
}
// Проверка возможности применения игрового эффекта к персонажу
bool ARPGCharacter::CanApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect)
{
	if (!AbilitySystemComponent || !GameplayEffect)
	{
		return false;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext(); // Создание контекста эффекта
	EffectContext.AddSourceObject(this); // Установка источника эффекта
	return AbilitySystemComponent->CanApplyAttributeModifiers(GameplayEffect->GetDefaultObject<UGameplayEffect>(), CharacterLevel, EffectContext); // Проверка возможности применения модификаторов атрибутов эффекта
}

// Called when the game starts or when spawned
void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
// Функция для установки тестовых способностей
void ARPGCharacter::SetTestAbilities()
{
	if (!AbilitySystemComponent) // Проверка наличия компонента системы способностей
	{
		return;
	}
	if (GetLocalRole() == ROLE_Authority) // Проверка роли сервера
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities) // Итерация по тестовым способностям
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this)); // Добавление способности в систему способностей
		}
	}
	
}
// Функция для установки способности ближнего боя
void ARPGCharacter::SetMeleeAbility()
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	/* Проверка роли сервера и наличия способности ближнего боя
	 * @param MeleeAbility - Способность ближнего боя для установки
	 */
	MeleeAbilitySpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MeleeAbility, GetCharacterLevel(), INDEX_NONE, this)); // Добавление способности в систему способностей
}

// Called every frame
void ARPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
// Переопределение функции PossessedBy для инициализации системы способностей при получении контроля
void ARPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Проверка наличия компонента системы способностей
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	if (EnableTestAbilities) // Если включены тестовые способности
	{
		SetTestAbilities(); // Установка тестовых способностей
	}
	
	ApplyDefaultAttributesEffects(); // Применение атрибутов по умолчанию
	SetMeleeAbility(); // Установка способности ближнего боя
	
}

// Реализация интерфейса IAbilitySystemInterface
UAbilitySystemComponent* ARPGCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
// Обработчик изменения здоровья персонажа вызывается при изменении атрибута здоровья
void ARPGCharacter::HandleHealthChanged(float DeltaValue, AActor* Causer)
{
	OnHealthChanged(DeltaValue, Causer); // Вызов события изменения здоровья в Blueprints
	if (GetHealth() <= 0.0f) // Проверка если здоровье персонажа
	{
		OnDead(); // Вызов события смерти персонажа в Blueprints
	}
}
// Обработчик изменения стамины персонажа вызывается при изменении атрибута стамины
void ARPGCharacter::HandleStaminaChanged(float DeltaValue, AActor* Causer)
{
	OnStaminaChanged(DeltaValue, Causer); // Вызов события изменения стамины в Blueprints
}
// Обработчик изменения адреналина персонажа вызывается при изменении атрибута адреналина
void ARPGCharacter::HandleAdrenalineChanged(float DeltaValue, AActor* Causer)
{
	OnAdrenalineChanged(DeltaValue, Causer); // Вызов события изменения адреналина в Blueprints
}


// Обработчик изменения очков опыта персонажа вызывается при изменении атрибута очков опыта
void ARPGCharacter::HandleExperiencePointsChanged(float DeltaValue)
{
	OnXPChanged(DeltaValue); // Вызов события изменения очков опыта в Blueprints
}
// Обработчик повышения уровня персонажа вызывается при достижении порога опыта
void ARPGCharacter::HandleCharacterLevelUp()
{
	CharacterLevel += 1; // Увеличение уровня персонажа на 1
	RemoveDefaultAttributesEffects(); // Удаление текущих атрибутов по умолчанию
	ApplyDefaultAttributesEffects(); // Применение атрибутов по умолчанию с новым уровнем
	OnCharacterLeveledUp(); // Вызов события повышения уровня персонажа в Blueprints
}


// Функция для применения атрибутов по умолчанию (здоровье, стамина, адреналин и т.д.)
void ARPGCharacter::ApplyDefaultAttributesEffects()
{
	// Создание контекста эффекта
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // Установка источника эффекта
	// Применение каждого эффекта из массива DefaultAttributeEffect
	for (TSubclassOf<class UGameplayEffect>& DefaultEffect : DefaultAttributeEffect)
	{
		// Создание спецификации эффекта для инициализации атрибутов
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultEffect, CharacterLevel, EffectContext);
		// Применение эффекта к самому себе для инициализации атрибутов
		if (NewHandle.IsValid()) // Проверка валидности спецификации эффекта
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent); // Применение эффекта к цели
		}
	}
}
// Функция для удаления атрибутов по умолчанию (здоровье, стамина, адреналин и т.д.)
void ARPGCharacter::RemoveDefaultAttributesEffects()
{
	FGameplayEffectQuery Query; // Создание запроса для поиска всех активных эффектов
	Query.EffectSource = this; // Установка источника эффекта как текущий объект
	AbilitySystemComponent->RemoveActiveEffects(Query); // Удаление всех активных эффектов
}

