#ifndef __RT_ISICG_MANDEL_BULB__IMPLICIT__
#define __RT_ISICG_MANDEL_BULB__IMPLICIT__

#include "implicit_surface.hpp"
namespace RT_ISICG
{
	class MandelBulb : public ImplicitSurface
	{
	  public:
		MandelBulb( const std::string p_name,
				   const Vec3f &	 p_center )
			: ImplicitSurface( p_name ), _center( p_center )
		{
		}

		float _sdf( const Vec3f & p_point ) const
		{
		
			Vec3f w = p_point;
			float m = dot( w, w );
			float dz = 1.f;
			for ( int i = 0; i < 8; i++ ) {
				 dz= 8.f * glm::pow( m, 3.5f ) * dz + 1.f;
				float r = glm::length( w );
				 float b = 8.f * glm::acos( w.y / r );
				float a = 8.f * glm::atan( w.x, w.z );
				 w		 = p_point+ glm::pow( r, 8.f ) * Vec3f( glm::sin( b ) * glm::sin( a ), glm::cos( b ), glm::sin( b ) * glm::cos( a ) );
				m = dot( w, w );
				 if ( m > 256.f ) break;


			}
			return 0.25f * glm::log( m ) * glm::sqrt( m ) / dz;
		}

	  private:
		Vec3f _center;
	};

} // namespace RT_ISICG

#endif 