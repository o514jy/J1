#include "pch.h"
#include "BuffComponent.h"

unordered_map<Protocol::BuffType, BuffPolicy*> _policies =
{
	{Protocol::BuffType::BUFF_TYPE_HIT, new HitBuffPolicy()}
};

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
