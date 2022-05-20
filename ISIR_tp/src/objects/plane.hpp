#ifndef __RT_ISICG_PLANE__
#define __RT_ISICG_PLANE__

#include "base_object.hpp"
#include "geometry/plane_geometry.hpp"
namespace RT_ISICG
{
	class Plane : public BaseObject
	{
	  public:
		Plane() = delete;
		virtual ~Plane() = default;
			
			Plane( const std::string & p_name, const Vec3f & p_point, const Vec3f & p_normal )
			: BaseObject( p_name ), _geometry( p_point, p_normal )
		{
		}
		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;
		private: 
			PlaneGeometry _geometry;

	};
} // namespace RT_ISICG
#endif
