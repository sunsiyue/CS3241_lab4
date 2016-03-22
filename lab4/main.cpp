// CS3241Lab1.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

/* header files for xcode */
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>

/* header files for lab machine */
//#include "GL\glut.h"

#define MAXPTNO 1000
#define NLINESEGMENT 32
#define NOBJECTONCURVE 8

using namespace std;

// Global variables that you can use
struct Point {
	int x,y;
};



// Storage of control points
int nPt = 0;
Point ptList[MAXPTNO];
Point ptList_C[MAXPTNO];

// Display options
bool displayControlPoints = true;
bool displayControlLines = true;
bool displayTangentVectors = false;
bool displayObjects = false;
bool C1Continuity = false;

void drawRightArrow(int transX, int transY, float ang)
{
    glTranslatef(transX, transY, 0);
    glRotated(ang, 0, 0, 1);
    
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
    glVertex2f(0,0);
    glVertex2f(100,0);
    glVertex2f(95,5);
    glVertex2f(100,0);
    glVertex2f(95,-5);
	glEnd();
    
    glRotated(-ang, 0, 0, 1);
    glTranslatef(-transX, -transY, 0);
}


void drawCaterpillar_part(int transX, int transY, float ang) {
    glTranslatef(transX, transY, 0);
    glRotated(ang, 0, 0, 1);
    
    int i;
    glBegin(GL_POLYGON);
    
    glColor3f(0.0, 0.8, 0.0);
    for(i=0;i<50;i++)
        glVertex2f(20*sin(2.0*M_PI*i/50.0),
                   20*cos(2.0*M_PI*i/50.0));
    
    
    glEnd();
    
    int l;
    glBegin(GL_LINE_STRIP);
    
    glColor3f(0.2, 0.2, 0.1);
    for(l=0;l<50;l++)
        glVertex2f(20*sin(2.0*M_PI*l/50.0),
                   20*cos(2.0*M_PI*l/50.0));
    
    glEnd();
    
    
    glTranslatef(0, 15, 0);
    int j;
    glBegin(GL_POLYGON);
    
    glColor3f(0.2, 0.2, 0.1);
    for(j=0;j<50;j++)
        glVertex2f(5*sin(2.0*M_PI*j/50.0),
                   7*cos(2.0*M_PI*j/50.0));
    
    glEnd();
    
    glTranslatef(0, -30, 0);
    int k;
    glBegin(GL_POLYGON);
    
    glColor3f(0.2, 0.2, 0.1);
    for(k=0;k<50;k++)
        glVertex2f(5*sin(2.0*M_PI*k/50.0),
                   7*cos(2.0*M_PI*k/50.0));
    
    glEnd();
    
    
    
    
    
    glTranslatef(0, 15, 0);
    
   
    glRotated(-ang, 0, 0, 1);
    glTranslatef(-transX, -transY, 0);
}



