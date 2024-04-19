#pragma once

#include "Utils/J1Types.h"

#include "Protocol.pb.h"

/** Manager **/
#define GetManager(name)	GetGameInstance()->GetSubsystem<UJ1##name##Manager>()

/** Screen Debug Message **/
#define ScreenDebugMessage(x) if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, x); }