#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

enum GundanControls
{ 
	LEVEL, LHANDX, RHANDX, LHANDZ, RHANDZ, LKNEEL, RKNEEL, 
	LLEGX, RLEGX, LLEGZ, RLEGZ, LSHANKZ, RSHANKZ, SWORD, 
	IKX, IKY, IKZ, LLEGXMAX, LLEGXMIN, LLEGZMAX, LLEGZMIN, LSHANKZMAX, LSHANKZMIN, IK, PIK,
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif