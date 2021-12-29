#include <cmath>
#include <windows.h>
#include "bird.h"
#include "config.h"
#include "BirdInput.h"

BirdInput::BirdInput(int i)
{
	index = i;
}

InputFrame BirdInput::GetFrame()
{
	InputFrame input;
	BIRDREADING* bird_data = &frame.reading[index]; // Isolate the data for this specific bird
	
	// Make all time data relative to the first frame receieved.
	if (birds_start == 0)
	{
		birds_start = frame.dwTime;
	}

	/* The position and orientation data is stored in bird_data. Positions are
	 * stored as nX, nY, and nZ, and angles are stored as nAzimuth, nElevation,
	 * and nRoll. To convert the positions to meters, multiply by 0.9144 / 32767.
	 * To convert orientations to radians, multiply by atan(1) * 4 / 32767.
	 * These values can then be translated to the appropriate coordinate bounds.
	 *
	 * Keep in mind that the coordinate system used by the returned Flock of
	 * Birds data is different than the one used to output to the screen. "Up"
	 * on the screen is the -z axis for the Flock of Birds and +y for OpenGL.
	 * "Right" on the screen is the +y axis for the Flock of Birds and +x for
	 * OpenGL.
	 */
	input.x = bird_data->position.nY * 0.9144f / 32767.f + CALxOFFSET;
	input.y = bird_data->position.nZ * 0.9144f / 32767.f + CALyOFFSET;
	input.theta = -(bird_data->angles.nRoll / 32767.f + 1.0f) * atan(1.0f) * 4;
	input.time = frame.dwTime - birds_start;

	return input;
}

bool BirdInput::ProcessData()
{
	if (birdFrameReady(GROUP_ID))
	{
		birdGetFrame(GROUP_ID, &frame);
		return true;
	}
	else
	{
		return false;
	}
}