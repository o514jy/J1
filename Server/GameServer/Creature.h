#pragma once
#include "Object.h"

class Creature : public Object
{
public:
	Creature();
	virtual ~Creature();

public:
	/** update tick **/
	virtual void UpdateTick() override;

public:
	/** initialize **/
	virtual void SetInfo(int32 templateId) override;

public:
	/** setter & getter **/
	
	SkillComponentRef GetSkillComponent() { return _skillComponent; }

	CreatureDataRef GetCreatureData() { return _creatureData; }

public:
	/** component **/

	/* skill */
	SkillComponentRef _skillComponent;

public:
	/** battle **/
	virtual void OnDamaged(ObjectRef attacker, BuffBaseRef buff) override;

protected:
	/** information **/
	CreatureDataRef _creatureData;
};

