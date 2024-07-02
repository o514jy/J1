#pragma once
#include <random>
#include "Define.h"
#include "FVector3.h"

class Utils
{
public:
	template<typename T>
	static T GetRandom(T min, T max)
	{
		// 시드값을 얻기 위한 random_device 생성.
		std::random_device randomDevice;
		// random_device 를 통해 난수 생성 엔진을 초기화 한다.
		std::mt19937 generator(randomDevice());
		// 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.

		// constexpr
		// T가 정수 일 때는 uniform_real_distribution<T>이 안되기 때문에 원래는 에러가 생기지만 
		// 컴파일 단계에서 둘 중 하나를 고를 수 있게 해준다.
		if constexpr (std::is_integral_v<T>) // 정수 타입일 경우
		{
			std::uniform_int_distribution<T> distribution(min, max);
			return distribution(generator);
		}
		else // 실수 타입일 경우
		{
			std::uniform_real_distribution<T> distribution(min, max);
			return distribution(generator);
		}
	}

	static std::string WstrToStr(const std::wstring& source)
	{
		return std::string().assign(source.begin(), source.end());
	}

	static std::wstring StrToWstr(const std::string& source)
	{
		return std::wstring().assign(source.begin(), source.end());
	}

	// math
	static pair<float, float> DirectionVectorNormalized(pair<float, float> start, pair<float, float> end)
	{
		// Calculate the direction vector
		float directionX = end.first - start.first;
		float directionY = end.second - start.second;

		// Calculate the length of the direction vector
		float length = sqrt(directionX * directionX + directionY * directionY);

		// Normalize the direction vector (divide each component by the length)
		float normalizedDirectionX = directionX / length;
		float normalizedDirectionY = directionY / length;

		// Return the normalized direction vector
		return make_pair(normalizedDirectionX, normalizedDirectionY);
	}

	static pair<float, float> DirectionVector(pair<float, float> start, pair<float, float> end)
	{
		// Calculate the direction vector
		float directionX = end.first - start.first;
		float directionY = end.second - start.second;

		// Return the normalized direction vector
		return make_pair(directionX, directionY);
	}

	static float DirectionVectorLen(Protocol::PosInfo* pos1, Protocol::PosInfo* pos2)
	{
		pair<float, float> tempPos1 = make_pair(pos1->x(), pos1->y());
		pair<float, float> tempPos2 = make_pair(pos2->x(), pos2->y());

		return DirectionVectorLen(tempPos1, tempPos2);
	}

	static float DirectionVectorLen(pair<float, float> start, pair<float, float> end)
	{
		// Calculate the direction vector
		float directionX = end.first - start.first;
		float directionY = end.second - start.second;

		// Calculate the length of the direction vector
		return sqrt(directionX * directionX + directionY * directionY);
	}

	static pair<float, float> FindNextTickPos2dToGo(Protocol::PosInfo* pos1, Protocol::PosInfo* pos2, float speed)
	{
		pair<float, float> tempPos1 = make_pair(pos1->x(), pos1->y());
		pair<float, float> tempPos2 = make_pair(pos2->x(), pos2->y());

		return FindNextTickPos2dToGo(tempPos1, tempPos2, speed);
	}

	static pair<float, float> FindNextTickPos2dToGo(pair<float, float> start, pair<float, float> end, float speed)
	{
		// 우선 거리를 잼
		float dist = DirectionVectorLen(start, end);

		// 남은 거리보다 내가 한번에 이동할 수 있는 거리가 길다면 목적지로 바로 리턴
		if (dist <= TICK_COUNT * speed * 0.001)
		{
			return end;
		}
		
		// 가야할 크기의 방향벡터 정규화
		pair<float, float> dirVec2d = DirectionVectorNormalized(start, end);

		// 다음 프레임에 갈 곳 구하기

		pair<float, float> nextPos;
		nextPos.first = start.first + dirVec2d.first * TICK_COUNT * speed * 0.001f;
		nextPos.second = start.second + dirVec2d.second * TICK_COUNT * speed * 0.001f;

		return nextPos;
	}

	// 최신
	
	static FVector3 Recast2UnrealPoint(const FVector3& Vector);
	static FVector3 Unreal2RecastPoint(const FVector3& Vector);
	static FVector3 Unreal2RecastPoint(const Protocol::PosInfo& posInfo);
};