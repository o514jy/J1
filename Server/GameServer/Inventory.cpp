#include "pch.h"
#include "Inventory.h"
#include "ItemBase.h"
#include "EquipmentItem.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::Add(ItemBaseRef item)
{
	_allItems.insert({ item->_itemId, item });

	Protocol::ItemStatus status = item->GetItemStatus();
	switch (status)
	{
	case Protocol::ItemStatus::ITEM_STATUS_EQUIPPED:
		_equippedItems.insert({ item->_itemInfo->item_slot_type(), static_pointer_cast<EquipmentItem>(item) });
		break;
	case Protocol::ItemStatus::ITEM_STATUS_INVENTORY:
		_inventoryItems.insert({ item->_itemId, item });
		break;
	default:
		break;
	}
	
	// send packet
	item->SendAddPacket(_owner);
}

void Inventory::SetInfo(PlayerRef owner)
{
	_owner = owner;
}
