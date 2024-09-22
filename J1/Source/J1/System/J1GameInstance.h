#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "J1GameInstance.generated.h"

class AJ1Player;
class AJ1MyPlayer;
class AJ1Monster;
class AJ1Boss;
class AJ1Portal;
class AJ1Projectile;
class UJ1SceneWidget;
class UJ1InventoryWidget;
class UJ1StartRoomSceneWidget;
class UUserWidget;
class UJ1DungeonStatusWidget;
class UJ1DungeonClearWidget;
class UJ1EquipmentSlotsWidget;
class UJ1InventorySlotsWidget;
class UJ1ItemSlotWidget;
class UJ1ItemEntryWidget;
class UJ1ItemDragWidget;
class UJ1DragDropOperation;
class UJ1PopupPosWidget;

class UNiagaraSystem;

UCLASS()
class J1_API UJ1GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UJ1GameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void Init() override;
	virtual void Shutdown() override;

public:
	virtual void BeginPlay();

public:
	/** creature **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Player> OtherPlayerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1MyPlayer> MyPlayerClass;

	/* monster */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Monster> GoblinSpearClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Monster> GoblinSlingshotClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Monster> DemonRedClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Boss> StartBossClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ShieldMaster;

	/** env **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Portal> PortalClass;

	/** projectile **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AJ1Projectile> RockForSlingshotClass;

	/** UI **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1DungeonStatusWidget> DungeonStatusWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1SceneWidget> SceneWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1DungeonClearWidget> DungeonClearWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UJ1SceneWidget> SceneWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UJ1DungeonClearWidget> DungeonClearWidgetInstance;

	/* Scene */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1StartRoomSceneWidget> StartRoomWidgetClass;
	/* Popup */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1InventoryWidget> InventoryWidgetClass;
	/* sub widget */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1EquipmentSlotsWidget> EquipmentSlotsWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1InventorySlotsWidget> InventorySlotsWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1ItemSlotWidget> ItemSlotWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1ItemEntryWidget> ItemEntryWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1ItemDragWidget> ItemDragWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UJ1PopupPosWidget> PopupPosWidgetClass;
};
