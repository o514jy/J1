#pragma once
#include "Creature.h"

class SpawningPool;

class Monster : public Creature
{
public:
	Monster();
	virtual ~Monster();

public:
	/** update tick **/
	virtual void UpdateTick() override;

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId) override;

public:
	/** setter & getter **/
	void SetTargetObject(ObjectRef object); 

	virtual void SetState(Protocol::MoveState moveState) override;
	void SetOwnerSpawningPool(SpawningPoolRef ownerPool);

	MonsterDataRef GetMonsterData() { return static_pointer_cast<MonsterData>(_creatureData); }

	virtual void Clear() override;

public:
	/** component **/

	/* skill */
	//SkillComponentRef _skillComponent;

	/* ai controller */
	MonsterAIControllerRef _aiController;

	ObjectRef _targetObject;

public:
	/** information **/
	SpawningPoolRef _ownerSpawningPool;
};

