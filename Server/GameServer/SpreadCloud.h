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

	// ��͸���Ʈ�� ����� �����´�� �о��ְ� ��Ƴ��� ����� ���� �޾Ƽ� ���߿� �������� ���ܽ�Ű��
	// ��� �� ������ projectile ���� ���� ���
	unordered_set<int32> safePlayers;
};
