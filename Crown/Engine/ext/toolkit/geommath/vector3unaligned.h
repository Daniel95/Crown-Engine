/* SIE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 07.008.001
* Copyright (C) 2019 Sony Interactive Entertainment Inc.
*/

#ifndef __VECTOR3UNALIGNED_H__
#define __VECTOR3UNALIGNED_H__

#include <vectormath.h>
#include <string.h>

// Vector3Unaligned is meant to store a Vector3, except that padding and alignment are
// 4-byte granular (GPU), rather than 16-byte (SSE). This is to bridge
// the SSE math library with GPU structures that assume 4-byte granularity.
// While a Vector3 has many operations, Vector3Unaligned can only be converted to and from Vector3.
struct Vector3Unaligned
{
	float x;
	float y;
	float z;
	Vector3Unaligned& operator=(const Vector3 &rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
		return *this;
	}
	Vector3Unaligned& operator=(const Vector4 &rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
		return *this;
	}
	float& operator [](unsigned idx)
	{
		idx %= 3;
		return idx == 0 ? x : (idx == 1 ? y : z);
	}
	float operator [](unsigned idx) const
	{
		idx %= 3;
		return idx == 0 ? x : (idx == 1 ? y : z);
	}
};

inline Vector3Unaligned ToVector3Unaligned( const Vector3& r )
{
	const Vector3Unaligned result = { r.getX(), r.getY(), r.getZ() };
	return result;
}

#endif
