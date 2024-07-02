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
		// �õ尪�� ��� ���� random_device ����.
		std::random_device randomDevice;
		// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
		std::mt19937 generator(randomDevice());
		// �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.

		// constexpr
		// T�� ���� �� ���� uniform_real_distribution<T>�� �ȵǱ� ������ ������ ������ �������� 
		// ������ �ܰ迡�� �� �� �ϳ��� �� �� �ְ� ���ش�.
		if constexpr (std::is_integral_v<T>) // ���� Ÿ���� ���
		{
			std::uniform_int_distribution<T> distribution(min, max);
			return distribution(generator);
		}
		else // �Ǽ� Ÿ���� ���
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
		// �켱 �Ÿ��� ��
		float dist = DirectionVectorLen(start, end);

		// ���� �Ÿ����� ���� �ѹ��� �̵��� �� �ִ� �Ÿ��� ��ٸ� �������� �ٷ� ����
		if (dist <= TICK_COUNT * speed * 0.001)
		{
			return end;
		}
		
		// ������ ũ���� ���⺤�� ����ȭ
		pair<float, float> dirVec2d = DirectionVectorNormalized(start, end);

		// ���� �����ӿ� �� �� ���ϱ�

		pair<float, float> nextPos;
		nextPos.first = start.first + dirVec2d.first * TICK_COUNT * speed * 0.001f;
		nextPos.second = start.second + dirVec2d.second * TICK_COUNT * speed * 0.001f;

		return nextPos;
	}

	// �ֽ�
	
	static FVector3 Recast2UnrealPoint(const FVector3& Vector);
	static FVector3 Unreal2RecastPoint(const FVector3& Vector);
	static FVector3 Unreal2RecastPoint(const Protocol::PosInfo& posInfo);
};