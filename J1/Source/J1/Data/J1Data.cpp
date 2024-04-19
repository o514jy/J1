#include "J1Data.h"

void UJ1GameData::ParseJsonData(const FString& path)
{
    // 파일을 읽기 전용으로 열기
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *path))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *path);
        return;
    }

    // JSON 파서 생성
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    // JSON 파일을 파싱하여 JsonObject에 로드
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file: %s"), *path);
        return;
    }

    const TArray<TSharedPtr<FJsonValue>> players = JsonObject->GetArrayField(TEXT("players"));
    {
        for (int32 i = 0; i < players.Num(); i++)
        {
            TSharedPtr<FJsonObject> player = players[i]->AsObject();

            UPlayerData* data = NewObject<UPlayerData>();
            /** creature **/
            data->DataId = player->GetIntegerField(TEXT("DataId"));
            data->CreatureType = player->GetStringField(TEXT("CreatureType"));
            data->DescriptionTextId = player->GetStringField(TEXT("DescriptionTextId"));
            data->ColliderRadius = player->GetNumberField(TEXT("ColliderRadius"));
            data->ColliderHalfHeight = player->GetNumberField(TEXT("ColliderHalfHeight"));
            data->MaxHp = player->GetNumberField(TEXT("MaxHp"));
            data->Atk = player->GetNumberField(TEXT("Atk"));
            data->MaxWalkSpeed = player->GetNumberField(TEXT("MaxWalkSpeed"));
            data->HpRate = player->GetNumberField(TEXT("HpRate"));
            data->AtkRate = player->GetNumberField(TEXT("AtkRate"));
            data->DefRate = player->GetNumberField(TEXT("DefRate"));
            data->MoveSpeedRate = player->GetNumberField(TEXT("MoveSpeedRate"));
            /** player **/
            data->SkillAttackId = player->GetIntegerField(TEXT("SkillAttackId"));
            data->SkillQId = player->GetIntegerField(TEXT("SkillQId"));
            data->SkillWId = player->GetIntegerField(TEXT("SkillWId"));
            data->SkillEId = player->GetIntegerField(TEXT("SkillEId"));
            data->SkillRId = player->GetIntegerField(TEXT("SkillRId"));
            data->SkillDashId = player->GetIntegerField(TEXT("SkillDashId"));

            PlayerData.Add(data->DataId, data);
        }
    }

    const TArray<TSharedPtr<FJsonValue>> skills = JsonObject->GetArrayField(TEXT("skills"));
    {
        for (int32 i = 0; i < skills.Num(); i++)
        {
            TSharedPtr<FJsonObject> skill = skills[i]->AsObject();

            USkillData* data = NewObject<USkillData>();
            /** skill **/
            data->DataId = skill->GetIntegerField(TEXT("DataId"));
            data->Name = skill->GetStringField(TEXT("Name"));
            data->Description = skill->GetStringField(TEXT("Description"));
            TArray<TSharedPtr<FJsonValue>> projIdList = skill->GetArrayField(TEXT("ProjectileIdList"));
            for (auto& projId : projIdList)
            {
                int32 id;
                if (projId->TryGetNumber(id))
                    data->ProjectileIdList.Add(id);
            }
            data->CoolTime = skill->GetNumberField(TEXT("CoolTime"));
            data->SkillDuration = skill->GetNumberField(TEXT("SkillDuration"));
            TArray<TSharedPtr<FJsonValue>> timeList = skill->GetArrayField(TEXT("AnimImpactTimeList"));
            for (auto& time : timeList)
            {
                int32 id;
                if (time->TryGetNumber(id))
                    data->AnimImpactTimeList.Add(id);
            }
            TArray<TSharedPtr<FJsonValue>> EffectIds = skill->GetArrayField(TEXT("EffectIdList"));
            for (auto& effectId : EffectIds)
            {
                int32 id;
                if (effectId->TryGetNumber(id))
                    data->EffectIdList.Add(id);
            }

            SkillData.Add(data->DataId, data);
        }
    }

    const TArray<TSharedPtr<FJsonValue>> effects = JsonObject->GetArrayField(TEXT("effects"));
    {
        for (int32 i = 0; i < effects.Num(); i++)
        {
            TSharedPtr<FJsonObject> effect = effects[i]->AsObject();

            FString effectType = effect->GetStringField(TEXT("EffectType"));

            if (effectType == TEXT("Rectangle"))
            {
                URectangleEffectData* data = NewObject<URectangleEffectData>();

                data->DataId = effect->GetIntegerField(TEXT("DataId"));
                data->EffectType = effectType;
                data->LeftUpPosY = effect->GetNumberField(TEXT("LeftUpPosY"));
                data->LeftUpPosX = effect->GetNumberField(TEXT("LeftUpPosX"));
                data->RightDownPosY = effect->GetNumberField(TEXT("RightDownPosY"));
                data->RightDownPosX = effect->GetNumberField(TEXT("RightDownPosX"));

                EffectData.Add(data->DataId, data);
            }
        }
    }
}
