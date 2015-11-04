// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"


// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );
	return traceRay( scene, r, vec3f(1.0,1.0,1.0), m_iDepth, 1.0 ).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth, double prev_index )
{
	isect i;

	// printf("depth: %d\n",depth);


	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.

		// (t, N, mtrl) <- scene.intersect(P, d)
		const double t = i.t;
		const vec3f N = i.N.normalize();
		const Material& mtrl = i.getMaterial();

		// Q <- ray(P, d) evaluated at t
		const vec3f Q = r.at(t);
		
		// I = shade(mtrl, scene, Q, N, d)
		vec3f I = mtrl.shade(scene, r, i);

		if (depth <= 0) {
			return I;
		}
		
		// R = reflectionDirection(    )
		vec3f reflectionPosition = r.at(t) + RAY_EPSILON * N;
		vec3f incidentDirection = r.getDirection().normalize();
		vec3f reflectionDirection = incidentDirection + 2 * (incidentDirection.dot(N)) * N;
		ray reflectionRay(reflectionPosition, reflectionDirection);

		// I <- I + mtrl.kr * traceRay(scene, Q, R)
		vec3f reflectionIntensity = traceRay(scene, reflectionRay, thresh, depth - 1, mtrl.index);
		for (int i = 0; i < 3; i++) {
			I[i] += mtrl.kr[i] * reflectionIntensity[i];
		}

		double n_i, n_t;
		if (mtrl.index != prev_index){  // if(ray is entering object) then 
			n_i = prev_index;
			n_t = mtrl.index;
		}else {
			n_i = mtrl.index;
			n_t = 1.0;
		}


		double n_r  = n_i/n_t;
		vec3f refractionPosition = r.at(t) - RAY_EPSILON * N;
		double tir_factor = 1-pow(n_r,2)*(1-pow(N.dot(-I),2));

		if(tir_factor>=RAY_EPSILON){ // if(notTIR(      )) then 
			vec3f refractionDirection = (n_r * N.dot(-I) - sqrt(tir_factor))*N + n_r*I; // ??equation may not be correct
			ray refractionRay(reflectionPosition, reflectionDirection);
			vec3f refractionIntensity = traceRay(scene, refractionRay, thresh, depth - 1, mtrl.index);
			for (int i = 0; i < 3; i++) {

				// I[i] *= (1-mtrl.kt[i]);
				I[i] += mtrl.kt[i] * refractionIntensity[i];
			}
		}

		return I.clamp();
		
	
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.

		return vec3f( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
	
	// Trace param 
	m_iDepth = 0;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

// Trace Param function

void RayTracer::setDepth(int depth) {
	m_iDepth = depth;
}