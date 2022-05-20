#include "triangle_mesh.hpp"
namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		 float tClosest = p_tMax;			   // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		if ( _triangles.size() < 8 )
		{
			if ( _intersectAll( p_ray, p_tMin, p_tMax, p_hitRecord ) )
			{
				p_hitRecord._object = this;
				return true;
			}
			return false;
		}
		if( _bvh.intersect( p_ray, p_tMin, p_tMax, p_hitRecord )){

			p_hitRecord._object = this;
			 return true;
		}
		else
			return false;
	}
	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _bvh.intersectAny( p_ray, p_tMin, p_tMax );

	}
	bool MeshTriangle::_intersectAll( const Ray & p_ray,
									  const float p_tMin,
									  const float p_tMax,
									  HitRecord & p_hitRecord ) const
	{
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;

		float  tClosest = p_tMax;			 // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t;
			if ( _triangles[ i ].intersect( p_ray, t ) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					tClosest = t;
					hitTri	 = i;
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			p_hitRecord._point	= p_ray.pointAtT( tClosest );
			p_hitRecord._normal = _triangles[ hitTri ].getFaceNormal();
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool MeshTriangle::_intersectAnyAll( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t;
			if ( _triangles[ i ].intersect( p_ray, t ) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
} // namespace RT_ISICG
  /*#include "triangle_mesh.hpp"
namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		 float tClosest = p_tMax;			   // Hit distance.
		size_t hitTri	= _triangles.size(); // Hit triangle id.
		
		if( _bvh.intersect( p_ray, p_tMin, p_tMax, p_hitRecord )){

			p_hitRecord._object = this;
			 return true;
		}
		else
			return false;
	}
	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _bvh.intersectAny( p_ray, p_tMin, p_tMax );

	}
} // namespace RT_ISICG*/