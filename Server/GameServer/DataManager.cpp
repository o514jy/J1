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
            dataRef->AtkRange = player["AtkRange"].GetFloat();
            dataRef->Def = player["Def"].GetFloat();
            dataRef->MaxWalkSpeed = player["MaxWalkSpeed"].GetFloat();
            dataRef->HpRate = player["HpRate"].GetFloat();
            dataRef->AtkRate = player["AtkRate"].GetFloat();
            dataRef->DefRate = player["DefRate"].GetFloat();
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
                dataRef->AnimImpactTimeList.push_back(impactTimes[j].GetFloat());
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
                dataRef->EffectType = effectType;
                dataRef->LeftUpPosY = effect["LeftUpPosY"].GetFloat();
                dataRef->LeftUpPosX = effect["LeftUpPosX"].GetFloat();
                dataRef->RightDownPosY = effect["RightDownPosY"].GetFloat();
                dataRef->RightDownPosX = effect["RightDownPosX"].GetFloat();

                _effectData.insert(make_pair(dataRef->DataId, dataRef));
            }
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
