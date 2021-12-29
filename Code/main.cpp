/* Control software for kinereach
See readme.txt for further details

Authors:
	Adrian Haith
	Promit Roy
	Aneesh Roy
	Aaron Wong
	Alkis Hadjiosif
*/

#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <direct.h>
#include <string>
#include <stdio.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "DataWriter.h"
#include "Object2D.h"
#include "Sound.h"
#include "ExtFile.h"
#include "Circle.h"
#include "HandCursor.h"
#include "EllipseE.h"
#include "Region2D.h"
#include "Trial.h"
#include "TrackBird.h"
#include "DynObj.h"
#include "Rect.h"
#include "SpeedBar.h"

#include <gl/GL.h>
#include <gl/GLU.h>

enum GameState // defines possible states to control the flow of each trial
{
	Idle = 0x01,     //00001
	WaitStart = 0x03, //00011
	Reach = 0x04,   //00100
	Return = 0x05,
	WaitStartReturn = 0x0C,   //01100
	Finished = 0x10,  //10000
	ITI_wait = 0x06,
	WaitGo = 0x07,
	Exiting = 0x08,
	Initial = 0x09,
	AlignHands = 0x11,
	InitPause = 0x12,
	WaitNoGo = 0x0A,
};

SDL_Event event;
SDL_Surface* screen = NULL;
/* COM ports where the Flock of Birds are connected. The first element is always
 * 0, and subsequent elements are the port numbers in order.
 */
bool quit = false;
WORD COM_port[5] = {0, 5, 6, 7, 8};
Object2D* player = NULL;
Region2D* skillRegion = NULL;
GLfloat verts[4][2] = {{.505, .230}, {.552, .283}, {.578, .309}, {.548, .339}};
GLfloat skillRegionCol[3] = {1.0f, 1.0f, .0f};
GLfloat failColor[3] = {1.0f, 0.0f, 0.0f};
Image* trialNum;
Image* textEarly;
Image* currentx;
Image* currenty;
Image* currentz;
Image* currentazimuth;
Image* currentelevation;
Image* currentroll;
Image* currentscore;
Image* tothits;

Rect* frame;
Rect* bar;
Rect* bg;
SpeedBar* speedBar;

Sound* hit = NULL;
Sound* beep = NULL;
Sound* fail = NULL;
TTF_Font* font = NULL;
SDL_Color textColor = {0, 0, 0};
DataWriter* writer = NULL;
GameState state;
Uint32 gameTimer;
Uint32 hoverTimer;
Uint32 ITI_Timer;
Uint32 movTimer;
Uint32 freezeTimer;
Uint32 textTimer;
Uint32 quitTimer;

GLfloat targDist;
GLfloat startDist;
GLfloat dProd;
GLfloat tDist;

Trial trial(TFILE); // load trial file

std::ofstream tFileCopy;
bool recordingStarted;
bool movedTooEarly;

//going to try to add multiple targets, cursors, and starts.
Circle* targCircle;
Circle* aimCircle;
Circle* cursCircle; // and each cursor will be controlled by that single deg of freedom
Circle* crossingPointCircle;
Circle* startCircle; //in this case, this will be related to the left-hand transmuter's x-movement
Circle* objCircle;
Circle* rightCircle;
Circle* leftCircle;
Circle* rightStart;
Circle* leftStart;
Circle* returnCircle;

Circle* aimingCircle[8]; // aiming targets

float targetColor[3] = {.5f, .5f, .5f};
float aimColor[3] = {1.0f, .5f, .5f};
float hitColor[3] = {.0f, .8f, .0f};
float startColor[3] = {.8f, .8f, .0f};
float cursColor[3] = {.0f, .0f, .5f};
float aimingColor[3] = {.8f, .8f, .8f};
float missColor[3] = {1.0f, .0f, .0f};
float stopColor[3] = {.69f,.77f,.87f};
float defaultColor[3] = {1.0f,1.0f,0.4f};
float objColor[3] = {1.0f,0.0f,0.4f};
float returnColor[3] = {1.0f,1.0f,1.0f};

