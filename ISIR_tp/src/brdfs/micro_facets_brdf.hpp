#ifndef __RT_ISICG_BRDF_MICROFACET__
#define __RT_ISICG_BRDF_MICROFACET__
#include "defines.hpp"
namespace RT_ISICG
{
	class MicrofacetBRDF
	{
	  public:
		MicrofacetBRDF( const float & p_rugosite,const Vec3f p_F0 ) : _rugosite( p_rugosite ), _F0( p_F0 ) {};
		inline Vec3f evaluate( const Vec3f & p_normal, const Vec3f & p_wi, const Vec3f & p_wo ) const
		{
			Vec3f h		   = glm::normalize( p_wi + p_wo );
			Vec3f n	   = p_normal;
			float D		   = getD( h, n );
			float G		   = getG( p_wi, p_wo, n );
			Vec3f fresnel  = getFresnel( h, n, p_wo );
			Vec3f specular = ( D * fresnel * G ) / ( 4 * glm::dot( p_wo, n ) * glm::dot( p_wi, n ) );
			return specular;
		}

	  private:
		float getD( const Vec3f & p_h, const Vec3f & p_normal ) const
		{
			float alpha2			  = ( _rugosite * _rugosite ) * ( _rugosite * _rugosite );
			float normalProportionDot = glm::dot( p_normal, p_h );
			float denominateur		  = ( ( normalProportionDot * normalProportionDot ) * ( alpha2 - 1.f ) + 1.f );
			float denominateur2		  = PIf * ( denominateur * denominateur );
			return alpha2 / denominateur2;
		}
		float getG( const Vec3f & p_wo, const Vec3f p_wi, const Vec3f p_normal ) const
		{
			float k	 = ( ( _rugosite + 1 ) * ( _rugosite + 1 ) ) / 8;
			float xo = glm::dot( p_wo, p_normal );
			float xi = glm::dot( p_wi, p_normal );

			float gO = xo / ( xo * ( 1 - k ) + k );
			float gI = xi / ( xi * ( 1 - k ) + k );

			return gO * gI;
		}
		Vec3f getFresnel( const Vec3f & p_h, const Vec3f & p_normal, const Vec3f & p_wo ) const
		{
			return _F0 + ( 1.f - _F0 ) * glm::pow( 1.f - glm::dot( p_h, p_wo ), 5.f );
		}

		float _rugosite = 0.f;
		Vec3f _F0		= VEC3F_ZERO;

	};
} // namespace RT_ISICG
#endif 