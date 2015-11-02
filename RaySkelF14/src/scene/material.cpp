#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::getTotalAmibientLightIntensity(Scene *scene, vec3f pt) const{
	vec3f intensity(0,0,0);
	for (auto const ambient_light : scene->getAmibientLights()){
		intensity += ambient_light->getColor(pt);
	}
	return intensity;

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

    vec3f V = -r.getDirection(); // View(EYES)
 	vec3f N = i.N.normalize(); // Normal
 	vec3f Q = r.at(i.t); // intersect_point

	vec3f Ia = getTotalAmibientLightIntensity(scene, Q);
	vec3f KaIa = prod(ka,Ia);
	vec3f intensity = (ke + KaIa);

	for (list<Light*>::const_iterator Light=scene->beginLights(); Light != scene->endLights(); ++Light){

		vec3f atten = (*Light)->distanceAttenuation(Q) * (*Light)->shadowAttenuation(Q);

		vec3f inDirVec = (-(*Light)->getDirection(Q)).normalize(); // Incedient ray
		vec3f R = (inDirVec - 2 * inDirVec.dot(N) * N).normalize(); // Reflection ray
		vec3f L = (*Light)->getDirection(Q);   // Light Source
		vec3f diffuse = kd * max(N.dot(L), 0.0);
		vec3f specular = ks * pow(max(R.dot(V),0.0),shininess*128);

		intensity[0] += max(atten[0] * (diffuse[0] + specular[0]),0.0);
		intensity[1] += max(atten[1] * (diffuse[1] + specular[1]),0.0);
		intensity[2] += max(atten[2] * (diffuse[2] + specular[2]),0.0);
	}

	return intensity;
}
