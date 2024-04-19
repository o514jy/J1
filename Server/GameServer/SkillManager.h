#pragma once

class SkillManager
{
public:
	SkillManager();
	~SkillManager();

public:
	SkillBaseRef GenerateSkillById(int32 id);
};

