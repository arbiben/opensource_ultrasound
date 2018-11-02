//
//  main.cpp
//  3D_Recon
//
//  Created by CHEN Liqi on 10/31/18.
//  Copyright © 2018 CHEN Liqi. All rights reserved.

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
static vector<double> plane_vector;
static vector <vector<double>> A;
static vector<double> B;

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(r*cos(c*du), h, r*sin(c*du), 0, 0, 0, 0, 1, 0);
//    glPushMatrix();

    // plane
    double z1 = 0.5 * plane_vector[0] + 0.5 * plane_vector[1] + plane_vector[2];
    double z2 = -0.5 * plane_vector[0] + 0.5 * plane_vector[1] + plane_vector[2];
    double z3 = 0.5 * plane_vector[0] - 0.5 * plane_vector[1] + plane_vector[2];
    double z4 = -0.5 * plane_vector[0] - 0.5 * plane_vector[1] + plane_vector[2];

    
    glBegin(GL_POLYGON);
    glColor3f(   0.5,  0.5, 0.5 );
    
    glVertex3f(  0.5, 0.5, z1 );
    glVertex3f(  -0.5,  0.5, z2 );
    glVertex3f( -0.5, -0.5, z4 );
    glVertex3f( 0.5,  -0.5, z3 );
    glEnd();
    
//    glPopMatrix();
    

    glPointSize(7.0f);//set point size to 10 pixels
    glColor3f(1.0f,0.0f,0.0f); //red color
    for (int i=0 ; i<A.size(); i++){
        double x, y, z;
        x = A[i][0]; y = A[i][1];
        z = B[i];
        // Draw intersection points

        glBegin(GL_POINTS); //starts drawing of points
        glVertex3f(x,y,z);
        glEnd();//end drawing of points
    }
    
    glFlush();
    glutSwapBuffers();
}

// Mouse click action: record old coordinate when click
void Mouse(int button, int state, int x, int y){
    if(state == GLUT_DOWN){
        oldmx = x;
        oldmy = y;
    }
}

// Mouse move action
void onMouseMove(int x,int y){
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

void init(){
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w,int h){
    glViewport( 0, 0, w, h );
    
    glMatrixMode( GL_PROJECTION );
    
    glLoadIdentity();
    
    gluPerspective(75.0f, (float)w/h, 1.0f, 1000.0f);
    
    glMatrixMode( GL_MODELVIEW );
    
}

void matrix_times_vector(vector<vector<double>> A, vector<double> V, vector<double>& ans){
    for (int i=0; i<A.size(); i++){
        int curr = 0;
        for (int j=0; j<A[0].size(); j++){
            curr += A[i][j]*V[j];
        }
        ans.push_back(curr);
    }
}

void fill_vectors(vector <vector<double>>& A, vector<double>& B){
    // get all values from raw.txt file and insert into matrix and vector
    string line;
    ifstream sampleFile;
    sampleFile.open("raw.txt");
    
    string delimiter = " ";
    while ( getline(sampleFile,line)){
        int first = 0; int last = line.find(delimiter);
        double x = stod(line.substr(first, last-first)) + 1;
        
        first = last+1; last = line.find(delimiter, first);
        
        double y = stod(line.substr(first, last-first));
        
        double z = stod(line.substr(last+1));
        vector <double> a_val {x, y, 1.0};
        A.push_back(a_val);
        B.push_back(z);
    }
    sampleFile.close();
}

void fill_transpose(vector<vector<double>> from, vector<vector<double>>& to){
    for (int i=0; i<from[0].size(); i++){
        to[i] = vector<double>(from.size());
        for (int j=0; j<from.size(); j++){
            to[i][j] = from[j][i];
        }
    }
}

void multiply_matrices(vector<vector<double>> A, vector<vector<double>> B, vector<vector<double>>& to){
    // initialize the vector
    for (int i=0; i<A.size(); i++){
        vector<double> a;
        to.push_back(a);
        for(int j=0; j<B[0].size(); j++) to[i].push_back(0);
    }
    
    for (int i=0; i<A.size(); i++){
        for (int j=0; j<B[0].size(); j++){
            double curr = 0;
            for (int k=0; k<B.size(); k++){
                curr += (A[i][k] * B[k][j]);
            }
            to[i][j] = curr;
        }
    }
}

int main(int argc, char * argv[]) {

    vector <vector<double>> inverse;
    double det;
    fill_vectors(A, B);
    vector <vector<double>> A_t(A[0].size()); // transpose of A
    fill_transpose(A, A_t);
    vector <vector<double>> A_t_A; // transpose A times A
    multiply_matrices(A_t, A, A_t_A);
    for (int i=0; i<A_t_A.size(); i++){
        vector<double> a;
        inverse.push_back(a);
        for(int j=0; j<A_t_A[0].size(); j++) inverse[i].push_back(0);
    }
    INVERT_3X3(inverse, det, A_t_A);
    
//    cout << det << endl;
    vector<vector<double>> temp;
    multiply_matrices(inverse, A_t, temp);
    matrix_times_vector(temp, B, plane_vector);
    
//    for (int i=0; i<A_t_A.size(); i++){
//        for (int j=0; j<A_t_A[0].size(); j++){
//            cout << A_t_A[i][j] << "\t" ;
//        }
//        cout << " " << endl;
//    }
//    cout << "\n" << endl;
//
//    for (int i=0; i<A.size(); i++){
//        for (int j=0; j<A[0].size(); j++){
//            cout << A[i][j] << "\t" ;
//        }
//        cout << "" << endl;
//    }
//
//        for (int i=0; i<A_t.size(); i++){
//            for (int j=0; j<A_t[0].size(); j++){
//                cout << A_t[i][j] << "\t" ;
//            }
//            cout << "" << endl;
//        }
    
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    glutInitWindowPosition(100, 100);
    
    glutInitWindowSize(400, 400);
    
    glutCreateWindow("ScanCube");
    
    init();
    
    glutReshapeFunc( reshape );
    
    glutDisplayFunc(display);
    
    glutMouseFunc(Mouse);
    
    glutMotionFunc(onMouseMove);
    
    glutMainLoop();
    
    return 0;
}

