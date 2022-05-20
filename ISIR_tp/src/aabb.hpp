#ifndef __RT_ISICG_AABB__
#define __RT_ISICG_AABB__
#include "ray.hpp"
#include <vector>

namespace RT_ISICG
{
	struct AABB
	{
	  public:
		AABB() = default;
		AABB( const Vec3f & p_min, const Vec3f & p_max ) : _min( p_min ), _max( p_max ) {}
		AABB( const Vec3f & p_firstVertices ) : _min( p_firstVertices ), _max( p_firstVertices ) {};

		~AABB() = default;
		inline const Vec3f & getMin() const { return _min; }
		inline const Vec3f & getMax() const { return _max; }
		// Returns true if the AABB is degenerated.

		inline bool isValid() const { return ( ( _min.x <= _max.x ) && ( _min.y <= _max.y ) && ( _min.z <= _max.z ) ); }
		// Extends the AABB with a point
		inline void extend( const Vec3f & p_point )
		{
			_min = glm::min( p_point, _min );
			_max = glm::max( p_point, _max );
		}
		// Extends the AABB with another AABB
		inline void extend( const AABB & p_aabb )
		{
			_min = glm::min(p_aabb.getMin(),_min);
			_max = glm::max(p_aabb.getMax(),_max);
		}
		inline bool containPoint( const Vec3f & p_point )
		{
			if ( p_point.x >= _min.x && p_point.y >= _min.y && p_point.z >= _min.z )
				if ( p_point.x <= _max.x && p_point.y <= _max.y && p_point.z <= _max.z ) return true;
			return false;
		}
		inline float getArea()
		{
			float tailleX = _max.x - _min.x;
			float tailleY = _max.y - _min.y;
			float tailleZ = _max.y - _min.y;
			return 2 * tailleX * tailleY + 2 * tailleX * tailleZ + 2 * tailleY * tailleZ;
		}
		// Returns the AABB diagonal vector.
		inline Vec3f diagonal() const { return _max - _min; }
		// Returns the AABB centroid.
		inline Vec3f centroid() const { return ( _min + _max ) * 0.5f; }
		// Returns the largest axis, 0 -> x, 1 -> y, 2 -> z
		inline size_t largestAxis() const
		{
			const Vec3f d = diagonal();
			if ( d.x > d.y && d.x > d.z )
				return 0;
			else if ( d.y > d.z )
				return 1;
			else
				return 2;
		}
		bool intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const;

	  private:
		Vec3f _min = Vec3f( FLT_MAX );
		Vec3f _max = Vec3f( -FLT_MAX );
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_AABB__