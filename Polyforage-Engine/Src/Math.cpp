#include "Math.h"

v3 v3::Rotate(const quat& rot) const
{
	quat conj = rot.Conjugate();
	quat w = rot * (*this) * conj;
	return v3(w.x, w.y, w.z);
}
