#ifndef __RT_ISICG_DEATH__STAR__IMPLICIT__
#define __RT_ISICG_DEATH__STAR__IMPLICIT__

#include "implicit_surface.hpp"
namespace RT_ISICG
{
	class DeathStar : public ImplicitSurface
	{
	  public:
		DeathStar( const std::string p_name,const Vec3f& p_center, const float p_ra,const float p_rb,const float p_di )
			: ImplicitSurface( p_name ),_center(p_center), _ra( p_ra ), _rb( p_rb ), _di(p_di)
		{
		}

		float _sdf( const Vec3f & p_point ) const { 

			Vec3f point = p_point - _center;
			rotateY( point, PIf*0.5f );

			float a = ( _ra * _ra - _rb * _rb + _di * _di ) / ( 2.f * _di );
			float b = glm::sqrt( glm::max( _ra * _ra - a * a, 0.f ) );

			Vec2f pointYZ  = Vec2f( point.y, point.z );

			Vec2f p = Vec2f( point.x, glm::length( pointYZ ) );
			if ( p.x * b - p.y * a > _di * glm::max( b - p.y, 0.f ) )
				return glm::length( p - Vec2f( a, b ));

			else
				return glm::max( glm::length( p ) - _ra,- glm::length( p - Vec2f( _di, 0 ) ) - _rb );
		}

	  private:
		float _ra;
		float _rb;
		float _di;
		Vec3f _center;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_DEATH__STAR__IMPLICIT__