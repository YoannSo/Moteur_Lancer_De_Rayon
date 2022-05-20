#include "point_light.hpp"
namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point ) const { 
		Vec3f direction = VEC3F_ZERO;
		float distance	= 0.f;
		Vec3f radiance	= VEC3F_ZERO;
		float pdf		= 1.f;

		distance = glm::distance( p_point, _position );

		radiance = _color * _power *  glm::pow(1.f/distance,2.f);

		direction = glm::normalize(p_point-_position);
		return LightSample(direction,distance,radiance,pdf);
	}
}