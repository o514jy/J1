#pragma once
#include "Object.h"

class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId) override;

public:
	/** setter & getter **/
	StatComponentRef GetStatComponent() { return _statComponent; }
	SkillComponentRef GetSkillComponent() { return _skillComponent; }

	CreatureDataRef GetCreatureData() { return _creatureData; }

public:
	/** component **/

	/* skill */
	SkillComponentRef _skillComponent;

public:
	/** battle **/
	virtual void OnDamaged(ObjectRef attacker, SkillBaseRef skill) override;

protected:
	/** information **/
	CreatureDataRef _creatureData;
};

