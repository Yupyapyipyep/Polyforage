#pragma once

#include "Types.h"
#include <math.h>

#define M_PI 3.1415926535897932
#define M_DEG_TO_RAD(deg) deg/180 * M_PI
#define M_RAD_TO_DEG(rad) rad/M_PI * 180

union v2
{
	v2() :
		x(0.0f), y(0.0f)
	{}

	v2(r32 x, r32 y) :
		x(x), y(y)
	{}

	struct
	{
		r32 x;
		r32 y;
	};

	struct
	{
		r32 u;
		r32 v;
	};

	inline v2 operator-(const v2& vec) const { return v2(x - vec.x, y - vec.y); }
};

struct quat;
union v3
{
	v3() :
		x(0.0f), y(0.0f), z(0.0f)
	{}

	v3(r32 x, r32 y, r32 z) :
		x(x), y(y), z(z)
	{}

	inline r32 Dot(const v3& vec) const { return x * vec.x + y * vec.y + z * vec.z; }
	inline r32 Length() const { return sqrtf(Dot(*this)); }
	inline v3 Cross(const v3& vec) const { return v3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }

	inline v3 Normalized() const
	{
		r32 len = Length();
		return v3(x / len, y / len, z / len);
	}

	inline v3 Rotate(const v3& axis, r32 deg) const
	{
		r32 rad = M_DEG_TO_RAD(deg);
		r32 sinAngle = sinf(-rad);
		r32 cosAngle = cosf(-rad);
		return Cross(axis * sinAngle) + (*this * cosAngle) + (axis * Dot(axis * (1.0f - cosAngle)));
	}

	v3 Rotate(const quat& rot) const;

	inline v3 operator*(r32 scaler)	   const { return v3(x * scaler, y * scaler, z * scaler); }
	inline v3 operator*(const v3& vec) const { return v3(x * vec.x, y * vec.y, z * vec.z); }
	inline v3 operator+(const v3& vec) const { return v3(x + vec.x, y + vec.y, z + vec.z); }
	inline v3 operator-(const v3& vec) const { return v3(x - vec.x, y - vec.y, z - vec.z); }

	inline v3& operator+=(const v3& vec) { x += vec.x; y += vec.y; z += vec.z; return *this; }
	inline v3& operator*=(r32 scaler) { x *= scaler; y *= scaler; z *= scaler; return *this; }
	inline v3& operator*=(const v3& vec) { x *= vec.x; y *= vec.y; z *= vec.z; return *this; }

	struct
	{
		r32 x;
		r32 y;
		r32 z;
	};

	struct
	{
		r32 r;
		r32 g;
		r32 b;
	};
};

union m4
{
	m4() :
		m00(0.0f), m01(0.0f), m02(0.0f), m03(0.0f),
		m10(0.0f), m11(0.0f), m12(0.0f), m13(0.0f),
		m20(0.0f), m21(0.0f), m22(0.0f), m23(0.0f),
		m30(0.0f), m31(0.0f), m32(0.0f), m33(0.0f)
	{}

	m4(r32 m00, r32 m01, r32 m02, r32 m03,
	   r32 m10, r32 m11, r32 m12, r32 m13,
	   r32 m20, r32 m21, r32 m22, r32 m23,
	   r32 m30, r32 m31, r32 m32, r32 m33) :
		m00(m00), m01(m01), m02(m02), m03(m03),
		m10(m10), m11(m11), m12(m12), m13(m13),
		m20(m20), m21(m21), m22(m22), m23(m23),
		m30(m30), m31(m31), m32(m32), m33(m33)
	{}

	struct
	{
		r32 m00; r32 m01; r32 m02; r32 m03;
		r32 m10; r32 m11; r32 m12; r32 m13;
		r32 m20; r32 m21; r32 m22; r32 m23;
		r32 m30; r32 m31; r32 m32; r32 m33;
	};

	struct
	{
		r32 m[4][4];
	};

	inline r32* operator[](u32 index) { return m[index]; }
	inline const r32* operator[](u32 index) const { return m[index]; }

