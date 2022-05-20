#include "direct_lighting_integrator.hpp"
namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			Vec3f Li = _directLightning( p_scene,p_ray, hitRecord, p_tMin, p_tMax );
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f DirectLightingIntegrator::_directLightning( const Scene & p_scene,
													  const Ray & p_ray,
													  HitRecord &	hitRecord,
													  const float	p_tMin,
													  const float	p_tMax ) const
	{
		int	  nbLumierePriseEnCompte = 1;
		int	  _nbLightSample		 = 1;
		Vec3f Li					 = Vec3f( 0.f );
		for ( const BaseLight * currentLight : p_scene.getLights() )
		{
			if ( currentLight->getIsSurface() ) _nbLightSample = 32;

			for ( int j = 0; j < _nbLightSample; j++ )
			{
				LightSample myLightSample = currentLight->sample( hitRecord._point );
				HitRecord	hitRecordForOmbrage;
				Ray			rayIntersectionToLight = Ray( hitRecord._point, -myLightSample._direction );
				rayIntersectionToLight.offset( hitRecord._normal );
				if ( !p_scene.intersectAny( rayIntersectionToLight, SHADOW_EPSILON, myLightSample._distance ) )
				{
					float cosTetha = glm::dot( -myLightSample._direction, hitRecord._normal );
					Li += hitRecord._object->getMaterial()->shade(p_ray,hitRecord,myLightSample) * myLightSample._radiance
						  * glm::max( 0.f, cosTetha );
					nbLumierePriseEnCompte++;
				}
			}
		}
		return Li / (float)(  _nbLightSample );
	}

} // namespace RT_ISICG