float score=0;

HandCursor* curs;
HandCursor* leftHand;
HandCursor* rightHand;
HandCursor* crossingPoint;

int trialDone = 0;
int totalhits = 0;
bool targetHit = 0;

bool stationary;
bool targHit;
bool perturbed = 0;

GLfloat pkVel = 0; // peak velocity

// Initializes everything and returns true if there were no errors
bool init();
// Initializes the Flock of Birds and returns true if successful
bool init_fob();
// Sets up OpenGL
void setup_opengl();
// Performs closing operations
void clean_up();
// Draws objects on the screen
void draw_screen();
// Main game loop
void game_update();
// Checks whether a given directory exists
bool dirExists(const std::string& dirName_in);
bool fExists(const std::string& dirName_in);

//BIRDSYSTEMCONFIG sysconfig;
TrackSYSCONFIG sysconfig;
TrackDATAFRAME dataframe;
//std:: wstringstream path;
bool trak_connected;
bool targetOn = false;

int main(int argc, char* args[])
{
	//HIGH_PRIORITY_CLASS
	if (SetPriorityClass(GetCurrentProcess(),ABOVE_NORMAL_PRIORITY_CLASS))
		std::cerr << "Promote process priority to Above Normal." << std::endl;
	else
		std::cerr << "Promote process priority failed." << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!init())
	{
		// There was an error during initialization
		return 1;
	}
	
	while (!quit)
	{
		bool inputs_updated = false;

		// Retrieve Flock of Birds data
		if (trak_connected)
		{
			// Update inputs from Flock of Birds
			inputs_updated = TrackBird::GetUpdatedSample(&sysconfig, &dataframe);
		}

		// Handle SDL events
		while (SDL_PollEvent(&event))
		{
			// See http://www.libsdl.org/docs/html/sdlevent.html for list of event types
			if (event.type == SDL_KEYDOWN)
			{
				// See http://www.libsdl.org/docs/html/sdlkey.html for Keysym definitions
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{

					quit = true;
				}
			}
			else if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		
		
		// Get data from input devices
		if (inputs_updated) // if there is a new frame of data
		{
			// critical line of code!!
			// relationship between trakstar pos and cursor
			//leftHand->UpdatePos((dataframe.x[3]-LX)*PHYSICAL_WIDTH/(RX-LX), (dataframe.y[3]-LY)*PHYSICAL_HEIGHT/(RY-LY));
			//rightHand->UpdatePos((dataframe.x[2]-LX)*PHYSICAL_WIDTH/(RX-LX), (dataframe.y[2]-LY)*PHYSICAL_HEIGHT/(RY-LY));
			leftHand->UpdatePos(dataframe.x[LHBIRD], dataframe.y[LHBIRD]);
			rightHand->UpdatePos(dataframe.x[RHBIRD], dataframe.y[RHBIRD]);
			if(BIMANUAL_MODE == 1)
			{
				if(ALTERNATE_MODE)
				{
					curs->UpdatePos(REVERSAL_MODE*(rightHand->GetY()-START_Y)+START_X,leftHand->GetX()+.2-START_X+START_Y);
				}
				else
				{
					curs->UpdatePos(REVERSAL_MODE*(leftHand->GetY()-START_Y)+START_X,rightHand->GetX()-.2-START_X+START_Y);
				}
			}
			else if(BIMANUAL_MODE == 2)
			{
				curs->UpdatePos(REVERSAL_MODE*(rightHand->GetX()-START_X)+START_X,leftHand->GetY()-START_Y+START_Y);
			}
			else if(BIMANUAL_MODE == 3)
			{
				curs->UpdatePos(REVERSAL_MODE*(rightHand->GetY()-START_Y)+START_X,rightHand->GetX()-.2-START_X+START_Y);
			}
			else if(BIMANUAL_MODE ==4)
			{
				curs->UpdatePos(REVERSAL_MODE*(.5*rightHand->GetX()+.5*leftHand->GetX()-START_X)+START_X, .5*rightHand->GetY()+.5*leftHand->GetY());
			}
			else
				//unimanual
			{
				if (HAND==RIGHTHAND)
				{
				curs->UpdatePos(REVERSAL_MODE*(rightHand->GetX()-START_X)+START_X, rightHand->GetY());
				}
				else if (HAND==LEFTHAND)
				{
					curs->UpdatePos(REVERSAL_MODE*(leftHand->GetX()-START_X)+START_X, leftHand->GetY());
				}

			}
			writer->Record(leftHand,rightHand,curs,state,perturbed,SDL_GetTicks());
			//speedBar->UpdateSpeed(pkVel);
		} 
		game_update(); // Run the game loop
		draw_screen();
	}
	clean_up();
	return 0;
}