Point calculate_p_position(Point p0, Point p1, float t)
{
    Point a;
    a.x = p0.x + (p1.x - p0.x) * t;
    a.y = p0.y + (p1.y - p0.y) * t;
    
    return a;
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
    Point curvePoints[32];
    
    if (!C1Continuity) {
        //#1*********************************************************************************display control points
        if(displayControlPoints)
        {
            glPointSize(5);
            glBegin(GL_POINTS);
            for(int i=0;i<nPt; i++)
            {
                glColor3f(0,0,0);
                glVertex2d(ptList[i].x,ptList[i].y);
                //cout << "current index i is "<< i <<endl;
            }
            glEnd();
            glPointSize(1);
        }
       
        if(displayControlLines)
        {
            
            glColor3f(0,1,0);
            glLineWidth(1);
            
            ////*#1*********************************************************************************display zigzag lines
            glBegin(GL_LINE_STRIP);
            for (int j=0; j<nPt; j++) {
                glVertex2d(ptList[j].x,ptList[j].y);
                
            }
            
            glEnd();
            
            //draw respective curves once in three points
            for (int k=0; k<nPt; k++) {
                if (k%3 == 0 && k != 0) {
                    glColor3f(0,0,1);
                    
                    
                    glBegin(GL_LINE_STRIP);
                    for (int t_value = 0; t_value<32; t_value++) {
                        //first layer
                        Point temp_p10 = calculate_p_position(ptList[k-3], ptList[k-2], t_value/32.0);
                        Point temp_p11 = calculate_p_position(ptList[k-2], ptList[k-1], t_value/32.0);
                        Point temp_p12 = calculate_p_position(ptList[k-1], ptList[k], t_value/32.0);
                        //second layer
                        Point temp_p20 = calculate_p_position(temp_p10, temp_p11, t_value/32.0);
                        Point temp_p21 = calculate_p_position(temp_p11, temp_p12, t_value/32.0);
                        //third layer
                        Point temp_p30 = calculate_p_position(temp_p20, temp_p21, t_value/32.0);
                        
                        glVertex2d(temp_p30.x,temp_p30.y);
                        
                        curvePoints[t_value].x = temp_p30.x;
                        curvePoints[t_value].y = temp_p30.y;

                    }
                    glVertex2d(ptList[k].x,ptList[k].y);
                    
                    glEnd();
                    
                    //draw tangent lines
                    if (displayTangentVectors) {
                        for (int t_counter = 0; t_counter < 32; t_counter++) {
                            if (t_counter%4 == 0) {
                                
                                //glTranslatef(ptList[0].x, ptList[0].y, 0);
                                //draw arrow
                                int currentX = curvePoints[t_counter].x;
                                int currentY = curvePoints[t_counter].y;
                                
                                
                                
                                float tempTan = (curvePoints[t_counter+1].y*1.0 - curvePoints[t_counter].y*1.0) / (curvePoints[t_counter+1].x*1.0 - curvePoints[t_counter].x*1.0);
                                
                                //cout << "delta X value is " << (curvePoints[t_counter+1].x - curvePoints[t_counter].x) <<endl;
                                //cout << "delta Y value is " << (curvePoints[t_counter+1].y - curvePoints[t_counter].y) <<endl;
                                //cout << "temptan value is " << tempTan << endl;
                                
                                
                                float refAng = atan (tempTan) * 180 / 3.1415926;
                                //cout << "refAng Value is " << refAng << endl;
                                
                                
                                drawRightArrow(currentX, currentY, refAng);
                                
                                
                            }
                            
                        }
                    }
                    
                
                }
            }
        }
       
        if(displayObjects) {
            
            glColor3f(0,1,0);
            glLineWidth(1);
            
            //draw respective curves once in three points
            for (int k=0; k<nPt; k++) {
                if (k%3 == 0 && k != 0) {

                    for (int t_value = 0; t_value<32; t_value++) {
                        //first layer
                        Point temp_p10 = calculate_p_position(ptList[k-3], ptList[k-2], t_value/32.0);
                        Point temp_p11 = calculate_p_position(ptList[k-2], ptList[k-1], t_value/32.0);
                        Point temp_p12 = calculate_p_position(ptList[k-1], ptList[k], t_value/32.0);
                        //second layer
                        Point temp_p20 = calculate_p_position(temp_p10, temp_p11, t_value/32.0);
                        Point temp_p21 = calculate_p_position(temp_p11, temp_p12, t_value/32.0);
                        //third layer
                        Point temp_p30 = calculate_p_position(temp_p20, temp_p21, t_value/32.0);
                    
                        curvePoints[t_value].x = temp_p30.x;
                        curvePoints[t_value].y = temp_p30.y;
                        
                    }
    
                    
                    
                    
                    //draw tangent lines
                    if (displayTangentVectors) {
                        for (int t_counter = 0; t_counter < 32; t_counter++) {
                            if (t_counter%4 == 0) {
                                
                                //glTranslatef(ptList[0].x, ptList[0].y, 0);
                                //draw arrow
                                int currentX = curvePoints[t_counter].x;
                                int currentY = curvePoints[t_counter].y;
                                
                                
                                
                                float tempTan = (curvePoints[t_counter+1].y*1.0 - curvePoints[t_counter].y*1.0) / (curvePoints[t_counter+1].x*1.0 - curvePoints[t_counter].x*1.0);
                                

                                
                                
                                float refAng = atan (tempTan) * 180 / 3.1415926;
                                //cout << "refAng Value is " << refAng << endl;
                                
                                
                                drawRightArrow(currentX, currentY, refAng);
                                
                                
                            }
                            
                        }
                    }
                    
                    
                    
                    
                    for (int o_counter = 0; o_counter < 32; o_counter++) {
                        if (o_counter%4 == 0) {
                            
                            int currentX = curvePoints[o_counter].x;
                            int currentY = curvePoints[o_counter].y;
                            float tempTan = (curvePoints[o_counter+1].y*1.0 - curvePoints[o_counter].y*1.0) / (curvePoints[o_counter+1].x*1.0 - curvePoints[o_counter].x*1.0);
                        
                            float refAng = atan (tempTan) * 180 / 3.1415926;

                            drawCaterpillar_part(currentX, currentY, refAng);
                            
                            
                        }
                        
                    }                 
                }
            }
        }
    }
    
    else { //C1 case
        //pre-process points in the array
        for (int c_counter = 0; c_counter<nPt; c_counter++) {
            
            if (c_counter%3 == 1 && c_counter != 1) {
                ptList_C[c_counter].x = (ptList[c_counter-1].x - ptList[c_counter-2].x) + ptList[c_counter-1].x;
                ptList_C[c_counter].y = (ptList[c_counter-1].y - ptList[c_counter-2].y) + ptList[c_counter-1].y;
            }
            else {
                ptList_C[c_counter] = ptList[c_counter];
            }
        }
    
        
        //#1*********************************************************************************display control points
        if(displayControlPoints)
        {
            glPointSize(5);
            glBegin(GL_POINTS);
            for(int i=0;i<nPt; i++)
            {
                glColor3f(0,0,0);
                glVertex2d(ptList_C[i].x,ptList_C[i].y);
            }
            glEnd();
            glPointSize(1);
        }
        
        
        
        if(displayControlLines)
        {
            
            glColor3f(0,1,0);
            glLineWidth(1);
            
            ////*#1*********************************************************************************display zigzag lines
            glBegin(GL_LINE_STRIP);
            for (int j=0; j<nPt; j++) {
                glVertex2d(ptList_C[j].x,ptList_C[j].y);
                
            }
            
            glEnd();
            
            //draw respective curves once in three points
            for (int k=0; k<nPt; k++) {
                if (k%3 == 0 && k != 0) {
                    glColor3f(0,0,1);
                    
                    
                    glBegin(GL_LINE_STRIP);
                    for (int t_value = 0; t_value<32; t_value++) {
                        //first layer
                        Point temp_p10 = calculate_p_position(ptList_C[k-3], ptList_C[k-2], t_value/32.0);
                        Point temp_p11 = calculate_p_position(ptList_C[k-2], ptList_C[k-1], t_value/32.0);
                        Point temp_p12 = calculate_p_position(ptList_C[k-1], ptList_C[k], t_value/32.0);
                        //second layer
                        Point temp_p20 = calculate_p_position(temp_p10, temp_p11, t_value/32.0);
                        Point temp_p21 = calculate_p_position(temp_p11, temp_p12, t_value/32.0);
                        //third layer
                        Point temp_p30 = calculate_p_position(temp_p20, temp_p21, t_value/32.0);
                        
                        glVertex2d(temp_p30.x,temp_p30.y);
                        // drawRightArrow(temp_p30.x,temp_p30.y);
                        
                        curvePoints[t_value].x = temp_p30.x;
                        curvePoints[t_value].y = temp_p30.y;
                        
                    }
                    glVertex2d(ptList_C[k].x,ptList_C[k].y);
                    
                    glEnd();
                    
                    //draw tangent lines
                    if (displayTangentVectors) {
                        for (int t_counter = 0; t_counter < 32; t_counter++) {
                            if (t_counter%4 == 0) {
                             
                                //draw arrow
                                int currentX = curvePoints[t_counter].x;
                                int currentY = curvePoints[t_counter].y;
                                
                                
                                
                                float tempTan = (curvePoints[t_counter+1].y*1.0 - curvePoints[t_counter].y*1.0) / (curvePoints[t_counter+1].x*1.0 - curvePoints[t_counter].x*1.0);

                                
                                float refAng = atan (tempTan) * 180 / 3.1415926;
                                //cout << "refAng Value is " << refAng << endl;
                                
                                
                                drawRightArrow(currentX, currentY, refAng);
                                
                                
                            }
                            
                        }
                    }
                    
                }
            }
        }
        
        if(displayObjects) {
            
            glColor3f(0,1,0);
            glLineWidth(1);
            
            //draw respective curves once in three points
            for (int k=0; k<nPt; k++) {
                if (k%3 == 0 && k != 0) {
                    
                    for (int t_value = 0; t_value<32; t_value++) {
                        //first layer
                        Point temp_p10 = calculate_p_position(ptList_C[k-3], ptList_C[k-2], t_value/32.0);
                        Point temp_p11 = calculate_p_position(ptList_C[k-2], ptList_C[k-1], t_value/32.0);
                        Point temp_p12 = calculate_p_position(ptList_C[k-1], ptList_C[k], t_value/32.0);
                        //second layer
                        Point temp_p20 = calculate_p_position(temp_p10, temp_p11, t_value/32.0);
                        Point temp_p21 = calculate_p_position(temp_p11, temp_p12, t_value/32.0);
                        //third layer
                        Point temp_p30 = calculate_p_position(temp_p20, temp_p21, t_value/32.0);
                        
                        curvePoints[t_value].x = temp_p30.x;
                        curvePoints[t_value].y = temp_p30.y;
                        
                    }
                    
                    
                    
                    if (displayTangentVectors) {
                        for (int t_counter = 0; t_counter < 32; t_counter++) {
                            if (t_counter%4 == 0) {
                                
                                //draw arrow
                                int currentX = curvePoints[t_counter].x;
                                int currentY = curvePoints[t_counter].y;
                                
                                
                                
                                float tempTan = (curvePoints[t_counter+1].y*1.0 - curvePoints[t_counter].y*1.0) / (curvePoints[t_counter+1].x*1.0 - curvePoints[t_counter].x*1.0);
                                
                                
                                float refAng = atan (tempTan) * 180 / 3.1415926;
                                //cout << "refAng Value is " << refAng << endl;
                                
                                
                                drawRightArrow(currentX, currentY, refAng);
                                
                                
                            }
                            
                        }
                    }
                    
                    
                    
                    
                    for (int o_counter = 0; o_counter < 32; o_counter++) {
                        if (o_counter%4 == 0) {
                            
                            int currentX = curvePoints[o_counter].x;
                            int currentY = curvePoints[o_counter].y;
                            float tempTan = (curvePoints[o_counter+1].y*1.0 - curvePoints[o_counter].y*1.0) / (curvePoints[o_counter+1].x*1.0 - curvePoints[o_counter].x*1.0);
                            
                            float refAng = atan (tempTan) * 180 / 3.1415926;
                            
                            drawCaterpillar_part(currentX, currentY, refAng);
                            
                            
                        }
                        
                    }                 
                }
            }
        }

        
        
        
    }
    

    
	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
}

