#include "ray.h"
#include "material.h"
#include "light.h"
#include "../UI/TraceUI.h"
extern class TraceUI *traceUI;


// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::getTotalAmibientLightIntensity(Scene *scene, vec3f pt) const{
	vec3f intensity(0,0,0);
	for (auto const ambient_light : scene->getAmibientLights()){
		intensity[0] += ambient_light->getColor(pt)[0] * traceUI->getAttenConstant();
		intensity[1] += ambient_light->getColor(pt)[1] * traceUI->getAttenLinear();
		intensity[2] += ambient_light->getColor(pt)[2] * traceUI->getAttenQuadra();
	}
	//printf("intensity:%d,%d,%d\n",intensity[0],intensity[1],intensity[2]);
	return intensity * traceUI->getAmbient();

}

vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	vec3f intensity = ke;
	vec3f N = i.N;
	vec3f Q = r.at(i.t);
	vec3f out_Q = Q + i.N*RAY_EPSILON;
	vec3f transparency = vec3f(1, 1, 1) - kt;
	intensity += prod(transparency, ka);
	
	for (Scene::cliter lightIter = scene->beginLights(); lightIter != scene->endLights(); lightIter++){		
		vec3f atten = (*lightIter)->distanceAttenuation(Q) * (*lightIter)->shadowAttenuation(out_Q);
		vec3f light = ((*lightIter)->getDirection(Q)).normalize();
		double angle = maximum(N.dot(light), 0.0);
		vec3f diffuse = prod(kd * angle, transparency);

		vec3f R = ((2 * (N.dot(light)) * N) - light).normalize();
		vec3f specular = ks * (pow(maximum(R * (-r.getDirection()), 0), shininess * 128.0));
		intensity += prod(atten, diffuse + specular);
	}
	
	return intensity.clamp();
}
