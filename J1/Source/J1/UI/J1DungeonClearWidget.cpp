#include "UI/J1DungeonClearWidget.h"
#include "Components/Button.h"
#include "J1/Game/Object/J1MyPlayer.h"
#include "J1/Game/Object/J1ObjectManager.h"
#include "J1/Network/J1NetworkManager.h"

UJ1DungeonClearWidget::UJ1DungeonClearWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UJ1DungeonClearWidget::~UJ1DungeonClearWidget()
{
}

void UJ1DungeonClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (QuitButton != nullptr)
		QuitButton->OnClicked.AddDynamic(this, &UJ1DungeonClearWidget::OnClickedQuitButton);
}

void UJ1DungeonClearWidget::OnClickedQuitButton()
{
	// 내보내기
    Protocol::C_TELEPORT telPkt;
    Protocol::ObjectInfo* info = telPkt.mutable_info();
    telPkt.set_start_room_type(Protocol::RoomType::ROOM_TYPE_DUNGEON_ROOM);
    telPkt.set_dest_room_type(Protocol::RoomType::ROOM_TYPE_START_ROOM);
    
    TObjectPtr<AJ1MyPlayer> player = GetManager(Object)->MyPlayer;
    if (player == nullptr)
        return;

    info->CopyFrom(*player->GetObjectInfo());
    GetManager(Network)->SendPacket(telPkt);
}
