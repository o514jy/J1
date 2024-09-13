#pragma once

#include "BuffPolicy.h"

class BuffComponent
{
public:
	BuffComponent();
	~BuffComponent();

public:
	/** static **/
	unordered_map<Protocol::BuffType, BuffPolicy*> _policies;

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

