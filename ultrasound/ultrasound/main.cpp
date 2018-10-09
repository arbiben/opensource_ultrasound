#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <math.h>
#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>


// ----------------------------------------------------------
// Global Variables
// ----------------------------------------------------------
// For rotate cube
double rotate_y = 0;
double rotate_x = 0;

// For moving probe
double step = -0.02;
double height = 0;
double inter_point = 0.01;

// for moving intersection points
double inter_ry = -0.0001;
//double inter_ly = -0.01;

// For dragging the view
static float c=M_PI/180.0f;
static int du=90,oldmy=-1,oldmx=-1;
//du - angle wrt y axis, y is up in OpenGL
static float r=1.5f,h=0.0f;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    
    gluLookAt(r*cos(c*du), h, r*sin(c*du), 0, 0, 0, 0, 1, 0);
    
    glPushMatrix();
    
    glTranslatef(0, height, 0);
    
    // Probe - Straight Line
    glBegin(GL_LINES);
    glLineWidth(3.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.7, 0.0, 0.0);
    glVertex3f(0.7, 0.0, 0.0);
    glEnd();

    glTranslatef(inter_ry,0,0);
    // Intersection point 1
    glPointSize(10.0f);//set point size to 10 pixels
    glColor3f(1.0f,0.0f,0.0f); //red color
    glBegin(GL_POINTS); //starts drawing of points
    glVertex3f(0.3,0.0,0.0);//right plane
    glEnd();//end drawing of points
    
//    // Intersection point 2
//    glPointSize(10.0f);            //set point size to 10 pixels
//    glColor3f(1.0,0.0,0.0);        //red
//    glBegin(GL_POINTS);            //starts drawing of points
//    glVertex3f(-0.3,0.0,0.0);   //left plane
//    glEnd();                       //end drawing of points

    glPopMatrix();
    
    glPushMatrix();
    
    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
    
    // FRONT
    glBegin(GL_POLYGON);
    glColor3f(   0.6,  0.6, 0.6 );
    glVertex3f(  0.3, -0.3, -0.3 );
    glVertex3f(  0.3,  0.3, -0.3 );
    glVertex3f( -0.3,  0.3, -0.3 );
    glVertex3f( -0.3, -0.3, -0.3 );
    glEnd();
    
    // BACK
    glBegin(GL_POLYGON);
    glColor3f(    0.5,  0.5, 0.5 );
    glVertex3f(  0.3, -0.3, 0.3 );
    glVertex3f(  0.3,  0.3, 0.3 );
    glVertex3f( -0.3,  0.3, 0.3 );
    glVertex3f( -0.3, -0.3, 0.3 );
    glEnd();
    
    // RIGHT
    glBegin(GL_POLYGON);
    glColor3f(   0.7,  0.7, 0.7 );
    glVertex3f( 0.3, -0.3, -0.3 );
    glVertex3f( 0.3,  0.3, -0.3 );
    glVertex3f( 0.3,  0.3,  0.3 );
    glVertex3f( 0.3, -0.3,  0.3 );
    glEnd();
    
    // LEFT
    glBegin(GL_POLYGON);
    glColor3f(    0.8,  0.8, 0.8 );
    glVertex3f( -0.3, -0.3,  0.3 );
    glVertex3f( -0.3,  0.3,  0.3 );
    glVertex3f( -0.3,  0.3, -0.3 );
    glVertex3f( -0.3, -0.3, -0.3 );
    glEnd();
    
    // TOP
    glBegin(GL_POLYGON);
    glColor3f(    0.4,  0.4, 0.4 );
    glVertex3f(  0.3,  0.3,  0.3 );
    glVertex3f(  0.3,  0.3, -0.3 );
    glVertex3f( -0.3,  0.3, -0.3 );
    glVertex3f( -0.3,  0.3,  0.3 );
    glEnd();
    
    // BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(    0.3,  0.3, 0.3 );
    glVertex3f(  0.3, -0.3, -0.3 );
    glVertex3f(  0.3, -0.3,  0.3 );
    glVertex3f( -0.3, -0.3,  0.3 );
    glVertex3f( -0.3, -0.3, -0.3 );
    glEnd();
    
    glPopMatrix();
    glFlush();
    
    glutSwapBuffers();
    
}

// Mouse click action: record old coordinate when click
void Mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    oldmx = x;
    oldmy = y;

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
    
    gluPerspective(75.0f, (float)w/h, 1.0f, 1000.0f);
    
    glMatrixMode( GL_MODELVIEW );
    
}

void selfMoving() {
//    // Rotate cube
    rotate_y = (int)(rotate_y + 1) % 360;
//    //rotate_x = (int)(rotate_x + 1) % 360;
//
//    //rotate_y = 0;
//    rotate_x = 0;
//
    inter_ry = inter_ry + inter_point;
    if(inter_ry < 0.0) inter_point = -inter_point;
    if(inter_ry > 0.15)  inter_point = -inter_point;
    
//    inter_ly = inter_ly - 0.01;
    
    // Move probe
//    height = height + step;
//    if(height < -0.3)
//        step = -step;
//    if(height > 0.3)
//        step = -step;
    
    // Render to display
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    glutInitWindowPosition(100, 100);
    
    glutInitWindowSize(400, 400);
    
    glutCreateWindow("ScanCube");
    
    init();
    
    glutReshapeFunc( reshape );
    
    glutDisplayFunc(display);
    
    glutIdleFunc(selfMoving);
    
    glutMouseFunc(Mouse);
    
    glutMotionFunc(onMouseMove);
    
    glutMainLoop();
    
    return 0;
}
