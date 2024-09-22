#include "UI/J1PopupPosWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

FVector2D UJ1PopupPosWidget::GetPopupWidgetPos(EUIType InType)
{
	FVector2D pos = FVector2D();

	switch (InType)
	{
	case EUIType::Inventory:
		pos = Cast<UCanvasPanelSlot>(InventoryPosText->Slot)->GetPosition();
		break;
	default:
		break;
	}

	return pos;
}
