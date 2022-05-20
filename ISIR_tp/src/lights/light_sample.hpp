#ifndef __RT_ISICG_LIGHT_SAMPLE__
#define __RT_ISICG_LIGHT_SAMPLE__
#include "defines.hpp"

namespace RT_ISICG
{
	struct LightSample
	{
	  public:
		LightSample() = default;
		LightSample( const Vec3f & p_direction, const float p_distance, const Vec3f & p_radiance, const float p_pdf )
			: _direction( p_direction ), _distance( p_distance ), _radiance( p_radiance ), _pdf( p_pdf )
		{
		}

		void  divideAllComponent(const int divider) { 
			 _direction = _direction / Vec3f( (float)divider );
			_radiance  = _radiance / Vec3f( (float)divider );
			_pdf	   = _pdf / (float)divider;
			_distance  = _distance / (float)divider;

		}
		Vec3f _direction = VEC3F_ZERO; // normalized direction toward light source
		float _distance	 = 0.f;
		Vec3f _radiance	 = WHITE; // radiance at given point divided by pdf
		float _pdf		 = 1.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_LIGHT_SAMPLE__
