#ifndef __RT_ISICG_BRDF_OREN_NAYAR__
#define __RT_ISICG_BRDF_OREN_NAYAR__
#include "defines.hpp"
namespace RT_ISICG
{
	class OrenNayarBRDF
	{
	  public:
		OrenNayarBRDF( const float p_sigma, const Vec3f p_kd ) : _kd( p_kd )
		{
			_A = 1 - ( 0.5 * ( ( p_sigma * p_sigma ) / ( p_sigma * p_sigma + 0.33 ) ) );
			_B = ( 0.45 * ( ( p_sigma * p_sigma ) / ( p_sigma * p_sigma + 0.09 ) ) );
		};
		// * INV_PIf : could be done in the constructor...
		inline Vec3f evaluate( const Vec3f & p_wi, const Vec3f & p_wo, const Vec3f & p_normal ) const
		{
			return VEC3F_ZERO;

			float costhetaI = glm::dot( p_wi, p_normal );
			float thetaI	= glm::acos( costhetaI );
			float costhetaO = glm::dot( p_wo, p_normal );
			float thetaO	= glm::acos( costhetaO );
			float _alpha	= glm::max( costhetaI, costhetaI );
			float _beta		= glm::min( costhetaI, costhetaI );

			Vec3f projectionWi	   = glm::normalize( p_wi - ( costhetaI * p_normal ) );
			Vec3f projectionNormal = glm::cross( p_normal, p_wi );
			Vec3f projectionWo	   = glm::normalize( p_wo - ( costhetaO * p_normal ) );
			float phi_wi		   = glm::dot( projectionNormal, projectionWi );
			float phi_wo		   = glm::dot( projectionNormal, projectionWo );

			
			return ( _kd * INV_PIf )
				   * ( _A
					   + ( _B * glm::max( 0.f, glm::cos( phi_wi - phi_wo ) ) ) * glm::sin( _alpha )
							 * glm::tan( _beta ) );
		}

	  private:
		Vec3f _kd;
		float _A = 0.f;
		float _B = 0.f;
	};
} // namespace RT_ISICG
#endif 