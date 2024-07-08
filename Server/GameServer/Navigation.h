#pragma once

#include "SampleInterfaces.h"
#include "RecastNavigation/Recast.h"
#include "RecastNavigation/DetourNavMesh.h"
#include "RecastNavigation/DetourNavMeshQuery.h"

/// These are just sample areas to use consistent values across the samples.
/// The use should specify these base on his needs.
enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
};

enum SamplePartitionType
{
	SAMPLE_PARTITION_WATERSHED,
	SAMPLE_PARTITION_MONOTONE,
	SAMPLE_PARTITION_LAYERS
};

class Navigation
{
public:
	Navigation(RoomBaseRef room);
	~Navigation();

protected:
	bool m_keepInterResults;
	float m_totalBuildTimeMs;

	unsigned char* m_triareas;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcConfig m_cfg;
	rcPolyMeshDetail* m_dmesh;

	/** detail data**/
	class InputGeom* m_geom;
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery;
	class dtCrowd* m_crowd;

	static const int MAX_AGENTS = 1024;

	unsigned char m_navMeshDrawFlags;

	float m_cellSize;
	float m_cellHeight;
	float m_agentHeight;
	float m_agentRadius;
	float m_agentMaxClimb;
	float m_agentMaxSlope;
	float m_regionMinSize;
	float m_regionMergeSize;
	float m_edgeMaxLen;
	float m_edgeMaxError;
	float m_vertsPerPoly;
	float m_detailSampleDist;
	float m_detailSampleMaxError;
	int m_partitionType;

	bool m_filterLowHangingObstacles;
	bool m_filterLedgeSpans;
	bool m_filterWalkableLowHeightSpans;

	//SampleTool* m_tool;
	//SampleToolState* m_toolStates[MAX_TOOLS];

	BuildContext* m_ctx;

	// owner
	weak_ptr<RoomBase> m_room;

	//SampleDebugDraw m_dd;

public:
	/** initialize & setter & getter **/
	void cleanup();
	void setContext(BuildContext* ctx) { m_ctx = ctx; }

	// tick
	void UpdateTick();

	virtual class dtNavMesh* getNavMesh() { return m_navMesh; }
	virtual class dtNavMeshQuery* getNavMeshQuery() { return m_navQuery; }
	virtual void SetAgentRadius(float radius) { m_agentRadius = radius; }
	virtual float getAgentRadius() { return m_agentRadius; }
	virtual void SetAgentHeight(float height) { m_agentHeight = height; }
	virtual float getAgentHeight() { return m_agentHeight; }
	virtual class dtCrowd* getCrowd() { return m_crowd; }

public:
	virtual void handleMeshChanged(class InputGeom* geom);
	virtual bool handleBuild();


};

