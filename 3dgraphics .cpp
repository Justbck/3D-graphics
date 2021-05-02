
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"

#endif

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

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char* textureFiles[TEXTURE_COUNT] = { "galaxy.tga", "hud.tga","crystal3.tga","earthday.tga","earthnight.tga","moon.tga","mars.tga","jupiterC.tga","crystal2.tga" };
const float partOf1 = 0.8f;


//for lighting if you want to experiment with these

GLfloat mKa[4] = { 0.11f,0.06f,0.11f,1.0f }; //ambient
GLfloat mKd[4] = { 0.43f,0.47f,0.54f,1.0f }; //diffuse
GLfloat mKs[4] = { 1.0f,1.0f,1.0f,1.0f }; //specular
GLfloat mKe[4] = { 0.5f,0.5f,0.0f,1.0f }; //emission

//key handling - only one shown simplly as an example
bool upKeyPressed = false;


//ROTATIONS


float earthRotation = 0.0;
float marsRotation = 0.0;
float moonRotation = 0.0;
float jupiterRotation = 0.0;
float crystalRotation = 0.0;



// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	 lightPos[] = { 100.0f, 100.0f, 50.0f, 1.0f };


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;

//end of intialisation 

//------------------------------------//




void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, iWidth, 0, iHeight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -iHeight, 0);
	glMatrixMode(GL_MODELVIEW);
}
//once you have finished your 2D you can reset your projection with this - it simply
//pops back the previous projection
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void drawTexturedQuad(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-100.0, 0.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(150.0, 100.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-150.0, 100.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}



void drawSpace(int image)
{

	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere


	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glColor4f(0., 0.6, 1.0, partOf1 + 0.2f);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-450.0, -400.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(450.0, -400.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(450.0, 400.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-450.0, 400.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


}


void drawHud(int image)
{

	//add some lighting normals for each vertex
	//draw the texture on the front
	//glEnable(GL_TEXTURE_2D);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glColor4f(1.0f, 0.7f, 1.0f, 1.0f - partOf1);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-250.0, -200.0, 100.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(250.0, -200.0, 100.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(250.0, 200.0, 100.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-250.0, 200.0, 100.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


}

void drawTorus(int image) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);

	glPushMatrix();
	//glColor3ub(255, 255, 255);
	//this specifies the shiness for the torus
	//the glMaterial functions set the reflecitivity of the materials
	//glMaterialf(GL_FRONT, GL_SHININESS, shine);
	//glRotatef(torusRotation, 0.0f, 1.0f, 0.0f);
	glutSolidTorus(10.0, 50.0, 50, 50);



	glDisable(GL_TEXTURE_2D);

}

void drawCircle(int image) {


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glColor4f(0.7f, 0.8f, 1.0f, 1.0f);
	glPushMatrix();
	//new type of sphere that can be textured
	//could also use a glutSolidSphere
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricTexture(quadric, TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 30.0f, 30, 17);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

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











void cameraUpDown()
{
	//code here

}
// Called to draw scene
void processCamera() {

	cameraUpDown();
	//need all the calls here to hand the camera (up down left right and rotate (roll))
}

void RenderScene(void)
{


	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode(GL_MODELVIEW);
	// a test point OR use the unproject code on Blackboard to locate points in the world space
		//use for locating points in your view
	   /* glEnable(GL_POINT_SMOOTH);
		 glPointSize(10.0);
		 glColor3f(1.0, 0.0, 0.0);
		 glBegin(GL_POINTS);
		 glVertex3f(0.0f, 0.0f, 100.0f);
		 glEnd();
		*/
	processCamera();
	glPushMatrix();

	glColor4f(1.0, 1.0, 1.0, partOf1 - 0.2f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//rotate the quad slightly
	glTranslatef(0.0, 0.0, -200.0);
	drawSpace(TEXTURE_SPACE);
	glTranslatef(0.0, 0.0, 200.0);
	drawHud(TEXTURE_HUD);
	glDisable(GL_BLEND);
	glPopMatrix();
	glPushMatrix();



	glTranslatef(0.0, 0.0, 200.0);
	glPushMatrix();
	//glRotatef(crystalRotation, 0.0f, 1.0f, 0.0f);
	//glScalef(10.0, 10.0, 10.0);
	//glutSolidIcosahedron();
	glPopMatrix();

	glPushMatrix();
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






	glPopMatrix();
	glutSwapBuffers();




}


// This function does any needed initialization on the rendering
// context.
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

//    glCullFace(GL_FRONT_AND_BACK);

	// Enable lighting
//	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightLessBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, redLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Black blue background
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}


void TimerFunc(int value)
{
	glutSwapBuffers();

	crystalRotation += 0.5f;
	if (crystalRotation > 360.0f)
	{
		crystalRotation = 0.0f;
	}


	marsRotation -= 0.2f;
	if (marsRotation > 360.0f)
	{
		marsRotation = 0.0;
	}


	earthRotation += 0.1f;
	if (earthRotation > 360.0f)
	{
		earthRotation = 0.0;
	}


	moonRotation -= 0.12f;
	if (moonRotation > 360.0f)
	{
		moonRotation = 0.0;
	}

	jupiterRotation += 0.1f;
	if (jupiterRotation > 360.0f)
	{
		jupiterRotation = 0.0;
	}









	glutPostRedisplay();
	glutTimerFunc(25, TimerFunc, 1);

}

void ChangeSize(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 1000
	//that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
	gluPerspective(55.0f, fAspect, 1.0, 1000.0); //may need to make larger depending on project
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	//pull the eye position back to view the scene
	gluLookAt(0.00, 0.00, 400.0,//eye
		0.00, 0.00, 0.00,//centre
		0.00, 1.00, 0.00);//up
}


void processsArrowKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		//process here
	}
	if (key == GLUT_KEY_RIGHT)
	{
		//process here

	}

	if (key == GLUT_KEY_UP)
	{
		//process here
		upKeyPressed = true;
	}

	if (key == GLUT_KEY_DOWN)
	{
		//process here
	}

}

void keyGoneUP(int key, int x, int y)
{
	//keys has been relasaed - used for resetting a boolean linked to a key being pressed - just one example here
	switch (key) {
	case GLUT_KEY_UP:
		upKeyPressed = false;
		break;
	}

}

void processKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		//set bool
		break;
	case 's':
		//set bool
		break;
	case 'a':
		//set bool
		//left
		break;
	case 'd':
		//set bool
		//right
		break;
	case ',':
		//set bool
		break;
	case '.':
		//set bool
		break;
	}
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);// a 4:3 ratio
	glutCreateWindow("Textures Tutorial");
	glutSpecialFunc(processsArrowKeys);
	glutSpecialUpFunc(keyGoneUP);
	glutKeyboardFunc(processKeys);
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);
	SetupRC();
	glutMainLoop();
	return 0;
}
