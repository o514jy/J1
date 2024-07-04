#pragma once

#include "SkillBase.h"

class FindSafeZone;

class SpreadCloud : public SkillBase
{
public:
	SpreadCloud();
	~SpreadCloud();

public:
	/** initialize **/
	virtual void SetInfo(CreatureRef owner, int32 templateId) override;

public:
	/** AnimEvent **/
	virtual void OnAnimCompleteHandler() override;
	virtual void OnAttackEvent(int32 timeCount) override;


public:
	/** Process Skill **/
	virtual void DoSkill(const Protocol::C_SKILL& skillPkt, Protocol::S_SKILL& skillPktToSend) override;

public:
	/** information **/
	weak_ptr<FindSafeZone> ownerGimmick;

	int32 _ownerGimmickDataId;

	// 기믹리스트는 기믹이 끝나는대로 밀어주고 살아남을 사람만 따로 받아서 나중에 판정에서 제외시키기
	// 기믹 딱 끝날때 projectile 해제 순간 기록
	unordered_set<uint64> safePlayers;
};

