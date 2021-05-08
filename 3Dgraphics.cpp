
//#include <iostream>
//#include <stdio.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <OpenGL/glext.h>
//#include <GLUT/glut.h>

// OpenGL toolkit for PC
#include <gl/freeglut.h>
#include <iostream>
#include <math.h>


#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

bool collision;

using namespace std;



//foward declare functions
void resetPerspectiveProjection();
void drawAngleWithScaledText(GLfloat scale);
void setOrthographicProjection();
void ChangeSize(int w, int h);
void RenderScene(void);
void TimerFunc(int value);

GLint crystalCollected = 0;

GLint Wwidth;
GLint Wheight;
GLfloat planetRotation = 0.0;
//used for the framerate
GLint frame = 0, timeTot = 0, timebase = 0;
char frameRate[12];




//ROTATIONS
float earthRotation = 0.0;
float marsRotation = 0.0;
float moonRotation = 0.0;
float jupiterRotation = 0.0;
float crystalRotation = 0.0;
float torusRotation = 0.0;
float lightChangeRotation = 0.0;







//camera floating
GLfloat cameraX = 0.0;
GLfloat cameraY = 0.0;
GLfloat cameraZ = 300.0;
bool moveCamera = false;
bool moveCameraLeft = false;
bool moveCameraRight = false;

GLfloat shipx = cameraX;
GLfloat shipy = cameraY;
GLfloat shipz = cameraZ;


typedef struct Point3D {
	float x;
	float y;
	float z;
} Point3D;

Point3D posCrystalHoop[6] =
{
	{0, 0, 10},
	{150, 210, 10},
	{400, 355, 10},
	{-200, -130, 10},
	{-350, -260, 10},
	{-500, -470, 10}
};


Point3D shipPosition[] =
{
	{0, 0, 300}
};




//TEXTURES
#define TEXTURE_SPACE 0
#define TEXTURE_HUD 1
#define TEXTURE_CRYSTAL 2
#define TEXTURE_EARTH_DAY 3
#define TEXTURE_EARTH_NIGHT 4
#define TEXTURE_MOON 5
#define TEXTURE_MARS 6
#define TEXTURE_JUPITER 7
#define TEXTURE_CRYSTAL1 8

#define TEXTURE_COUNT 9
GLuint  textures[TEXTURE_COUNT];

const char* textureFiles[TEXTURE_COUNT] = { "galaxy.tga", "hud.tga","crystal3.tga","earthday.tga","earthnight.tga","moon.tga","mars.tga","jupiterC.tga","crystal2.tga" };
const float partOf1 = 0.8f;

//LIGHT 

//Lighting intensity and colour
GLfloat  greenLight[] = { 0.0f, 0.3f, 0.0f, 1.0f };
GLfloat  redLight[] = { 0.9f, 0.0f, 0.0f, 1.0f };
GLfloat  blueLight[] = { 0.0f, 0.0f, 0.7f, 1.0f };
GLfloat  whiteLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };

//Position of the lights
GLfloat	 lightPos0[] = { -150.0f, 0.0f, 0.0f, 0.0f };
GLfloat	 lightPos1[] = { 150.0f, 0.0f, 0.0f, 0.0f };
GLfloat	 lightPos2[] = { -100.0f, 100.0f, 0.0f, 0.0f };
GLfloat shine = 80;

// here are the new light positions - at 120 degrees apart around the centre
GLfloat	 lightPos3[] = { 0.0f, 0.0f, -120.0f, 0.0f };
GLfloat	 lightPos4[] = { -100.0f, 0.0f, 20.0f, 0.0f };
GLfloat	 lightPos5[] = { 60.0f, 0.0f, 60.0f, 0.0f };

//materials for the new lights
GLfloat mat_specular[] = { 0.5, 1.0, 1.0, 1.0 }; //material specular value
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //specular value for lights


bool hud = true;

//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;


//this function pops back to the last projection
void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


//used for drawing text
void displayText(GLfloat x, GLfloat y, GLdouble r, GLdouble g, GLdouble b, const char* string, bool stroke) {
	GLint j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (GLint i = 0; i < j; i++)
	{
		if (stroke)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
		}

	}
}


