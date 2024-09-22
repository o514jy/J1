#pragma once

#include "CoreMinimal.h"
#include "J1Core.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "J1InventoryManager.generated.h"

class UJ1ItemBase;
class UJ1EquipmentItem;
class AJ1MyPlayer;

UCLASS()
class J1_API UJ1InventoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/** initialize **/
	void SetInfo(TObjectPtr<AJ1MyPlayer> InOwner);

public:
	/** setter & getter **/
	TArray<TObjectPtr<UJ1ItemBase>> GetAllInventoryItemArray();

public:
	/** process **/
	void Add(const Protocol::ItemInfo& InItemInfo);

public:
	/** helper **/


public:
	/** information **/
	TMap<uint64, TObjectPtr<UJ1ItemBase>> AllItems;
	TMap<Protocol::ItemSlotType, TObjectPtr<UJ1EquipmentItem>> EquippedItems;
	TMap<uint64, TObjectPtr<UJ1ItemBase>> InventoryItems;

	TObjectPtr<AJ1MyPlayer> Owner;
};
