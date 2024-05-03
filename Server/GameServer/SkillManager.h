#pragma once

class SkillManager
{
public:
	SkillManager();
	~SkillManager();

public:
	SkillBaseRef GenerateSkillById(int32 id);

	ProjectileRef GenerateProjectileById(int32 id = 0);

	GimmickBaseRef GenerateGimmickById(int32 id);
};

