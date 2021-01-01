/* SIE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 07.008.001
* Copyright (C) 2019 Sony Interactive Entertainment Inc.
*/

#ifndef __MATRIX4UNALIGNED_H__
#define __MATRIX4UNALIGNED_H__

#include "vector4unaligned.h"
#include <vectormath.h>

// Matrix4Unaligned is meant to store a Matrix4, except that padding and alignment are
// 4-byte granular (GPU), rather than 16-byte (SSE). This is to bridge
// the SSE math library with GPU structures that assume 4-byte granularity.
// While a Matrix4 has many operations, Matrix4Unaligned can only be converted to and from Matrix4.
struct Matrix4Unaligned
{
	Vector4Unaligned x;
	Vector4Unaligned y;
	Vector4Unaligned z;
	Vector4Unaligned w;
	Matrix4Unaligned &operator=(const Matrix4 &rhs)
	{
		memcpy(this, &rhs, sizeof(*this));
		return *this;
	}
	Vector4Unaligned& operator [](unsigned idx)
	{
		idx %= 4;
		return idx == 0 ? x : (idx == 1 ? y : (idx == 2 ? z : w));
	}
	Vector4Unaligned operator [](unsigned idx) const
	{
		idx %= 4;
		return idx == 0 ? x : (idx == 1 ? y : (idx == 2 ? z : w));
	}
};

inline Matrix4Unaligned ToMatrix4Unaligned( const Matrix4& r )
{
	const Matrix4Unaligned result = { ToVector4Unaligned(r.getCol0()), ToVector4Unaligned(r.getCol1()), ToVector4Unaligned(r.getCol2()), ToVector4Unaligned(r.getCol3()) };
	return result;
}

#endif
