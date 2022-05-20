#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"
#include <algorithm>

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();
		if ( _triangles->size() <= _maxTrianglesPerLeaf )
		{
			_root->_left			= nullptr;
			_root->_right			= nullptr;
			_root->_firstTriangleId = 0;
			_root->_lastTriangleId	= (int)_triangles->size() - 1;
			for ( int i = 0; i < _root->_lastTriangleId; i++ )
			{
				_root->_aabb.extend( (*_triangles)[i].getAabb() );
			}
		}
		else
			_buildRec( _root, 0, (int)_triangles->size(), 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		if ( !_root->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;
		return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord );
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		if ( !_root->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;
		return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
	}

	bool triFunctionX( TriangleMeshGeometry i, TriangleMeshGeometry j )
	{
		return ( i.getCentroid().x < j.getCentroid().x );
	}
	bool triFunctionY( TriangleMeshGeometry i, TriangleMeshGeometry j )
	{
		return ( i.getCentroid().y < j.getCentroid().y );
	}
	bool triFunctionZ( TriangleMeshGeometry i, TriangleMeshGeometry j )
	{
		return ( i.getCentroid().z < j.getCentroid().z );
	}

	void cpy_hit( HitRecord & src, HitRecord & dest, const Vec3f & ray_direction )
	{
		dest._object = src._object;
		dest._point		= src._point;
		dest._distance	= src._distance;
		dest._normal	= src._normal;
		dest.faceNormal( ray_direction );


	}
	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		// printf( "idfirst: %i, idlast: %i\n", p_firstTriangleId, p_lastTriangleId );
		//< calcul aabb noeud >
		for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
		{
			p_node->_aabb.extend( _triangles->at( i ).getAabb() );
			
		}

		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;
		p_node->_left			 = nullptr;
		p_node->_right			 = nullptr;

		// if ! < critere arret >
		if ( p_depth < _maxDepth && p_lastTriangleId - p_firstTriangleId > _maxTrianglesPerLeaf )
		{
			AABB AABB_centre = AABB();
			for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
				AABB_centre.extend( (*_triangles)[i].getCentroid() );


			int	   largestAxe = (int)p_node->_aabb.largestAxis();
			float  milieu	  = ( AABB_centre.getMin()[ largestAxe ] + AABB_centre.getMax()[ largestAxe ] ) * 0.5f;
			int	   nbTrie	  = p_lastTriangleId - p_firstTriangleId;


			switch ( largestAxe )
			{
				// Returns the largest axis, 0 -> x, 1 -> y, 2 -> z

			case 0:
				std::partial_sort( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   triFunctionX );
				break;
			case 1:
				std::partial_sort( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   triFunctionY );
				break;
			case 2:
				std::partial_sort( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   triFunctionZ );
				break;
			}
			int idMid = -1;
			for ( int i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
			{
				if ( ( *_triangles )[ i ].getCentroid()[ largestAxe ] >= milieu )
				{
					idMid = i;
					break;
				}
			}
			if ( idMid == -1 ) { idMid = p_lastTriangleId; }

			p_node->_left  = new BVHNode();
			p_node->_right = new BVHNode();
			_buildRec( p_node->_left, p_firstTriangleId, idMid, p_depth + 1 );
			_buildRec( p_node->_right, idMid, p_lastTriangleId, p_depth + 1 );
		}
		else
		{
			p_node->_left  = nullptr;
			p_node->_right = nullptr;
		}
	}
	void BVH::_buildRecSAH(BVHNode* p_node,
		const unsigned int p_firstTriangleId,
		const unsigned int p_lastTriangleId,
		const unsigned int p_depth) {
		
		int nbTriangle = p_lastTriangleId - p_firstTriangleId+1;
		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;
		p_node->_left			 = nullptr;
		p_node->_right			 = nullptr;
		for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
		{
			p_node->_aabb.extend( _triangles->at( i ).getAabb() );
		}
		
		if ( p_depth < _maxDepth && p_lastTriangleId - p_firstTriangleId > _maxTrianglesPerLeaf )
		{
			AABB AABB_centre = AABB();
			for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
				AABB_centre.extend( ( *_triangles )[ i ].getCentroid() );

			int	   largestAxe = (int)p_node->_aabb.largestAxis();
			float  milieu	  = ( AABB_centre.getMin()[ largestAxe ] + AABB_centre.getMax()[ largestAxe ] ) * 0.5f;
			int	   nbTrie	  = p_lastTriangleId - p_firstTriangleId;

			switch ( largestAxe )
			{
			case 0:
				std::partial_sort( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   triFunctionX );
				break;
			case 1:
				std::partial_sort( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   triFunctionY );
				break;
			case 2:
				std::partial_sort( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   _triangles->begin() + p_firstTriangleId + nbTrie,
								   triFunctionZ );
				break;
			}
			int idMid = -1;
			for ( int i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
			{
				if ( ( *_triangles )[ i ].getCentroid()[ largestAxe ] >= milieu )
				{
					idMid = i;
					break;
				}
			}
			if ( idMid == -1 ) { idMid = p_lastTriangleId; }

			int minTriangleGauche = 0;
			float minSah= FLT_MAX;
			int	  nbSubPossible		= nbTriangle - 2; 
			for (int i = 0; i < nbSubPossible; i++) {
				AABB gauche=AABB();
				AABB droite = AABB();
				for (int j = 0; j <nbTriangle; j++) {
					if ( j <= i ) { 
						gauche.extend( ( *_triangles )[ j ].getAabb() );
					}
					else
					{
						droite.extend( ( *_triangles )[ j ].getAabb() );
					}
				}
				float areaGauche = gauche.getArea();
				float areaDroite = droite.getArea();
				float sahCourant= ( areaGauche * ( i+1  ) + areaDroite * ( nbTriangle - i -1) ) / p_node->_aabb.getArea();
				
				if ( sahCourant < minSah ) { 
					minTriangleGauche = i;
					minSah			  = sahCourant;
				}
			}


			
			p_node->_left  = new BVHNode();
			p_node->_right = new BVHNode();
			_buildRecSAH( p_node->_left, p_firstTriangleId, p_firstTriangleId + minTriangleGauche, p_depth + 1 );
			_buildRecSAH( p_node->_right, p_firstTriangleId + minTriangleGauche, p_lastTriangleId, p_depth + 1 );
		}
	}
	

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( p_node->isLeaf() )
		{
			float t, tmin = p_tMax + 1;
			int	  id_min = -1;

			for ( int i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				if ( _triangles->at( i ).intersect( p_ray, t ) )
					if ( t <= p_tMax && t >= p_tMin && t < tmin )
					{
						tmin   = t;
						id_min = i;
					}

			if ( id_min > 0 )
			{
				p_hitRecord._point	= p_ray.pointAtT( tmin );
				p_hitRecord._normal = _triangles->at( id_min ).getFaceNormal();
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._distance = tmin;
				p_hitRecord._object	  = nullptr;

				return true;
			}
			return false;
		}
		else
		{
			HitRecord ht_left, ht_right;
			bool	  left_intersection = false, right_intersection = false;

			if ( p_node->_left->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
			{
				left_intersection = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, ht_left );
			}

			if ( p_node->_right->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
			{
				right_intersection = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, ht_right );
			}

			if ( left_intersection && right_intersection )
			{
				if ( ht_left._distance < ht_right._distance )
				{ 
					cpy_hit( ht_left,p_hitRecord , p_ray.getDirection() );
				}
				else
				{
					cpy_hit( ht_right,p_hitRecord, p_ray.getDirection() );
				}

				return true;
			}
			else if ( left_intersection )
			{
				cpy_hit( ht_left, p_hitRecord, p_ray.getDirection() );
				return true;
			}
			else if ( right_intersection )
			{
				cpy_hit( ht_right, p_hitRecord, p_ray.getDirection() );
				return true;
			}

			return false;
		}
	}

	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( p_node->isLeaf() )
		{
			float t = -1;
			for ( int i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
				if ( _triangles->at( i ).intersect( p_ray, t ) )
					if ( t <= p_tMax && t >= p_tMin ) return true;
			return false;
		}
		else
		{
			bool left_intersection = false;
			if ( p_node->_left->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
				left_intersection = _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
			if ( left_intersection ) return true;

			if ( p_node->_right->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
				return _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );
			return false;
		}
	}

	

} // namespace RT_ISICG
