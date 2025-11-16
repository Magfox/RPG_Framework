// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCController.h"

#include "RPGCharacter.h"
#include "Perception/AIPerceptionSystem.h"

void ANPCController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	Super::SetGenericTeamId(NewTeamID);
	// Обновление системы восприятия ИИ при изменении команды
	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());
}
// Получение отношения команды к другому актеру
ETeamAttitude::Type ANPCController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other); // Приведение другого актера к типу APawn для получения его команды 
	if (OtherPawn == nullptr) // Если приведение не удалось
	{
		return ETeamAttitude::Neutral; // Возврат нейтрального отношения
	}

	auto CharacterTeamAgent = Cast<IGenericTeamAgentInterface>(&Other); // Приведение другого актера к интерфейсу IGenericTeamAgentInterface для получения его команды
	class IGenericTeamAgentInterface* ControllerTeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetClass());// Приведение контроллера другого актера к интерфейсу IGenericTeamAgentInterface для получения его команды
	if (CharacterTeamAgent == nullptr && ControllerTeamAgent == nullptr) // Если оба приведения не удались
	{
		return ETeamAttitude::Neutral; // Возврат нейтрального отношения
	}

	FGenericTeamId OtherTeamId = FGenericTeamId(); // Инициализация идентификатора команды другого актера
	if (ControllerTeamAgent != nullptr) // Если приведение контроллера другого актера удалось
	{
		OtherTeamId = ControllerTeamAgent->GetGenericTeamId(); // Получение идентификатора команды контроллера другого актера
	}
	else if (CharacterTeamAgent != nullptr) // Если приведение другого актера удалось
	{
		OtherTeamId = CharacterTeamAgent->GetGenericTeamId();// Получение идентификатора команды другого актера для получения его команды
	}
	
	FGenericTeamId MyTeamID = GetGenericTeamId();// Получение идентификатора команды этого контроллера
	if (OtherTeamId == EFraction::Civilians) // Если другой актер является гражданским
	{
		return ETeamAttitude::Neutral; // Возврат нейтрального отношения
	}

	if (OtherTeamId == MyTeamID) // Если другой актер принадлежит той же команде что и этот контроллер
	{
		return ETeamAttitude::Friendly;// Возврат дружелюбного отношения
	}

	return ETeamAttitude::Hostile; // Возврат враждебного отношения
	
	
}