	inline m4 operator*(const m4& mat) const
	{
		m4 res;
		for (u32 i = 0; i < 4; i++)
		{
			for (u32 j = 0; j < 4; j++)
			{
				res[i][j] = m[i][0] * mat[0][j] +
					m[i][1] * mat[1][j] +
					m[i][2] * mat[2][j] +
					m[i][3] * mat[3][j];
			}
		}
		return res;
	}

	inline void InitIdentity()
	{
		for (u32 i = 0; i < 4; i++)
			for (u32 j = 0; j < 4; j++)
				m[i][j] = i == j ? 1.0f : 0.0f;
	}
	
	inline m4 Transposed() const
	{
		m4 res;
		for (u32 i = 0; i < 4; i++)
			for (u32 j = 0; j < 4; j++)
				res[i][j] = m[j][i];
		return res;

	}

	inline static m4 CreateIdentity()
	{
		return m4(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateTranslation(const v3& translation)
	{
		return m4(1.0f, 0.0f, 0.0f, translation.x,
				  0.0f, 1.0f, 0.0f, translation.y,
				  0.0f, 0.0f, 1.0f, translation.z,
				  0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateScale(const v3& scale)
	{
		return m4(scale.x, 0.0f, 0.0f, 0.0f,
				  0.0f, scale.y, 0.0f, 0.0f,
				  0.0f, 0.0f, scale.z, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateRotationX(r32 deg)
	{
		r32 rad = M_DEG_TO_RAD(deg);
		r32 cosAngle = cosf(deg);
		r32 sinAngle = sinf(deg);

		return m4(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, cosAngle, -sinAngle, 0.0f,
				  0.0f, sinAngle, cosAngle, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateRotationY(r32 deg)
	{
		r32 rad = M_DEG_TO_RAD(deg);
		r32 cosAngle = cosf(deg);
		r32 sinAngle = sinf(deg);

		return m4(cosAngle, 0.0f, -sinAngle, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  sinAngle, 0.0f, cosAngle, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateRotationZ(r32 deg)
	{
		r32 rad = M_DEG_TO_RAD(deg);
		r32 cosAngle = cosf(deg);
		r32 sinAngle = sinf(deg);

		return m4(cosAngle, -sinAngle, 0.0f, 0.0f,
			sinAngle, cosAngle, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateRotation(const v3& deg)
	{
		return CreateRotationX(deg.x) * (CreateRotationY(deg.y) * CreateRotationZ(deg.z));
	}

	inline static m4 CreateRotation(const v3& forward, const v3& up, const v3& right)
	{
		return m4(right.x, right.y, right.z, 0.0f,
			up.x, up.y, up.z, 0.0f,
			forward.x, forward.y, forward.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreateRotation(const v3& forward, const v3& up)
	{
		v3 right = up.Cross(forward);
		v3 newUp = forward.Cross(right);

		return CreateRotation(forward, newUp, right);
	}

	inline static m4 CreateTransformation(const v3& pos, const v3& scale, const v3& rot)
	{
		return CreateTranslation(pos) * (CreateRotation(rot) * CreateScale(scale));
	}

	inline static m4 CreateOrthographic(r32 left, r32 right, r32 bottom, r32 top, r32 n, r32 f)
	{
		return m4(2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, -2.0f / (f - n), -(f + n) / (f - n),
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	inline static m4 CreatePerspective(r32 width, r32 height, r32 fov, r32 znear, r32 zfar)
	{
		r32 ar = width / height;
		r32 tanHalfFov = tanf(M_DEG_TO_RAD(fov) / 2.0f);
		r32 zrange = znear - zfar;

		return m4(1.0f / (ar * tanHalfFov), 0.0f, 0.0f, 0.0f,
			0.0f, -1.0f / tanHalfFov, 0.0f, 0.0f,
			0.0f, 0.0f, (-znear - zfar) / zrange, 2 * zfar * znear / zrange,
			0.0f, 0.0f, 1.0f, 0.0f);
	}
};

struct quat
{
	inline quat(r32 x, r32 y, r32 z, r32 w) :
		x(x), y(y), z(z), w(w)
	{}

	inline quat() :
		x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{}

	inline quat(const v3& axis, r32 deg)
	{
		r32 halfAngle = M_DEG_TO_RAD(deg) / 2.0f;
		r32 sinHalfAngle = sinf(halfAngle);
		r32 cosHalfAngle = cosf(halfAngle);

		x = axis.x * sinHalfAngle;
		y = axis.y * sinHalfAngle;
		z = axis.z * sinHalfAngle;
		w = cosHalfAngle;
	}

	inline quat(const m4& rotation)
	{
		w = sqrtf(1.0f + rotation[0][0] + rotation[1][1] + rotation[2][2]) / 2.0f;
		r32 w4 = w * 4.0f;
		x = (rotation[2][1] - rotation[1][2]) / w4;
		y = (rotation[0][2] - rotation[2][0]) / w4;
		z = (rotation[1][0] - rotation[0][1]) / w4;
	}

	inline quat Conjugate() const { return quat(-x, -y, -z, w); }
	inline r32 Dot(const quat& q) const { return x * q.x + y * q.y + z * q.z + w * q.w; }
	inline r32 Length() const { return sqrtf(Dot(*this)); }

	inline quat Normalized() const
	{
		r32 len = Length();
		return quat(x / len, y / len, z / len, w / len);
	}

	inline quat Lerp(const quat& q, r32 t) const { return *this * t + q * (1.0f - t); }

	inline quat Slerp(const quat& q, r32 t) const
	{
		quat q3;
		r32 dot = Dot(q);

		if (dot < 0.0f)
		{
			dot = -dot;
			q3 = q * -1.0f;
		}
		else
		{
			q3 = q;
		}

		if (dot < 0.95f)
		{
			r32 angle = acosf(dot);
			return (*this * sinf(angle + (1.0f - t)) + q3 * sinf(angle * t)) * (1.0f / sinf(angle));
		}
		else
		{
			return Lerp(q3, t);
		}
	}

	inline m4 CreateRotationMatrix() const
	{
		v3 forward(2.0f * (x * z - w * y), 2.0f * (y * z + w * x), 1.0f - 2.0f * (x * x + y * y));
		v3 up(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z), 2.0f * (y * z - w * x));
		v3 right(1.0f - 2.0f * (y * y + z * z), 2.0f * (x * y - w * z), 2.0f * (x * z + w * y));

		return m4::CreateRotation(forward, up, right);
	}

	inline v3 GetForward() const { return v3(0, 0, 1).Rotate(*this); }
	inline v3 GetBack() const { return v3(0, 0, -1).Rotate(*this); }
	inline v3 GetUp() const { return v3(0, 1, 0).Rotate(*this); }
	inline v3 GetDown() const { return v3(0, -1, 0).Rotate(*this); }
	inline v3 GetRight() const { return v3(1, 0, 0).Rotate(*this); }
	inline v3 GetLeft() const { return v3(-1, 0, 0).Rotate(*this); }

	inline quat operator+(const quat& q) const { return quat(x + q.x, y + q.y, z + q.z, w + q.w); }
	inline quat operator*(r32 scaler) const { return quat(x * scaler, y * scaler, z * scaler, w * scaler); }

	inline quat operator*(const quat& q) const
	{
		return quat(x * q.w + w * q.x + y * q.z - z * q.y,
			y * q.w + w * q.y + z * q.x - x * q.z,
			z * q.w + w * q.z + x * q.y - y * q.x,
			w * q.w - x * q.x - y * q.y - z * q.z);
	}

	inline quat operator*(const v3& vec) const
	{
		return quat(w * vec.x + y * vec.z - z * vec.y,
			w * vec.y + z * vec.x - x * vec.z,
			w * vec.z + x * vec.y - y * vec.x,
			-x * vec.x - y * vec.y - z * vec.z);
	}

	r32 x;
	r32 y;
	r32 z;
	r32 w;
};