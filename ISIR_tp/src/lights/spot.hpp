#ifndef __RT_ISICG_SPOT_LIGHT_LIGHT__
#define __RT_ISICG_SPOT_LIGHT_LIGHT__
#include "base_light.hpp"
namespace RT_ISICG
{
	class SpotLight : public BaseLight
	{
	  public:
		virtual ~SpotLight() = default;
		SpotLight( const Vec3f & p_position,
				   const Vec3f & p_color,
				   const float	 p_power,
				   const float	 p_totalWidth,
				   const float	 cosFallOfStart )
			: BaseLight( p_color, p_power, false ), _position( p_position ), _cosTotalWidth( p_totalWidth ),
			  _cosFallOfStart( cosFallOfStart )
		{
		}
		LightSample			 sample( const Vec3f & p_point ) const override;
		inline const Vec3f & getPosition() const { return _position; }

	  private:
		Vec3f		_position = VEC3F_ZERO;
		const float _cosTotalWidth;
		const float _cosFallOfStart;
	};
} // namespace RT_ISICG
#endif