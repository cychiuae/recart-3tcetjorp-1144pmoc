#include <cmath>

#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    vec3f intensity = vec3f(0, 0, 0);
    ray r(P, getDirection(P).normalize());
    isect intersect;
    bool isIntersect = scene->intersect(r, intersect);
    if ( !isIntersect ){
    	intensity = vec3f(1, 1, 1);
    }else{
		const Material& m = intersect.getMaterial();
    	intensity = m.kt;
    }

    return intensity;
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	return 1.0;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    vec3f intensity = vec3f(0, 0, 0);
    vec3f d = getDirection(P);
    isect intersect;
    ray r = ray(P,d);
    bool isIntersect = scene->intersect(r, intersect);
    if(!isIntersect){
    	intensity = vec3f(1,1,1);
    }else{
    	const Material& m = intersect.getMaterial();
    	vec3f i_position = vec3f(r.at(intersect.t));
    	bool isBeforeTheLightSrc = (i_position - P).length() - (position - P).length();
	    if( isBeforeTheLightSrc ){
			intensity = vec3f(0,0,0);
	    }else{
	    	intensity = vec3f(1,1,1);
	    }    	
    } 

    return intensity;
}



double AmbientLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	return 1.0;
}

vec3f AmbientLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f AmbientLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	return vec3f(0, 0, 0);
}

vec3f AmbientLight::getDirection(const vec3f& P) const
{
	return (P).normalize();
}