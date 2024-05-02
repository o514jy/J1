#include "pch.h"
#include "Boss.h"
#include "GimmickComponent.h"

Boss::Boss()
{
	//objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_BOSS);

	_gimmickComponent = nullptr;
}

Boss::~Boss()
{
}

void Boss::UpdateTick()
{
	__super::UpdateTick();
}

void Boss::SetInfo(int32 templateId)
{
	__super::SetInfo(templateId);

	/* component */
	_gimmickComponent = make_shared<GimmickComponent>();
	_gimmickComponent->SetInfo(static_pointer_cast<Boss>(shared_from_this()));
}

GimmickComponentRef Boss::GetGimmickComponent()
{
	return _gimmickComponent;
}
