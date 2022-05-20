#include "cameras/perspective_camera.hpp"
#include "defines.hpp"
#include "renderer.hpp"

namespace RT_ISICG
{
	int main( int argc, char ** argv )
	{
		const int imgWidth	= 600;
		const int imgHeight = 400;

		// Create a texture to render the scene.
		Texture img = Texture( imgWidth, imgHeight );
		std::cout << "TP NOTE YOANN SOCHAJ:" << std::endl;
		int input = -1;
		while ( input > 8 || input < 0 )
		{
			std::cout << "------------------------------------" << std::endl;
			std::cout << "1-FIN TP1 (mise en place+ Sphere + Antiallisaing: 32)" << std::endl;
			std::cout << "2-FIN TP2  (plane + eclairage + Debut Rayon d'ombrage)" << std::endl;
			std::cout << "3-FIN TP3 (quad + Ombre Douche)" << std::endl;
			std::cout << "4-FIN TP4 (Les materiaux)" << std::endl;
			std::cout << "5-FIN TP5 Reflexion et Refraction Scene finale " << std::endl;
			std::cout << "6- TP6 Ma salle de conference" << std::endl;
			std::cout << "7- TP7 les surfaces implicites" << std::endl;
			std::cout << "8- scene de demonstration" << std::endl;

			std::cout << "------------------------------------" << std::endl;
			std::cout << "ENTREZ LE NUMERO DU TP QUE VOUS VOULEZ VOIR" << std::endl;
			std::cin >> input;
			if ( input > 6 || input < 0 ) { std::cout << "ENTREZ LE NUMERO DE TP VALIDE" << std::endl; }
		}
		// Create and init scene.
		Scene scene;
		scene.init(input);
		int choixCamera = -1;

		// Create a perspective camera.
		Vec3f pos_camera = VEC3F_ZERO;
		Vec3f pos_lookAt = VEC3F_ZERO;
		Renderer renderer;

		switch ( input )
		{
			case 1:
				renderer.setIntegrator( IntegratorType::RAY_CAST );
				while ( choixCamera > 4 || choixCamera < 0 )
				{
					std::cout << "------------------------------------" << std::endl;
					std::cout << "1-Pos:(0,0,-2) Look(0,0,79)" << std::endl;
					std::cout << "2-Pos:(1,0,0) Look(1,0,1)" << std::endl;
					std::cout << "3-Pos:(0,1,0) Look(0,1,1)" << std::endl;
					std::cout << "4-Pos:(4,-1,0) Look(-1,-1,2)" << std::endl;
					std::cout << "------------------------------------" << std::endl;
					std::cout << "ENTREZ LE NUMERO DE LA CAMERA QUE VOUS VOULEZ VOIR" << std::endl;
					std::cin >> choixCamera;
					if ( choixCamera > 4 || choixCamera < 0 )
					{
						std::cout << "ENTREZ LE NUMERO DE CAMERA VALIDE" << std::endl;
					}
				}
				switch ( choixCamera )
				{
					case 1: 
						pos_camera = Vec3f( 0.f, 0.f, -2.f ); 
						pos_lookAt = Vec3f( 0.f, 0.f, 79.f );
						break;
					case 2:
						pos_camera = Vec3f( 1.f, 0.f, 0.f );
						pos_lookAt = Vec3f( 1.f, 0.f, 1.f );
						break;
					case 3:
						pos_camera = Vec3f( 0.f, 1.f, 0.f );
						pos_lookAt = Vec3f( 0.f,1.f, 1.f );
						break;
					case 4:
						pos_camera = Vec3f( 4.f, -1.f, 0.f );
						pos_lookAt = Vec3f( -1.f, -1.f, 2.f );
						break;
				}
				break;
			case 2:
				renderer.setIntegrator( IntegratorType::POINT_LIGHT );

				 pos_camera = Vec3f( 0.f, 0.f, -2.f );
				 pos_lookAt = Vec3f( 0.f, 0.f, 79.f );
				break;
			case 3:
				renderer.setIntegrator( IntegratorType::POINT_LIGHT );

				 pos_camera = Vec3f( 0.f, 0.f, -2.f );
				 pos_lookAt = Vec3f( 0.f, 0.f, 79.f );
				break;
			case 4:
				renderer.setIntegrator( IntegratorType::POINT_LIGHT );

				pos_camera = Vec3f( 0.f, 0.f, 0.f );
				pos_lookAt = Vec3f( 0.f, 0.f, 1.f );
				break;
		case 5: 
			renderer.setIntegrator( IntegratorType::WHITTED_INTEGRATOR );

			pos_camera = Vec3f( 0.f, 2.f, -7.f );
			pos_lookAt = Vec3f( 0.f, 2.f, -1.f );
			break;
		case 6:
			renderer.setIntegrator( IntegratorType::WHITTED_INTEGRATOR );
			pos_camera = Vec3f( -250.f, 500.f, 330.f );
			pos_lookAt = Vec3f( 0.f, 350.f, 100.f );
			break;
		case 7:
			renderer.setIntegrator( IntegratorType::WHITTED_INTEGRATOR );
			pos_camera = Vec3f( 0.f, 2.f, -3.f );
			pos_lookAt = Vec3f( 0.f, 0.f, 0.f );
			break;
		case 8:
			renderer.setIntegrator( IntegratorType::WHITTED_INTEGRATOR );
			pos_camera = Vec3f( 0.f, 0.f, -8.f );
			pos_lookAt = Vec3f( 0.f, 0.f, 0.f );
			break;
		default: break;
		}
		 
		Vec3f p_up = Vec3f( 0.f, 1.f, 0.f );
		float FOV = 60.f;

		PerspectiveCamera camera( pos_camera, pos_lookAt, p_up, FOV, float( imgWidth ) / imgHeight );

		int choixAnti = 1;
		int nbSample  =1;

		renderer.setNbPixelSamples( nbSample );
		renderer.setBackgroundColor( GREY );

		// Launch rendering.
		std::cout << "Rendering..." << std::endl;
		std::cout << "- Image size: " << imgWidth << "x" << imgHeight << std::endl;

		float renderingTime = renderer.renderImage( scene, &camera, img );

		std::cout << "-> Done in " << renderingTime << "ms" << std::endl;

		// Save rendered image.
		const std::string imgName = "image.jpg";
		img.saveJPG( RESULTS_PATH + imgName );

		return EXIT_SUCCESS;
	}
} // namespace RT_ISICG

int main( int argc, char ** argv )
{
	try
	{
		return RT_ISICG::main( argc, argv );
	}
	catch ( const std::exception & e )
	{
		std::cerr << "Exception caught:" << std::endl << e.what() << std::endl;
	}
}
