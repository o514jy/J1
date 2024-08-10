#pragma once

#include "Utils/J1Types.h"

#include "Protocol.pb.h"

/** Manager **/
#define GetManager(name)	GetGameInstance()->GetSubsystem<UJ1##name##Manager>()

/** Screen Debug Message **/
#define ScreenDebugMessageString(x) if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, x); }
#define ScreenDebugMessageNotString(x) ScreenDebugMessageString(FString::Printf(TEXT("%f"), x));

#define LevelName_Dungeon_One TEXT("DungeonMap_One")
#define LevelName_Start_Room TEXT("DevMap")