#include "Game/Inventory/Item/J1EquipmentItem.h"
#include "Data/J1Data.h"

UJ1EquipmentItem::UJ1EquipmentItem()
{
	EquipType = Protocol::ItemSubType::ITEM_SUB_TYPE_NONE;
}

UJ1EquipmentItem::~UJ1EquipmentItem()
{
}

void UJ1EquipmentItem::SetInfo(int32 InTemplateId, TObjectPtr<AJ1Player> InOwner, TObjectPtr<UItemData> InItemData)
{
	Super::SetInfo(InTemplateId, InOwner, InItemData);

	if (InItemData->ItemType != TEXT("Equipment"))
		return;

	ItemData = Cast<UEquipmentItemData>(InItemData);
	if (ItemData->ItemSubType == TEXT("Leg"))
	{
		EquipType = Protocol::ItemSubType::ITEM_SUB_TYPE_LEG;
	}
}
