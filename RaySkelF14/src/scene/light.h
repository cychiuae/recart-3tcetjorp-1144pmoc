#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};


class PointLight
	: public Light
{
public:
	PointLight( Scene *scene, const vec3f& pos, const vec3f& color, double c_coeff = 0.0, double l_coeff = 0.0, double q_coeff = 1.0 )
		: Light(scene, color), position(pos), const_coeff(c_coeff), lin_coeff(l_coeff), quard_coeff(q_coeff) {}
	virtual vec3f  shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f  getColor( const vec3f& P ) const;
	virtual vec3f  getDirection( const vec3f& P ) const;
protected:
	vec3f    position;
	double   const_coeff;
	double   lin_coeff;
	double   quard_coeff;

};
class SpotLight
	: public Light
{
public:
	SpotLight( Scene *scene, const vec3f& pos, const vec3f& color) : Light(scene, color), position(pos){}
	virtual vec3f  shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f  getColor( const vec3f& P ) const;
	virtual vec3f  getDirection( const vec3f& P ) const;
protected:
	vec3f    position;


};

class AmbientLight : public SceneElement
{
public:

	AmbientLight( Scene *scene, const vec3f& c ) : color(c), SceneElement(scene) {}
	virtual vec3f getColor( const vec3f& P ) const;

protected:
	vec3f color;

};

#endif // __LIGHT_H__
