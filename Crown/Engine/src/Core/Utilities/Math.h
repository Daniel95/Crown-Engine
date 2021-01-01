#pragma once

#include <cmath>

namespace Crown {
	
template<class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
	assert(!(hi < lo));
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

inline float Sign(float a_Value) {
	return a_Value > 0.0f ? 1.0f : -1.0f;
}

template<typename T>
struct Vec2T
{
	union
	{
		struct
		{
			T x, y;
		};
		T c[2]{ 0 };
	};

	constexpr Vec2T() noexcept : x(0), y(0) {}
	constexpr Vec2T(T value) noexcept : x(value), y(value) {}
	constexpr Vec2T(T x, T y) noexcept : x(x), y(y) {}

	T& operator[](int index)
	{
		return c[index];
	}

	Vec2T<T> operator-(Vec2T<T> vec) const
	{
		return { x - vec.x, y - vec.y };
	}

	Vec2T<T> operator+(Vec2T<T> vec) const
	{
		return { x + vec.x, y + vec.y };
	}

	Vec2T<T> operator*(Vec2T<T> vec) const
	{
		return { x * vec.x, y * vec.y };
	}

	Vec2T<T> operator/(Vec2T<T> vec) const
	{
		return { x / vec.x, y / vec.y };
	}

	Vec2T<T> operator*(T value) const
	{
		return { x * value, y * value };
	}

	Vec2T<T> operator/(T value) const
	{
		return { x / value, y / value };
	}

	bool operator==(Vec2T<T> vec) const
	{
		return x == vec.x && y == vec.y;
	}
};

template<typename T>
struct Vec3T
{
	union
	{
		struct
		{
			T x, y, z;
		};
		T c[3]{ 0 };
	};

	constexpr Vec3T() noexcept : x(0), y(0), z(0) {}
	constexpr Vec3T(T value) noexcept : x(value), y(value), z(value) {}
	constexpr Vec3T(T x, T y, T z) noexcept : x(x), y(y), z(z) {}

	T& operator[](int index)
	{
		return c[index];
	}

	void operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
	}

	void operator/=(T value)
	{
		x /= value;
		y /= value;
		z /= value;
	}

	void operator+=(T value)
	{
		x += value;
		y += value;
		z += value;
	}

	void operator-=(T value)
	{
		x += value;
		y += value;
		z += value;
	}

	void operator+=(Vec3T<T> vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	Vec3T<T> operator-() const
	{
		return { -x, -y, -z };
	}

	Vec3T<T> operator-(const Vec3T<T> vec) const
	{
		return { x - vec.x, y - vec.y, z - vec.z };
	}

	Vec3T<T> operator+(const Vec3T<T> vec) const
	{
		return { x + vec.x, y + vec.y, z + vec.z };
	}

	Vec3T<T> operator*(const Vec3T<T> vec) const
	{
		return { x * vec.x, y * vec.y, z * vec.z };
	}

	Vec3T<T> operator/(const Vec3T<T> vec) const
	{
		return { x / vec.x, y / vec.y, z / vec.z };
	}

	Vec3T<T> operator/(T value) const
	{
		return { x / value, y / value, z / value };
	}

	Vec3T<T> operator*(T value) const
	{
		return { x * value, y * value, z * value };
	}

	bool operator==(Vec3T<T> vec) const
	{
		return (x == vec.x) && (y == vec.y) && (z == vec.z);
	}

	Vec3T<T> Normalize()
	{
		T length = Length();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}

	T Length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	T Length2() const
	{
		return x * x + y * y + z * z;
	}

	static Vec3T<T> Normal(const Vec3T<T>& v)
	{
		Vec3T<T> vec = v;
		vec.Normalize();
		return vec;
	}

	static Vec3T<T> Normal(const Vec3T<T>& v0, const Vec3T<T>& v1, const Vec3T<T>& v2)
	{
		Vec3T<T> v10 = v1 - v0;
		Vec3T<T> v20 = v2 - v0;
		Vec3T<T> n = Vec3T<T>::Cross(v10, v20);

		T len2 = n.Length2();
		if (len2 > 0.0f) {
			n /= sqrtf(len2);
		}

		return n;
	}

	static Vec3T<T> Cross(Vec3T<T> v, Vec3T<T> k)
	{
		return { v.y * k.z - v.z * k.y,
				v.z * k.x - v.x * k.z,
				v.x * k.y - v.y * k.x };
	}

	static T Dot(Vec3T<T> v, Vec3T<T> k)
	{
		return v.x * k.x + v.y * k.y + v.z * k.z;
	}

	static T Length(Vec3T<T> v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
};

template<typename T>
struct Vec4T
{
	union
	{
		struct
		{
			T x, y, z, w;
		};
		T c[4]{ 0 };
	};

	constexpr Vec4T() noexcept : x(0), y(0), z(0), w(0) {}
	constexpr Vec4T(T value) noexcept : x(value), y(value), z(value), w(value) {}
	constexpr Vec4T(T x, T y, T z, T w) noexcept : x(x), y(y), z(z), w(w) {}

	T& operator[](int index)
	{
		return c[index];
	}

	void operator*=(T value)
	{
		x *= value;
		y *= value;
		z *= value;
		w *= value;
	}

	void operator/=(T value)
	{
		x /= value;
		y /= value;
		z /= value;
		w /= value;
	}

	void operator+=(T value)
	{
		x += value;
		y += value;
		z += value;
		w += value;
	}

	void operator-=(T value)
	{
		x += value;
		y += value;
		z += value;
		w += value;
	}

	void operator+=(Vec4T<T> vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		w += vec.z;
	}

	Vec4T<T> operator-() const
	{
		return { -x, -y, -z, -w };
	}

	Vec4T<T> operator-(const Vec4T<T> vec) const
	{
		return { x - vec.x, y - vec.y, z - vec.z, w - vec.w };
	}

	Vec4T<T> operator+(const Vec4T<T> vec) const
	{
		return { x + vec.x, y + vec.y, z + vec.z, w + vec.w };
	}

	Vec4T<T> operator*(const Vec4T<T> vec) const
	{
		return { x * vec.x, y * vec.y, z * vec.z, w * vec.w };
	}

	Vec4T<T> operator/(const Vec4T<T> vec) const
	{
		return { x / vec.x, y / vec.y, z / vec.z, w / vec.w };
	}

	Vec4T<T> operator/(T value) const
	{
		return { x / value, y / value, z / value, w / value };
	}

	Vec4T<T> operator*(T value) const
	{
		return { x * value, y * value, z * value, w * value };
	}

	bool operator==(Vec4T<T> vec) const
	{
		return (x == vec.x) && (y == vec.y) && (z == vec.z) && (w == vec.w);
	}
};

using Vec2 = Vec2T<float>;
using IVec2 = Vec2T<int>;
using Vec3 = Vec3T<float>;
using IVec3 = Vec3T<int>;
using Vec4 = Vec4T<float>;
using IVec4 = Vec4T<int>;

}
