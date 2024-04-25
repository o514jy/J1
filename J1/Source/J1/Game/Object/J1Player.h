#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1Creature.h"
#include "InputActionValue.h"
#include "Protocol.pb.h"
#include "J1Player.generated.h"

UCLASS()
class J1_API AJ1Player : public AJ1Creature
{
	GENERATED_BODY()

public:
	AJ1Player();
	virtual ~AJ1Player();

protected:
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

public:
	/** setter & getter **/
	TObjectPtr<UPlayerData> GetPlayerData() { return Cast<UPlayerData>(CreatureData); }

public:
	bool IsMyPlayer();

public:
	virtual void SetInfo(const Protocol::ObjectInfo& InObjectInfo) override;
	
public: 
	/* Network */
	virtual void ProcessMove(const Protocol::PosInfo& Info) override;

	virtual void ProcessSkill(const Protocol::S_SKILL& InSkillPkt) override;
};