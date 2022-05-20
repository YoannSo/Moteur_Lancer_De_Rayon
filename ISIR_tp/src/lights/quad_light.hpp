#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__
#include "base_light.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		virtual ~QuadLight() = default;
		QuadLight( const Vec3f & p_position,
				   const Vec3f & p_u,
				   const Vec3f & p_v,
				   const Vec3f & p_color,
				   const float	 p_power = 1.f )
			: BaseLight( p_color, p_power,true ) ,_position( p_position ), _v( p_v ), _u( p_u )
		{
			_normal = glm::normalize( glm::cross( p_u, p_v ) );
			_area	= glm::length( p_v ) * glm::length( p_u );
		}
		LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO;
		Vec3f _u		= VEC3F_ZERO;
		Vec3f _v		= VEC3F_ZERO;
		Vec3f _normal	= VEC3F_ZERO;
		float _area		= 0.f;
	};
} // namespace RT_ISICG
#endif