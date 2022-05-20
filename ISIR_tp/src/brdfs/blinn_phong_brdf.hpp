#ifndef __RT_ISICG_BRDF_BLINN__PHONG__
#define __RT_ISICG_BRDF_BLINN__PHONG__
#include "defines.hpp"
namespace RT_ISICG
{
	class BlinnPhongBRDF
	{
	  public:
		BlinnPhongBRDF( const Vec3f & p_ks, int p_s ) : _ks( p_ks ), _s( p_s ) {};
		inline Vec3f evaluate( const Vec3f & p_wo, const Vec3f & p_wi, const Vec3f & p_normal ) const
		{
			Vec3f h			= glm::normalize( -p_wo + p_wi );
			float cos_alpha = glm::max( dot( h, p_normal ), 0.0f );

			return _ks * glm::pow( cos_alpha, float( _s ) );
		}
		inline const Vec3f & getKS() const { return _ks; }

	  private:
		Vec3f _ks = WHITE;
		int	  _s  = 0;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_BLINN__PHONG__