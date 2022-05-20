#include "quad_light.hpp"
#include "utils/random.hpp"

	namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f direction = VEC3F_ZERO;
		float distance	= 0.f;
		Vec3f radiance	= VEC3F_ZERO;
		float pdf		= 1.f;


		float posRandV = randomFloat();
		float posRandU = randomFloat();
		Vec3f posInQuad = _position + ( posRandU * _u + posRandV * _v );

		distance  = glm::distance( p_point, posInQuad );
		direction = glm::normalize( p_point - posInQuad );
		pdf = ( 1 / _area ) * ( ( distance * distance ) / glm::dot( _normal, -direction ) );
		radiance = (_color * _power) / pdf;

		return LightSample( direction, distance, radiance, pdf );
	}
} // namespace RT_ISICG