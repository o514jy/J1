#pragma once

#include "Navigation.h"
#include "RecastNavigation/DetourNavMesh.h"
#include "RecastNavigation/DetourNavMeshQuery.h"

class NavDevice
{
public:
	/** initialize **/
	NavDevice();
	virtual void init(Navigation* sample, ObjectRef owner);
	virtual void reset();

	virtual void AddAgentToCrowd();
	virtual void SubAgentToCrowd();

public:
	/** setter & getter **/
	void SetStartPos(float sPos[]);
	void SetEndPos(float ePos[]);

	void SetSposSet(bool flag);
	bool GetSposSet() { return m_sposSet; }
	void SetEposSet(bool flag);
	bool GetEposSet() { return m_eposSet; }

public:
	/** use outside for findpath **/
	void CalculatePath(); // NavMeshTesterTool::recalc
	void SetMoveTarget(FVector3 destPos);

	void CopyPathFromDevice(vector<FVector3>& path);

	void StopMovement();

	int agentIdx;

private:
	weak_ptr<Object> m_owner;

	//float m_targetPos[3];
	dtPolyRef m_targetRef;

	Navigation* m_sample;

	dtNavMesh* m_navMesh;
	dtNavMeshQuery* m_navQuery;

	dtQueryFilter m_filter;

	dtStatus m_pathFindStatus;

	enum ToolMode
	{
		TOOLMODE_PATHFIND_FOLLOW,
		TOOLMODE_PATHFIND_STRAIGHT,
		TOOLMODE_PATHFIND_SLICED,
		TOOLMODE_RAYCAST,
		TOOLMODE_DISTANCE_TO_WALL,
		TOOLMODE_FIND_POLYS_IN_CIRCLE,
		TOOLMODE_FIND_POLYS_IN_SHAPE,
		TOOLMODE_FIND_LOCAL_NEIGHBOURHOOD
	};

	ToolMode m_toolMode;

	int m_straightPathOptions;

	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;

	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	dtPolyRef m_polys[MAX_POLYS];
	dtPolyRef m_parent[MAX_POLYS];
	int m_npolys;
	float m_straightPath[MAX_POLYS * 3];
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];
	int m_nstraightPath;
	float m_polyPickExt[3];
	float m_smoothPath[MAX_SMOOTH * 3];
	int m_nsmoothPath;
	float m_queryPoly[4 * 3];

	static const int MAX_RAND_POINTS = 64;
	float m_randPoints[MAX_RAND_POINTS * 3];
	int m_nrandPoints;
	bool m_randPointsInCircle;

	float m_spos[3];
	float m_epos[3];
	float m_hitPos[3];
	float m_hitNormal[3];
	bool m_hitResult;
	float m_distanceToWall;
	float m_neighbourhoodRadius;
	float m_randomRadius;
	bool m_sposSet;
	bool m_eposSet;

	int m_pathIterNum;
	dtPolyRef m_pathIterPolys[MAX_POLYS];
	int m_pathIterPolyCount;
	float m_prevIterPos[3], m_iterPos[3], m_steerPos[3], m_targetPos[3];

	static const int MAX_STEER_POINTS = 10;
	float m_steerPoints[MAX_STEER_POINTS * 3];
	int m_steerPointCount;

	/** helper **/
	static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
		const float minTargetDist,
		const dtPolyRef* path, const int pathSize,
		float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
		float* outPoints = 0, int* outPointCount = 0);

	static bool inRange(const float* v1, const float* v2, const float r, const float h);

	static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery);
};

