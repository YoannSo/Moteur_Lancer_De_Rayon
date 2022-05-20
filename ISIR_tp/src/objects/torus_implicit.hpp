#ifndef __RT_ISICG_TORUS__IMPLICIT__
#define __RT_ISICG_TORUS__IMPLICIT__

#include "implicit_surface.hpp"
namespace RT_ISICG
{
	class TorusImplicit : public ImplicitSurface
	{
	  public:
		TorusImplicit( const std::string p_name, const Vec3f & p_center, Vec2f p_radius )
			: ImplicitSurface( p_name ), _radius( p_radius ), _center( p_center )
		{
		}

		float _sdf( const Vec3f & p_point ) const
		{
			Vec3f point	   = p_point - _center;
			rotateY( point, -PIf /3 );

			Vec2f pointXZ  = Vec2f( point.x, point.z );

			Vec2f q = Vec2f( glm::length( pointXZ ) - _radius.x, point.y );

			return glm::length( q ) - _radius.y;
		}

	  private:
		Vec2f _radius;
		Vec3f _center;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE__IMPLICIT__