//DRAW FUNCTIONS
void drawSpace(int image)
{

	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); 
	// glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere


	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glColor4f(0., 0.6, 1.0, partOf1 + 0.2f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-800.0, -800.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(800.0, -800.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(800.0, 800.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-800.0, 800.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


}



void drawHud(int image)
{

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.8, 0.8, 0.8, 0.4);

	glBindTexture(GL_TEXTURE_2D, textures[image]);
	//glTranslatef(0, 0, 0);
	glBegin(GL_QUADS);

	//glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	//glVertex2f(-2500, -1500);

	glVertex2f(-Wwidth, -Wheight);

	glTexCoord2f(1.0, 0.0);
	//glVertex2f(2500, -1500);

	glVertex2f(Wwidth, -Wheight);


	glTexCoord2f(1.0, 1.0);
	//glVertex2f(2500, 1500);

	glVertex2f(Wwidth, Wheight);

	glTexCoord2f(0.0, 1.0);
	//glVertex2f(-2500, 1500);

	glVertex2f(-Wwidth, Wheight);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	//glFlush();

	//glPopMatrix();


}


void drawEarth(int image) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glTranslated(40, -20.0, 0.0);
	glColor4f(0.7f, 0.8f, 1.0f, 1.0f);
	glPushMatrix();
	//new type of sphere that can be textured
	//could also use a glutSolidSphere
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 20.0f, 30, 17);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


}

void drawMoon(int image) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glTranslated(100, 0.0, 0.0);
	glColor4f(0.7f, 0.8f, 1.0f, 1.0f);
	glPushMatrix();
	//new type of sphere that can be textured
	//could also use a glutSolidSphere
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 20.0f, 30, 17);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void drawMars(int image) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glTranslated(-100, -10.0, 0.0);
	glColor4f(0.7f, 0.8f, 1.0f, 1.0f);
	glPushMatrix();
	//new type of sphere that can be textured
	//could also use a glutSolidSphere
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 15.0f, 30, 17);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}



void drawTexturedHud(int image)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(0.8, 0.8, 0.8, 0.4);

	glBindTexture(GL_TEXTURE_2D, textures[image]);
	//glTranslatef(0, 0, 0);
	glBegin(GL_QUADS);

	//glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	//glVertex2f(-2500, -1500);

	glVertex2f(-Wwidth, -Wheight);

	glTexCoord2f(1.0, 0.0);
	//glVertex2f(2500, -1500);

	glVertex2f(Wwidth, -Wheight);


	glTexCoord2f(1.0, 1.0);
	//glVertex2f(2500, 1500);

	glVertex2f(Wwidth, Wheight);

	glTexCoord2f(0.0, 1.0);
	//glVertex2f(-2500, 1500);

	glVertex2f(-Wwidth, Wheight);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	//glFlush();

	//glPopMatrix();

}

char score[12];
char xCoord[12];
char yCoord[12];
char zCoord[12];


void drawAngleWithScaledText(GLfloat scale)
{
	glPushMatrix();
	double r = 0.0;
	double g = 1.0;
	double b = 0.0;

	//draw the text offset from the box
	glTranslatef(40.0, 80.0, 0.0);
	glColor3f(0.8, 0.3, 0.8);
	//sprintf_s(teapotRotationString, " SPACE GAME ", crystalRotation);
	sprintf_s(score, "SCORE :%3.0f");// NOT SURE WHY CRYSTAL COLLECTED NOT WORKING
	//sprintf(yCoord, "y coordinate: %4.2f", shipPosition);

	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText

	displayText(500, 100, r, g, b, score, true); //SETTING FOR TEXT
	//displayText(500, 100, r, g, b, yCoord, true); //SETTING FOR TEXT

	glPopMatrix();
}
void drawAngleWithScaledText1(GLfloat scale)
{
	glPushMatrix();
	double r = 1.0;
	double g = 1.0;
	double b = 0.0;

	//draw the text offset from the box
	glTranslatef(40.0, 120.0, 0.0);
	glColor3f(0.8, 0.3, 0.8);

	sprintf(xCoord, "Ship position X: %4.2f", shipPosition->x);


	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText

	displayText(500, 100, r, g, b, xCoord, true); //SETTING FOR TEXT
	//displayText(500, 100, r, g, b, yCoord, true); //SETTING FOR TEXT

	glPopMatrix();


}

