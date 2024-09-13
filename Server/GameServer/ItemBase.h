#pragma once

#include "Data.h"

static atomic<uint64> ItemInstanceCount = 1;

class ItemBase
{
public:
	ItemBase();
	~ItemBase();

public:
	/** generate & initialize **/
	static ItemBaseRef MakeItem(int32 templateId, Protocol::ItemSlotType defaultSlot = Protocol::ItemSlotType::ITEM_SLOT_TYPE_INVENTORY);
	
	static Protocol::ItemStatus GetItemStatus(Protocol::ItemSlotType itemSlotType);
	Protocol::ItemStatus GetItemStatus() { return GetItemStatus(_itemInfo->item_slot_type()); }

	bool IsEquipped() { return GetItemStatus() == Protocol::ItemStatus::ITEM_STATUS_EQUIPPED; }
	bool IsInInventory() { return GetItemStatus() == Protocol::ItemStatus::ITEM_STATUS_INVENTORY; }

public:
	/** network **/
	void SendAddPacket(PlayerRef owner);
	
public:
	Protocol::ItemInfo* _itemInfo;
	ItemDataRef _itemData;
	uint64 _itemId;
};

