#include "J1Creature.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "J1/J1GameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "J1/UI/J1HpBarWidget.h"
#include "J1/Game/Object/J1ObjectManager.h"
#include "J1/Data/J1AnimData.h"
#include "J1/Data/J1Data.h"
#include "J1/Data/J1NiagaraData.h"
#include "J1/Data/J1DataManager.h"
#include "J1/System/J1AssetManager.h"
#include "J1/System/J1GameInstance.h"
#include "J1/Game/Controllers/J1CreatureController.h"
#include "J1/Game/Stat/J1StatComponent.h"
#include "J1/Game/Skill/J1SkillComponent.h"
#include "J1/Game/Skill/J1SkillBase.h"
#include "J1/Game/Object/J1MyPlayer.h"


// Sets default values
AJ1Creature::AJ1Creature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ObjectInfo = new Protocol::ObjectInfo();
	PosInfo = new Protocol::PosInfo();
	
	
	/** Configure character movement **/
	{
		GetCharacterMovement()->bRequestedMoveUseAcceleration = false;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
		GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
		GetCharacterMovement()->bConstrainToPlane = true;
		GetCharacterMovement()->bSnapToPlaneAtStart = true;
		GetCharacterMovement()->bRunPhysicsWithNoController = true;
	}
	
	/** Ai Controller **/
	{
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
		AIControllerClass = AJ1CreatureController::StaticClass();
	}

	/** Add Indicator **/
	{
		//auto a = UJ1AssetManager::GetAssetByName<>("IndicatorComponent");
	}

	// Hp Bar
	{
		HpBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
		HpBarComponent->SetupAttachment(GetRootComponent());
	}

	// temp
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
}

AJ1Creature::~AJ1Creature()
{
	delete ObjectInfo;

	CreatureData = nullptr;
}

void AJ1Creature::BeginPlay()
{
	Super::BeginPlay();
	
	SetNowPosInfo();

	{
		FVector Location = GetActorLocation();
		PosInfo->set_dest_x(Location.X);
		PosInfo->set_dest_y(Location.Y);
		PosInfo->set_dest_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);

		SetMoveState(Protocol::MOVE_STATE_IDLE);
	}
}

void AJ1Creature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetNowPosInfo();
}

void AJ1Creature::SetMoveState(Protocol::MoveState State)
{
	if (PosInfo->state() == State)
		return;

	PosInfo->set_state(State);

	// idle일 경우 멈춰주기
	if (State == Protocol::MoveState::MOVE_STATE_IDLE)
	{
		GetController()->StopMovement();
	}
	else if (State == Protocol::MoveState::MOVE_STATE_DEAD)
	{
		GetController()->StopMovement();
		OnDead();
	}
}

void AJ1Creature::SetPosInfo(const Protocol::PosInfo& Info, bool Forced)
{
	if (PosInfo->object_id() != 0)
	{
		assert(PosInfo->object_id() == Info.object_id());
	}

	PosInfo->CopyFrom(Info);

	if (Forced == true)
	{
		FVector Location(Info.x(), Info.y(), Info.z());
		SetActorLocation(Location);
	}

	SetMoveState(Info.state());
}

void AJ1Creature::SetObjectInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	if (InObjectInfo.object_id() == 0)
		return;

	ObjectInfo->CopyFrom(InObjectInfo);
}

void AJ1Creature::SetNowPosInfo(FVector InLocation)
{
	// 본인의 위치를 업데이트해야 하는 경우
	if (InLocation == FVector())
	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);
	}
	else
	{
		FVector Location = GetActorLocation();
		PosInfo->set_x(Location.X);
		PosInfo->set_y(Location.Y);
		PosInfo->set_z(Location.Z);
		PosInfo->set_yaw(GetControlRotation().Yaw);
	}
}

void AJ1Creature::SetDestPosInfo(FVector InLocation)
{
	PosInfo->set_dest_x(InLocation.X);
	PosInfo->set_dest_y(InLocation.Y);
	PosInfo->set_dest_z(InLocation.Z);
}

void AJ1Creature::OnDamaged(TObjectPtr<AActor> InAttacker, TObjectPtr<UJ1SkillBase> InSkill)
{
	if (InAttacker == nullptr)
		return;

	TObjectPtr<AJ1Creature> creature = Cast<AJ1Creature>(InAttacker);
	if (creature == nullptr)
		return;


}

void AJ1Creature::HandleGameplayEvent(FGameplayTag EventTag)
{
	if (EventTag.MatchesTag(TemplateTag) == true)
	{
		if (EventTag.MatchesTag(SkillComponent->SkillComponentTag) == true)
		{
			SkillComponent->HandleGameplayEvent(EventTag);
		}
	}
}

void AJ1Creature::destroyProjectile()
{
	if (UpZone != nullptr)
	{
		UpZone->Destroy();
		UpZone = nullptr;
	}

	if (LeftZone != nullptr)
	{
		LeftZone->Destroy();
		LeftZone = nullptr;
	}

	if (DownZone != nullptr)
	{
		DownZone->Destroy();
		DownZone = nullptr;
	}

	if (RightZone != nullptr)
	{
		RightZone->Destroy();
		RightZone = nullptr;
	}
}

void AJ1Creature::ProcessMove(const Protocol::PosInfo& Info)
{
	Cast<AJ1CreatureController>(Controller)->ProcessMove(Info);
}

void AJ1Creature::ProcessSkill(const Protocol::S_SKILL& InSkillPkt)
{
	SkillComponent->DoSkill(InSkillPkt);
}

void AJ1Creature::ProcessNotifyPos(const Protocol::PosInfo& Info)
{
	Cast<AJ1CreatureController>(Controller)->ProcessNotifyPos(Info);
}

