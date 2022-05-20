#ifndef __RT_ISICG_COOK_TORRANCE_MATERIAL__
#define __RT_ISICG_COOK_TORRANCE_MATERIAL__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/micro_facets_brdf.hpp"
namespace RT_ISICG
{
	class CookTorrance : public BaseMaterial
	{
	  public:
		CookTorrance( const std::string & p_name,
						 const Vec3f &	p_diffuse,
						 float			p_metalness,
						float		    p_rugosite,
						const Vec3f&    p_F0)
			: BaseMaterial( p_name ), _lambertBRDF( p_diffuse ), _multifacetBRDF(p_rugosite,p_F0),
			  _metalness( p_metalness )
		{
		}

		virtual ~CookTorrance() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			

			return ( 1 - _metalness ) * _lambertBRDF.evaluate() + _metalness * _multifacetBRDF.evaluate(p_hitRecord._normal, -p_lightSample._direction, glm::normalize( -p_ray.getDirection() ) );
		}

		inline const Vec3f & getFlatColor() const override { return _lambertBRDF.getKd(); }

	  protected:
		LambertBRDF _lambertBRDF;
		MicrofacetBRDF _multifacetBRDF;
		float		_metalness;
			
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MULTIFACET_MATERIAL__
