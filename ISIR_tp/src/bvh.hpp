#ifndef __RT_ISICG_BVH__
#define __RT_ISICG_BVH__

#include "aabb.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "hit_record.hpp"
#include <vector>

namespace RT_ISICG
{
	class TriangleMeshGeometry;

	/* COMPARATEURS */
	struct comparateurMillieuX
	{
		float millieu;
		bool  operator()( TriangleMeshGeometry t1, TriangleMeshGeometry t2 ) const
		{
			// return t1.getVecMin().x < millieu && t2.getVecMin().x > millieu;
			// return t1.getVecMin().x < t2.getVecMin().x;
			return t1.getCentroid().x < t2.getCentroid().x;
		}
	};

	struct comparateurMillieuY
	{
		float millieu;
		bool  operator()( TriangleMeshGeometry t1, TriangleMeshGeometry t2 ) const
		{
			// return t1.getVecMin().y < millieu && t2.getVecMin().y > millieu;
			// return t1.getVecMin().y < t2.getVecMin().y;
			return t1.getCentroid().y < t2.getCentroid().y;
		}
	};

	struct comparateurMillieuZ
	{
		float millieu;
		bool  operator()( TriangleMeshGeometry t1, TriangleMeshGeometry t2 ) const
		{
			// return t1.getVecMin().z < millieu && t2.getVecMin().z > millieu;
			// return t1.getVecMin().z < t2.getVecMin().z;
			return t1.getCentroid().z < t2.getCentroid().z;
		}
	};

	struct BVHNode
	{
		BVHNode() = default;
		~BVHNode()
		{
			delete _left;
			delete _right;
		}
		bool isLeaf() const { return ( _left == nullptr && _right == nullptr ); }

		AABB		 _aabb;
		BVHNode *	 _left			  = nullptr;
		BVHNode *	 _right			  = nullptr;
		unsigned int _firstTriangleId = 0;
		unsigned int _lastTriangleId  = 0;
	};

	class BVH
	{
	  public:
		BVH() = default;

		~BVH() { delete _root; }

		// Build the BVH from a list of triangles (call _buildRec).
		void build( std::vector<TriangleMeshGeometry> * p_triangles );

		// Search for the nearest intersection with the ray (call _intersectRec).
		bool intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const;
		// Search for the any intersection with the ray (call _intersectRec).
		bool intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const;


	  private:
		void _buildRec( BVHNode *		   p_node,
						const unsigned int p_firstTriangleId,
						const unsigned int p_nbTriangles,
						const unsigned int p_depth );
		void _buildRecSAH( BVHNode *			 p_node,
						  const unsigned int p_firstTriangleId,
						  const unsigned int p_nbTriangles,
						  const unsigned int p_depth );


		bool _intersectRec( const BVHNode * p_node,
							const Ray &		p_ray,
							const float		p_tMin,
							const float		p_tMax,
							HitRecord &		p_hitRecord ) const;

		bool _intersectAnyRec( const BVHNode * p_node,
							   const Ray &	   p_ray,
							   const float	   p_tMin,
							   const float	   p_tMax ) const;

	  private:
		std::vector<TriangleMeshGeometry> * _triangles = nullptr;
		BVHNode *							_root	   = nullptr;

		const unsigned int _maxTrianglesPerLeaf = 8;
		const unsigned int _maxDepth			= 32;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BVH__
