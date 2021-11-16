#include "stdafx.h"
#include "Transform.h"

namespace Pitbull
{
namespace math
{
	Transform::Transform() : Transform{ 0.f, 0.f, 0.f } {}

	Transform::Transform(float _x, float _y, float _z) : x{ _x }, y{ _y }, z{ _z } {}

	Transform::Transform(const Transform& Transform) = default;
} // namespace math
} // namespace Pitbull