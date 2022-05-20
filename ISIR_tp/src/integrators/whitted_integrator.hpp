#ifndef __RT_ISICG_DIRECT_WHITTED_INTERGRATOR__
#define __RT_ISICG_DIRECT_WHITTED_INTERGRATOR__
#include "direct_lighting_integrator.hpp"
namespace RT_ISICG
{
	class WhittedIntegrator : public DirectLightingIntegrator
	{
	  public:
		WhittedIntegrator() : DirectLightingIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::WHITTED_INTEGRATOR; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

		private:
		Vec3f trace( const Scene & p_scene,
					 const Ray &   p_ray,
					 const float   p_tMin,
					 const float   p_tMax,
					 int		   p_nbBounce,float n1_ior ) const;
		  float fresnel( const float n1, const float n2, const float cosi, const float cost ) const;

		 const int _nbBounces = 5;
	};
} // namespace RT_ISICG
#endif