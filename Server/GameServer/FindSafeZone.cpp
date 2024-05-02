#include "pch.h"
#include "FindSafeZone.h"
#include "Projectile.h"
#include "ObjectManager.h"
#include "SafeZone.h"

FindSafeZone::FindSafeZone()
{
}

FindSafeZone::~FindSafeZone()
{
}

void FindSafeZone::SetInfo(BossRef owner, int32 templateId)
{
	__super::SetInfo(owner, templateId);


}

void FindSafeZone::DoGimmick()
{
	__super::DoGimmick();

	// �ϴ� ���Ʒ� �������� �ΰ� ����
	SafeZoneRef upZone = GenerateSafeZone(Protocol::Direction::DIR_UP);
	SafeZoneRef downZone = GenerateSafeZone(Protocol::Direction::DIR_DOWN);

	// ������ ���ʿ����� ������ ���Ʒ� ���������� ������ ��û�Ѵ�.
}

SafeZoneRef FindSafeZone::GenerateSafeZone(Protocol::Direction dir)
{
	SafeZoneRef safeZone = nullptr;

	safeZone = static_pointer_cast<SafeZone>(GObjectManager->CreateProjectile(
		_gimmickData->DataId,
		static_pointer_cast<Creature>(_owner),
		nullptr,
		static_pointer_cast<GimmickBase>(shared_from_this())
	));

	safeZone->SetDir(dir);
	safeZone->SetInfo(_owner, static_pointer_cast<GimmickBase>(shared_from_this()), _gimmickData->DataId);
	
	_gimmickList.insert(make_pair(dir, safeZone));

	return safeZone;
}

void FindSafeZone::OnEvent(int32 eventCount)
{
	__super::OnEvent(eventCount);


}
