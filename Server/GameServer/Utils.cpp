#include "pch.h"
#include "Utils.h"

FVector3 Utils::Recast2UnrealPoint(const FVector3& Vector)
{
	return FVector3(-Vector.X * 100.f, -Vector.Z * 100.f, Vector.Y * 100.f);
}

FVector3 Utils::Unreal2RecastPoint(const FVector3& Vector)
{
	return FVector3(-Vector.X * 0.01f, Vector.Z * 0.01f, -Vector.Y * 0.01f);
}

FVector3 Utils::Unreal2RecastPoint(const Protocol::PosInfo& posInfo)
{
	FVector3 pos;
	pos.X = posInfo.x();
	pos.Y = posInfo.y();
	pos.Z = posInfo.z();

	return Unreal2RecastPoint(pos);
}
