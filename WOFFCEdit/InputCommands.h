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

	// Interaction Controls
	bool				isShiftDown;

	// Mouse Controls & Variables
	bool				leftMousePressed;
	bool				leftMouseHeld;
	bool				rightMousePressed;

	// Camera movement - mouse
	int					mouseDeltaX;
	int					mouseDeltaY;

	// Undo / Redo Selection
};