void drawAngleWithScaledText2(GLfloat scale)
{
	glPushMatrix();
	double r = 1.0;
	double g = 0.0;
	double b = 1.0;

	//draw the text offset from the box
	glTranslatef(40.0, 160.0, 0.0);
	glColor3f(0.8, 0.3, 0.8);

	sprintf(yCoord, "Ship position Y: %4.2f", shipPosition->y);


	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText

	displayText(500, 100, r, g, b, yCoord, true); //SETTING FOR TEXT
	//displayText(500, 100, r, g, b, yCoord, true); //SETTING FOR TEXT

	glPopMatrix();

}

void drawAngleWithScaledText3(GLfloat scale)
{
	glPushMatrix();
	double r = 0.0;
	double g = 1.0;
	double b = 1.0;

	//draw the text offset from the box
	glTranslatef(40.0, 200.0, 0.0);
	glColor3f(0.8, 0.3, 0.8);

	sprintf(zCoord, "Ship position Z: %4.2f", shipPosition->z);


	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText

	displayText(500, 100, r, g, b, zCoord, true); //SETTING FOR TEXT
	//displayText(500, 100, r, g, b, yCoord, true); //SETTING FOR TEXT

	glPopMatrix();

}


void drawFrameRate()
{
	//this draws the framerate on the screen - useful for testing animation
	frame++;
	timeTot = glutGet(GLUT_ELAPSED_TIME);
	if (timeTot - timebase > 1000)
	{
		sprintf_s(frameRate, "FPS: %4.2f", frame * 1000.0);

		timebase = timeTot;
		frame = 0;
	}

	//set the text to white
	glColor3f(1.0f, 1.0f, 1.0f);

	//this stop the text being affected by the current tranformation by reseting it
	glLoadIdentity();
	displayText(30, 30, 1, 0, 0, frameRate, false);
}


//sets up the orthographics projection for 2D overlay
void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, Wwidth, 0, Wheight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -Wheight, 0);
	//set for drawing again
	glMatrixMode(GL_MODELVIEW);
}



//WHEN RESIZE THE WINDOW NEED TO CHANGE ALSO THE SIZE OF THE HUD

void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	Wwidth = w;
	Wheight = h;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;
	//need this for changing projection

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 625
	//gluPerspective(100.0f, fAspect, 1.0, 1600.0);
	gluPerspective(100.0f, fAspect, 1.0, 800.0);
	//gluPerspective(45.0f, fAspect, 1.0, 600.0);
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);


}
void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN)
	{
		moveCamera = !moveCamera;
		/*	cameraX = 0.0;
			cameraY = 0.0;
			cameraZ = 200.0;*/
		if (!moveCamera) {
			/*cameraX = 0.0;*/
			cameraY = cameraY - 10;
			shipPosition->y = shipPosition->y - 10;
			/*cameraZ = 50;*/
		}

	}


	if (key == GLUT_KEY_UP)
	{
		moveCamera = !moveCamera;
		/*cameraX = 0.0;
		cameraY = 0.0;
		cameraZ = 200.0;*/
		if (!moveCamera) {
			//cameraX = 0.0;
			cameraY = cameraY + 10;
			shipPosition->y = shipPosition->y + 10;
			//cameraZ = 50.0;
		}
	}

	if (key == GLUT_KEY_RIGHT) //rotating spacecraft
	{
		moveCamera = !moveCamera;
		/*	cameraX = 0.0;
			cameraY = 0.0;
			cameraZ = 200.0;*/
		if (!moveCamera) {
			cameraX += 50.0;
			cameraY = 0.0;
			cameraZ = 200.0;
		}
	}

	if (key == GLUT_KEY_LEFT) //rotating spacecraft
	{
		moveCamera = !moveCamera;
		/*	cameraX = 0.0;
			cameraY = 0.0;
			cameraZ = 200.0;*/
		if (!moveCamera) {
			cameraX -= 50.0;
			cameraY = 0.0;
			cameraZ = 200.0;
		}
	}

	glutPostRedisplay();
}

