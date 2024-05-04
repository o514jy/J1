#pragma once

class GimmickComponent
{
public:
	GimmickComponent();
	~GimmickComponent();

public:
	/** initialize **/
	void SetInfo(BossRef owner);

	void DoGimmick(int32 gimmickId);

	void AddGimmick(int32 gimmickId);

public:
	/** setter & getter **/
	void SetActiveGimmick(GimmickBaseRef gimmick);
	GimmickBaseRef GetActiveGimmick();

public:
	/** information **/
	BossRef _owner;

	GimmickBaseRef _activeGimmick;

	queue<int32> _gimmickIdQueue;

	//vector<GimmickBaseRef> _canActiveGimmickList;

	unordered_map<int32, GimmickBaseRef> _gimmicks;
};

