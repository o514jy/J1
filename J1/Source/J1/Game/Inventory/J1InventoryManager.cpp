#include "Game/Inventory/J1InventoryManager.h"
#include "Object/J1MyPlayer.h"
#include "Item/J1EquipmentItem.h"
#include "Item/J1ItemBase.h"
#include "Data/J1DataManager.h"

void UJ1InventoryManager::SetInfo(TObjectPtr<AJ1MyPlayer> InOwner)
{
	Owner = InOwner;
}

TArray<TObjectPtr<UJ1ItemBase>> UJ1InventoryManager::GetAllInventoryItemArray()
{
	TArray<TObjectPtr<UJ1ItemBase>> array;
	InventoryItems.GenerateValueArray(array);

	return array;
}

void UJ1InventoryManager::Add(const Protocol::ItemInfo& InItemInfo)
{
	TObjectPtr<UItemData>* itemDataPtr = Owner->GetManager(Data)->GameData->ItemData.Find(InItemInfo.template_id());
	if (itemDataPtr == nullptr)
		return;
	TObjectPtr<UItemData> itemData = *itemDataPtr;
	
	TObjectPtr<UJ1ItemBase> item = nullptr;
	//if (itemData->ItemType == L"Equipment")
	//{
	//	item = 
	//}

	//AllItems.Add(InItemInfo.item_id(), )
}
