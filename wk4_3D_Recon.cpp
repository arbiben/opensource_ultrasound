//
//  main.cpp
//  3D_Recon
//
//  Created by CHEN Liqi on 10/31/18.
//  Copyright © 2018 CHEN Liqi. All rights reserved.
//

#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <math.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/vvector.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


// For dragging the view
static double c=M_PI/180.0f;
static int du=90,oldmy=-1,oldmx=-1;
//du - angle wrt y axis, y is up in OpenGL
static double r=1.5f,h=0.0f;


void display(){
    
}


// Mouse click action: record old coordinate when click
void Mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN){
        oldmx = x;
        oldmy = y;
    }
}

// Mouse move action
void onMouseMove(int x,int y)
{
    du += x - oldmx;
    // move left-right
    h +=0.03f*(y-oldmy);
    // move up-down
    if(h>1.0f)
        h=1.0f;
    // limit watch point's y axis
    else
        if(h<-1.0f) h=-1.0f;
    oldmx=x;
    oldmy=y;
    
    // Render to display
    glutPostRedisplay();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w,int h)
{
    glViewport( 0, 0, w, h );
    
    glMatrixMode( GL_PROJECTION );
    
    glLoadIdentity();
    
    gluPerspective(75.0f, (double)w/h, 0.5f, 1000.0f);
    
    glMatrixMode( GL_MODELVIEW );
    
}

int fill_vectors(vector <vector<double>> A, vector<double> B){
    // get all values from raw.txt file and insert into matrix and vector
    
    string line;
    ifstream sampleFile("raw.txt");
    
    if (sampleFile.is_open()){
        while ( getline(sampleFile,line)){
            int first = 0; int last = line.find(delimiter);
            double x = line.substr(first, last);
            first = last; last = line.find(delimiter, last);
            double y = line.substr(first, last);
            double z = line.substr(last);
            
            vector <double> a_val {x, y, 1.0};
            A.push_back(a_val);
            B.push_back(z);
        }
        sampleFile.close();
    }
}

int main(int argc, const char * argv[]) {
    
//    glutInit(&argc, argv);
    vector <vector<double>> A;
    vector <double> B;
    fill_vectors(A, B);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    glutInitWindowPosition(100, 100);
    
    glutInitWindowSize(400, 400);
    
    glutCreateWindow("ScanCube");
    
    init();
    
    glutReshapeFunc( reshape );
    
    glutDisplayFunc(display);
    
//    glutIdleFunc(selfMoving);
    
    glutMouseFunc(Mouse);
    
    glutMotionFunc(onMouseMove);
    
    glutMainLoop();
    
    return 0;
}