//after turning off not turning on
void ZoomInOut(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'h': //hud on/off
		if (hud) hud = 0.0;
		else hud = 1.0;
		break;


	case 'w': // move camera forward
		moveCamera = !moveCamera;

		if (!moveCamera) {
			//cameraX = 0.0;
			//cameraY += 5;
			//cameraZ = 50.0;
			cameraZ = cameraZ - 10;
			shipPosition->z = shipPosition->z - 10;
			if (cameraZ < -400) {
				cameraZ = -200;
				shipPosition->z = -200;
			}
		}

	case 's': // move camera backward
		moveCamera = !moveCamera;
		if (!moveCamera) {
			//cameraX = 0.0;
			//cameraY += 5;
			/*cameraZ = 50.0;*/
			cameraZ = cameraZ + 10;// z cannot be more than 375
			shipPosition->z = shipPosition->z + 10;
			if (cameraZ > 400) {
				cameraZ = 200;
				shipPosition->z = 200;
			}
		}


	case 'a': // move camera left
		cameraX = cameraX - 10;// z cannot be more than 375
		shipPosition->x = shipPosition->x - 10;
		moveCamera = !moveCamera;
		/*	cameraX = 0.0;
			cameraY = 0.0;
			cameraZ = 200.0;*/
		if (!moveCamera) {
			cameraX -= 50.0;
			cameraY = 0.0;
			cameraZ = 100.0;
		}





	case 'd': // move camera right
		cameraX = cameraX + 10;// z cannot be more than 375
		shipPosition->x = shipPosition->x + 10;
		moveCamera = !moveCamera;
		/*	cameraX = 0.0;
			cameraY = 0.0;
			cameraZ = 200.0;*/
		if (!moveCamera) {
			cameraX += 50.0;
			cameraY = 0.0;
			cameraZ = 100.0;
		}

	}

	glutPostRedisplay();
}




