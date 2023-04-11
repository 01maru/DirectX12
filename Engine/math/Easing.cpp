#include "Easing.h"
#include <math.h>

template<typename T>
T Easing::lerp(T a, T b, T t)
{
	return a + t * (b - a);
}
template float Easing::lerp<float>(float, float, float);
template double Easing::lerp<double>(double, double, double);

template<typename T>
T Easing::EaseInOut(T startpos, T endpos, T time, int num)
{
	if (time < 0.5)
	{
		return lerp(startpos, endpos, (T)(pow(2, num - 1) * pow(time, num)));
	}
	return lerp(startpos, endpos, (T)(1 - pow(-2 * time + 2, num) / 2));
}
template float Easing::EaseInOut<float>(float, float, float, int);
template double Easing::EaseInOut<double>(double, double, double, int);

template<typename T>
T Easing::EaseInBack(T startpos, T endpos, T time, int num, double p1)
{
	const double c1 = p1;
	const double c3 = c1 + 1;

	return lerp(startpos, endpos, (T)pow((c3 * time * time * time - c1 * time * time), num));
}
template float Easing::EaseInBack<float>(float, float, float, int, double);
template double Easing::EaseInBack<double>(double, double, double, int, double);

template<typename T>
T Easing::EaseOutBack(const T startpos, const T endpos, T time, int num, double p1)
{
	double c1 = p1;
	double c3 = c1 + 1;

	return lerp(startpos, endpos, (T)pow(1 + c3 * pow(time - 1, 3) + c1 * pow(time - 1, 2), num));
}
template float Easing::EaseOutBack<float>(float, float, float, int, double);
template double Easing::EaseOutBack<double>(double, double, double, int, double);

template<typename T>
T Easing::EaseIn(T start, T end, T time, int num)
{
	return lerp(start, end, (T)pow(time, num));
}
template float Easing::EaseIn<float>(float, float, float, int);
template double Easing::EaseIn<double>(double, double, double, int);

template<typename T>
T Easing::EaseOut(T start, T end, T time, int num)
{
	return lerp(start, end, (T)(1 - pow(1 - time, num)));
}
template float Easing::EaseOut<float>(float, float, float, int);
template double Easing::EaseOut<double>(double, double, double, int);
