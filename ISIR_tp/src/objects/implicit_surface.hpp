#ifndef __RT_ISICG_IMPLICIT_SURFACE__
#define __RT_ISICG_IMPLICIT_SURFACE__

#include "base_object.hpp"

namespace RT_ISICG
{
	class ImplicitSurface : public BaseObject
	{
	  public:
		ImplicitSurface()		   = delete;
		virtual ~ImplicitSurface() = default;

		ImplicitSurface( const std::string & p_name ) : BaseObject( p_name ) {}

		// Check for nearest intersection between p_tMin and p_tMax : if found fill p_hitRecord.
		virtual bool intersect( const Ray & p_ray,
								const float p_tMin,
								const float p_tMax,
								HitRecord & p_hitRecord ) const override;

		// Check for any intersection between p_tMin and p_tMax.
		virtual bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

	  protected:
		inline void rotateX( Vec3f & p_point, float p_rot ) const
		{
			float py = p_point.y, pz = p_point.z;
			p_point.y = cos( p_rot ) * py - sin( p_rot ) * pz;
			p_point.z = sin( p_rot ) * py + cos( p_rot ) * pz;
		};
		inline void rotateY(Vec3f& p_point, float p_rot)const {
			float px = p_point.x, pz = p_point.z;
			p_point.x = cos( p_rot ) * px + sin( p_rot ) * pz;
			p_point.z = -sin( p_rot ) * px + cos( p_rot ) * pz;
		}
		inline void rotateZ(Vec3f& p_point, float p_rot) const {
			float px = p_point.x, py = p_point.y;
			p_point.x = cos( p_rot ) * px - sin( p_rot ) * py;
			p_point.y = sin( p_rot ) * px + cos( p_rot ) * py;
		}

	  private:
		// Signed Distance Function
		virtual float _sdf( const Vec3f & p_point ) const = 0;


		// Evaluate normal by computing gradient at 'p_point'
		virtual Vec3f _evaluateNormal( const Vec3f & p_point ) const
		{
			float e = 0.0001f; // pas FLT_EPSILON car trop petit
			return glm::normalize( Vec3f( _sdf( Vec3f( p_point.x + e, p_point.y, p_point.z ) )
											  - _sdf( Vec3f( p_point.x - e, p_point.y, p_point.z ) ),
										  _sdf( Vec3f( p_point.x, p_point.y + e, p_point.z ) )
											  - _sdf( Vec3f( p_point.x, p_point.y - e, p_point.z ) ),
										  _sdf( Vec3f( p_point.x, p_point.y, p_point.z + e ) )
											  - _sdf( Vec3f( p_point.x, p_point.y, p_point.z - e ) ) ) );
		}

	  private:
		const float _minDistance = 1e-4f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SURFACE__
