#ifndef __RT_ISICG_TRIANGLE_GEOMETRY__
#define __RT_ISICG_TRIANGLE_GEOMETRY__
#include "base_geometry.hpp"
#include "aabb.hpp"
namespace RT_ISICG
{
	class MeshTriangle;
	class TriangleMeshGeometry : public BaseGeometry
	{
	  public:
		TriangleMeshGeometry()			= delete;
		virtual ~TriangleMeshGeometry() = default;
		TriangleMeshGeometry( const unsigned int p_v0,
							  const unsigned int p_v1,
							  const unsigned int p_v2,
							  MeshTriangle *	 p_refMesh );
		bool				 intersect( const Ray & p_ray, float & p_t )const;
		inline const Vec3f & getFaceNormal() const { return _faceNormal; }
		const AABB  getAabb() { return _aabb; }
		Vec3f& getCentroid() { return _centroid;}
		 
	  private:
		MeshTriangle * _refMesh;
		union
		{
			struct
			{
				unsigned int _v0, _v1, _v2;
			};
			unsigned int _v[ 3 ] = { 0, 0, 0 };
		};
		Vec3f _centroid;
		 Vec3f _faceNormal = VEC3F_ZERO;
		AABB  _aabb;
	};
} // namespace RT_ISICG
#endif // __RT_ISICG_TRIANGLE_GEOMETRY__