#include "Game/Skill/J1RangedNormalAttack.h"
#include "Object/J1ObjectManager.h"
#include "Object/J1Creature.h"

UJ1RangedNormalAttack::UJ1RangedNormalAttack()
{
	DestPos = FVector();
	ProjectileInfo = nullptr;
	ProjectileInfo = new Protocol::ObjectInfo();
}

UJ1RangedNormalAttack::~UJ1RangedNormalAttack()
{
	delete(ProjectileInfo);
}

void UJ1RangedNormalAttack::SetInfo(TObjectPtr<AJ1Creature> InOwner, int32 InTemplateId)
{
	Super::SetInfo(InOwner, InTemplateId);
}

void UJ1RangedNormalAttack::DoSkill(const Protocol::S_SKILL& InSkillPkt)
{
	Super::DoSkill(InSkillPkt);

	DestPos.X = InSkillPkt.simple_pos_info().x();
	DestPos.Y = InSkillPkt.simple_pos_info().y();
	DestPos.Z = InSkillPkt.simple_pos_info().z();

	ProjectileInfo->CopyFrom(InSkillPkt.projectile_object_info());
}

void UJ1RangedNormalAttack::OnAttackEvent(int32 InTimeCount)
{
	Super::OnAttackEvent(InTimeCount);

	if (InTimeCount == SkillData->AnimImpactTimeList.Num())
	{
		return;
	}

	// 투사체 생성
	Owner->GetManager(Object)->SpawnObject(*ProjectileInfo, Owner, this, DestPos);
}