bool init()
{
	
	printf("Initializing.\n");
	// Initialize trakStar
	trak_connected = TrackBird::InitializeBird(&sysconfig);

	// Initialize SDL, OpenGL, SDL_mixer, and SDL_ttf
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
		SDL_OPENGL | (WINDOWED ? 0 : SDL_FULLSCREEN));
	if (screen == NULL)
	{
		return false;
	}
	setup_opengl();
	//Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 512);
	if (TTF_Init() == -1)
	{
		return false;
	}

	//turn off the computer cursor
	SDL_ShowCursor(0);

	// Load files and initialize pointers
	font = TTF_OpenFont("Resources/arial.ttf", 28);

	//adding new starting circles one for each degree of freedom, ideally
	targCircle = new Circle(.5f, .5f, TARGET_RADIUS, targetColor);
	aimCircle = new Circle(.5f, .5f, TARGET_RADIUS*3/4, aimColor);
	cursCircle = new Circle(.2f, .2f, CURSOR_RADIUS, cursColor);
	returnCircle = new Circle(.5f, .5f, TARGET_DIST, returnColor);
	crossingPointCircle = new Circle(.2f, .2f, CURSOR_RADIUS, objColor);
	startCircle = new Circle(.5f, .5f, START_RADIUS, startColor);
	objCircle = new Circle(.5f, .5f, OBJECT_RADIUS, objColor);
	leftCircle = new Circle(.2f, .2f, CURSOR_RADIUS, hitColor);
	rightCircle = new Circle(.2f, .2f, CURSOR_RADIUS, targetColor);
	leftStart = new Circle(.5f, .5f, CURSOR_RADIUS, hitColor);
	rightStart = new Circle(.5f, .5f, CURSOR_RADIUS, targetColor);

	for(int i=0; i<8; i++)
	{
		aimingCircle[i] = new Circle(START_X+.12*sin(2*3.14159*(i+.5)/8), START_Y+.12*cos(2*3.14159*(i+.5)/8), TARGET_RADIUS/2, targetColor);
		aimingCircle[i]->On();
		aimingCircle[i]->BorderOff();
	}

	frame = new Rect(.05f,.05f,.05f,.2f,targetColor);
	bar = new Rect(.05f,.05f,.025f,.2f,targetColor);
	bg = new Rect(.05f,.05f,.025f,.2f,targetColor);
	speedBar = new SpeedBar(.1f,.2f,.025f,.2f,MIN_SPEED,MAX_SPEED,frame,bar,bg); // initialize speed bar with these two rectangles

	// Assign array index 0 of controller and cursor to correspond to mouse control
	//controller[0] = new MouseInput();
	//cursor[0] = new Object2D(smallCirc);
	hit = new Sound("Resources/coin.wav");
	beep = new Sound("Resources/beep-7.wav");
	fail = new Sound("Resources/barrierBeep.wav");

	curs = new HandCursor(cursCircle);
	crossingPoint = new HandCursor(crossingPointCircle);
	leftHand = new HandCursor(leftCircle);
	rightHand = new HandCursor(rightCircle);
	curs->SetOrigin(START_X, START_Y);


	if(INITIAL_ALIGNMENT)
	{
	leftStart->SetPos(START_X+LH_START_X,START_Y+LH_START_Y);
	rightStart->SetPos(START_X+RH_START_X,START_Y+RH_START_Y);
	leftStart->On();
	rightStart->On();
	leftStart->BorderOff();
	rightStart->BorderOff();
	rightHand->On();
	leftHand->On();

	}
	else
	{
	leftStart->SetPos(START_X+LH_START_X,START_Y+LH_START_Y);
	rightStart->SetPos(START_X+RH_START_X,START_Y+RH_START_Y);
	leftStart->Off();
	rightStart->Off();
	leftStart->BorderOff();
	rightStart->BorderOff();

	}

	targCircle->Off();
	targetOn = false;
	targCircle->SetPos(START_X+trial.TargX(),START_Y+trial.TargY());

	aimCircle->Off();
	GLfloat x_aim = trial.TargX()*cos(-trial.Rotation()*3.14159/180)+trial.TargY()*sin(-trial.Rotation()*3.14159/180)+START_X;
	GLfloat y_aim = -trial.TargX()*sin(-trial.Rotation()*3.14159/180)+trial.TargY()*cos(-trial.Rotation()*3.14159/180)+START_Y;
	aimCircle->SetPos(x_aim,y_aim);

	//startCircle->Off();
	curs->Off();
	crossingPoint->Off();

	startCircle->SetPos(START_X, START_Y);
	returnCircle->SetPos(START_X, START_Y);
	returnCircle->FillOff();
	returnCircle->SetBorderWidth(0.002f);
	returnCircle->SetBorderColor(cursColor);

	/* To create text, call a render function from SDL_ttf and use it to create
	 * an Image object. See http://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf.html#SEC42
	 * for a list of render functions.
	 */
	delete textEarly;
	textEarly = new Image(TTF_RenderText_Blended(font, "TOO EARLY", textColor));
	//text2 = new Image(TTF_RenderText_Blended(font, "there is text here", textColor));

	// Create directory with date and time etc
		time_t current_time = time(0);
		tm* ltm = localtime(&current_time);
		std::stringstream path;
		path << "1000";
		/*
		path << std::setw(4) << std::setfill('0') << ltm->tm_year + 1900;
		path << std::setw(2) << std::setfill('0') << ltm->tm_mon + 1;
		path << std::setw(2) << std::setfill('0') << ltm->tm_mday;
		path << "_";
		path << std::setw(2) << std::setfill('0') << ltm->tm_hour;
		path << std::setw(2) << std::setfill('0') << ltm->tm_min;
		path << std::setw(2) << std::setfill('0') << ltm->tm_sec;
		path << ".txt";*/
		//LPCWSTR tmp = (LPCWSTR) path.str().c_str();
		//LPCSTR tmp = (LPCSTR) path.str().c_str();
		//tmp = "Data5";
	//CreateDirectoryA(tmp, NULL);
	//	CreateDirectory(L"Data2", NULL);
	writer = new DataWriter();


	std::stringstream TFpath;
	TFpath << DATA_PATH << "/tFile.tgt";

	// Check if Data Directory Exists. If not, create it.
	bool dirExist = dirExists(DATA_PATH);
	if(!dirExist){
		mkdir(DATA_PATH);
	}
	/*else{
		if(fExists(TFpath.str())){
			return false;
		}
	}*/
	dirExist = dirExists(DATA_PATH);
	if(!dirExist){
		return false;
	}	
	
	tFileCopy.open(TFpath.str(),std::ios::out);
	//tFileCopy << "Testing\n";

	SDL_WM_SetCaption("KineReach Demo", NULL);

	//start->SetPos(PHYSICAL_WIDTH / 2, PHYSICAL_HEIGHT / 2); // Set the start marker in the center

	if(INITIAL_ALIGNMENT)
	{
		state = AlignHands; // Set the initial game state
		//state = Initial;
	}
	else
	{
		state = InitPause;
	}

	//tFile = ExtFile("tFile.tgt",3);
	//cursor[0]->SetSize(.01f,.01f);
	//start->SetSize(.02f,.02f);
	//target->SetSize(.04f,.04f);

	// render new trial number image
	std::stringstream textss;
	textss << trial.Number();
	delete trialNum;
	trialNum = new Image(TTF_RenderText_Blended(font, textss.str().c_str(), textColor));

	// render new image to display score
	std::stringstream textssscore;
	textssscore << 0;
	delete currentscore;
	currentscore = new Image(TTF_RenderText_Blended(font, textssscore.str().c_str(), textColor));

	//render new image to display hit counter
	std::stringstream textsstotalhits;
	textsstotalhits << 0;
	delete tothits;
	tothits = new Image(TTF_RenderText_Blended(font, textsstotalhits.str().c_str(), textColor));

	

	return true;
}


