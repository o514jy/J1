#include "pch.h"
#include "BuffPolicy.h"
#include "BuffBase.h"
#include "BuffComponent.h"
#include "Creature.h"

void HitBuffPolicy::ApplyBuff(CreatureRef caster, CreatureRef target, BuffDataRef buffData)
{
	if (target == nullptr)
		return;

	float casterDamage = caster->GetCreatureData()->Atk * buffData->BuffAmountRate;
	float finalDamage = max(0, casterDamage - target->GetCreatureData()->Def);

	target->OnDamaged(caster, finalDamage);
}

void HitBuffPolicy::RevertBuff(CreatureRef target, BuffDataRef buffData)
{
}

void DummyBuffPolicy::ApplyBuff(CreatureRef caster, CreatureRef target, BuffDataRef buffData)
{
}

void DummyBuffPolicy::RevertBuff(CreatureRef target, BuffDataRef buffData)
{
}
