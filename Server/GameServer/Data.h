#pragma once

class CreatureData
{
public:
	int32 DataId;
	wstring CreatureType;
	wstring DescriptionTextId;
	float ColliderRadius;
	float ColliderHalfHeight;
	float MaxHp;
	float Atk;
	float Def;
	float MaxWalkSpeed;
	float MoveSpeedRate;
	int32 SkillAttackId;
};

class PlayerData : public CreatureData
{
public:
	int32 SkillQId;
	int32 SkillWId;
	int32 SkillEId;
	int32 SkillRId;
	int32 SkillDashId;
};

class MonsterData : public CreatureData
{
public:
	wstring MonsterType;
	int32 AdvancedSkillId;
	float DefaultAtkRange;
	float SearchMaxDistance;
	float ChaseMaxDistance;
};

class BossData : public MonsterData
{
public:

};

class SkillData
{
public:
	int32 DataId;
	wstring Name;
	wstring Description;
	vector<int32> ProjectileIdList;
	int32 CoolTime;
	int32 SkillDuration;
	vector<int32> AnimImpactTimeList;
	vector<int32> BuffIdList;
	vector<int32> EffectIdList;
};

class EffectData
{
public:
	int32 DataId;
	int32 OwnerSkillDataId;
	wstring EffectType;
};

class RectangleEffectData : public EffectData
{
public:
	float ForwardLength;
	float BackwardLength;
	float LeftLength;
	float RightLength;
};

class PizzaEffectData : public EffectData
{
public:
	float Radius;
	float Theta;
};

class CircleEffectData : public EffectData
{
public:
	float Radius;
};

class ProjectileData
{
public:
	int32 DataId;
	wstring OwnerType;
	int32 OwnerSkillDataId;
	int32 OwnerGimmickDataId;
	wstring Name;
	int32 Duration;
	float MoveSpeed;
	vector<int32> ImpactTimeList;
	vector<int32> BuffIdList;
	vector<int32> EffectIdList;
};

class BuffData
{
public:
	int32 DataId;
	wstring BuffType;
	wstring BuffDurationType;
	float BuffAmountRate;
	float BuffDurationPeriod;
	float BuffDurationMagnitude;
};

class GimmickData
{
public:
	int32 DataId;
	wstring Name;
	wstring DescriptionText;
	int32 Duration;
	int32 CoolTime;
	vector<int32> EventTimeList;
	vector<int32> ProjectileIdList;
	vector<int32> MissionIdList;
	vector<int32> SkillIdList;
};

class ItemData
{
public:
	int32 DataId;
	wstring Name;
	wstring DescriptionText;
	wstring ItemType;
	wstring ItemSubType;
};

class EquipmentData : public ItemData
{
public:
	int32 MoveSpeedBonus;
};