static void setup_opengl()
{
	glClearColor(255, 255, 255, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glShadeModel(GL_SMOOTH);   
	/* The default coordinate system has (0, 0) at the bottom left. Width and
	 * height are in meters, defined by PHYSICAL_WIDTH and PHYSICAL_HEIGHT
	 * (config.h). If MIRRORED (config.h) is set to true, everything is flipped
	 * horizontally.
	 */
	glOrtho(MIRRORED ? PHYSICAL_WIDTH : 0, MIRRORED ? 0 : PHYSICAL_WIDTH,
		0, PHYSICAL_HEIGHT, -1.0f, 1.0f);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

	//glEnable(GL_TEXTURE_2D);
}

void clean_up()
{
	//delete dataframe;
	delete curs;
	delete crossingPoint;
	delete rightHand;
	delete leftHand;

	//delete start;
	//delete target;
	delete targCircle;
	delete aimCircle;

	delete hit;
	delete beep;
	//delete text;
	//delete text2;
	delete writer;
	tFileCopy.close();
	Mix_CloseAudio();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	if (trak_connected)
	{
		TrackBird::ShutDownBird(&sysconfig);
	}
}

static void draw_screen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	returnCircle->Draw();

	//for (int i=0; i<8; i++)
	//{
	//	aimingCircle[i]->Draw();
	//}
	aimCircle->Draw();
	targCircle->Draw();
	
	startCircle->Draw();


	leftStart->Draw();
	rightStart->Draw();

	speedBar->Draw();

	curs->Draw();
	crossingPoint->Draw();

	if(0)//HANDS_VISIBLE)
	{
		leftHand->Draw();
		rightHand->Draw();
	}

	//write the trial number
	trialNum->Draw(PHYSICAL_WIDTH*23/24, PHYSICAL_HEIGHT*23/24);
	if (movedTooEarly)
	{
		textEarly->Draw(startCircle->GetX(),startCircle->GetY()+0.05);
	}

	SDL_GL_SwapBuffers();
	glFlush();
}

