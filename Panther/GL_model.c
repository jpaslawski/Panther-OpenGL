// Gl_template.c
//Wy��czanie b��d�w przed "fopen"
#define  _CRT_SECURE_NO_WARNINGS



// �adowanie bibliotek:

#ifdef _MSC_VER                         // Check if MS Visual C compiler
#  pragma comment(lib, "opengl32.lib")  // Compiler-specific directive to avoid manually configuration
#  pragma comment(lib, "glu32.lib")     // Link libraries
#endif




// Ustalanie trybu tekstowego:
#ifdef _MSC_VER        // Check if MS Visual C compiler
#   ifndef _MBCS
#      define _MBCS    // Uses Multi-byte character set
#   endif
#   ifdef _UNICODE     // Not using Unicode character set
#      undef _UNICODE 
#   endif
#   ifdef UNICODE
#      undef UNICODE 
#   endif
#endif
#include <windows.h>            // Window defines
#include <gl\gl.h>              // OpenGL
#include <gl\glu.h>             // GLU library
#include <math.h>				// Define for sqrt
#include <stdio.h>
#include "resource.h"           // About box resource identifiers.

#define glRGB(x, y, z)	glColor3ub((GLubyte)x, (GLubyte)y, (GLubyte)z)
#define BITMAP_ID 0x4D42		// identyfikator formatu BMP
#define GL_PI 3.14

// Color Palette handle
HPALETTE hPalette = NULL;

// Application name and instance storeage
static LPCTSTR lpszAppName = "Panther";
static HINSTANCE hInstance;

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


static GLsizei lastHeight;
static GLsizei lastWidth;

// Opis tekstury
BITMAPINFOHEADER	bitmapInfoHeader;	// nag��wek obrazu
unsigned char*		bitmapData;			// dane tekstury
unsigned int		texture[2];			// obiekt tekstury

//Obr�t poszczeg�lnych cz�ci czo�gu
float obrotWieza = 0;
float ruchLufa = 0;

// Declaration for Window procedure
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam);

// Dialog procedure for about box
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam);

// Set Pixel Format function - forward declaration
void SetDCPixelFormat(HDC hDC);



// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector		
	length = (float)sqrt((vector[0] * vector[0]) +
		(vector[1] * vector[1]) +
		(vector[2] * vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}



// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat nRange = 100.0f;
	GLfloat fAspect;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	lastWidth = w;
	lastHeight = h;

	fAspect = (GLfloat)w / (GLfloat)h;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange * h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange * w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Establish perspective: 
	/*
	gluPerspective(60.0f,fAspect,1.0,400);
	*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
{
	// Light values and coordinates
	//GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	//GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	//GLfloat  specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
	//GLfloat	 lightPos[] = { 0.0f, 150.0f, 150.0f, 1.0f };
	//GLfloat  specref[] =  { 1.0f, 1.0f, 1.0f, 1.0f };


	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	//glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	// Enable lighting
	//glEnable(GL_LIGHTING);

	// Setup and enable light 0
	//glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	//glEnable(GL_LIGHT0);

	// Enable color tracking
	//glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	//glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
	//glMateriali(GL_FRONT,GL_SHININESS,128);


	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Black brush
	glColor3f(0.0, 0.0, 0.0);
}

void skrzynka(void)
{
	glColor3d(0.8, 0.7, 0.3);


	glEnable(GL_TEXTURE_2D); // W��cz teksturowanie

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3d(0, 0, 1);
	glTexCoord2d(1.0, 1.0); glVertex3d(25, 25, 25);
	glTexCoord2d(0.0, 1.0); glVertex3d(-25, 25, 25);
	glTexCoord2d(0.0, 0.0); glVertex3d(-25, -25, 25);
	glTexCoord2d(1.0, 0.0); glVertex3d(25, -25, 25);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glNormal3d(1, 0, 0);
	glTexCoord2d(1.0, 1.0); glVertex3d(25, 25, 25);
	glTexCoord2d(0.0, 1.0); glVertex3d(25, -25, 25);
	glTexCoord2d(0.0, 0.0); glVertex3d(25, -25, -25);
	glTexCoord2d(1.0, 0.0); glVertex3d(25, 25, -25);
	glEnd();

	glDisable(GL_TEXTURE_2D); // Wy��cz teksturowanie



	glBegin(GL_QUADS);
	glNormal3d(0, 0, -1);
	glVertex3d(25, 25, -25);
	glVertex3d(25, -25, -25);
	glVertex3d(-25, -25, -25);
	glVertex3d(-25, 25, -25);

	glNormal3d(-1, 0, 0);
	glVertex3d(-25, 25, -25);
	glVertex3d(-25, -25, -25);
	glVertex3d(-25, -25, 25);
	glVertex3d(-25, 25, 25);

	glNormal3d(0, 1, 0);
	glVertex3d(25, 25, 25);
	glVertex3d(25, 25, -25);
	glVertex3d(-25, 25, -25);
	glVertex3d(-25, 25, 25);

	glNormal3d(0, -1, 0);
	glVertex3d(25, -25, 25);
	glVertex3d(-25, -25, 25);
	glVertex3d(-25, -25, -25);
	glVertex3d(25, -25, -25);
	glEnd();
}


// LoadBitmapFile
// opis: �aduje map� bitow� z pliku i zwraca jej adres.
//       Wype�nia struktur� nag��wka.
//	 Nie obs�uguje map 8-bitowych.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// wska�nik pozycji pliku
	BITMAPFILEHEADER	bitmapFileHeader;		// nag��wek pliku
	unsigned char		*bitmapImage;			// dane obrazu
	int					imageIdx = 0;		// licznik pikseli
	unsigned char		tempRGB;				// zmienna zamiany sk�adowych

	// otwiera plik w trybie "read binary"
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// wczytuje nag��wek pliku
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// sprawdza, czy jest to plik formatu BMP
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// wczytuje nag��wek obrazu
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// ustawia wska�nik pozycji pliku na pocz�tku danych obrazu
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// przydziela pami�� buforowi obrazu
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// sprawdza, czy uda�o si� przydzieli� pami��
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// wczytuje dane obrazu
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// sprawdza, czy dane zosta�y wczytane
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// zamienia miejscami sk�adowe R i B 
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// zamyka plik i zwraca wska�nik bufora zawieraj�cego wczytany obraz
	fclose(filePtr);
	return bitmapImage;
}

void sruba(float x, float y, float z, float r, float h, double color)
{
	double x1, z1, alpha, PI = 3.14;
	if (y < 0) h = -h;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(color, color, color);
		glVertex3d(x, y + h, z);
		for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
		{
			x1 = r * sin(alpha) + x;
			z1 = r * cos(alpha) + z;
			glVertex3d(x1, y + h, z1);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(color - 0.1, color - 0.1, color - 0.1);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x1 = r * sin(alpha) + x;
			z1 = r * cos(alpha) + z;
			glVertex3d(x1, y, z1);
			glVertex3d(x1, y + h, z1);
		}
		glEnd();
	}
}

void wieza(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glColor3d(0.2, 0.2, 0.2);
		glBegin(GL_QUADS);
		glVertex3d(75, -56, 6);
		glVertex3d(55, -44, 44);
		glVertex3d(55, 44, 44);
		glVertex3d(75, 56, 6);

		glColor3d(0.5, 0.5, 0.5);
		glVertex3d(55, 44, 44);
		glVertex3d(75, 56, 6);
		glVertex3d(-34, 56, 6);
		glVertex3d(-28, 44, 44);

		glVertex3d(-34, 56, 6);
		glVertex3d(-28, 44, 44);
		glVertex3d(-28, -44, 44);
		glVertex3d(-34, -56, 6);

		glVertex3d(-28, -44, 44);
		glVertex3d(-34, -56, 6);
		glVertex3d(75, -56, 6);
		glVertex3d(55, -44, 44);

		glColor3d(0.2, 0.2, 0.2);
		glVertex3d(75, -56, 6);
		glVertex3d(75, 56, 6);
		glVertex3d(-34, 56, 6);
		glVertex3d(-34, -56, 6);

		glVertex3d(55, -44, 44);
		glVertex3d(55, 44, 44);
		glVertex3d(-28, 44, 44);
		glVertex3d(-28, -44, 44);
		glEnd();


	}
}

void wlaz(void)
{
	//warstwa ni�sza
	double x, y, alpha, PI = 3.14;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_QUAD_STRIP);
		glColor3d(0.2, 0.1, 0);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = 18.75 * sin(alpha) + 31.25;
			y = 18.75 * cos(alpha) + 15.5;
			glVertex3d(x, y, 44);
			glVertex3d(x, y, 52);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0, 0);
		glVertex3d(31.25, 15.5, 52);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = 18.75 * sin(alpha) + 31.25;
			y = 18.75 * cos(alpha) + 15.5;
			glVertex3d(x, y, 52);
		}
		glEnd();

		//warstwa wy�sza
		glBegin(GL_QUAD_STRIP);
		glColor3d(0.2, 0.1, 0);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x = 17.25 * sin(alpha) + 31.25;
			y = 17.25 * cos(alpha) + 15.5;
			glVertex3d(x, y, 44);
			glVertex3d(x, y, 57.5);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0, 0);
		glVertex3d(31.25, 15.5, 52);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			x = 17.25 * sin(alpha) + 31.25;
			y = 17.25 * cos(alpha) + 15.5;
			glVertex3d(x, y, 57.5);
		}
		glEnd();
	}
}

void jarzmo(void)
{
	double x, z, alpha, PI = 3.14;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0.1, 0.1);
		glVertex3d(-31, -44, 27.5);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			z = 16.5 * cos(alpha) + 27.5;
			x = 16.5 * sin(alpha) - 31;
			glVertex3d(x, -44, z);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.4, 0.4, 0.4);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			z = 16.5 * sin(alpha) + 27.5;
			x = 16.5 * cos(alpha) - 31;
			glVertex3d(x, -44, z);
			glVertex3d(x, 44, z);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0.1, 0.1);
		glVertex3d(-31, 44, 27.5);
		for (alpha = 0; alpha >= -2 * PI; alpha -= PI / 8.0)
		{
			z = 16.5 * sin(alpha) + 27.5;
			x = 16.5 * cos(alpha) - 31;
			glVertex3d(x, 44, z);
		}
		glEnd();
	}
}

void lufa(void)
{
	double x, y, z, alpha, PI = 3.14;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0.1, 0.1);
		glVertex3d(0, -43, 0);
		for (alpha = 0 * PI / 8.0; alpha >= -7 * PI / 8.0; alpha -= PI / 8.0)
		{
			z = 22.5 * cos(alpha);
			x = 22.5 * sin(alpha);
			glVertex3d(x, -43, z);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.4, 0.4, 0.4);
		for (alpha = 4 * PI / 8.0; alpha <= 11 * PI / 8.; alpha += PI / 8.0)
		{
			z = 22.5 * sin(alpha);
			x = 22.5 * cos(alpha);
			glVertex3d(x, -43, z);
			glVertex3d(x, 43, z);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0.1, 0.1);
		glVertex3d(0, 43, 0);
		for (alpha = 11 * PI / 8.0; alpha >= 3 * PI / 8.0; alpha -= PI / 8.0)
		{
			z = 22.5 * sin(alpha);
			x = 22.5 * cos(alpha);
			glVertex3d(x, 43, z);
		}
		glEnd();
		glBegin(GL_QUAD_STRIP);
		glColor3d(0.1, 0.1, 0.1);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			y = 4.5 * sin(alpha);
			z = 4.5 * cos(alpha);
			glVertex3d(0, y, z);
			glVertex3d(-160, y, z);
		}
		glEnd();
		//zako�czenie
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.7, 0.7, 0.7);
		glVertex3d(-157, 0, 0);
		for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
		{
			z = 5 * sin(alpha);
			y = 5 * cos(alpha);
			glVertex3d(-157, y, z);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.7, 0.7, 0.7);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			z = 7 * sin(alpha);
			y = 7 * cos(alpha);
			glVertex3d(-110, 0, 0);
			glVertex3d(-158, y, z);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.3, 0.3, 0.3);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			z = 7 * sin(alpha);
			y = 7 * cos(alpha);
			glVertex3d(-158, y, z);
			glVertex3d(-159, y, z);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.3, 0.3, 0.3);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			z = 5.5 * sin(alpha);
			y = 5.5 * cos(alpha);
			glVertex3d(-159, y, z);
			glVertex3d(-166, y, z);
		}
		glEnd();
	}
}

void kadlub(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_QUADS);
		glColor3d(0.3, 0.6, 0.3);
		//g�ra 
		glVertex3d(154, -60.5, 6);
		glVertex3d(-95.5, -60.5, 6);
		glVertex3d(-95.5, 60.5, 6);
		glVertex3d(154, 60.5, 6);
		//d� - podwozie g�rne
		glVertex3d(139, -76.5, -16.5);
		glVertex3d(-126, -76.5, -16.5);
		glVertex3d(-126, 76.5, -16.5);
		glVertex3d(139, 76.5, -16.5);
		//d� - podwozie dolne
		glColor3d(0.1, 0.1, 0.1);
		glVertex3d(-126, -44, -57.5);
		glVertex3d(112, -44, -57.5);
		glVertex3d(112, 44, -57.5);
		glVertex3d(-126, 44, -57.5);
		//boki
		glColor3d(0.1, 0.2, 0.1);
		glVertex3d(139, -76.5, -16.5);
		glVertex3d(154, -60.5, 6);
		glVertex3d(154, 60.5, 6);
		glVertex3d(139, 76.5, -16.5);

		glVertex3d(154, 60.5, 6);
		glVertex3d(139, 76.5, -16.5);
		glVertex3d(-126, 76.5, -16.5);
		glVertex3d(-95.5, 60.5, 6);

		glVertex3d(154, -60.5, 6);
		glVertex3d(139, -76.5, -16.5);
		glVertex3d(-126, -76.5, -16.5);
		glVertex3d(-95.5, -60.5, 6);

		glVertex3d(-126, 76.5, -16.5);
		glVertex3d(-95.5, 60.5, 6);
		glVertex3d(-95.5, -60.5, 6);
		glVertex3d(-126, -76.5, -16.5);
		//ty�
		glColor3d(0.6, 0.1, 0);
		glVertex3d(139, -44, -16.5);
		glVertex3d(112, -44, -57.5);
		glVertex3d(112, 44, -57.5);
		glVertex3d(139, 44, -16.5);
		//prz�d
		glVertex3d(-126, -44, -16.5);
		glVertex3d(-154, -44, -34);
		glVertex3d(-154, 44, -34);
		glVertex3d(-126, 44, -16.5);
		//prz�d - dolna p�yta
		glVertex3d(-126, -44, -57.5);
		glVertex3d(-154, -44, -34);
		glVertex3d(-154, 44, -34);
		glVertex3d(-126, 44, -57.5);
		glEnd();

		glColor3d(0.2, 0.4, 0.2);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(139, -44, -16.5);
		glVertex3d(112, -44, -57.5);
		glVertex3d(-126, -44, -16.5);
		glVertex3d(-154, -44, -34);
		glVertex3d(-126, -44, -57.5);
		glVertex3d(112, -44, -57.5);
		glEnd();

		glColor3d(0.2, 0.4, 0.2);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3d(139, 44, -16.5);
		glVertex3d(112, 44, -57.5);
		glVertex3d(-126, 44, -16.5);
		glVertex3d(-154, 44, -34);
		glVertex3d(-126, 44, -57.5);
		glVertex3d(112, 44, -57.5);
		glEnd();
	}
}

void blachaBoczna(float y)
{
	int a = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_QUAD_STRIP);
		for (float x1 = -126.0; x1 <= 141.0; x1 += 12.0)
		{
			if (a % 2 == 0) { glColor3d(0.3, 0.5, 0); }
			else { glColor3d(0.25, 0.1, 0); }
			glVertex3f(x1, y, -36.5);
			glVertex3f(x1, y, -16.5);
			a++;
		}
		glEnd();
	}
}

void kolo(double x, double y, double z)
{
	double x1, x2, x3, z1, z2, z3, alpha, PI = 3.14, h = 5;
	if (y < 0) h = -5;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0.1, 0.1, 0.1);
		glVertex3d(x, y, z);
		for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
		{
			x1 = 21 * sin(alpha) + x;
			z1 = 21 * cos(alpha) + z;
			glVertex3d(x1, y, z1);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.3, 0.3, 0.3);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			x1 = 21 * sin(alpha) + x;
			z1 = 21 * cos(alpha) + z;
			glVertex3d(x1, y - h, z1);
			glVertex3d(x1, y, z1);
		}
		glEnd();

		sruba(x, y, z, 8, 1, 0.6);
		for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
		{
			//sruby zewnetrzne 
			x2 = 19 * sin(alpha) + x;
			z2 = 19 * cos(alpha) + z;
			sruba(x2, y, z2, 1, 1, 0.7);
			
			//sruby wewnetrzne
			x3 = 6 * sin(alpha) + x;
			z3 = 6 * cos(alpha) + z;
			sruba(x3, y, z3, 1, 2, 0.2);
		}
	}
}

void gasienicaGorna(float y)
{
	float z = -16.5,  color = 0.1;
	int a = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_QUAD_STRIP);
		for (float x1 = -130.0; x1 <= 110.0; x1 += 5.0)
		{
			if (a % 2 == 0) { color = 0.2; }
			else { color = 0.7; }
			glColor3d(color, color, color);
			glVertex3f(x1, y, z);
			glVertex3f(x1, y + 28, z);

			if (x1 < -121.0)
			{
				z -= 1.5;
			}
			else if (x1 > 101.0)
			{
				z += 1.5;
			}
			else if(x1 > -121.0 && x1 < -81.0)
			{
				z -= 0.8;
			}
			else if (x1 > -81.0 && x1 < -41.0)
			{
				z -= 0.4;
			}
			else if (x1 > 21.0 && x1 < 61.0)
			{
				z += 0.4;
			}
			else if (x1 > 61.0 && x1 < 101.0)
			{
				z += 0.9;
			}
			a++;
		}
		glEnd();
	}
}

void gasienicaDolna(float y)
{
	float x, z = -56.5, color = 0.1;
	int a = 0;
	double alpha, PI = 3.14;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_QUAD_STRIP);
		for (float x1 = -145.0; x1 <= 128.0; x1 += 5.0)
		{
			if (a % 2 == 0) { color = 0.2; }
			else { color = 0.7; }
			glColor3d(color, color, color);
			glVertex3f(x1, y, z);
			glVertex3f(x1, y + 28, z);

			if (x1 <= -130.0)
			{
				z -= 2.5;
			}
			else if (x1 >= 110.0)
			{
				z += 3;
			}
			else if (x1 > -130.0 && x1 < -105.0)
			{
				z -= 1.5;
			}
			else if (x1 > 85.0 && x1 < 110.0)
			{
				z += 1.7;
			}
			a++;
		}
		glEnd();

		//przednie zaokr�glenie
		glBegin(GL_QUAD_STRIP);
		for (alpha = -PI + PI / 14.0; alpha <= PI / 7.0; alpha += PI / 14.0)
		{
			if (a % 2 == 0) { color = 0.2; }
			else { color = 0.7; }
			glColor3d(color, color, color);
			x = 21.5 * sin(alpha) - 137;
			z = 21.5 * cos(alpha) - 36.5;
			glVertex3d(x, y, z);
			glVertex3d(x, y + 28, z);
			a++;
		}
		glEnd();

		//tylne zaokr�glenie
		glBegin(GL_QUAD_STRIP);
		for (alpha = - PI / 14.0; alpha <= PI - PI / 14.0; alpha += PI / 14.0)
		{
			if (a % 2 == 0) { color = 0.2; }
			else { color = 0.7; }
			glColor3d(color, color, color);
			x = 21.5 * sin(alpha) + 114;
			z = 21.5 * cos(alpha) - 38.5;
			glVertex3d(x, y, z);
			glVertex3d(x, y + 28, z);
			a++;
		}
		glEnd();
	}
}

void ukladGasienicowy(void)
{
	kolo(-137, 74.5, -36.5);
	kolo(-101.5, 69.5, -50.5);
	kolo(-74, 74.5, -50.5);
	kolo(-48, 69.5, -50.5);
	kolo(-21.5, 74.5, -50.5);
	kolo(6, 69.5, -50.5);
	kolo(34, 74.5, -50.5);
	kolo(60, 69.5, -50.5);
	kolo(86, 74.5, -50.5);
	kolo(114, 69.5, -38.5);
	
	gasienicaGorna(-70);
	gasienicaDolna(-70);
	gasienicaGorna(42);
	gasienicaDolna(42);

	kolo(-137, -74.5, -36.5);
	kolo(-101.5, -69.5, -50.5);
	kolo(-74, -74.5, -50.5);
	kolo(-48, -69.5, -50.5);
	kolo(-21.5, -74.5, -50.5);
	kolo(6, -69.5, -50.5);
	kolo(34, -74.5, -50.5);
	kolo(60, -69.5, -50.5);
	kolo(86, -74.5, -50.5);
	kolo(114, -69.5, -38.5);
}

void wydech(float r1, float r2, float h1, float h2)
{
	double x1, y1, alpha, PI = 3.14;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0, 0, 0);
		glVertex3d(0, 0, 0);
		for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
		{
			x1 = r1 * sin(alpha);
			y1 = r1 * cos(alpha);
			glVertex3d(x1, y1, 0);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.2, 0.1, 0);
		for (alpha = 0.0; alpha <= PI; alpha += PI / 8.0)
		{
			x1 = r1 * sin(alpha);
			y1 = r1 * cos(alpha);
			glVertex3d(x1, y1, 0);
			glVertex3d(x1, y1, h1);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0, 0, 0);
		glVertex3d(4, 0, 0);
		for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
		{
			x1 = r2 * sin(alpha) + 4;
			y1 = r2 * cos(alpha);
			glVertex3d(x1, y1, 0);
		}
		glEnd();

		glBegin(GL_QUAD_STRIP);
		glColor3d(0.2, 0.1, 0);
		for (alpha = 0.0; alpha <= 2*PI; alpha += PI / 8.0)
		{
			x1 = r2 * sin(alpha) + 4;
			y1 = r2 * cos(alpha);
			glVertex3d(x1, y1, 0);
			glVertex3d(x1, y1, h2);
		}
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		glColor3d(0, 0, 0);
		glVertex3d(4, 0, h2);
		for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
		{
			x1 = r2 * sin(alpha) + 4;
			y1 = r2 * cos(alpha);
			glVertex3d(x1, y1, h2);
		}
		glEnd();
	}
}


void stozek(double r, double h)
{
	double x, y, alpha, PI = 3.14;
	glBegin(GL_TRIANGLE_FAN);
	glColor3d(0, 0, 0);
	glVertex3d(0, 0, 0);
	for (alpha = 0; alpha <= 8 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		y = r * cos(alpha);
		glVertex3d(x, y, 0);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glColor3d(0.2, 0.1, 0);
	for (alpha = 0.0; alpha <= 2 * PI; alpha += PI / 8.0)
	{
		x = r * sin(alpha);
		y = r * cos(alpha);
		glVertex3d(x, y, 0);
		glVertex3d(0, 0, h);
	}
	glEnd();

}

// Called to draw scene
void RenderScene(void)
{
	//float normal[3];	// Storeage for calculated surface normal

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 0.0f, 1.0f);
	/////////////////////////////////////////////////////////////////
	// MIEJSCE NA KOD OPENGL DO TWORZENIA WLASNYCH SCEN:		   //
	/////////////////////////////////////////////////////////////////

	//Spos�b na odr�nienie "przedniej" i "tylniej" �ciany wielok�ta:
	glPolygonMode(GL_BACK, GL_LINE);
	glScalef(0.5, 0.5, 0.5);
	glPushMatrix();
	glRotatef(obrotWieza, 0, 0, 1);
	wieza();
	wlaz();
	jarzmo();
		glPushMatrix();
		glTranslatef(-31, 0, 27.5);
		glRotatef(ruchLufa, 0, 1, 0);
		lufa();
		glPopMatrix();
	glPopMatrix();
	kadlub();
	blachaBoczna(-76.5);
	blachaBoczna(76.5);
	ukladGasienicowy();
		glPushMatrix();
		glTranslatef(130, -18, -28);
		glRotatef( 34, 0, 1, 0);
		wydech(8, 3, 40, 50);
		glTranslatef(0, 36, 0);
		wydech(8, 3, 40, 50);
		glPopMatrix();
	glPopMatrix();
	//Uzyskanie siatki:
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	//Wyrysowanie prostokata:
	//glRectd(-10.0,-10.0,20.0,20.0);

	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	glMatrixMode(GL_MODELVIEW);
	// Flush drawing commands
	glFlush();
}


// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
		1,                                                              // Version of this structure    
		PFD_DRAW_TO_WINDOW |                    // Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |					// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,                       // Double buffered
		PFD_TYPE_RGBA,                          // RGBA Color mode
		24,                                     // Want 24bit color 
		0,0,0,0,0,0,                            // Not used to select mode
		0,0,                                    // Not used to select mode
		0,0,0,0,0,                              // Not used to select mode
		32,                                     // Size of depth buffer
		0,                                      // Not used to select mode
		0,                                      // Not used to select mode
		PFD_MAIN_PLANE,                         // Draw in main plane
		0,                                      // Not used to select mode
		0,0,0 };                                // Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}



// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
{
	HPALETTE hRetPal = NULL;	// Handle to palette to be created
	PIXELFORMATDESCRIPTOR pfd;	// Pixel Format Descriptor
	LOGPALETTE *pPal;			// Pointer to memory for logical palette
	int nPixelFormat;			// Pixel format index
	int nColors;				// Number of entries in palette
	int i;						// Counting variable
	BYTE RedRange, GreenRange, BlueRange;
	// Range for each color entry (7,7,and 3)


// Get the pixel format index and retrieve the pixel format description
	nPixelFormat = GetPixelFormat(hDC);
	DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	// Does this pixel format require a palette?  If not, do not create a
	// palette and just return NULL
	if (!(pfd.dwFlags & PFD_NEED_PALETTE))
		return NULL;

	// Number of entries in palette.  8 bits yeilds 256 entries
	nColors = 1 << pfd.cColorBits;

	// Allocate space for a logical palette structure plus all the palette entries
	pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));

	// Fill in palette header 
	pPal->palVersion = 0x300;		// Windows 3.0
	pPal->palNumEntries = nColors; // table size

	// Build mask of all 1's.  This creates a number represented by having
	// the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
	// pfd.cBlueBits.  
	RedRange = (1 << pfd.cRedBits) - 1;
	GreenRange = (1 << pfd.cGreenBits) - 1;
	BlueRange = (1 << pfd.cBlueBits) - 1;

	// Loop through all the palette entries
	for (i = 0; i < nColors; i++)
	{
		// Fill in the 8-bit equivalents for each component
		pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
		pPal->palPalEntry[i].peRed = (unsigned char)(
			(double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);

		pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
		pPal->palPalEntry[i].peGreen = (unsigned char)(
			(double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

		pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
		pPal->palPalEntry[i].peBlue = (unsigned char)(
			(double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

		pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
	}


	// Create the palette
	hRetPal = CreatePalette(pPal);

	// Go ahead and select and realize the palette for this device context
	SelectPalette(hDC, hRetPal, FALSE);
	RealizePalette(hDC);

	// Free the memory used for the logical palette structure
	free(pPal);

	// Return the handle to the new palette
	return hRetPal;
}


// Entry point of all Windows programs
int APIENTRY WinMain(HINSTANCE       hInst,
	HINSTANCE       hPrevInstance,
	LPSTR           lpCmdLine,
	int                     nCmdShow)
{
	MSG                     msg;            // Windows message structure
	WNDCLASS        wc;                     // Windows class structure
	HWND            hWnd;           // Storeage for window handle

	hInstance = hInst;

	// Register Window style
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// No need for background brush for OpenGL window
	wc.hbrBackground = NULL;

	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = lpszAppName;

	// Register the window class
	if (RegisterClass(&wc) == 0)
		return FALSE;


	// Create the main application window
	hWnd = CreateWindow(
		lpszAppName,
		lpszAppName,

		// OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,

		// Window position and size
		50, 50,
		400, 400,
		NULL,
		NULL,
		hInstance,
		NULL);

	// If window was not created, quit
	if (hWnd == NULL)
		return FALSE;


	// Display the window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	// Process application messages until the application closes
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}




// Window procedure, handles all messages for this program
LRESULT CALLBACK WndProc(HWND    hWnd,
	UINT    message,
	WPARAM  wParam,
	LPARAM  lParam)
{
	static HGLRC hRC;               // Permenant Rendering context
	static HDC hDC;                 // Private GDI Device context

	switch (message)
	{
		// Window creation, setup for OpenGL
	case WM_CREATE:
		// Store the device context
		hDC = GetDC(hWnd);

		// Select the pixel format
		SetDCPixelFormat(hDC);

		// Create palette if needed
		hPalette = GetOpenGLPalette(hDC);

		// Create the rendering context and make it current
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		SetupRC();
		glGenTextures(2, &texture[0]);                  // tworzy obiekt tekstury			

		// �aduje pierwszy obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\checker.bmp", &bitmapInfoHeader);

		glBindTexture(GL_TEXTURE_2D, texture[0]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// �aduje drugi obraz tekstury:
		bitmapData = LoadBitmapFile("Bitmapy\\crate.bmp", &bitmapInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[1]);       // aktywuje obiekt tekstury

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		// tworzy obraz tekstury
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
			bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

		if (bitmapData)
			free(bitmapData);

		// ustalenie sposobu mieszania tekstury z t�em
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		break;

		// Window is being destroyed, cleanup
	case WM_DESTROY:
		// Deselect the current rendering context and delete it
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);

		// Delete the palette if it was created
		if (hPalette != NULL)
			DeleteObject(hPalette);

		// Tell the application to terminate after the window
		// is gone.
		PostQuitMessage(0);
		break;

		// Window is resized.
	case WM_SIZE:
		// Call our function which modifies the clipping
		// volume and viewport
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;


		// The painting function.  This message sent by Windows 
		// whenever the screen needs updating.
	case WM_PAINT:
	{
		// Call OpenGL drawing code
		RenderScene();

		SwapBuffers(hDC);

		// Validate the newly painted client area
		ValidateRect(hWnd, NULL);
	}
	break;

	// Windows is telling the application that it may modify
	// the system palette.  This message in essance asks the 
	// application for a new palette.
	case WM_QUERYNEWPALETTE:
		// If the palette was created.
		if (hPalette)
		{
			int nRet;

			// Selects the palette into the current device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries from the currently selected palette to
			// the system palette.  The return value is the number 
			// of palette entries modified.
			nRet = RealizePalette(hDC);

			// Repaint, forces remap of palette in current window
			InvalidateRect(hWnd, NULL, FALSE);

			return nRet;
		}
		break;


		// This window may set the palette, even though it is not the 
		// currently active window.
	case WM_PALETTECHANGED:
		// Don't do anything if the palette does not exist, or if
		// this is the window that changed the palette.
		if ((hPalette != NULL) && ((HWND)wParam != hWnd))
		{
			// Select the palette into the device context
			SelectPalette(hDC, hPalette, FALSE);

			// Map entries to system palette
			RealizePalette(hDC);

			// Remap the current colors to the newly realized palette
			UpdateColors(hDC);
			return 0;
		}
		break;

		// Key press, check for arrow keys to do cube rotation.
	case WM_KEYDOWN:
	{
		if (wParam == VK_UP)
			xRot -= 3.0f;

		if (wParam == VK_DOWN)
			xRot += 3.0f;

		if (wParam == VK_LEFT)
			yRot -= 3.0f;

		if (wParam == VK_RIGHT)
			yRot += 3.0f;

		if (wParam == VK_F1)
		{
			if (ruchLufa < -6.0f && obrotWieza > 150.0f && obrotWieza < 210.0f)
			{
				ruchLufa = -6.0f;
			}
			else if(ruchLufa < -6.0f && obrotWieza < -150.0f && obrotWieza > -210.0f)
			{
				ruchLufa = -6.0f;
			}
			else
				obrotWieza += 3.0f;
		}

		if (wParam == VK_F2)
		{
			if (ruchLufa < -6.0f && obrotWieza < -150.0f && obrotWieza > -210.0f)
			{
				ruchLufa = -6.0f;
			}
			else if (ruchLufa < -6.0f && obrotWieza > 150.0f && obrotWieza < 210.0f)
			{
				ruchLufa = -6.0f;
			}
			else
				obrotWieza -= 3.0f;
		}

		if (wParam == VK_F3 && ruchLufa <= 12.0f)
			ruchLufa += 1.0f;
	
		if (wParam == VK_F4 && ruchLufa >= -8.0f)
		{
			if (ruchLufa <= -6.0f && obrotWieza < -150.0f && obrotWieza > -210.0f)
			{
				ruchLufa = -6.0f;
			}
			else if (ruchLufa <= -6.0f && obrotWieza > 150.0f && obrotWieza < 210.0f)
			{
				ruchLufa = -6.0f;
			}
			else
				ruchLufa -= 1.0f;
		}
		
		xRot = (const int)xRot % 360;
		yRot = (const int)yRot % 360;
		obrotWieza = (const int)obrotWieza % 360;

		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	// A menu command
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// Exit the program
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;

			// Display the about box
		case ID_HELP_ABOUT:
			DialogBox(hInstance,
				MAKEINTRESOURCE(IDD_DIALOG_ABOUT),
				hWnd,
				AboutDlgProc);
			break;
		}
	}
	break;


	default:   // Passes it on if unproccessed
		return (DefWindowProc(hWnd, message, wParam, lParam));

	}

	return (0L);
}




// Dialog procedure.
BOOL APIENTRY AboutDlgProc(HWND hDlg, UINT message, UINT wParam, LONG lParam)
{

	switch (message)
	{
		// Initialize the dialog box
	case WM_INITDIALOG:
	{
		int i;
		GLenum glError;

		// glGetString demo
		SetDlgItemText(hDlg, IDC_OPENGL_VENDOR, glGetString(GL_VENDOR));
		SetDlgItemText(hDlg, IDC_OPENGL_RENDERER, glGetString(GL_RENDERER));
		SetDlgItemText(hDlg, IDC_OPENGL_VERSION, glGetString(GL_VERSION));
		SetDlgItemText(hDlg, IDC_OPENGL_EXTENSIONS, glGetString(GL_EXTENSIONS));

		// gluGetString demo
		SetDlgItemText(hDlg, IDC_GLU_VERSION, gluGetString(GLU_VERSION));
		SetDlgItemText(hDlg, IDC_GLU_EXTENSIONS, gluGetString(GLU_EXTENSIONS));


		// Display any recent error messages
		i = 0;
		do {
			glError = glGetError();
			SetDlgItemText(hDlg, IDC_ERROR1 + i, gluErrorString(glError));
			i++;
		} while (i < 6 && glError != GL_NO_ERROR);


		return (TRUE);
	}
	break;

	// Process command messages
	case WM_COMMAND:
	{
		// Validate and Make the changes
		if (LOWORD(wParam) == IDOK)
			EndDialog(hDlg, TRUE);
	}
	break;

	// Closed from sysbox
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		break;
	}

	return FALSE;
}
