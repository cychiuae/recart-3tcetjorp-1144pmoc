//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;

	Fl_Slider*          m_AttenContantSlider;
	Fl_Slider*          m_attenLinearSlider;
	Fl_Slider*          m_attenQuadraSlider;
	Fl_Slider*          m_ambientSlider;
	Fl_Slider*          m_intensitySlider;
	Fl_Slider*          m_distanceSlider;
	Fl_Slider*          m_superSamplingSlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();

	double      getAttenConstant();
	double      getAttenLinear();
	double      getAttenQuadra();
	double      getAmbient();
	int         getIntensity();
	double      getDistance();
	int         getSuperSampling();

private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	double      m_nAttenConstant;
	double      m_nAttenLinear;
	double      m_nAttenQuadra;
	double      m_nAmbient;
	int         m_nIntensity;
	double      m_nDistance;
	int         m_nSuperSampling;

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);

	static void cb_attenConstantSlides(Fl_Widget* o, void* v);
	static void cb_attenLinearSlides(Fl_Widget* o, void* v);
	static void cb_attenQuadraSlides(Fl_Widget* o, void* v);
	static void cb_ambientSlides(Fl_Widget* o, void* v);
	static void cb_intensitySlides(Fl_Widget* o, void* v);
	static void cb_distanceSlides(Fl_Widget* o, void* v);
	static void cb_supersamplingSlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
};

#endif
