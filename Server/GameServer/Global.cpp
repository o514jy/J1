#include "pch.h"
#include "Global.h"
#include "ObjectManager.h"
#include "RoomManager.h"
#include "DataManager.h"
#include "SkillManager.h"

shared_ptr<ObjectManager> GObjectManager = nullptr;
shared_ptr<RoomManager> GRoomManager = nullptr;
shared_ptr<DataManager> GDataManager = nullptr;
shared_ptr<SkillManager> GSkillManager = nullptr;

class Global
{
public:
	Global()
	{
		GObjectManager = make_shared<ObjectManager>();
		GRoomManager = make_shared<RoomManager>();
		GDataManager = make_shared<DataManager>();
		GSkillManager = make_shared<SkillManager>();
	}

	~Global()
	{
		GObjectManager = nullptr;
		GRoomManager = nullptr;
		GDataManager = nullptr;
		GSkillManager = nullptr;
	}
} Global;