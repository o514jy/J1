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
	float AtkRange;
	float Def;
	float MaxWalkSpeed;
	float HpRate;
	float AtkRate;
	float DefRate;
	float MoveSpeedRate;
};

class PlayerData : public CreatureData
{
public:
	int32 SkillAttackId;
	int32 SkillQId;
	int32 SkillWId;
	int32 SkillEId;
	int32 SkillRId;
	int32 SkillDashId;
};

class MonsterData : public CreatureData
{
public:

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
	float CoolTime;
	float SkillDuration;
	vector<float> AnimImpactTimeList;
	vector<int32> EffectIdList;
};

class EffectData
{
public:
	int32 DataId;
	wstring EffectType;
};

class RectangleEffectData : public EffectData
{
public:
	float LeftUpPosY;
	float LeftUpPosX;
	float RightDownPosY;
	float RightDownPosX;
};
