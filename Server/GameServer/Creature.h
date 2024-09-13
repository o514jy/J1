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

	void SetCreatureData(CreatureDataRef creatureData);
	CreatureDataRef GetCreatureData() { return _creatureData; }

	virtual void Clear() override;

public:
	/** component **/

	/* skill */
	SkillComponentRef _skillComponent;

public:
	/** battle **/
	virtual void OnDamaged(ObjectRef attacker, BuffBaseRef buff) override; // legacy
	virtual void OnDamaged(ObjectRef attacker, float damage) override;

protected:
	/** information **/
	CreatureDataRef _creatureData;
};

