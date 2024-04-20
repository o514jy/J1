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
	/** component **/
	
	/* stat */
	StatComponentRef _statComponent;

	/* skill */
	SkillComponentRef _skillComponent;

protected:
	/** information **/
	CreatureDataRef _creatureData;
};

