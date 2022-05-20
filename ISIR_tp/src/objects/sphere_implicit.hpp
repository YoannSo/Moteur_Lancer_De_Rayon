#ifndef __RT_ISICG_SPHERE__IMPLICIT__
#define __RT_ISICG_SPHERE__IMPLICIT__

#include "implicit_surface.hpp"
namespace RT_ISICG
{
	class SphereImplicit : public ImplicitSurface
	{
	  public:
		SphereImplicit( const std::string p_name, const Vec3f & p_center, const float & p_radius )
			: ImplicitSurface( p_name ), _radius( p_radius ), _center( p_center )
		{
		}

		float _sdf( const Vec3f & p_point ) const { return glm::distance( p_point, _center ) - _radius; }

	  private:
		float _radius = 1.f;
		Vec3f _center = Vec3f( 0.f, 1.f, 1.f );
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_SPHERE__IMPLICIT__
