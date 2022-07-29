#pragma once

namespace MathHelper
{
	inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 q)
	{
		XMFLOAT3 euler{};

		euler.y = atan2f(2.f * q.x * q.w + 2.f * q.y * q.z, 1.f - 2.f * (q.z * q.z + q.w * q.w));     // Yaw 
		euler.x = asinf(2.f * (q.x * q.z - q.w * q.y));                             // Pitch 
		euler.z = atan2f(2.f * q.x * q.y + 2.f * q.z * q.w, 1.f - 2.f * (q.y * q.y + q.z * q.z));      // Roll 

		return euler;
	}

	inline bool XMFloat4Equals(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	inline bool XMFloat3Equals(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	inline bool XMFloat2Equals(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline float randF(float min, float max)
	{
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float diff = max - min;
		const float r = random * diff;
		return min + r;
	}

	inline LONG BinaryClamp(LONG x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}

	inline float Deg2Rad(float value)
	{
		return value / 180 * 3.14f;
	}

	inline float DistanceSqr(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		float sqrDistance = (b.x - a.x) * (b.x - a.x) + 
			(b.y - a.y) * (b.y - a.y) +
			(b.z - a.z) * (b.z - a.z);

		return abs(sqrDistance);
	}
	inline float Distance2DSqr(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		float sqrDistance = (b.x - a.x) * (b.x - a.x) + (b.z - a.z) * (b.z - a.z);

		return abs(sqrDistance);
	}
	inline float Distance2DSqr(const XMFLOAT3& a, const XMFLOAT2& b)
	{
		float sqrDistance = (b.x - a.x) * (b.x - a.x) + (b.y - a.z) * (b.y - a.z);

		return abs(sqrDistance);
	}
	inline float RandomFloat(float min, float max)
	{
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}

	template<typename T>
	void Clamp(T& value, T hi, T lo)
	{
		if (value > hi)
			value = hi;

		if (value < lo)
			value = lo;
	}
}