#include "pch.h"
#include "Utils.h"

FVector3 Utils::Recast2UnrealPoint(const FVector3& Vector, float halfHeight)
{
	// Recast ÁÂÇ¥¸¦ Unreal Áß¾Ó ÁÂÇ¥°è·Î º¯È¯
	FVector3 CenterPosition = FVector3(-Vector.X * 100.f, -Vector.Z * 100.f, Vector.Y * 100.f);
	return CenterPosition + FVector3(0.f, 0.f, halfHeight);
}

FVector3 Utils::Unreal2RecastPoint(const FVector3& Vector, float halfHeight)
{
	// Áß¾Ó ÁÂÇ¥¸¦ °è»êÇÏ°í Recast ÁÂÇ¥°è·Î º¯È¯
	FVector3 CenterPosition = Vector + FVector3(0, 0, halfHeight);
	return FVector3(-CenterPosition.X * 0.01f, CenterPosition.Z * 0.01f, -CenterPosition.Y * 0.01f);
}

FVector3 Utils::Unreal2RecastPoint(const Protocol::PosInfo& posInfo, float halfHeight)
{
	FVector3 pos;
	pos.X = posInfo.x();
	pos.Y = posInfo.y();
	pos.Z = posInfo.z();

	return Unreal2RecastPoint(pos, halfHeight);
}
