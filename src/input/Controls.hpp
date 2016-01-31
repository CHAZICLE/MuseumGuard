#ifndef __CONTROLS_H_INCLUDED__
#define __CONTROLS_H_INCLUDED__

#include "util/gl.h"

typedef int Control;

// Actions: bits 1,2
#define CONTROL_ACTION_PRESS 1
#define CONTROL_ACTION_RELEASE 2
#define CONTROL_ACTION_REPEAT 3

// Action sources: bits 3,4,5
#define CONTROL_ACTION_KEY 4
#define CONTROL_ACTION_MOUSE 8
#define CONTROL_ACTION_SCROLL 16

#define CONTROL_KEYACTION_PRESS (CONTROL_ACTION_KEY+CONTROL_ACTION_PRESS)
#define CONTROL_KEYACTION_RELEASE (CONTROL_ACTION_KEY+CONTROL_ACTION_RELEASE)
#define CONTROL_KEYACTION_REPEAT (CONTROL_ACTION_KEY+CONTROL_ACTION_REPEAT)

#define CONTROL_MOUSEBUTTONACTION_PRESS (CONTROL_ACTION_MOUSE+CONTROL_ACTION_PRESS)
#define CONTROL_MOUSEBUTTONACTION_RELEASE (CONTROL_ACTION_MOUSE+CONTROL_ACTION_RELEASE)
#define CONTROL_MOUSEBUTTONACTION_REPEAT (CONTROL_ACTION_MOUSE+CONTROL_ACTION_REPEAT)

#define CONTROL_GUI_NEXT 0
#define CONTROL_GUI_PREV 1
#define CONTROL_GUI_UP 2
#define CONTROL_GUI_DOWN 3
#define CONTROL_GUI_LEFT 4
#define CONTROL_GUI_RIGHT 5

#define CONTROL_GUI_SELECT GLFW_MOUSE_BUTTON_1
#define CONTROL_GUI_ESCAPE GLFW_KEY_ESCAPE

#endif
