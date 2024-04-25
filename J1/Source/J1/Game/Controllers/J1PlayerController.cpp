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

        // 이동 명령 생성
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalLocation(location);
        MoveRequest.SetAcceptanceRadius(0.f);

        // AI에게 이동 명령을 내림
        MoveTo(MoveRequest);
    }
}

void AJ1PlayerController::ProcessSkill(const Protocol::SkillSlot& slot)
{
	Super::ProcessSkill(slot);

}
