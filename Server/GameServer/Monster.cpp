#include "pch.h"
#include "Monster.h"
#include "MonsterAIController.h"

Monster::Monster()
{
	objectInfo->set_creature_type(Protocol::CreatureType::CREATURE_TYPE_MONSTER);
	objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_GENERAL);
	
	_skillComponent = nullptr;
	_aiController = nullptr;
	_targetObject = nullptr;
}

Monster::~Monster()
{
	_skillComponent = nullptr;
	_aiController = nullptr;
	_targetObject = nullptr;
}

void Monster::UpdateTick()
{
	__super::UpdateTick();

	if (_aiController != nullptr)
		_aiController->UpdateTick();
}

void Monster::SetInfo(int32 templateId)
{
	__super::SetInfo(templateId);

	// skill component
	_aiController = make_shared<MonsterAIController>();
	_aiController->SetInfo(static_pointer_cast<Object>(shared_from_this()));
}

void Monster::SetTargetObject(ObjectRef object)
{
	_targetObject = object;
}
