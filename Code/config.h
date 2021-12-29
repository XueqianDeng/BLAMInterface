#ifndef CONFIG_H
#define CONFIG_H
#pragma once

#include "SDL_opengl.h"

////////// Set paths for session file and data /////////////////
//#define TFILE "Resources/tgt_files_updated/Subject_32/B9.tgt" // File to load that defines block
//#define DATA_PATH "Data/MB_32/B9" // Folder to store data in
#define TFILE "Resources/tgt_files_dense_2side_with_x/Subject_14/B1.tgt" // File to load that defines block
#define DATA_PATH "Data/test3/B1" // Folder to store data in
///////////////////////////////////////////////////////////////

#define BIMANUAL_MODE 0 // 1 = two hands, 0 = one hand only
#define REVERSAL_MODE 1 // 1 = original, -1 = reversed
#define ALTERNATE_MODE 0 // 1 = hands swapped, 0 = original
#define RETURN_CIRC 1 // use the return circle that guides you back to the center or not

#define HANDS_VISIBLE 1 //
#define INITIAL_ALIGNMENT 0

#define REV 1-2*REVERSAL_MODE
#define TARGHITS 50
#define HAND RIGHTHAND
#define LEFTHAND 1
#define RIGHTHAND 3

// The parameter below controls whether you want to use an additional wait time during which the target is present but the participant should not yet move
#define USE_WAIT 1
#define GO_TIME 500

// Configurable constants
#define NCOLS 8

#define TEST_MODE 0 // 1=test, 0=expt
#define PRACTICE_MODE 0// 1=practice, no reinforcement given; 0=expt
#define DAY_TWO 0//0=day 1 or explore, 1=day 2 no score shown
#define SCREEN_WIDTH  1920//1280//1280//
#define SCREEN_HEIGHT  1080//720//800//1050
#define SCREEN_BPP  32
#define WINDOWED  false
#define MIRRORED  true
#define PHYSICAL_WIDTH   1.21f // meters
#define PHYSICAL_RATIO  (PHYSICAL_WIDTH / SCREEN_WIDTH) // meters per pixel
#define PHYSICAL_HEIGHT  (SCREEN_HEIGHT * PHYSICAL_RATIO) // meters

#define CALxOFFSET 0.62445f   //meters
#define CALyOFFSET 0.17263f   //meters

#define ITI 500
#define START_RADIUS  0.005f // meters
#define TARGET_RADIUS  0.005f
//#define TARGET_RADIUS  0.02f
#define HTARGET_RADIUS 0.0075f
#define CURSOR_RADIUS 0.0025f
#define VIS_RADIUS .015f // radius from start within which the cursor is visible
#define OBJECT_RADIUS .004f

#define GROUP_ID  1
#define READ_TIMEOUT  2000
#define WRITE_TIMEOUT  2000
#define BAUD_RATE  115200
#define BIRDCOUNT  4
#define TARGET_DIST .12

#define MOV_TIME 80000 // movement time limit in ms
#define FREEZE_TIME 500
#define VEL_THR 0.0005 //km changed from 0.0002
#define VEL_THR_OBJ 0.005 // object velocity threshold
#define LOW_VEL_THR 0.0001

#define MAX_SPEED .0022f
#define MIN_SPEED .0028f

#define START_X .6
//#define START_Y .2
#define START_Y .25
#define START_Z .2

#define PI 3.14159

// trakSTAR config
#define SAMPRATE 130
#define TRACKTYPE 0
#define FILTER_WIDE false
#define FILTER_NARROW false
#define FILTER_DC 1.0f

#define LHBIRD 1
#define RHBIRD 3

//calibration parameters
#define CALxOFFSET 0.61133f - .005f//.62445f   //meters
#define CALyOFFSET 0.16694f+0.014f//.17263f   //meters
#define CALxyROTANG 0.025f//-0.02140f //radians

#define LX .6128f
#define LY .1811f
#define RX .6462f
#define RY .1617f

#define DELTA .01

#define LH_START_X 0
#define LH_START_Y -.1
#define RH_START_X -.1
#define RH_START_Y 0

#define INIT_WAIT 1000

#endif
