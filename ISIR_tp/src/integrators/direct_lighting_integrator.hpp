#ifndef __RT_ISICG_DIRECT_LIGHTING_INTERGRATOR__
#define __RT_ISICG_DIRECT_LIGHTING_INTERGRATOR__
#include "base_integrator.hpp"
namespace RT_ISICG
{
	class DirectLightingIntegrator : public BaseIntegrator
	{
	  public:
		DirectLightingIntegrator() : BaseIntegrator() {}
		virtual ~DirectLightingIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::POINT_LIGHT; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax )const override;


	  protected:
		Vec3f _directLightning( const Scene &	  p_scene,
								const Ray & p_ray,
								HitRecord &		hitRecord,
								const float		p_tMin,
								const float		p_tMax  ) const;

	};
} // namespace RT_ISICG
#endif