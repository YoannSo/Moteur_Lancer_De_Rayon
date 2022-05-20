#include "scene.hpp"
#include "materials/color_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"
#include "materials/lambert_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/cook_torrancel.hpp"
#include "materials/mirror_material.hpp"
#include "materials/transparent_material.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "objects/triangle_mesh.hpp"
#include "objects/sphere_implicit.hpp"
#include "objects/torus_implicit.hpp"
#include "objects/death_star_implicit.hpp"
#include "objects/mandelbulb.hpp"
#include "lights/spot.hpp"
namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}
	
	void Scene::init(int p_tpId)
	{
		switch ( p_tpId )
		{
			case 1:
				_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
				_addMaterial( new ColorMaterial( "Blue", BLUE ) );
				_attachMaterialToObject( "Blue", "Sphere1" );
				break;
			case 2:
				_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
				_addObject( new Plane( "firstPlane", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
				_addMaterial( new ColorMaterial( "Blue", BLUE ) );
				_addMaterial( new ColorMaterial( "Red", RED ) );
				_attachMaterialToObject( "Blue", "Sphere1" );
				_attachMaterialToObject( "Red", "firstPlane" );
				// add light
				_addLight( new PointLight( Vec3f( 1.f, 10.f, 1.f ), WHITE, 100 ) );
				break;
			case 3:
				_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
				_addObject( new Plane( "firstPlane", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
				_addMaterial( new ColorMaterial( "Blue", BLUE ) );
				_addMaterial( new ColorMaterial( "Red", RED ) );
				_attachMaterialToObject( "Blue", "Sphere1" );
				_attachMaterialToObject( "Red", "firstPlane" );
				_addLight( new QuadLight(
					Vec3f( 1.f, 10.f, 2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 2.f ), WHITE, 20 ) );
				break;
			case 4:
				 _addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
				//_addObject(new Plane("firstPlane", Vec3f( 0.f, -2.f,0.f ),Vec3f( 0.f, -1.f, 0.f )));
				// Add materials.
				//_addMaterial( new LambertMaterial( "RedLambert", RED ) );
				 _addMaterial( new CookTorrance(
					 "cook", Vec3f( 1.f, 0.85f, 0.57f ), 0.5f, 0.4f, Vec3f( 1.f, 0.85f, 0.57f ) ) );
				 // Link objects and materials.
				_attachMaterialToObject( "cook", "Sphere1" );
				//_attachMaterialToObject( "RedLambert", "firstPlane" );
				// add light
				_addLight( new PointLight( Vec3f( 0.f, 0.f, -2.f ), WHITE, 60 ) );
				break;
			case 5:
				_addMaterial( new LambertMaterial( " WhiteMatte ", WHITE ) );
				_addMaterial( new LambertMaterial( " RedMatte ", RED ) );
				_addMaterial( new LambertMaterial( " GreenMatte ", GREEN ) );
				_addMaterial( new LambertMaterial( " BlueMatte ", BLUE ) );
				_addMaterial( new LambertMaterial( " GreyMatte ", GREY ) );
				_addMaterial( new LambertMaterial( " MagentaMatte ", MAGENTA ) );
				_addMaterial( new MirrorMaterial( " MirrorMaterial " ) );
				_addMaterial( new TransparentMaterial( " TransparentMaterial " ) );
				_addObject( new Sphere( " Sphere1 ", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere1 " );
				_addObject( new Sphere( " Sphere2 ", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
				_attachMaterialToObject( " TransparentMaterial ", " Sphere2 " );
				_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
				_attachMaterialToObject( " GreyMatte ", " PlaneGround " );
				_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
				_attachMaterialToObject( " RedMatte ", " PlaneLeft " );
				_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
				_attachMaterialToObject( " GreenMatte ", " PlaneCeiling " );
				_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
				_attachMaterialToObject( " BlueMatte ", " PlaneRight " );
				_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
				_attachMaterialToObject( " MagentaMatte ", " PlaneFront " );
				_addLight( new PointLight( Vec3f( 0.f, 5.f, 0.f ), WHITE, 100.f ) );
				break;
			case 6:
			
				loadFileTriangleMesh( "conf", DATA_PATH + "conference/conference.obj" );
				
				_addLight( new QuadLight(
					Vec3f( 900.f, 600.f, -300.f ), Vec3f( -800.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 300.f ), WHITE, 40 ) );
				break;
			case 7:
				_addMaterial( new LambertMaterial( " RedMatte ", RED ) );
				_addMaterial( new LambertMaterial( " GreenMatte ", GREEN ) );
				_addMaterial( new LambertMaterial( " BlueMatte ", BLUE ) );
				_addMaterial( new LambertMaterial( " GreyMatte ", GREY ) );
				_addMaterial( new LambertMaterial( " MagentaMatte ", MAGENTA ) );
				_addMaterial( new LambertMaterial( " YellowMatte ", YELLOW ) );
				_addMaterial( new LambertMaterial( " CyanMatte ", CYAN ) );
				_addMaterial( new TransparentMaterial( " TransparentMaterial " ) );
				_addMaterial( new MirrorMaterial( " MirrorMaterial " ) );

				_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
				
			_addMaterial( new CookTorrance(
					"cook", Vec3f( 1.f, 0.85f, 0.57f ), 1.f, 0.4f, Vec3f( 1.f, 0.85f, 0.57f ) ) );
				_addObject( new MandelBulb( "Fractale", Vec3f( 0.f, 0.f, 0.f ) ) );
			/**/ _attachMaterialToObject( "cook", "Fractale" );

				/**/_attachMaterialToObject( " GreyMatte ", " PlaneGround " );
				_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
				_attachMaterialToObject( " RedMatte ", " PlaneLeft " );
				_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
				_attachMaterialToObject( " GreenMatte ", " PlaneCeiling " );
				_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
				_attachMaterialToObject( " BlueMatte ", " PlaneRight " );
				_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
				_attachMaterialToObject( " MirrorMaterial ", " PlaneFront " );
				_addObject( new Plane( " PlaneRear ", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
				_attachMaterialToObject( " YellowMatte ", " PlaneRear " );
				_addLight( new PointLight( Vec3f( 0.f, 2.f, -3.f ), WHITE, 60.f ) );
				//_addLight( new QuadLight(
					//Vec3f( 1.f, 10.f, 2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 2.f ), WHITE, 20 ) );
				break;
			case 8: 
				_addMaterial( new CookTorrance(
					"cook", Vec3f( 1.f, 0.85f, 0.57f ), 1.f, 0.4f, Vec3f( 1.f, 0.85f, 0.57f ) ) );
				_addMaterial( new LambertMaterial( " WhiteMatte ", WHITE ) );
				_addMaterial( new LambertMaterial( " RedMatte ", RED ) );
				_addMaterial( new LambertMaterial( " GreenMatte ", GREEN ) );
				_addMaterial( new LambertMaterial( " BlueMatte ", BLUE ) );
				_addMaterial( new LambertMaterial( " GreyMatte ", GREY ) );
				_addMaterial( new LambertMaterial( " MagentaMatte ", MAGENTA ) );
				_addMaterial( new MirrorMaterial( " MirrorMaterial " ) );
				_addMaterial( new TransparentMaterial( " TransparentMaterial " ) );
				_addObject( new Sphere( " Sphere1 ", Vec3f( -2.f, 0.f, 3.f ), 1.f ) );
				_attachMaterialToObject( "cook", " Sphere1 " );

				_addObject( new Sphere( " Sphere3 ", Vec3f( -5.f, 3.f, 5.f ), 0.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere3 " );
				_addObject( new Sphere( " Sphere4 ", Vec3f( 5.f, 3.f, 5.f ), 0.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere4 " );

				_addObject( new Sphere( " Sphere5 ", Vec3f( -5.f, 2.f, 0.f ), 0.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere5 " );
				_addObject( new Sphere( " Sphere6 ", Vec3f( 5.f, 1.f, 4.f ), 0.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere6 " );

				_addObject( new Sphere( " Sphere7 ", Vec3f( -5.f, 0.f, 3.f ), 0.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere7 " );
				_addObject( new Sphere( " Sphere8 ", Vec3f( 5.f, 2.f, -1.f ), 0.5f ) );
				_attachMaterialToObject( " MirrorMaterial ", " Sphere8 " );


				_addObject( new Sphere( " Sphere2 ", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
				_attachMaterialToObject( " TransparentMaterial ", " Sphere2 " );
				_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
				_attachMaterialToObject( " GreyMatte ", " PlaneGround " );
				_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
				_attachMaterialToObject( " RedMatte ", " PlaneLeft " );
				_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
				_attachMaterialToObject( " GreenMatte ", " PlaneCeiling " );
				_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
				_attachMaterialToObject( " BlueMatte ", " PlaneRight " );
				_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 50.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
				_attachMaterialToObject( " MagentaMatte ", " PlaneFront " );
				//_addLight( new PointLight( Vec3f( 0.f, 5.f, 0.f ), WHITE, 100.f ) );
				_addObject( new TorusImplicit( "torus0", Vec3f( 2.f, 1.f, 0.f ), Vec2f(1.8f, 0.7f) ) );
				_addLight( new QuadLight(
					Vec3f( 0.f, 5.f, 0.f ), Vec3f( -4.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 4.f ), GREY, 80 ) );
				loadFileTriangleMesh( "conf", DATA_PATH + "bunny.obj" );
				break;

			default: break;
			
		}
		
		
	}
	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y-2.f ,mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y - 2.f, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}
			
			triMesh->buildBVH();
			_addObject( triMesh );
			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				_addMaterial( new PlasticMaterial( std::string( mtlName.C_Str() ), kd, ks, s,0.5f ) );
				_attachMaterialToObject( mtlName.C_Str(), meshName );
			}
			
			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}


	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	
	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, tMax ) )
			{ hit = true;
			}
		}
		return hit;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
