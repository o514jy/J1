#include "pch.h"
#include "BuffInstant.h"
#include "Object.h"
#include "SkillBase.h"

BuffInstant::BuffInstant()
{
}

BuffInstant::~BuffInstant()
{
}

void BuffInstant::ApplyBuff()
{
	if (_owner == nullptr)
		return;

	_owner->OnDamaged(static_pointer_cast<Object>(_ownerSkill->_owner), static_pointer_cast<BuffBase>(shared_from_this()));
}
