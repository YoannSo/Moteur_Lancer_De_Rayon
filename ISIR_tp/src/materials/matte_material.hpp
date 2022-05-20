#ifndef __RT_ISICG_MATTE_MATERIAL__
#define __RT_ISICG_Matte_MATERIAL__

#include "materials/base_material.hpp"
#include "brdfs/oren_nayar_brdf.hpp"
namespace RT_ISICG
{
	class MatteMaterial : public BaseMaterial
	{
	  public:
		MatteMaterial( const std::string & p_name,
						 const Vec3f &		 p_diffuse)
			: BaseMaterial( p_name )
		{
		}

		virtual ~MatteMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return VEC3F_ZERO;
		}

		inline const Vec3f & getFlatColor() const override { return VEC3F_ZERO; }

	  protected:
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_LAMBERT_MATERIAL__
