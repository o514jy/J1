#include "J1CreatureController.h"
#include "J1/Game/Object/J1Creature.h"
#include "Navigation/PathFollowingComponent.h"

AJ1CreatureController::AJ1CreatureController()
{
}

AJ1CreatureController::~AJ1CreatureController()
{
}

void AJ1CreatureController::ProcessMove(const Protocol::PosInfo& posInfo)
{
	Super::ProcessMove(posInfo);

	if (GetPawn() != nullptr && GetWorld()->GetNavigationSystem())
	{
		TObjectPtr<AJ1Creature> player = Cast<AJ1Creature>(GetPawn());
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

void AJ1CreatureController::ProcessSkill(const Protocol::SkillSlot& slot)
{
	
}

void AJ1CreatureController::Tick(float DeltaTime)
{
	TObjectPtr<AJ1Creature> owner = Cast<AJ1Creature>(GetPawn());

	if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_IDLE)
	{
		UpdateIdle();
	}
	else if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_RUN)
	{
		UpdateRun();
	}
	else if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_SKILL)
	{
		UpdateSkill();
	}
	else if (owner->GetMoveState() == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		UpdateDead();
	}
}

void AJ1CreatureController::UpdateIdle()
{
}

void AJ1CreatureController::UpdateRun()
{
}

void AJ1CreatureController::UpdateSkill()
{
}

void AJ1CreatureController::UpdateDead()
{
}
