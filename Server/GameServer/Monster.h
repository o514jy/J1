#pragma once
#include "Creature.h"

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

public:
	/** component **/

	/* skill */
	SkillComponentRef _skillComponent;

	/* ai controller */
	MonsterAIControllerRef _aiController;

	ObjectRef _targetObject;
};

