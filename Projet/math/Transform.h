#pragma once

namespace Pitbull
{
namespace math
{
	class Transform {
	public:
		float x, y, z;

		Transform();
		Transform(float x, float y, float z);
		Transform(const Transform& Transform);
	};
} // namespace math
} // namespace Pitbull
