#pragma once
#include <random>

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
};