#include "pch.h"
#include "BuffComponent.h"

BuffComponent::BuffComponent()
{
	_owner = nullptr;
}

BuffComponent::~BuffComponent()
{
	_owner = nullptr;
	ActiveBuffs.clear();
}

void BuffComponent::SetInfo(CreatureRef owner)
{
	_owner = owner;
}

vector<BuffBaseRef> BuffComponent::GenerateBuffs(vector<int32> buffIds, SkillBaseRef skill)
{
	vector<BuffBaseRef> generatedBuffs;

	for (int32 buffId : buffIds)
	{

	}

	return generatedBuffs;
}
