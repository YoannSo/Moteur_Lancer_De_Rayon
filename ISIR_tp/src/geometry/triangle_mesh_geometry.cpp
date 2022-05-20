#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"
namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
		_aabb		= AABB( _refMesh->_vertices[ p_v0 ] );
		_aabb.extend( _refMesh->_vertices[ p_v1 ] );
		_aabb.extend( _refMesh->_vertices[ p_v2 ] );
		_centroid = ( _refMesh->_vertices[ _v0 ] + _refMesh->_vertices[ _v1 ] + _refMesh->_vertices[ _v2 ] ) / 3.f;
	}
	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t ) const
	{
		const float EPSILON = 0.0000000001f;

		const Vec3f & o	   = p_ray.getOrigin();
		const Vec3f & d	   = p_ray.getDirection();
		const Vec3f & v0   = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1   = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2   = _refMesh->_vertices[ _v2 ];
		
		const Vec3f	  edge1 = v1 - v0;
		const Vec3f	  edge2 = v2 - v0;

		const Vec3f pvec = glm::cross( d, edge2 );
		const float det	 = glm::dot( edge1, pvec );
		
		if ( det > -EPSILON && det < EPSILON ) return false;

		const float inv_det = 1.f / det;
		const Vec3f tvec	= o - v0;
		const Vec3f qvec	= glm::cross( tvec, edge1 );
		
		float u = glm::dot( tvec, pvec ) * inv_det;
		if ( u < 0.f || u > 1.f ) return false;

		float v = glm::dot( d, qvec ) * inv_det;
		if ( v < 0.f || u + v > 1.f ) return false;

		float t = inv_det * glm::dot( edge2, qvec );
		if ( t > EPSILON )
		{
			p_t = t;
			return true;
		}
		
		else
			return false;



	
	}
} // namespace RT_ISICG