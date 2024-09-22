#include "Event/J1EventManager.h"
#include "UI/J1UIManager.h"
#include "UI/J1PopupWidget.h"

void UJ1EventManager::OnBroadcastEvent(EEventType InType)
{
	switch (InType)
	{
	case EEventType::ToggleInventory:
		OnToggleInventory();
		break;
	default:
		break;
	}
}

void UJ1EventManager::OnToggleInventory()
{
	TObjectPtr<UJ1UIManager> UIManager = GetManager(UI);
	TObjectPtr<UJ1PopupWidget> inven = UIManager->GetPopupWidget(EUIType::Inventory);

	if (inven->GetbIsOpened() == true)
		inven->ClosePopupWidget();
	else
		UIManager->ShowPopupWidget(EUIType::Inventory);
	
}
