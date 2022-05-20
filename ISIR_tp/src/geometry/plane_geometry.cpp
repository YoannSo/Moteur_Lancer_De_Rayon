#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t ) const
	{
		Vec3f po		   = ( this->_point - p_ray.getOrigin() );
		float numerateur   = glm::dot( po, this->_normal );
		float denominateur = glm::dot( p_ray.getDirection(), this->_normal );
		float t			   = numerateur / denominateur;

		if ( t > 0 )
		{
			p_t = t;
			return true;
		}
		return false;
	}

}
