#pragma once

class StatComponent : public enable_shared_from_this<StatComponent>
{
public:
	StatComponent();
	~StatComponent();

public:
	void UpdateTick();

public:
	/** initialize **/
	void SetInfo(CreatureRef owner, CreatureDataRef creatureData, Protocol::CreatureType creatureType);

public:
	/** setter & getter **/
	void SetHp(float hp);
	float GetHp();

	void SetMaxHp(float maxHp);
	float GetMaxHp();

	void SetAtk(float atk);
	float GetAtk();

	void SetDef(float def);
	float GetDef();

public:
	/** network **/
	Protocol::StatInfo* _statInfo;

public:
	/** refresh **/
	void RefreshAll();

private:
	/** information **/
	CreatureRef _owner;

	/** stat **/

	/* creature */
	float _hp;

	float _baseMaxHp;
	float _maxHp;

	float _baseAtk;
	float _atk;

	float _baseDef;
	float _def;

private:
	/** dirty flag to refresh stat **/
	bool _shouldRefresh;
};

