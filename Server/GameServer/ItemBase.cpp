#include "pch.h"
#include "ItemBase.h"
#include "DataManager.h"
#include "EquipmentItem.h"
#include "Player.h"

ItemBase::ItemBase()
{
	_itemInfo = nullptr;
	_itemData = nullptr;
	_itemId = 0;

	_itemInfo = new Protocol::ItemInfo();
}

ItemBase::~ItemBase()
{
}

ItemBaseRef ItemBase::MakeItem(int32 templateId, Protocol::ItemSlotType defaultSlot)
{
	ItemDataRef itemData = GDataManager->GetItemDataById(templateId);
	if (itemData == nullptr)
		return nullptr;

	ItemBaseRef item = nullptr;

	wstring itemType = itemData->ItemType;
	if (itemType == L"Equipment")
	{
		wstring itemSubType = itemData->ItemSubType;
		if (itemSubType == L"Leg")
		{
			item = make_shared<EquipmentItem>();
		}
	}

	if (item != nullptr)
	{
		item->_itemId = ItemInstanceCount++;
		item->_itemData = itemData;
		item->_itemInfo->set_template_id(templateId);
		item->_itemInfo->set_item_slot_type(Protocol::ItemSlotType::ITEM_SLOT_TYPE_NONE);
		item->_itemInfo->set_item_slot_type(defaultSlot);
	}

	return item;
}

Protocol::ItemStatus ItemBase::GetItemStatus(Protocol::ItemSlotType itemSlotType)
{
	if (Protocol::ItemSlotType::ITEM_SLOT_TYPE_NONE < itemSlotType && itemSlotType < Protocol::ItemSlotType::ITEM_SLOT_TYPE_EQUIPMENT_MAX)
		return Protocol::ItemStatus::ITEM_STATUS_EQUIPPED;

	if (itemSlotType == Protocol::ItemStatus::ITEM_STATUS_INVENTORY)
		return Protocol::ItemStatus::ITEM_STATUS_INVENTORY;

	return Protocol::ItemStatus::ITEM_STATUS_NONE;
}

void ItemBase::SendAddPacket(PlayerRef owner)
{
	Protocol::S_ADD_ITEM pkt;
	{
		Protocol::ItemInfo* itemInfo = pkt.mutable_item_info();
		itemInfo->CopyFrom(*_itemInfo);
	}

	if (owner->GetRoomRef() == nullptr || owner->GetRoomRef() == GEmptyRoom)
		return;

	SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
	if (auto session = owner->session.lock())
		session->Send(sendBuffer);
}
