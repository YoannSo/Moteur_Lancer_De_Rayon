#ifndef __RT_ISICG_PLANE_GEOMETRY__
#define __RT_ISICG_PLANE_GEOMETRY__

#include "base_geometry.hpp"

namespace RT_ISICG
{
	class PlaneGeometry : public BaseGeometry
	{
	  public:
		PlaneGeometry() = delete;
		virtual ~PlaneGeometry() = default;

		PlaneGeometry( const Vec3f & p_point, const Vec3f & p_normal ) : _point( p_point ), _normal( p_normal ) {}

		inline const Vec3f & getPoint() const { return _point; }
		inline const Vec3f & getNormal() const { return _normal; }

		bool intersect( const Ray & p_ray, float & p_t ) const;

		private : 
			Vec3f _point = VEC3F_ZERO;
			Vec3f _normal			   = VEC3F_ZERO;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_PLANE_GEOMETRY__
