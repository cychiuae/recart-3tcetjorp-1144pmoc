#include <cmath>
#include "light.h"
#include "../UI/TraceUI.h"
extern class TraceUI *traceUI;


double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
 
    const vec3f &dir = getDirection(P);
    vec3f intensity(1.0, 1.0, 1.0);
    vec3f point = P + dir * RAY_EPSILON;
    while (!intensity.iszero()) {
        isect i;
        ray shadowRay(point, dir);
        if (!scene->intersect(shadowRay, i)) {
            return intensity;
        }
        intensity = prod(intensity, i.getMaterial().kt);
        point = shadowRay.at(i.t) + dir * RAY_EPSILON;
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
    
    double atten;
	double c = traceUI->getAttenConstant();
	double l = traceUI->getAttenLinear();
	double q = traceUI->getAttenQuadra();
    double r = (position - P).length();
    atten = 1/( c + l*r + q*pow(r,2) );
    if(atten < 1){
        atten = 1;
    }
	return atten;
}

vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
    vec3f intensity(1.0, 1.0, 1.0);
    const vec3f &dir = getDirection(P);
    vec3f point = P + dir * RAY_EPSILON;
    while (!intensity.iszero()){
        isect i;
        ray shadowRay(point, dir);
        const double light_t = (position - point).length();
        if (!scene->intersect(shadowRay, i) || i.t >= light_t) {
            return intensity;
        }

        intensity = prod(intensity, i.getMaterial().kt);
        point = shadowRay.at(i.t) + dir * RAY_EPSILON;
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

