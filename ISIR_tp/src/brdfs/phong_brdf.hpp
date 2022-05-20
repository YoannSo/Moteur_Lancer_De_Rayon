#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__
#include "defines.hpp"
namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks,int p_s ) : _ks( p_ks ),_s(p_s) {};
		inline Vec3f evaluate(const Vec3f& p_normal,const Vec3f& p_wi) const
		{
			const Vec3f wr	  = glm::reflect( p_wi, p_normal );
			float		alpha = glm::dot( p_wi, wr );
			return _ks * (float)glm::pow( alpha, _s );
		;
		}
		inline const Vec3f & getKS() const { return _ks; }

	  private:
		Vec3f _ks = WHITE;
		int	  _s	 = 0;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_BRDF_PHONG__