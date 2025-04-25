#pragma once

struct InputCommands
{
	// Camera Controls
	bool				forward;
	bool				back;
	bool				right;
	bool				left;
	bool				rotRight;
	bool				rotLeft;
	bool				up;
	bool				down;

	// Editor Controls
	bool				isShiftDown;
	bool				shouldpick;

	// Mouse Controls
	bool				mouse_LB_Down;
	bool				mouse_LB_Held;
	bool				mouse_RB_Down;

	// Camera movement - mouse 
	int					mouse_X;
	int					mouse_Y;
	

	
};
