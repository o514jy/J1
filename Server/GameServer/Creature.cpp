#include "pch.h"
#include "Creature.h"
#include "DataManager.h"
#include "SkillComponent.h"
#include "StatComponent.h"

Creature::Creature()
{
	//objectInfo->set_object_type(Protocol::ObjectType::OBJECT_TYPE_CREATURE);

	_skillComponent = nullptr;
	_creatureData = nullptr;
}

Creature::~Creature()
{
	_skillComponent = nullptr;
	_creatureData = nullptr;
}

void Creature::SetInfo(int32 templateId)
{
	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	//posInfo->CopyFrom(InObjectInfo.pos_info());

	Protocol::ObjectType objectType = objectInfo->object_type();
	if (objectType == Protocol::OBJECT_TYPE_CREATURE)
	{
		Protocol::CreatureType creatureType = objectInfo->creature_type();
		if (creatureType == Protocol::CREATURE_TYPE_PLAYER)
		{
			_creatureData = GDataManager->GetPlayerDataById(_templateId);
			
			_statComponent = make_shared<StatComponent>();
			_statComponent->SetInfo(static_pointer_cast<Creature>(shared_from_this()), _creatureData, creatureType);
		}
		else if (creatureType == Protocol::CREATURE_TYPE_MONSTER)
		{

		}
	}

	// skill component
	_skillComponent = make_shared<SkillComponent>();
	_skillComponent->SetInfo(static_pointer_cast<Creature>(shared_from_this()), _creatureData);
}

void Creature::OnDamaged(ObjectRef attacker, SkillBaseRef skill)
{
	__super::OnDamaged(attacker, skill);

	CreatureRef creature = static_pointer_cast<Creature>(attacker);
	if (creature == nullptr)
		return;

	StatComponentRef stats = creature->GetStatComponent();
	// todo : calculate final damage
	float finalDamage = max(0, stats->GetAtk() - stats->GetDef());
	// calculate final hp
	float finalHp = max(0, stats->GetHp() - finalDamage);

	if (finalHp <= 0)
	{
		stats->SetHp(finalHp);
		// OnDead(attacker, skill);
	}
}

