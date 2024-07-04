#include "Game/Object/J1Portal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Network/J1NetworkManager.h"
#include "Object/J1MyPlayer.h"

AJ1Portal::AJ1Portal()
{
    // Create and attach the collision component
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetCollisionProfileName("Trigger");
    CollisionComponent->SetupAttachment(RootComponent);

    // Bind the overlap event
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AJ1Portal::OnOverlapBegin);
}

AJ1Portal::~AJ1Portal()
{
}

void AJ1Portal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this))
    {
        // Change the level
        //UGameplayStatics::OpenLevel(this, LevelName_Dungeon_One);

        // Register changing the level
        Protocol::C_TELEPORT telPkt;
        Protocol::ObjectInfo* info = telPkt.mutable_info();
        telPkt.set_start_room_type(Protocol::RoomType::ROOM_TYPE_START_ROOM);
        telPkt.set_dest_room_type(Protocol::RoomType::ROOM_TYPE_DUNGEON_ROOM);

        AJ1MyPlayer* player = Cast<AJ1MyPlayer>(OtherActor);
        if (player == nullptr)
            return;

        info->CopyFrom(*player->GetObjectInfo());
        GetManager(Network)->SendPacket(telPkt);
    }
}
