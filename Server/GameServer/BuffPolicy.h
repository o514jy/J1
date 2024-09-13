#pragma once

class BuffPolicy
{
public:
	virtual void ApplyBuff(CreatureRef caster, CreatureRef target, BuffDataRef buffData) = 0;
	virtual void RevertBuff(CreatureRef target, BuffDataRef buffData) = 0;
};

class HitBuffPolicy : public BuffPolicy
{
public:
	virtual void ApplyBuff(CreatureRef caster, CreatureRef target, BuffDataRef buffData);
	virtual void RevertBuff(CreatureRef target, BuffDataRef buffData);
};