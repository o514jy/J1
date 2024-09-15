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
	virtual void ApplyBuff(CreatureRef caster, CreatureRef target, BuffDataRef buffData) override;
	virtual void RevertBuff(CreatureRef target, BuffDataRef buffData) override;
};

class DummyBuffPolicy : public BuffPolicy
{
	virtual void ApplyBuff(CreatureRef caster, CreatureRef target, BuffDataRef buffData) override;
	virtual void RevertBuff(CreatureRef target, BuffDataRef buffData) override;
};