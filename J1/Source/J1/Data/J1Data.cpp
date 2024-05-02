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
            data->Def = player->GetNumberField(TEXT("Def"));
            data->MaxWalkSpeed = player->GetNumberField(TEXT("MaxWalkSpeed"));
            data->MoveSpeedRate = player->GetNumberField(TEXT("MoveSpeedRate"));
            data->SkillAttackId = player->GetIntegerField(TEXT("SkillAttackId"));
            /** player **/
            data->SkillQId = player->GetIntegerField(TEXT("SkillQId"));
            data->SkillWId = player->GetIntegerField(TEXT("SkillWId"));
            data->SkillEId = player->GetIntegerField(TEXT("SkillEId"));
            data->SkillRId = player->GetIntegerField(TEXT("SkillRId"));
            data->SkillDashId = player->GetIntegerField(TEXT("SkillDashId"));

            PlayerData.Add(data->DataId, data);
        }
    }

    const TArray<TSharedPtr<FJsonValue>> bosses = JsonObject->GetArrayField(TEXT("bosses"));
    {
        for (int32 i = 0; i < bosses.Num(); i++)
        {
            TSharedPtr<FJsonObject> boss = bosses[i]->AsObject();

            UBossData* data = NewObject<UBossData>();
            /** creature **/
            data->DataId = boss->GetIntegerField(TEXT("DataId"));
            data->CreatureType = boss->GetStringField(TEXT("CreatureType"));
            data->DescriptionTextId = boss->GetStringField(TEXT("DescriptionTextId"));
            data->ColliderRadius = boss->GetNumberField(TEXT("ColliderRadius"));
            data->ColliderHalfHeight = boss->GetNumberField(TEXT("ColliderHalfHeight"));
            data->MaxHp = boss->GetNumberField(TEXT("MaxHp"));
            data->Atk = boss->GetNumberField(TEXT("Atk"));
            data->Def = boss->GetNumberField(TEXT("Def"));
            data->MaxWalkSpeed = boss->GetNumberField(TEXT("MaxWalkSpeed"));
            data->MoveSpeedRate = boss->GetNumberField(TEXT("MoveSpeedRate"));
            data->SkillAttackId = boss->GetIntegerField(TEXT("SkillAttackId"));
            /** Monster **/
            data->MonsterType = boss->GetStringField(TEXT("MonsterType"));
            data->AdvancedSkillId = boss->GetIntegerField(TEXT("AdvancedSkillId"));
            data->DefaultAtkRange = boss->GetNumberField(TEXT("DefaultAtkRange"));
            data->SearchMaxDistance = boss->GetNumberField(TEXT("SearchMaxDistance"));
            data->ChaseMaxDistance = boss->GetNumberField(TEXT("ChaseMaxDistance"));
            /** Boss **/

            BossData.Add(data->DataId, data);
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
            data->OwnerSkillDataId = skill->GetIntegerField(TEXT("OwnerSkillDataId"));
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
            TArray<TSharedPtr<FJsonValue>> BuffIds = skill->GetArrayField(TEXT("BuffIdList"));
            for (auto& buffId : BuffIds)
            {
                int32 id;
                if (buffId->TryGetNumber(id))
                    data->BuffIdList.Add(id);
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
                data->ForwardLength = effect->GetNumberField(TEXT("ForwardLength"));
                data->BackwardLength = effect->GetNumberField(TEXT("BackwardLength"));
                data->LeftLength = effect->GetNumberField(TEXT("LeftLength"));
                data->RightLength = effect->GetNumberField(TEXT("RightLength"));

                EffectData.Add(data->DataId, data);
            }
            else if (effectType == TEXT("Pizza"))
            {
                UPizzaEffectData* data = NewObject<UPizzaEffectData>();

                data->DataId = effect->GetIntegerField(TEXT("DataId"));
                data->EffectType = effectType;
                data->Radius = effect->GetNumberField(TEXT("Radius"));
                data->Theta = effect->GetNumberField(TEXT("Theta"));

                EffectData.Add(data->DataId, data);
            }
            else if (effectType == TEXT("Circle"))
            {
                UCircleEffectData* data = NewObject<UCircleEffectData>();

                data->DataId = effect->GetIntegerField(TEXT("DataId"));
                data->EffectType = effectType;
                data->Radius = effect->GetNumberField(TEXT("Radius"));

                EffectData.Add(data->DataId, data);
            }
        }
    }

    const TArray<TSharedPtr<FJsonValue>> projectiles = JsonObject->GetArrayField(TEXT("projectiles"));
    {
        for (int32 i = 0; i < projectiles.Num(); i++)
        {
            TSharedPtr<FJsonObject> projectile = projectiles[i]->AsObject();

            UProjectileData* data = NewObject<UProjectileData>();

            data->DataId = projectile->GetIntegerField(TEXT("DataId"));
            data->OwnerSkillDataId = projectile->GetIntegerField(TEXT("OwnerSkillDataId"));
            data->Name = projectile->GetStringField(TEXT("Name"));
            data->Duration = projectile->GetNumberField(TEXT("Duration"));
            data->MoveSpeed = projectile->GetNumberField(TEXT("MoveSpeed"));
            TArray<TSharedPtr<FJsonValue>> ImpactTimes = projectile->GetArrayField(TEXT("ImpactTimeList"));
            for (auto& buffId : ImpactTimes)
            {
                int32 id;
                if (buffId->TryGetNumber(id))
                    data->ImpactTimeList.Add(id);
            }
            TArray<TSharedPtr<FJsonValue>> BuffIds = projectile->GetArrayField(TEXT("BuffIdList"));
            for (auto& buffId : BuffIds)
            {
                int32 id;
                if (buffId->TryGetNumber(id))
                    data->BuffIdList.Add(id);
            }
            TArray<TSharedPtr<FJsonValue>> EffectIds = projectile->GetArrayField(TEXT("EffectIdList"));
            for (auto& effectId : EffectIds)
            {
                int32 id;
                if (effectId->TryGetNumber(id))
                    data->EffectIdList.Add(id);
            }

            ProjectileData.Add(data->DataId, data);
        }
    }

    const TArray<TSharedPtr<FJsonValue>> buffs = JsonObject->GetArrayField(TEXT("buffs"));
    {
        for (int32 i = 0; i < buffs.Num(); i++)
        {
            TSharedPtr<FJsonObject> buff = buffs[i]->AsObject();

            UBuffData* data = NewObject<UBuffData>();

            data->DataId = buff->GetIntegerField(TEXT("DataId"));
            data->BuffType = buff->GetStringField(TEXT("BuffType"));
            data->BuffDurationType = buff->GetStringField(TEXT("BuffDurationType"));
            data->BuffAmountRate = buff->GetNumberField(TEXT("BuffAmountRate"));
            data->BuffDurationPeriod = buff->GetNumberField(TEXT("BuffDurationPeriod"));
            data->BuffDurationMagnitude = buff->GetNumberField(TEXT("BuffDurationMagnitude"));

            BuffData.Add(data->DataId, data);
        }
    }

    const TArray<TSharedPtr<FJsonValue>> gimmicks = JsonObject->GetArrayField(TEXT("gimmicks"));
    {
        for (int32 i = 0; i < gimmicks.Num(); i++)
        {
            TSharedPtr<FJsonObject> gimmick = gimmicks[i]->AsObject();

            UGimmickData* data = NewObject<UGimmickData>();

            data->DataId = gimmick->GetIntegerField(TEXT("DataId"));
            data->Name = gimmick->GetStringField(TEXT("Name"));
            data->DescriptionText = gimmick->GetStringField(TEXT("DescriptionText"));
            data->Duration = gimmick->GetNumberField(TEXT("Duration"));
            TArray<TSharedPtr<FJsonValue>> MissionIds = gimmick->GetArrayField(TEXT("MissionIdList"));
            for (auto& MissionId : MissionIds)
            {
                int32 id;
                if (MissionId->TryGetNumber(id))
                    data->MissionIdList.Add(id);
            }
            TArray<TSharedPtr<FJsonValue>> SkillIds = gimmick->GetArrayField(TEXT("SkillIdList"));
            for (auto& SkillId : SkillIds)
            {
                int32 id;
                if (SkillId->TryGetNumber(id))
                    data->SkillIdList.Add(id);
            }

            GimmickData.Add(data->DataId, data);
        }
    }
}
