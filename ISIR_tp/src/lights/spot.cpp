#include "lights/spot.hpp"
namespace RT_ISICG
{
	LightSample SpotLight::sample( const Vec3f & p_point ) const
	{
		Vec3f direction			  = VEC3F_ZERO;
		float distance			  = 0.f;
		Vec3f radiance			  = VEC3F_ZERO;
		float pdf				  = 1.f;
		direction				  = glm::normalize( p_point - _position );
		Vec3f directiontoTheLight = glm::normalize( _position - p_point );
		float theta				  = directiontoTheLight.z;
		float fallOff;

		if ( theta < _cosTotalWidth )
			fallOff = 0;
		else if ( theta < _cosFallOfStart )
			fallOff = 1;
		else
		{
			float delta = ( theta - _cosTotalWidth ) / ( _cosFallOfStart - _cosTotalWidth );

			fallOff		= ( delta * delta ) * ( delta * delta );
		}

		distance = glm::distance( p_point, _position );
		radiance = fallOff * _color * _power * glm::pow( 1.f / distance, 2.f );
		return LightSample( direction, distance, radiance, pdf );
	}

} // namespace RT_ISICG