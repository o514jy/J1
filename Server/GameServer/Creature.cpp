#include "pch.h"
#include "Creature.h"
#include "DataManager.h"
#include "SkillComponent.h"
#include "StatComponent.h"
#include "BuffBase.h"

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

void Creature::OnDamaged(ObjectRef attacker, BuffBaseRef buff)
{
	__super::OnDamaged(attacker, buff);

	// todo : calculate final damage
	float finalDamage = max(0, buff->GetFinalAmount() - GetCreatureData()->Def);
	// calculate final hp
	float finalHp = max(0, GetStatComponent()->GetHp() - finalDamage);

	// process buff type


	GetStatComponent()->SetHp(finalHp); // 0일때 사망처리는 sethp 쪽에서 해준다.
}

