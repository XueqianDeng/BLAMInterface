#include "config.h"
#include "MouseInput.h"

InputFrame MouseInput::GetFrame()
{
	InputFrame input;
	input.x = x;
	input.y = y;
	input.theta = 0.0f;
	input.time = SDL_GetTicks();

	return input;
}

void MouseInput::ProcessEvent(SDL_Event event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		// Convert position data to meters
		x = (GLfloat)event.motion.x * PHYSICAL_RATIO;
		y = (GLfloat)(SCREEN_HEIGHT - event.motion.y) * PHYSICAL_RATIO;
	}
}