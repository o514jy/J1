#include "pch.h"
#include "DataManager.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <fstream>
#include <codecvt>
#include <locale>


using namespace rapidjson;
using namespace std;

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
}

void DataManager::Init()
{
	ParseJsonData(L"../../Common/Data/GameData.json");
}


void DataManager::ParseJsonData(const WCHAR* path)
{
    WRITE_LOCK;

    try {
        // JSON 파일을 읽기 모드로 열기
        ifstream ifs(path);
        if (!ifs.is_open()) {
            throw runtime_error("failed to open the file");
        }

        // 파일 내용을 문자열로 읽기
        string json((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));

        // JSON 문자열 파싱
        Document doc;
        if (doc.Parse(json.c_str()).HasParseError()) {
            cout << "JSON 파싱 오류." << endl;
            return;
        }

        /** players **/
        Value& players = doc["players"];
        for (int i = 0; i < players.Size(); i++)
        {
            PlayerDataRef dataRef = make_shared<PlayerData>();
            Value& player = players[i];
            dataRef->DataId = player["DataId"].GetInt();
            dataRef->CreatureType = Utils::StrToWstr(player["CreatureType"].GetString());
            dataRef->DescriptionTextId = Utils::StrToWstr(player["DescriptionTextId"].GetString());
            dataRef->ColliderRadius = player["ColliderRadius"].GetFloat();
            dataRef->ColliderHalfHeight = player["ColliderHalfHeight"].GetFloat();
            dataRef->MaxHp = player["MaxHp"].GetFloat();
            dataRef->Atk = player["Atk"].GetFloat();
            dataRef->Def = player["Def"].GetFloat();
            dataRef->MaxWalkSpeed = player["MaxWalkSpeed"].GetFloat();
            dataRef->MoveSpeedRate = player["MoveSpeedRate"].GetFloat();
            dataRef->SkillAttackId = player["SkillAttackId"].GetInt();
            dataRef->SkillQId = player["SkillQId"].GetInt();
            dataRef->SkillWId = player["SkillWId"].GetInt();
            dataRef->SkillEId = player["SkillEId"].GetInt();
            dataRef->SkillRId = player["SkillRId"].GetInt();
            dataRef->SkillDashId = player["SkillDashId"].GetInt();

            _playerData.insert(make_pair(dataRef->DataId, dataRef));
        }

        /** bosses **/
        Value& bosses = doc["bosses"];
        for (int i = 0; i < bosses.Size(); i++)
        {
            BossDataRef dataRef = make_shared<BossData>();
            Value& boss = bosses[i];
            dataRef->DataId = boss["DataId"].GetInt();
            dataRef->CreatureType = Utils::StrToWstr(boss["CreatureType"].GetString());
            dataRef->DescriptionTextId = Utils::StrToWstr(boss["DescriptionTextId"].GetString());
            dataRef->ColliderRadius = boss["ColliderRadius"].GetFloat();
            dataRef->ColliderHalfHeight = boss["ColliderHalfHeight"].GetFloat();
            dataRef->MaxHp = boss["MaxHp"].GetFloat();
            dataRef->Atk = boss["Atk"].GetFloat();
            dataRef->Def = boss["Def"].GetFloat();
            dataRef->MaxWalkSpeed = boss["MaxWalkSpeed"].GetFloat();
            dataRef->MoveSpeedRate = boss["MoveSpeedRate"].GetFloat();
            dataRef->SkillAttackId = boss["SkillAttackId"].GetInt();
            dataRef->MonsterType = Utils::StrToWstr(boss["MonsterType"].GetString());
            dataRef->AdvancedSkillId = boss["AdvancedSkillId"].GetInt();
            dataRef->DefaultAtkRange = boss["DefaultAtkRange"].GetFloat();
            dataRef->SearchMaxDistance = boss["SearchMaxDistance"].GetFloat();
            dataRef->ChaseMaxDistance = boss["ChaseMaxDistance"].GetFloat();

            _bossData.insert(make_pair(dataRef->DataId, dataRef));
        }

        /** skills **/
        Value& skills = doc["skills"];
        for (int i = 0; i < skills.Size(); i++)
        {
            SkillDataRef dataRef = make_shared<SkillData>();
            Value& skill = skills[i];
            dataRef->DataId = skill["DataId"].GetInt();
            dataRef->Name = Utils::StrToWstr(skill["Name"].GetString());
            dataRef->Description = Utils::StrToWstr(skill["Description"].GetString());
            Value& projIds = skill["ProjectileIdList"];
            for (int j = 0; j < projIds.Size(); j++)
            {
                dataRef->ProjectileIdList.push_back(projIds[j].GetInt());
            }
            dataRef->CoolTime = skill["CoolTime"].GetFloat();
            dataRef->SkillDuration = skill["SkillDuration"].GetFloat();
            Value& impactTimes = skill["AnimImpactTimeList"];
            for (int j = 0; j < impactTimes.Size(); j++)
            {
                dataRef->AnimImpactTimeList.push_back(impactTimes[j].GetInt());
            }
            Value& buffIds = skill["BuffIdList"];
            for (int j = 0; j < buffIds.Size(); j++)
            {
                dataRef->BuffIdList.push_back(buffIds[j].GetInt());
            }
            Value& effectIds = skill["EffectIdList"];
            for (int j = 0; j < effectIds.Size(); j++)
            {
                dataRef->EffectIdList.push_back(effectIds[j].GetInt());
            }

            _skillData.insert(make_pair(dataRef->DataId, dataRef));
        }

        /** effects **/
        Value& effects = doc["effects"];
        for (int i = 0; i < effects.Size(); i++)
        {
            Value& effect = effects[i];
            
            wstring effectType = Utils::StrToWstr(effect["EffectType"].GetString());
            if (effectType == L"Rectangle")
            {
                RectangleEffectDataRef dataRef = make_shared<RectangleEffectData>();
                dataRef->DataId = effect["DataId"].GetInt();
                dataRef->OwnerSkillDataId = effect["OwnerSkillDataId"].GetInt();
                dataRef->EffectType = effectType;
                dataRef->ForwardLength = effect["ForwardLength"].GetFloat();
                dataRef->BackwardLength = effect["BackwardLength"].GetFloat();
                dataRef->LeftLength = effect["LeftLength"].GetFloat();
                dataRef->RightLength = effect["RightLength"].GetFloat();

                _effectData.insert(make_pair(dataRef->DataId, dataRef));
            }
            else if (effectType == L"Pizza")
            {
                PizzaEffectDataRef dataRef = make_shared<PizzaEffectData>();
                dataRef->DataId = effect["DataId"].GetInt();
                dataRef->OwnerSkillDataId = effect["OwnerSkillDataId"].GetInt();
                dataRef->EffectType = effectType;
                dataRef->Radius = effect["Radius"].GetFloat();
                dataRef->Theta = effect["Theta"].GetFloat();

                _effectData.insert(make_pair(dataRef->DataId, dataRef));
            }
            else if (effectType == L"Circle")
            {
                CircleEffectDataRef dataRef = make_shared<CircleEffectData>();
                dataRef->DataId = effect["DataId"].GetInt();
                dataRef->OwnerSkillDataId = effect["OwnerSkillDataId"].GetInt();
                dataRef->EffectType = effectType;
                dataRef->Radius = effect["Radius"].GetFloat();

                _effectData.insert(make_pair(dataRef->DataId, dataRef));
            }
        }

        /** projectiles **/
        Value& projectiles = doc["projectiles"];
        for (int i = 0; i < projectiles.Size(); i++)
        {
            ProjectileDataRef dataRef = make_shared<ProjectileData>();
            Value& projectile = projectiles[i];
            dataRef->DataId = projectile["DataId"].GetInt();
            dataRef->OwnerSkillDataId = projectile["OwnerSkillDataId"].GetInt();
            dataRef->Name = Utils::StrToWstr(projectile["Name"].GetString());
            dataRef->Duration = projectile["Duration"].GetFloat();
            dataRef->MoveSpeed = projectile["MoveSpeed"].GetFloat();
            Value& impactTimes = projectile["ImpactTimeList"];
            for (int j = 0; j < impactTimes.Size(); j++)
            {
                dataRef->ImpactTimeList.push_back(impactTimes[j].GetInt());
            }
            Value& buffIds = projectile["BuffIdList"];
            for (int j = 0; j < buffIds.Size(); j++)
            {
                dataRef->BuffIdList.push_back(buffIds[j].GetInt());
            }
            Value& effectIds = projectile["EffectIdList"];
            for (int j = 0; j < effectIds.Size(); j++)
            {
                dataRef->EffectIdList.push_back(effectIds[j].GetInt());
            }

            _projectileData.insert(make_pair(dataRef->DataId, dataRef));
        }

        /** buffs **/
        Value& buffs = doc["buffs"];
        for (int i = 0; i < buffs.Size(); i++)
        {
            Value& buff = buffs[i];

            BuffDataRef dataRef = make_shared<BuffData>();
            dataRef->DataId = buff["DataId"].GetInt();
            dataRef->BuffType = Utils::StrToWstr(buff["BuffType"].GetString());
            dataRef->BuffDurationType = Utils::StrToWstr(buff["BuffDurationType"].GetString());
            dataRef->BuffAmountRate = buff["BuffAmountRate"].GetFloat();
            dataRef->BuffDurationPeriod = buff["BuffDurationPeriod"].GetFloat();
            dataRef->BuffDurationMagnitude = buff["BuffDurationMagnitude"].GetFloat();

            _buffData.insert(make_pair(dataRef->DataId, dataRef));
        }

        /** gimmicks **/
        Value& gimmicks = doc["gimmicks"];
        for (int i = 0; i < gimmicks.Size(); i++)
        {
            Value& gimmick = gimmicks[i];

            GimmickDataRef dataRef = make_shared<GimmickData>();
            dataRef->DataId = gimmick["DataId"].GetInt();
            dataRef->Name = Utils::StrToWstr(gimmick["Name"].GetString());
            dataRef->DescriptionText = Utils::StrToWstr(gimmick["DescriptionText"].GetString());
            dataRef->Duration = gimmicks["Duration"].GetFloat();
            Value& missionIds = gimmicks["MissionIdList"];
            for (int j = 0; j < missionIds.Size(); j++)
            {
                dataRef->MissionIdList.push_back(missionIds[j].GetInt());
            }
            Value& skillIds = gimmicks["SkillIdList"];
            for (int j = 0; j < skillIds.Size(); j++)
            {
                dataRef->SkillIdList.push_back(skillIds[j].GetInt());
            }

            _gimmickData.insert(make_pair(dataRef->DataId, dataRef));
        }
    }
    catch (std::exception& e) {
        // 예외 처리
        std::cerr << "Error: " << e.what() << std::endl;
        return;
    }
}

PlayerDataRef DataManager::GetPlayerDataById(int32 id)
{
    if (_playerData.find(id) == _playerData.end())
		return nullptr;

	return _playerData[id];
}

BossDataRef DataManager::GetBossDataById(int32 id)
{
    if (_bossData.find(id) == _bossData.end())
        return nullptr;

    return _bossData[id];
}

SkillDataRef DataManager::GetSkillDataById(int32 id)
{
    if (_skillData.find(id) == _skillData.end())
        return nullptr;

    return _skillData[id];
}

EffectDataRef DataManager::GetEffectDataById(int32 id)
{
    if (_effectData.find(id) == _effectData.end())
        return nullptr;

    return _effectData[id];
}

ProjectileDataRef DataManager::GetProjectileDataById(int32 id)
{
    if (_projectileData.find(id) == _projectileData.end())
        return nullptr;

    return _projectileData[id];
}

BuffDataRef DataManager::GetBuffDataById(int32 id)
{
    if (_buffData.find(id) == _buffData.end())
        return nullptr;

    return _buffData[id];
}

GimmickDataRef DataManager::GetGimmickDataById(int32 id)
{
    if (_gimmickData.find(id) == _gimmickData.end())
        return nullptr;

    return _gimmickData[id];
}
