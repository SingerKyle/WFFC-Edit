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

	// Mouse Controls & Variables
	bool leftMousePressed;
	bool rightMousePressed;

	// Camera movement - mouse
	float mouseDeltaX;
	float mouseDeltaY;
	float prevMouseX;
	float prevMouseY;
};
