#ifndef __CONTROLS_H_INCLUDED__
#define __CONTROLS_H_INCLUDED__

#include "util/gl.h"

typedef int Control;

#define CONTROL_ACTION_PRESS 0
#define CONTROL_ACTION_RELEASE 1
#define CONTROL_ACTION_REPEAT 2
#define CONTROL_ACTION_MOUSE 3
#define CONTROL_ACTION_SCROLL 4

#define CONTROL_GUI_NEXT 0
#define CONTROL_GUI_PREV 1
#define CONTROL_GUI_UP 2
#define CONTROL_GUI_DOWN 3
#define CONTROL_GUI_LEFT 4
#define CONTROL_GUI_RIGHT 5

#define CONTROL_GUI_SELECT GLFW_MOUSE_BUTTON_1
#define CONTROL_GUI_ESCAPE GLFW_KEY_ESCAPE

#endif
