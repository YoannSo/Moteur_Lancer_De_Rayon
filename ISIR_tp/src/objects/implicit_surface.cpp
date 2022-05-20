#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float t			 = p_tMin;
		float d			 = 0;
		Vec3f currentPos = p_ray.getOrigin();
		while ( t < p_tMax )
		{
			currentPos = p_ray.pointAtT( t );
			d		   = _sdf( currentPos );
			t += d;
			if ( d < _minDistance )
			{
				Vec3f closestPoint	= p_ray.pointAtT( t );
				p_hitRecord._point	= closestPoint;
				p_hitRecord._normal = _evaluateNormal( closestPoint );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = t;
				p_hitRecord._object	  = this;
				return true;
			}
		}
		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t			 = p_tMin;
		float d			 = 0;
		Vec3f currentPos = p_ray.getOrigin();
		Vec3f dir		 = p_ray.getDirection();
		while ( t < p_tMax )
		{
			currentPos = p_ray.pointAtT( t );
			d		   = _sdf( currentPos );
			t += d;
			if ( d < _minDistance ) { return true; }
		}
		return false;
	}

	
} // namespace RT_ISICG
