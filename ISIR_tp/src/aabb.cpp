#include "aabb.hpp"

namespace RT_ISICG
{
	
	
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float tmin = ( _min.x - p_ray.getOrigin().x ) / p_ray.getDirection().x;
		float tmax = ( _max.x - p_ray.getOrigin().x ) / p_ray.getDirection().x;

		if ( tmin > tmax ) std::swap( tmin, tmax );

		float tymin = ( _min.y - p_ray.getOrigin().y ) / p_ray.getDirection().y;
		float tymax = ( _max.y - p_ray.getOrigin().y ) / p_ray.getDirection().y;

		if ( tymin > tymax ) std::swap( tymin, tymax );

		if ( ( tmin > tymax )||( tymin > tmax ) ) return false;

		if ( tymin > tmin ) tmin = tymin;

		if ( tymax < tmax ) tmax = tymax;

		float tzmin = ( _min.z - p_ray.getOrigin().z ) / p_ray.getDirection().z;
		float tzmax = ( _max.z - p_ray.getOrigin().z ) / p_ray.getDirection().z;

		if ( tzmin > tzmax ) std::swap( tzmin, tzmax );

		if ( ( tmin > tzmax )||( tzmin > tmax ) ) return false;

		if ( tzmin > tmin ) tmin = tzmin;

		if ( tzmax < tmax ) tmax = tzmax;

		return true;
	}
} // namespace RT_ISICG