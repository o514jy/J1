#include "J1PlayerController.h"
#include "J1/Game/Object/J1Player.h"
#include "Navigation/PathFollowingComponent.h"

void AJ1PlayerController::ProcessMove(const Protocol::PosInfo& posInfo)
{
	Super::ProcessMove(posInfo);

    if (GetPawn() != nullptr && GetWorld()->GetNavigationSystem())
    {
        TObjectPtr<AJ1Player> player = Cast<AJ1Player>(GetPawn());
        player->SetPosInfo(posInfo);

        FVector location;
        location.X = posInfo.dest_x();
        location.Y = posInfo.dest_y();
        location.Z = posInfo.dest_z();

        // �̵� ��� ����
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(location);
        MoveRequest.SetAcceptanceRadius(0.f);

        // AI���� �̵� ����� ����
        MoveTo(MoveRequest);
    }
}

void AJ1PlayerController::ProcessSkill(const Protocol::SkillSlot& slot)
{
	Super::ProcessSkill(slot);

}