void AJ1Creature::ProcessProjectile(const Protocol::ProjectileInfo& Info)
{
	// 일단 여기선 aoe 켜주기
	// NS_FindSafeZone_SafeZone

	//UJ1NiagaraData* NiagaraData = UJ1AssetManager::GetAssetByName<UJ1NiagaraData>("NiagaraData");
	//FJ1NiagaraEntry& Entry = NiagaraData->NameToNiagaraEntry[TEXT("NS_FindSafeZone_SafeZone")];
	//TObjectPtr<UNiagaraSystem> niagara = Entry.Niagara;
	//
	//FVector loc;
	//loc.X = Info.spawn_simple_pos_info().x();
	//loc.Y = Info.spawn_simple_pos_info().y();
	//loc.Z = Info.spawn_simple_pos_info().z();
	//TObjectPtr<UNiagaraComponent> NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, loc);

	//TSubclassOf<AActor> shield = Cast<UJ1GameInstance>(GetGameInstance())->ShieldMaster;
	//FVector loc;
	//loc.X = Info.spawn_simple_pos_info().x();
	//loc.Y = Info.spawn_simple_pos_info().y();
	////loc.Z = Info.spawn_simple_pos_info().z();
	//loc.Z = 0;
	//if (Info.safe_zone_dir() == Protocol::DIR_UP)
	//	UpZone = GetWorld()->SpawnActor<AActor>(shield, loc, FRotator());
	//else if (Info.safe_zone_dir() == Protocol::DIR_LEFT)
	//	LeftZone = GetWorld()->SpawnActor<AActor>(shield, loc, FRotator());
	//else if (Info.safe_zone_dir() == Protocol::DIR_RIGHT)
	//	RightZone = GetWorld()->SpawnActor<AActor>(shield, loc, FRotator());
	//else if (Info.safe_zone_dir() == Protocol::DIR_DOWN)
	//	DownZone = GetWorld()->SpawnActor<AActor>(shield, loc, FRotator());
	//
	//FTimerHandle TimerHandle;
	//float Delay = 10.0f; // 삭제까지의 시간 (초)
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AJ1Creature::destroyProjectile, Delay, false);
}

void AJ1Creature::SetInfo(const Protocol::ObjectInfo& InObjectInfo)
{
	if (InObjectInfo.object_id() == 0)
		return;

	SetObjectInfo(InObjectInfo);

	ObjectInfo->set_allocated_pos_info(PosInfo);

	ObjectId = ObjectInfo->object_id();
	TemplateId = ObjectInfo->template_id();

	PosInfo->set_object_id(ObjectId);
	SetPosInfo(InObjectInfo.pos_info(), true);

	Protocol::ObjectType objectType = ObjectInfo->object_type();
	Protocol::CreatureType creatureType = ObjectInfo->creature_type();
	if (objectType == Protocol::ObjectType::OBJECT_TYPE_CREATURE)
	{
		if (creatureType == Protocol::CreatureType::CREATURE_TYPE_PLAYER)
		{
			CreatureData = GetManager(Data)->GameData->PlayerData[TemplateId];
		}
		else if (creatureType == Protocol::CreatureType::CREATURE_TYPE_MONSTER)
		{
			Protocol::MonsterType monsterType = ObjectInfo->monster_type();
			if (monsterType == Protocol::MonsterType::MONSTER_TYPE_BOSS)
			{
				CreatureData = GetManager(Data)->GameData->BossData[TemplateId];
			}
			else if (monsterType == Protocol::MonsterType::MONSTER_TYPE_GENERAL)
			{
				CreatureData = GetManager(Data)->GameData->MonsterData[TemplateId];
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = CreatureData->MaxWalkSpeed;

		TemplateTag = GetManager(Data)->SetTemplateTagByDataId(TemplateId);
	}

	/** add components **/
	/* widget component */
	TSubclassOf<UUserWidget> hpwidget = UJ1AssetManager::GetClassByName<UUserWidget>("HpBar");
	//TSubclassOf<UUserWidget> hpwidget = Cast<UUserWidget>(object);
	if (hpwidget != nullptr)
	{
		HpBarComponent->SetWidgetClass(hpwidget);
		HpBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComponent->SetDrawAtDesiredSize(true);
		HpBarComponent->SetRelativeLocation(FVector(0, 0, CreatureData->ColliderHalfHeight * 1.2f));
	}
	/* stat component */
	StatComponent = NewObject<UJ1StatComponent>(this, UJ1StatComponent::StaticClass(), TEXT("StatComponent"));
	StatComponent->SetInfo(this, CreatureData, creatureType);
	/* skill component */
	SkillComponent = NewObject<UJ1SkillComponent>(this, UJ1SkillComponent::StaticClass(), TEXT("SkillComponent"));
	SkillComponent->SetInfo(this, CreatureData);
}

void AJ1Creature::OnDead()
{
	int32 OwnerDataId = GetTemplateId();
	UJ1AnimData* InputData = UJ1AssetManager::GetAssetByName<UJ1AnimData>("AnimData");
	FJ1AnimSet& AnimSet = InputData->OwnerIdToAnimSet[OwnerDataId];
	FJ1AnimEntry& AnimEntry = AnimSet.SkillIdToAnimEntry[-1];
	UAnimMontage* Montage = AnimEntry.Montage;
	
	PlayAnimMontage(Montage);

	// Set a timer to call destroy function after 2 seconds
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AJ1Creature::DelayDestroyed, 2.0f, false);
}

void AJ1Creature::DelayDestroyed()
{
	GetManager(Object)->DespawnObject(ObjectId);
}

bool AJ1Creature::IsMyPlayer()
{
	return Cast<AJ1MyPlayer>(this) != nullptr;
}
