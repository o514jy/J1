#include "J1Boss.h"
#include "J1/Game/Gimmick/J1GimmickComponent.h"

void AJ1Boss::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	Super::SetInfo(InObjectInfo);

	/** add components **/
	/* gimmick component */
	GimmickComponent = NewObject<UJ1GimmickComponent>(this, UJ1GimmickComponent::StaticClass(), TEXT("GimmickComponent"));
	GimmickComponent->SetInfo(this);
}

void AJ1Boss::ProcessGimmick(const Protocol::S_GIMMICK& GimmickPkt)
{
	GimmickComponent->DoGimmick(GimmickPkt);
}