void RenderScene(void)
{



	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//LIGHTS
	//Set lighting position for the first three lights
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0); // we position the first light at -150 in the x axis with a reference to a pointer that holds these values
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);


	glPushMatrix();
		glVertex3f(lightPos0[0], lightPos0[1], lightPos0[2]);
		glColor3ub(0, 255, 0);
		glVertex3f(lightPos1[0], lightPos1[1], lightPos1[2]);
		glColor3ub(0, 0, 255);
		glVertex3f(lightPos2[0], lightPos2[1], lightPos2[2]);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glRotatef(lightChangeRotation, 0.0f, 1.0f, 0.0f);
		glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
	glPopMatrix();

	glPushMatrix();
		glRotatef(lightChangeRotation, 0.0f, 1.0f, 0.0f);
		glLightfv(GL_LIGHT4, GL_POSITION, lightPos4);
	glPopMatrix();

	glPushMatrix();
		glRotatef(lightChangeRotation, 0.0f, 1.0f, 0.0f);
		glLightfv(GL_LIGHT5, GL_POSITION, lightPos5);
		glColor3ub(0, 0, 255);
		glVertex3f(lightPos5[0], lightPos5[1], lightPos5[2]);
		glEnd();
	glPopMatrix();


	gluLookAt(cameraX, cameraY, cameraZ,//eye
		0.00, 0.00, 0.00,//centre
		0.00, 1.00, 0.00);//up 


	//PLANETS
	glPushMatrix();
	glTranslatef(0.0, 0.0, -200.0);
	glTranslatef(-50.0, 0.0, 0.0);
	glRotatef(marsRotation, 0.0f, 1.0f, 0.0f);
	drawMars(TEXTURE_MARS);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100.0, 0.0, 0.0);
	glRotatef(earthRotation, 0.0f, 1.0f, 0.0f);
	drawEarth(TEXTURE_EARTH_DAY);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 50.0, 0.0);
	glRotatef(moonRotation, 0.0f, 1.0f, 0.0f);
	drawMoon(TEXTURE_MOON);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-90.0, -50.0, 0.0);
	glRotatef(jupiterRotation, 0.0f, 1.0f, 0.0f);
	drawEarth(TEXTURE_JUPITER);
	glTranslatef(60.0, 0.00, 0.00f);
	glPopMatrix();




	//TORUS AND CRYSTALS

	//torus
	glPushMatrix();
	glColor3ub(20, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(4.0, 25.0, 25, 25);
	glPopMatrix();


	//crystal
	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.4f);
	//drawCrystal
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glScalef(15.0, 15.0, 15.0);
	glRotatef(crystalRotation, 0.0f, 5.0f, 0.0f);
	glutSolidIcosahedron();
	glDisable(GL_BLEND);
	glPopMatrix();



	glTranslated(150.0, 20.0, 0.0);
	//torus 2
	glPushMatrix();
	glColor3ub(20, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(4.0, 25.0, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.4f);
	//drawCrystal
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glScalef(15.0, 15.0, 15.0);
	glRotatef(crystalRotation, 0.0f, 5.0f, 0.0f);
	glutSolidIcosahedron();
	glDisable(GL_BLEND);
	glPopMatrix();




	glTranslated(120.0, 40.0, 0.0);
	//torus 2
	glPushMatrix();
	glColor3ub(20, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(4.0, 25.0, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.4f);
	//drawCrystal
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glScalef(15.0, 15.0, 15.0);
	glRotatef(crystalRotation, 0.0f, 5.0f, 0.0f);
	glutSolidIcosahedron();
	glDisable(GL_BLEND);
	glPopMatrix();



	//torus 3 
	glTranslated(-300.0, 40.0, 0.0);
	glPushMatrix();
	glColor3ub(20, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(4.0, 25.0, 25, 25);
	glPopMatrix();

	glPushMatrix();
	//glColor3ub(200, 200, 250);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.4f);
	//drawCrystal
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glScalef(15.0, 15.0, 15.0);
	glRotatef(crystalRotation, 0.0f, 5.0f, 0.0f);
	glutSolidIcosahedron();
	glDisable(GL_BLEND);
	glPopMatrix();


	//torus 3 
	glTranslated(-200.0, -40.0, 0.0);
	glPushMatrix();
	glColor3ub(20, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(4.0, 25.0, 25, 25);
	glPopMatrix();

	glPushMatrix();
	//glColor3ub(200, 200, 250);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.4f);
	//drawCrystal
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glScalef(15.0, 15.0, 15.0);
	glRotatef(crystalRotation, 0.0f, 5.0f, 0.0f);
	glutSolidIcosahedron();
	glDisable(GL_BLEND);
	glPopMatrix();

	//torus 3 
	glTranslated(100.0, -70.0, 0.0);
	glPushMatrix();
	glColor3ub(20, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(4.0, 25.0, 25, 25);
	glPopMatrix();

	glPushMatrix();
	//glColor3ub(200, 200, 250);
	//bind the texture 
	//glColor4f(1.0, 1.0, 1.0, partOf1 - 0.2f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.4f);
	//drawCrystal
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	glScalef(15.0, 15.0, 15.0);
	glRotatef(crystalRotation, 0.0f, 5.0f, 0.0f);
	glutSolidIcosahedron();
	glDisable(GL_BLEND);
	glPopMatrix();

	//SPACE

	glPushMatrix();



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(10.0, 0.0, 10.0, partOf1 - 0.2f);
	glTranslated(120.0, 0.0, -600.0);
	drawSpace(TEXTURE_SPACE);
	glTranslated(-50.0, 0.0, 200.0);
	glDisable(GL_BLEND);
	glRotatef(-270.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -400.0);
	drawSpace(TEXTURE_SPACE);
	glTranslatef(-150.0, -200.0, -100.0);
	glRotatef(-80.0, 1.0, 0.0, 0.0);
	drawSpace(TEXTURE_SPACE);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -400.0, -400.0);
	drawSpace(TEXTURE_SPACE);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawSpace(TEXTURE_SPACE);

glPopMatrix();
	


	if (hud) {
	glTranslated(100.0, -50.0, 0.0);
	glScaled(0.7, 0.7, 0.7);
	drawTexturedHud(TEXTURE_HUD);
	}


	///////2D OVERLAY
	glPushMatrix();
	glLoadIdentity();
	//glDisable(GL_CULL_FACE);
	//glClear(GL_DEPTH_BUFFER_BIT);
	setOrthographicProjection();
	glTranslated(0.0,0.0,0.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(20.0, 40.0);
	glVertex2f(20.0, 220.0);
	glEnd();

	//drawFrameRate();




	// score
	drawAngleWithScaledText(0.3f);
	// X coord
	drawAngleWithScaledText1(0.2f);
	// Y coord
	drawAngleWithScaledText2(0.2f);
	// Z coord
	drawAngleWithScaledText3(0.2f);







	resetPerspectiveProjection();

	glPopMatrix();
	glutSwapBuffers();
}


//// This function does any needed initialization on the rendering
//// context.
void SetupRC()
{
	//textures

	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//try these too
	   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}

	//enable textures
	glEnable(GL_TEXTURE_2D);


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside

	// Enable lighting
	glEnable(GL_LIGHTING);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, whiteLight); //ambient white light
															 //glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);


	glLightfv(GL_LIGHT0, GL_DIFFUSE, redLight);

	glEnable(GL_LIGHT0);
	// Setup and enable light 1
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, greenLight);

	glEnable(GL_LIGHT1);
	// Setup and enable light 2
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);
	glEnable(GL_LIGHT2);

	//new lights

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, redLight);
	glEnable(GL_LIGHT3);

	glLightfv(GL_LIGHT4, GL_DIFFUSE, greenLight);
	glEnable(GL_LIGHT4);

	glLightfv(GL_LIGHT5, GL_DIFFUSE, blueLight);
	glEnable(GL_LIGHT5);
	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glShadeModel(GL_SMOOTH);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//here we set all the new light to have a high specular value - so if material is set to be shiny it will appear that way
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light_specular);

	//    glCullFace(GL_FRONT_AND_BACK);

		// Enable lighting
	//	glEnable(GL_LIGHTING);
		//glEnable(GL_POINT_SMOOTH);
		// Setup and enable light 0
		//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightLessBright);
		//glLightfv(GL_LIGHT0, GL_DIFFUSE, redLight);
		//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		//glEnable(GL_LIGHT0);

		// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Black blue background
	//glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}

