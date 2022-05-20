#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/blinn_phong_brdf.hpp"
namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name,
						 const Vec3f &		 p_diffuse,
						 const Vec3f &		 p_speculaire,
						 int				 p_s,
						 float				 p_pourcentageDiffuse )
			: BaseMaterial( p_name ), _lambertBRDF( p_diffuse ), _blinnPhongBRDF( p_speculaire, p_s ),
			  _phongBRDF( p_speculaire, p_s ), _pourcentageDiffuse( p_pourcentageDiffuse )
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			Vec3f wh = ( p_ray.getDirection() + ( p_lightSample._direction ) ) * 0.5f;
			Vec3f specular
				= _blinnPhongBRDF.evaluate( p_lightSample._direction, p_ray.getDirection(), p_hitRecord._normal );
			Vec3f specular2 = _phongBRDF.evaluate( p_hitRecord._normal, p_ray.getDirection() );

			return _lambertBRDF.evaluate() * _pourcentageDiffuse + specular * ( 1 - _pourcentageDiffuse );
		}

		inline const Vec3f & getFlatColor() const override { return _lambertBRDF.getKd() + _blinnPhongBRDF.getKS(); }

	  protected:
		LambertBRDF	   _lambertBRDF;
		PhongBRDF	   _phongBRDF;
		BlinnPhongBRDF _blinnPhongBRDF;
		float		   _pourcentageDiffuse;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_LAMBERT_MATERIAL__
