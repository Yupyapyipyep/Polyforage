#pragma once

#include "Math.h"

class Transform
{
public:
	Transform() :
		pos(0.0f, 0.0f, 0.0f),
		scale(1.0f, 1.0f, 1.0f),
		rot(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	Transform(const v3& pos, const v3& scale, const quat& rot) :
		pos(pos),
		scale(scale),
		rot(rot)
	{}

	inline void Translate(const v3& translation)
	{
		pos += translation;
	}

	inline void Translate(const v3& direction, r32 amount)
	{
		pos += direction * amount;
	}

	inline void Scale(const v3& scale)
	{
		this->scale *= scale;
	}

	inline void Scale(r32 scale)
	{
		this->scale *= scale;
	}

	inline void Rotate(const quat& Rot)
	{
		rot = (Rot * rot).Normalized();
	}

	inline void Rotate(const v3& axis, r32 deg)
	{
		Rotate(quat(axis, deg));
	}

	inline m4 CreateTransformMatrix()
	{
		return m4::CreateTranslation(pos) * (m4::CreateScale(scale) * rot.CreateRotationMatrix());
	}

	inline m4 CreateNonScaledTransformMatrix()
	{
		return m4::CreateTranslation(pos) * rot.CreateRotationMatrix();
	}
public:
	v3 pos;
	v3 scale;
	quat rot;
};
