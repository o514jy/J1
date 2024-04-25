#include "pch.h"
#include "Boss.h"

Boss::Boss()
{
	objectInfo->set_monster_type(Protocol::MonsterType::MONSTER_TYPE_BOSS);
}

Boss::~Boss()
{
}
