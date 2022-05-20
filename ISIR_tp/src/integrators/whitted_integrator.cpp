#include "integrators/whitted_integrator.hpp"
namespace RT_ISICG
{
		Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			Vec3f Li = trace( p_scene, p_ray, p_tMin, p_tMax,0,1.f );
			// direct lighting envoie la couleur du pix, donc recursion a faire a part, herite de directLightintegrator comme ca _directLightning est le meme
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f WhittedIntegrator::trace(	const Scene & p_scene,
												const Ray &	p_ray,
												const float	p_tMin,
												const float	 p_tMax,
												int			 p_nbBounce,float n1_ior ) const
	{

		HitRecord hitRecord;
		if ( p_nbBounce > _nbBounces ) return BLACK;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				
				Ray		  reflectRay = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				HitRecord reflectHitRecord;
				reflectRay.offset( hitRecord._normal );
				if ( p_scene.intersect( reflectRay, p_tMin, p_tMax, reflectHitRecord ) )
				{
					return trace( p_scene, reflectRay, p_tMin, p_tMax, p_nbBounce + 1, n1_ior );
				}
			}
			else if ( hitRecord._object->getMaterial()->isTransparent() )
			{
				Vec3f  reflect	 = Vec3f( 0.f );
				float	cosi	   = glm::dot( hitRecord._normal, -p_ray.getDirection() );
	
				float n2_ior;

				if ( n1_ior != 1.f )
					n2_ior = 1.f;
				else
					n2_ior = 1.3f;

				float eta = n1_ior / n2_ior;

				Ray		  reflectRay = Ray( hitRecord._point, glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				HitRecord reflectHitRecord;
				reflectRay.offset( hitRecord._normal );
				reflect = trace( p_scene, reflectRay, p_tMin, p_tMax, p_nbBounce + 1, n1_ior );
				
				
				
				Vec3f vec_refract = glm::refract( p_ray.getDirection(), hitRecord._normal, eta );
				float cost = glm::dot( vec_refract, -hitRecord._normal );
				float R			= fresnel( n1_ior, n2_ior, cosi, cost );
				
				if ( R!=1 )
				{
					Ray ray_refraction = Ray( hitRecord._point, vec_refract );
					ray_refraction.offset( -hitRecord._normal );
					Vec3f refraction_color = trace( p_scene, ray_refraction, p_tMin, p_tMax, p_nbBounce + 1, n2_ior );

					return R * reflect + ( 1.f - R ) * refraction_color;
				}
				else
				{
					return reflect;
				}
			}

			else
			{
				return _directLightning( p_scene, p_ray, hitRecord, p_tMin, p_tMax );
			}
		}
			
	}
	float WhittedIntegrator::fresnel( const float n1, const float n2, const float cosi, const float cost )const
	{
	

		float Rs   = (  n1 * cosi  -  n2 * cost  ) / (  n1 * cosi  +  n2 * cost  );
		float Rp = (  n1 * cost  -  n2 * cosi  ) / (  n1 * cost  +  n2 * cosi  );

		return ( Rs * Rs + Rp * Rp ) * 0.5f;
	} 
	
}
	
// namespace RT_ISICG