void game_update()
{

	startDist = curs->Distance(startCircle);
	targDist = curs->Distance(targCircle);

	switch (state)
	{
		case AlignHands:
			if(leftHand->Distance(leftStart)<START_RADIUS & rightHand->Distance(rightStart)<START_RADIUS)
			{
				leftHand->Off();
				rightHand->Off();
				//curs->On();
				leftStart->Off();
				rightStart->Off();
				//startCircle->On();
				hoverTimer = SDL_GetTicks();
				state = InitPause;
			}
			break;

		case InitPause:
			if(SDL_GetTicks()-hoverTimer>INIT_WAIT)
			{
				state = Idle;
				curs->On();
				startCircle->On();
			}

		case Initial: // wait for experimenter to press return key to start
			if (event.key.keysym.sym == SDLK_RETURN)
			{
				GLfloat x_aim = trial.TargX()*cos(-trial.Rotation()*3.14159/180)+trial.TargY()*sin(-trial.Rotation()*3.14159/180)+START_X;
				GLfloat y_aim = -trial.TargX()*sin(-trial.Rotation()*3.14159/180)+trial.TargY()*cos(-trial.Rotation()*3.14159/180)+START_Y;
				targCircle->SetPos(trial.TargX()+START_X,trial.TargY()+START_Y);
				aimCircle->SetPos(x_aim,y_aim);
				targCircle->On();
				if (trial.ShowAim())
				{
					aimCircle->On();
				}
				state = Idle;
			}
			break;

		case Idle: // wait for player to start trial by entering start circle

			stationary = abs(curs->GetVel())<VEL_THR;
			if(startDist<VIS_RADIUS)
			{
				returnCircle->Off();
				curs->On();
			}
			else
			{
				if (RETURN_CIRC & startDist<TARGET_DIST)
				{
					curs->Off();
					returnCircle->SetDiameter(startDist);
					returnCircle->On();
				}
			}
			
			if (startDist<START_RADIUS & stationary)
			{
				hoverTimer = SDL_GetTicks();
				
				if(!recordingStarted)
				{	// start recording data in a new file (if not started already)
					writer->Close();
					std::stringstream ss;
					if(trial.Number() < 10)
					{
						ss << DATA_PATH << "/Trial_00" << trial.Number()<< "_";
					}
					else if(trial.Number()<100)
					{
						ss << DATA_PATH <<"/Trial_0" << trial.Number() << "_";
					}
					else
					{
						ss << DATA_PATH <<"/Trial_" << trial.Number() << "_";
					}
					writer = new DataWriter(&sysconfig,ss.str().c_str());

					for (int i=0;i<NCOLS;i++)
					{
						tFileCopy << trial.curTrial()[i] << ' ';
					}
					tFileCopy << "\n"; 
					recordingStarted = true;
				}
				
				pkVel = 0; // keep track of peak velocity
				speedBar->UpdateSpeed(pkVel);

				curs->SetRotation(trial.Rotation());
				curs->SetMirror(trial.Mirror());

				state = WaitNoGo;

			}
			break;

		case WaitNoGo: // wait for ITI ms before presenting target. Incorporates WaitGo state in earlier version
			if (curs->Distance(startCircle) > START_RADIUS && targetOn == false)
			{   // go back to Idle if player leaves start circle
				hoverTimer = SDL_GetTicks();
				state = Idle;
			}
			
			else if (SDL_GetTicks() - hoverTimer >= ITI)
			{
				movedTooEarly = false;

				//startCircle->Off();

				targCircle->On();
				targetOn = true;
				if (trial.ShowAim())
				{
					aimCircle->On();
				}
				if (USE_WAIT)
				{
					//reset hoverTimer
					hoverTimer = SDL_GetTicks();
					state = WaitGo;
				}
				else
				{
				beep->Play(); // play GO cue
				gameTimer = SDL_GetTicks();
				trialDone = 0;
				state = WaitStart;
				}
			}
			break;

		case WaitGo: // wait for WAITGO ms before presenting GO cue
			if (curs->Distance(startCircle) > START_RADIUS)
			{   // go back to Idle if player leaves start circle
				hoverTimer = SDL_GetTicks();
				state = WaitNoGo;
				movedTooEarly = true;
				//delete textEarly;
				//textEarly = new Image(TTF_RenderText_Blended(font, "TOO EARLY", textColor));
				//textEarly->Draw(startCircle->GetX(), startCircle->GetY()+0.05);
				//textEarly->Draw(PHYSICAL_WIDTH*12/24, PHYSICAL_HEIGHT*12/24);
			}
			
			else if (SDL_GetTicks() - hoverTimer >= GO_TIME)
			{
				//startCircle->Off();
			//	targCircle->On();
			//	if (trial.ShowAim())
			//	{
			//		aimCircle->On();
			//	}
				beep->Play(); // play GO cue
				gameTimer = SDL_GetTicks();
				trialDone = 0;
				state = WaitStart;
			}
			break;

		case WaitStart:	// wait for subject to start moving ANY cursor
			
			//if (curs->GetVel()>VEL_THR)
			if(curs->Distance(startCircle)>START_RADIUS)
			{
				movTimer = SDL_GetTicks();
				if (!trial.VisualFeedback())
				{
					curs->Off();
				}
				state = Reach;
			}
			break;


		case Reach: // wait for subject to move
			
			if(curs->GetVel()>pkVel)
			{
				pkVel = curs->GetVel();
				//speedBar->UpdateSpeed(pkVel);
			}

			targHit = curs->HitTarget(targCircle);
			stationary = abs(curs->GetVel())<VEL_THR;

			// NB - bad notation, but targHit registers whether target has been hit on this loop (whether by cursor or object, as appropriate). targetHit indicates whether it has been hit at all in this trial.
			//if(((handMode &curs->HitTarget(targCircle)) | (!handMode & object->HitTarget(targCircle))) & !targetHit)

				//if(!perturbed & startDist > targCircle->Distance(startCircle)/3)
				
			dProd = (targCircle->GetX()-startCircle->GetX())*(curs->GetX()-startCircle->GetX()) + (targCircle->GetY()-startCircle->GetY())*(curs->GetY()-startCircle->GetY());
			tDist = targCircle->Distance(startCircle);

			if(!perturbed & dProd/(tDist*tDist) > .333)
			{
				GLfloat dx = (targCircle->GetX()-startCircle->GetX());
				GLfloat dy = (targCircle->GetY()-startCircle->GetY());
				GLfloat dxn = dx/sqrtf(dx*dx + dy*dy);
				GLfloat dyn = dy/sqrtf(dx*dx + dy*dy);
	
				GLfloat psize = 0;//trial.Perturbation();
				//curs->Perturb(dyn*psize,-dxn*psize); // orthogonal perturbation
				targCircle->SetPos(targCircle->GetX()+psize*dyn,targCircle->GetY()-psize*dxn);
				perturbed = 1;
			}

			// If no visual feedback, movement ends when subject stops (provided it is at least 50% of the target distance away)
			if (!trial.VisualFeedback() & stationary & curs->Distance(startCircle)>0.5*TARGET_DIST) {
				trial.Next();

				// create new trial number image
				std::stringstream textss;
				textss << trial.Number();
				delete trialNum;
				trialNum = new Image(TTF_RenderText_Blended(font, textss.str().c_str(), textColor));

				if(trial.Number()==0) //Regardless of how many hits, end session after expt is over (or, if second day, end session here).
				{   // trial number = 0 --> end experiment
					quitTimer = SDL_GetTicks();
					state = Exiting;
				}
				else
				{
					// reset flags
					targetHit = 0;
					perturbed = 0;
					recordingStarted = false;
					
					// show speed
					speedBar->UpdateSpeed(pkVel);
					
					//display score
					ITI_Timer = SDL_GetTicks();
					state = ITI_wait;
				}
			}

			if(targHit & stationary & !targetHit & trial.Type()==1)
			{   // check whether subject has hit the target
				if (trial.VisualFeedback()) {
				//hit->Play();
				//targCircle->SetColor(hitColor);
				}
				
				targetHit = 1;
				//curs->Off();
			
				//startCircle->SetPos(targCircle->GetX(),targCircle->GetY());
				trial.Next();

				// create new trial number image
				
				std::stringstream textss;
				textss << trial.Number();
				delete trialNum;
				trialNum = new Image(TTF_RenderText_Blended(font, textss.str().c_str(), textColor));

				if(trial.Number()==0) //Regardless of how many hits, end session after expt is over (or, if second day, end session here).
				{   // trial number = 0 --> end experiment
					quitTimer = SDL_GetTicks();
					state = Exiting;
				}
				else
				{
					// reset flags
					targetHit = 0;
					perturbed = 0;
					recordingStarted = false;
					
					// show speed
					speedBar->UpdateSpeed(pkVel);
					
					//display score
					ITI_Timer = SDL_GetTicks();
					state = ITI_wait;
				}

				/*if(totalhits >= TARGHITS)
				{
					quitTimer = SDL_GetTicks();
					state = Exiting;
				}*/
			}

			if(trial.Type()==2 & curs->Distance(startCircle)>TARGET_DIST) {
				//crossingPoint->UpdatePos(curs->GetX(),curs->GetY());
				//GLfloat* freezePosition = curs->GetCrossingPoint(startCircle->GetX(),startCircle->GetY(),TARGET_DIST);
				//crossingPoint->UpdatePos(freezePosition[0],freezePosition[1]);

				curs->CalcCrossingPoint(startCircle->GetX(),startCircle->GetY(),TARGET_DIST);
				crossingPoint->UpdatePos(curs->GetXfb(),curs->GetYfb());
				if (crossingPoint->Distance(targCircle)<TARGET_RADIUS)
				{
				//hit->Play();
				//targCircle->SetColor(hitColor);
				}
				
				// Alkis 12/5/2017: remove mirroring from the return movement (this will kick on in the next InterTrial state) 2/5/2018: moved this from the beginning of Return
				curs->Off();
				//curs->SetMirror(0); I moved this later

				if (trial.VisualFeedback())
				{
					crossingPoint->On();
				}
				freezeTimer = SDL_GetTicks();
				state = Return;
			}

			if(SDL_GetTicks()-movTimer>MOV_TIME)
			{
					targetHit = 0;
					perturbed = 0;
					recordingStarted = false;
					
					// show speed
					speedBar->UpdateSpeed(pkVel);
					
					std::stringstream textss;
					textss << trial.Number();
					delete trialNum;
					trialNum = new Image(TTF_RenderText_Blended(font, textss.str().c_str(), textColor));

					//display score
					ITI_Timer = SDL_GetTicks();
					trial.Next();
					state = ITI_wait;
			}

			break;

		case Return:

			// The (awkward) solution below is to remove any suspicion that the displayed cursor might be something between the mirrored and the non-mirrored version, as cursor data suggested it might be happening right before
			// the cursor is turned off. I thus changed it so the cursor becomes non-mirrored well into the freezing time (during which the cursor is invisible anyway, and only crossingPoint is visible). Alkis 2/6/2018
			if (SDL_GetTicks()-freezeTimer>FREEZE_TIME/2)
			{
				curs->SetMirror(0);
			}

			if (SDL_GetTicks()-freezeTimer>FREEZE_TIME)
			{
				crossingPoint->Off();
				trial.Next();

				// create new trial number image
				std::stringstream textss;
				textss << trial.Number();
				delete trialNum;
				trialNum = new Image(TTF_RenderText_Blended(font, textss.str().c_str(), textColor));

				if(trial.Number()==0)
				{   // trial number = 0 --> end experiment
					quitTimer = SDL_GetTicks();
					state = Exiting;
				}
				else
				{
					// reset flags
					targetHit = 0;
					perturbed = 0;
					recordingStarted = false;
					
					// show speed
					speedBar->UpdateSpeed(pkVel);

					state = ITI_wait;
				}
			}
			break;

		case ITI_wait:
			// Wait 1000ms and set state to Idle
			if (SDL_GetTicks() - ITI_Timer >= ITI)
			{
				targCircle->Off();
				targetOn = false;
				targCircle->SetColor(targetColor);
				aimCircle->Off();

				startCircle->On();
				targCircle->SetPos(trial.TargX()+START_X,trial.TargY()+START_Y);

				GLfloat x_aim = trial.TargX()*cos(-trial.Rotation()*3.14159/180)+trial.TargY()*sin(-trial.Rotation()*3.14159/180)+START_X;
				GLfloat y_aim = -trial.TargX()*sin(-trial.Rotation()*3.14159/180)+trial.TargY()*cos(-trial.Rotation()*3.14159/180)+START_Y;
				aimCircle->SetPos(x_aim,y_aim);
				//curs->Perturb(0,0);
				state = Idle;
			}
			break;
			

		case Exiting:
			if(SDL_GetTicks()-quitTimer > 3000)
			{
				quit = true;
			}
			break;
	}
}

bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}

bool fExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  return true;
}

//void pix2m(x,y)

