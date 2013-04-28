/********************************************************
 RANDOM Graph 

 Purpose: To generate a random graph.  It was designed to
          to simulate a communication network that can
		  be built for wireless nodes in an ad-hoc 
		  environment, hence, the use of variable names 
		  such as trans_radius.  The graph that's created is 
		  a bit crude, but it is intended only for 
		  entertainment purposes, not real research.
		  A line is drawn between two dots, if they're 
		  within the specified transmission radius.
 Successfully compiled with MSVC 6.0
*********************************************************/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <GL/glut.h>  //To compile this program, you'll need glut installed

#include "myroutines.h"

using namespace std;

/**********************************
	Global Variables
**********************************/
int height,  //The window's dimensions
	width,
	trans_radius=60;

const int MAX_ROWS = 600, 
		  MAX_COLS = 400,
		  MAX_NODES = 700; //The maximum number of allowable nodes

int myworld[MAX_ROWS][MAX_COLS];  //Stores the location of nodes


//Make sure that we're within the bounds of the array
bool valid(int i, int j)
{
	return(i >= 0 && i < MAX_ROWS && j >= 0 && j < MAX_COLS);
}


void drawDot(int x, int y) {
  glBegin(GL_POINTS);
  glColor3d (0.0,0.0,0.0);
  glVertex2i(x,y); 
  glEnd();
}

// Draw one line
void drawLine(float x1, float y1, float x2, float y2) {
  //glEnable(GL_LINE_SMOOTH);  //antialiasing
  glLineWidth(0.2);
  glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
  glEnd();
}

//////////////////////////////////////////////////////////
// This function calculates the length of the hypothenuse
// of a right triangle, using the following formula:
//
//            dist=sqrt(n^2+m^2)
//
//////////////////////////////////////////////////////////
double calcDist(int n, int m){

	return sqrt(n*n + m*m);
}

//-------------------------------------------------
// Input: x,y coordinates of a node
// Output:  None
// Purpose: Checks the surrounding cells of a given
//          node and draws a line if it's within
//          the transmission radius.
//-------------------------------------------------
void testDist(int i, int j){

	int count = 1;
	double num;

	/*  Commented out b/c the other
	    conditions may be faster if the trans_radius < MAX_COLS
	for (int a=0; a<MAX_ROWS; a++){
		for (int b=0; b<MAX_COLS; b++){
	*/
	for (int a=i-trans_radius; a < i+trans_radius; a++){
		for (int b=j-trans_radius; b < j+trans_radius; b++){		

			if ( valid(a, b) && (myworld[a][b]==1)){	
				num = calcDist(i-a,j-b);  /*calculate the distance between these 2 pts*/

				if (num <= trans_radius)
					drawLine(i,j,a,b);													
			}
		}		  
	}

			
}

void drawWorld(){

	int dx=1, dy=1;

	for (int i=0; i<MAX_ROWS; i++){
		for (int j=0; j < MAX_COLS; j++){

			if (myworld[i][j]==1) {
			
				drawDot(i,j);
				
				testDist(i,j);  /* draw a line between two points if they're 
								   within the transmission distance
				                  */  
			}
		}
	}

}

/**********************************

Initialize the graph nodes placement randomly
It's important to play around with
the increments for ints i and j, along with
adjusting the interval for randomizer for
optimal distribution

**********************************/
void initWorld(){

int randomizer; 
int nodeCount=0;

    srand(time(0)); //set the seed to the current time

          
	for (int i=10; i<MAX_ROWS && nodeCount<MAX_NODES; i+=5){

		for (int j=10; j<MAX_COLS && nodeCount<MAX_NODES; j+=5){
		
   		   randomizer=boundedRand(0, 100); /* choice of the interval will
										   help determine the density of the points*/
   		   
		   if (randomizer > 3){  //percentage% of space occupation

				myworld[i][height-j] = 0;
		   }
		   else{
				myworld[i][height-j] = 1;
				nodeCount++;
				//cout << i << "," << j << endl;

		   }
		}
		
	}


}

//*************************************************************
// Keyboard callback function
void myKeyboard(unsigned char key, int x, int y) {

  switch(key) {
  case 'c':
    //Clear the screen if the user presses c
	cout << "Cleared the screen" << endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
    
    break;
    
  case 'q':
	 exit(0);

  case 'r':	   

	cout << "Redrawing with a new seed..." << endl;
	initWorld();  // reinitialize the screen with new nodes
	drawWorld();
	glFlush();  //Important: always flush the queue after drawing
	
    break;

  case 'd':	  

	cout << "Drawing..." << endl;
	drawWorld();  // draw the contents!!! 
	glFlush();    // flush the queue after we've drawn everything
	
    break;

  default:
    cout << key << " is an invalid choice. Try again. " << endl;
  }
}

void displayFunc(void){
  // clear the entire window to the background color
  glClear(GL_COLOR_BUFFER_BIT); 
  glClearColor(255.0, 255.0, 255.0, 255.0); 

  // draw the contents!!! 
  drawWorld();

  glFlush(); // flush the queue to paint the dots
}



void main(int ac, char** av) {
  int winSizeX=600, 
	  winSizeY=400;

  
  width = winSizeX;
  height = winSizeY;

  initWorld();	
 

  // initialize things using OpenGL utility toolkit (glut)
  glutInit(&ac, av); // initialize the toolkit

  // single disply and RGB color mapping
  glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB); // set display mode
  glutInitWindowSize(winSizeX,winSizeY); // set window size
  glutInitWindowPosition(0,0); // set window position on screen
  glutCreateWindow("Graph Generator"); // set window title

  glutKeyboardFunc(myKeyboard); // register the keyboard action function

  // displayFunc will iterate all the objects and draw them on the screen.
  glutDisplayFunc(displayFunc); //register the redraw function
  

  glClearColor(255.0, 255.0, 255.0, 255.0);  // set the background to white
  glClear(GL_COLOR_BUFFER_BIT); // clear the buffer

  glMatrixMode(GL_PROJECTION); // setup coordinate system
  glLoadIdentity();
  gluOrtho2D(0.0, winSizeX, 0.0, winSizeY);
  glShadeModel( GL_FLAT );
  glFlush();

  cout << "MENU OPTIONS: " << endl;
  cout << "[d]     draw graph\n";
  cout << "[r]     reinitialize\n";
  cout << "[c]     clear\n";
  cout << "[q]     quit\n";

  glutMainLoop(); // enter the unending main loop

}
