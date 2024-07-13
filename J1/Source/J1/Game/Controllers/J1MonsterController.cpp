#include "Game/Controllers/J1MonsterController.h"
#include "Game/Object/J1Monster.h"
#include "Game/Object/J1Player.h"
#include "Navigation/PathFollowingComponent.h"

void AJ1MonsterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJ1MonsterController::UpdateIdle()
{
	Super::UpdateIdle();

	if (GetOwner() == nullptr)
		return;

	if (GetOwner()->GetTargetPlayer() == nullptr)
		return;

	TObjectPtr<AJ1Player> targetPlayer = GetOwner()->GetTargetPlayer();
	if (targetPlayer != nullptr)
		GetOwner()->SetMoveState(Protocol::MoveState::MOVE_STATE_RUN);
}

void AJ1MonsterController::UpdateRun()
{
	Super::UpdateRun();

	if (GetOwner() == nullptr)
		return;

	if (GetOwner()->GetTargetPlayer() == nullptr)
		return;

	// Move to Target
	if (GetWorld()->GetNavigationSystem() == nullptr)
		return;

	TObjectPtr<AJ1Player> player = GetOwner()->GetTargetPlayer();

	//FVector location;
	//location.X = player->GetObjectInfo()->pos_info().x();
	//location.Y = player->GetObjectInfo()->pos_info().y();
	//location.Z = player->GetObjectInfo()->pos_info().z();

	// 이동 명령 생성
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(player);
	float OwnerAttackDist = GetOwner()->GetMonsterData()->DefaultAtkRange;
	// OwnerAttackDist + player->GetCreatureData()->ColliderRadius
	MoveRequest.SetAcceptanceRadius(OwnerAttackDist);


	// AI에게 이동 명령을 내림
	MoveTo(MoveRequest);
	
}

void AJ1MonsterController::UpdateSkill()
{
	Super::UpdateSkill();
}

void AJ1MonsterController::UpdateDead()
{
	Super::UpdateDead();
}
