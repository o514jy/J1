#include "Game/Inventory/Item/J1ItemBase.h"
#include "Data/J1Data.h"
#include "Data/J1DataManager.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "Game/Inventory/J1InventoryManager.h"
#include "Game/Object/J1Player.h"

UJ1ItemBase::UJ1ItemBase()
{
	
}

UJ1ItemBase::~UJ1ItemBase()
{
}

TObjectPtr<UJ1ItemBase> UJ1ItemBase::MakeItem(const Protocol::ItemInfo& InItemInfo, TObjectPtr<AJ1Player> InOwner)
{
	if (InOwner == nullptr)
		return nullptr;

	auto data = InOwner->GetManager(Data)->GameData->ItemData.Find(InItemInfo.template_id());
	if (data == nullptr)
		return nullptr;
	// todo : item ¸¸µé±â
	
	return nullptr;
}

void UJ1ItemBase::SetInfo(int32 InTemplateId, TObjectPtr<AJ1Player> InOwner, TObjectPtr<UItemData> InItemData)
{
	TemplateId = InTemplateId;
	Owner = InOwner;
}
