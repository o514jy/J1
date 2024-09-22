#include "UI/Inventory/J1InventoryWidget.h"
#include "J1EquipmentSlotsWidget.h"
#include "J1InventorySlotsWidget.h"
#include "Components/Button.h"

#include "Components/CanvasPanelSlot.h"

void UJ1InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (InventoryNameButton)
    {
        InventoryNameButton->OnPressed.AddDynamic(this, &UJ1InventoryWidget::OnInventoryNameButtonPressed);
        InventoryNameButton->OnReleased.AddDynamic(this, &UJ1InventoryWidget::OnInventoryNameButtonReleased);
    }

}

void UJ1InventoryWidget::OnInventoryNameButtonPressed()
{
    StartUpdate();
}

void UJ1InventoryWidget::OnInventoryNameButtonReleased()
{
    StopUpdate();
}

void UJ1InventoryWidget::UpdateLocation()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController && MainCanvasPanel)
    {
        // 뷰포트 상의 마우스 위치를 가져오기
        float MouseX, MouseY;
        if (PlayerController->GetMousePosition(MouseX, MouseY))
        {
            // 마우스 위치로 캔버스 슬롯의 위치 설정
            FVector2D NewPosition(MouseX, MouseY);
            MainCanvasPanel->SetPosition(NewPosition);
        }
    }
}

void UJ1InventoryWidget::StartUpdate()
{
    GetWorld()->GetTimerManager().SetTimer(UpdateLocationTimerHandle, this, &UJ1InventoryWidget::UpdateLocation, 0.01f, true);
}

void UJ1InventoryWidget::StopUpdate()
{
    GetWorld()->GetTimerManager().ClearTimer(UpdateLocationTimerHandle);
}
