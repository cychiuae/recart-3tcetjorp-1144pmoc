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
    
    vec3f intensity;
    vec3f dir = getDirection(P);
    ray r(P + dir * RAY_EPSILON, getDirection(P).normalize());
    isect intersect;
    bool isIntersect = scene->intersect(r, intersect);
    if ( !isIntersect ){
    	intensity = vec3f(1.0, 1.0, 1.0);
    }else{
		const Material& m = intersect.getMaterial();
    	intensity = m.kt;
    }
    // return intensity  = vec3f(1,1,0);
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
    double atten;
    double c = this->const_coeff;
    double l = this->lin_coeff;
    double q = this->quard_coeff;
    double r = (position - P).length();
    atten = 1/( c + l*r + q*pow(r,2) );
    if(atten <= 1){
        atten = 1;
    }
	return atten;
}

vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    vec3f intensity = vec3f(0, 0, 0);
    vec3f d = getDirection(P);
    isect intersect;
    ray r = ray(P + d * RAY_EPSILON,d);
    bool isIntersect = scene->intersect(r, intersect);
    if(!isIntersect){
        intensity = vec3f(1,1,1);
    }else{
        const Material& m = intersect.getMaterial();
        vec3f intersect_pt = vec3f(r.at(intersect.t));
        //bool lightSrcIsInFrontOfTheObj = (intersect_pt - P).length() - (position - P).length();
        if( (intersect_pt - P).length() > (position - P).length() ){
            intensity = vec3f(1,1,1);
        }else{
            intensity = m.kt;// vec3f(0,0,0);//m.kt;
        }       
    } 

    return intensity;
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

double SpotLight::distanceAttenuation( const vec3f& P ) const
{
    return 1.0;
}

vec3f SpotLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    vec3f intensity = vec3f(0, 0, 0);

    return intensity;
}

vec3f SpotLight::getColor( const vec3f& P ) const
{
    // Color doesn't depend on P 
    return color;
}

vec3f SpotLight::getDirection( const vec3f& P ) const
{
    return (position - P).normalize();
}


vec3f AmbientLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