void readFile()
{
    
	std::ifstream file;
    file.open("savefile.txt");
	file >> nPt;
    
	if(nPt>MAXPTNO)
	{
		cout << "Error: File contains more than the maximum number of points." << endl;
		nPt = MAXPTNO;
	}
    
	for(int i=0;i<nPt;i++)
	{
		file >> ptList[i].x;
		file >> ptList[i].y;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void writeFile()
{
	std::ofstream file;
    file.open("savefile.txt");
    file << nPt << endl;
    
	for(int i=0;i<nPt;i++)
	{
		file << ptList[i].x << " ";
		file << ptList[i].y << endl;
	}
    file.close();// is not necessary because the destructor closes the open file by default
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
            break;
            
		case 'w':
		case 'W':
			writeFile();
            break;
            
		case 'T':
		case 't':
			displayTangentVectors = !displayTangentVectors;
            break;
            
		case 'o':
		case 'O':
			displayObjects = !displayObjects;
            break;
            
		case 'p':
		case 'P':
			displayControlPoints = !displayControlPoints;
            break;
            
		case 'L':
		case 'l':
			displayControlLines = !displayControlLines;
            break;
            
		case 'C':
		case 'c':
			C1Continuity = !C1Continuity;
            break;
            
		case 'e':
		case 'E':
			// Do something to erase all the control points added
            //Point nothing = [null, null];

            for (int k=0; k<MAXPTNO; k++) {
                ptList[k].x = NULL;
                ptList[k].y = NULL;
                
            }
            
            nPt = 0;
            
            
            break;
            
		case 'Q':
		case 'q':
			exit(0);
            break;
            
		default:
            break;
	}
    
	glutPostRedisplay();
}



void mouse(int button, int state, int x, int y)
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};
	if((button == MOUSE_LEFT_BUTTON)&&(state == GLUT_UP))
	{
		if(nPt==MAXPTNO)
		{
			cout << "Error: Exceeded the maximum number of points." << endl;
			return;
		}
		ptList[nPt].x=x;
		ptList[nPt].y=y;
		nPt++;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 4"<< endl<< endl;
	cout << "Left mouse click: Add a control point"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "P: Toggle displaying control points" <<endl;
	cout << "L: Toggle displaying control lines" <<endl;
	cout << "E: Erase all points (Clear)" << endl;
	cout << "C: Toggle C1 continuity" <<endl;	
	cout << "T: Toggle displaying tangent vectors" <<endl;
	cout << "O: Toggle displaying objects" <<endl;
	cout << "R: Read in control points from \"savefile.txt\"" <<endl;
	cout << "W: Write control points to \"savefile.txt\"" <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 4");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
    
	return 0;
}
