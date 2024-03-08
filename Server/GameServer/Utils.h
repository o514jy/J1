#pragma once
#include <random>

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
};

