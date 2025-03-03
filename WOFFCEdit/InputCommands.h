#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;
	bool up;
	bool down;
	bool isShiftDown;

	// Mouse Controls & Variables
	bool leftMousePressed;
	bool leftMouseHeld;
	bool rightMousePressed;

	// Camera movement - mouse
	int mouseDeltaX;
	int mouseDeltaY;
};
