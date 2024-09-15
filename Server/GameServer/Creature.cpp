#include "pch.h"
#include "Creature.h"
#include "DataManager.h"
#include "SkillComponent.h"
#include "StatComponent.h"
#include "BuffBase.h"
#include "BuffComponent.h"

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

void Creature::UpdateTick()
{
	__super::UpdateTick();
}

void Creature::SetInfo(int32 templateId)
{
	__super::SetInfo(templateId);

	_objectId = objectInfo->object_id();
	_templateId = objectInfo->template_id();

	Protocol::ObjectType objectType = objectInfo->object_type();
	_objectType = objectType;
	Protocol::CreatureType creatureType = objectInfo->creature_type();
	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		if (creatureType == Protocol::CreatureType::CREATURE_TYPE_PLAYER)
		{
			_creatureData = GDataManager->GetPlayerDataById(_templateId);
		}
		else if (creatureType == Protocol::CreatureType::CREATURE_TYPE_MONSTER)
		{
			Protocol::MonsterType monsterType = objectInfo->monster_type();
			if (monsterType == Protocol::MonsterType::MONSTER_TYPE_BOSS)
			{
				_creatureData = GDataManager->GetBossDataById(_templateId);
			}
			else if (monsterType == Protocol::MonsterType::MONSTER_TYPE_GENERAL)
			{
				_creatureData = GDataManager->GetMonsterDataById(_templateId);
			}
		}
	}

	_colliderRadius = _creatureData->ColliderRadius;
	_colliderHalfHeight = _creatureData->ColliderHalfHeight;

	/** component **/
	/* stat component */
	_statComponent = make_shared<StatComponent>();
	_statComponent->SetInfo(static_pointer_cast<Creature>(shared_from_this()), _creatureData, creatureType);
	/* skill component */
	_skillComponent = make_shared<SkillComponent>();
	_skillComponent->SetInfo(static_pointer_cast<Creature>(shared_from_this()), _creatureData);
	/* buff component */
	_buffComponent = make_shared<BuffComponent>();
	_buffComponent->SetInfo(static_pointer_cast<Creature>(shared_from_this()));
}

void Creature::SetCreatureData(CreatureDataRef creatureData)
{
	_creatureData = creatureData;
}

void Creature::Clear()
{
	__super::Clear();

	_skillComponent = nullptr;
	_creatureData = nullptr;
}

void Creature::OnDamaged(ObjectRef attacker, BuffBaseRef buff)
{
	__super::OnDamaged(attacker, buff);

	// 이미 죽었으면 데미지 처리x
	if (GetState() == Protocol::MOVE_STATE_DEAD)
		return;

	// todo : calculate final damage
	float finalDamage = max(0, buff->GetFinalAmount() - GetCreatureData()->Def);
	cout << _objectId << " has damaged : " << finalDamage << "\n";
	// calculate final hp
	float finalHp = max(0, GetStatComponent()->GetHp() - finalDamage);
	// process buff type


	GetStatComponent()->SetHp(finalHp); // 0일때 사망처리는 sethp 쪽에서 해준다.
}

void Creature::OnDamaged(ObjectRef attacker, float damage)
{
	__super::OnDamaged(attacker, damage);

	// 이미 죽었으면 데미지 처리x
	if (GetState() == Protocol::MOVE_STATE_DEAD)
		return;

	float finalHp = max(0, GetStatComponent()->GetHp() - damage);

	// 0일때 사망처리는 sethp 쪽에서 해준다.
	GetStatComponent()->SetHp(finalHp);
}