void TimerFunc(int value)
{
	//glutTimerFunc(25, TimerFunc, 1);
	//glutPostRedisplay();



	//glutSwapBuffers();




	//lightning change
	lightChangeRotation += 5.0;
	if (lightChangeRotation > 360.0f)
	{
		lightChangeRotation = 0.0f;
	}


	marsRotation -= 1.7f;
	if (marsRotation > 360.0f)
	{
		marsRotation = 0.0;
	}


	earthRotation += 1.0f;
	if (earthRotation > 360.0f)
	{
		earthRotation = 0.0;
	}


	moonRotation -= 1.2f;
	if (moonRotation > 360.0f)
	{
		moonRotation = 0.0;
	}

	jupiterRotation += 1.1f;
	if (jupiterRotation > 360.0f)
	{
		jupiterRotation = 0.0;
	}


	crystalRotation -= 5.0;
	if (crystalRotation > 360.0f)
	{
		crystalRotation = 0.0f;
	}
	printf("Crystal rotation is  %f \n", crystalRotation);

	torusRotation += 2.0f;
	if (torusRotation > 360.0f)
	{
		torusRotation = 0.0f;
	}

	planetRotation -= 2.0f;
	if (planetRotation > 360.0f)
	{
		planetRotation = 0.0f;
	}

	//if (!moveCameraLeft) {
	   // cameraX = cameraZ - 2;
	   // if (cameraZ > 1000.0) {
		  //  /*if (cameraY < 50.0)*/
		  //  cameraY = cameraY + 5;

	   // }
	//}

	//if (moveCamera) {
	   ///* cameraZ = cameraZ - 4;*/
	   // if (cameraZ < 600.0) {
		  //  if (cameraY < 50.0)
			 //   cameraY = cameraY + 5;

	   // }
	//}


	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);

}


void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	cout << "" << endl;
	cout << "       GAME INSTRUCTIONS :      " << endl;
	cout << "" << endl;
	cout << "1) PRESS THE ARROW KEYS, LEFT AND RIGHT, TO CHANGE ORIENTATION OF THE CAMERA " << endl;
	cout << "" << endl;
	cout << "2) PRESS THE W,A,D TO MOVE THE SPACESHIP" << endl;
	cout << "" << endl;
	cout << "-----------------------------------" << endl;
	cout << "" << endl;
	cout << "	Have fun!	" << endl;
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1280, 700);// a 16:9 ratio
	glutCreateWindow(" SPACE GAME ");

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);

	init();


	glutSpecialFunc(specialKeyInput);
	glutKeyboardFunc(ZoomInOut);
	//glutKeyboardFunc(keyInput2);

	SetupRC();
	glutMainLoop();
	return 0;
}