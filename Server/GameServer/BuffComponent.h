#pragma once

class BuffComponent
{
public:
	BuffComponent();
	~BuffComponent();

public:
	/** initialize **/
	void SetInfo(CreatureRef owner);

public:
	/** process **/
	vector<BuffBaseRef> GenerateBuffs(vector<int32> buffIds, SkillBaseRef skill);

public:
	/** information **/
	CreatureRef _owner;

	vector<BuffBaseRef> ActiveBuffs;
};

