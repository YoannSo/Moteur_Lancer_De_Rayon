#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"
#include "integrators/direct_lighting_integrator.hpp"
#include "integrators/whitted_integrator.hpp"
#include <omp.h>

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST:
		
			_integrator = new RayCastIntegrator();
			break;
		
		case IntegratorType::POINT_LIGHT: 
			_integrator = new DirectLightingIntegrator();
			break;
		case IntegratorType::WHITTED_INTEGRATOR: 
			_integrator = new WhittedIntegrator(); 
			break;
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();
		
		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();
		float pMin=0, pMax=FLT_MAX;
		#pragma omp parallel for
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				Vec3f averageColor = Vec3f( 0.f );

				for ( int sample = 0; sample < _nbPixelSamples; sample++ )
				{
					float randI = (float)std::rand() / RAND_MAX;
					float randJ = (float)std::rand() / RAND_MAX;

					const Ray myRay = p_camera->generateRay( (float)( i + randI ) / (float)( width - 1 ),
															 (float)( j + randJ ) / (float)( height - 1 ) );
					
					averageColor += _integrator->Li( p_scene, myRay, pMin, pMax );

				}
					Vec3f color = averageColor / (float)_nbPixelSamples;
				//std::cout << color.x << " " << color.y << " " << color.z << std::endl;
				p_texture.setPixel( i, j,glm::clamp( color, Vec3f(0,0,0),  Vec3f(1,1,1)));

			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
