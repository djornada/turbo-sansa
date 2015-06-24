#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include "Player.h"
#include <stdlib.h>
#include <iostream>
#include <ctime>

using namespace std;

#define CUBE 0
#define SPHERE 1

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float cRadius = 20.0f; // our radius distance from our character
float lastx, lasty;
bool keys[256];

//positions of the cubes
float positionz[1000];
float positionx[1000];

clock_t start;
int enemiesKilled = 0;

//existing cubes
bool exist[1000];
int type[1000];

int     main(int argc, char **argv);

void    cube(void);
void    cubepositions(void);
void    display(void);
void    enable(void);
void    init(void);
void    keyPress(unsigned char key, int x, int y);
void    keyUp(unsigned char key, int x, int y);
void    keyboard(void);
void    mouseMovement(int x, int y);
void    reshape(int w, int h);
void    testColisions();
void    desenhaChao();
void    fall();

void cubepositions (void) { //set the positions of the cubes

        for (int i=0; i<1000; i++)
        {
                type[i] = rand()%2;
                //cout << type[i] << endl;
                positionz[i] = rand()%100 + 1;
                positionx[i] = rand()%100 + 1;
        }
}

//draw the cube
void cube (void) {
        for (int i=0; i<1000 - 1; i++)
        {
                if (exist[i]) {
                        glPushMatrix();
                        glTranslated(-positionx[i + 1] * 10, 0, -positionz[i + 1] * 10);
                        if (type[i] == CUBE)
                                glutSolidCube(2);
                        if (type[i] == SPHERE)
                                glutSolidSphere(1, 10, 10);
                        glPopMatrix();
                }
        }
}

void testColisions (void) {
        for (int i = 0; i < 1000; i++) {
                float enemyX = -positionx[i + 1] * 10;
                float enemyZ = -positionz[i + 1] * 10;
                if ((xpos < enemyX + 2 && xpos > enemyX - 2) && (zpos < enemyZ + 2 && zpos > enemyZ - 2) ) {
                        if (exist[i]) {
                                if (type[i] == CUBE) {
                                        enemiesKilled += 1;
                                        cout << "Score: " << enemiesKilled << endl;
                                }
                                if (type[i] == SPHERE) {
                                        enemiesKilled += 5;
                                        cout << "Score: " << enemiesKilled << endl;
                                }
                        } 
                        exist[i] = false;
                }
        }
        
}

void init (void) {
        cubepositions();
        for (int i = 0; i < 1000; i++) {
                exist[i] = true;
        }
}

void enable (void) {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glShadeModel(GL_SMOOTH);
}

void display(void)
{
        double ttime = (clock() - start) / (double) CLOCKS_PER_SEC;
        if (ttime > 3){
                cout << "Score: " << enemiesKilled << endl;
                keys[27] = true;
        }
        keyboard();
        glClearColor (0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        enable();
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -cRadius);
        glRotatef(xrot,1.0,0.0,0.0);
        glColor3f(1.0f, 0.0f, 0.0f);
        glutSolidTeapot(2);
        glRotatef(yrot,0.0,1.0,0.0);
        glTranslated(-xpos,0.0f,-zpos);
        glColor3f(1.0f, 1.0f, 1.0f);
        cube();
        desenhaChao();
        testColisions();
        glutSwapBuffers();
}

void reshape(int w, int h)
{
        glViewport(0, 0, (GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
}
void keyPress(unsigned char key, int x, int y)
{
        keys[key] = true;
}
void keyUp(unsigned char key, int x, int y)
{
        keys[key] = false;
}
void keyboard(void)
{
        float xrotrad, yrotrad;
        yrotrad = (yrot/180 * 3.141592654f);
        xrotrad = (xrot/180 * 3.141592654f);

        if (keys['w'])
        {
                xpos   += float(sin(yrotrad));
                zpos   -= float(cos(yrotrad));
                ypos   -= float(sin(xrotrad));
        }

        if (keys['s'])
        {
                xpos   -= float(sin(yrotrad));
                zpos   += float(cos(yrotrad));
                ypos   += float(sin(xrotrad));
        }

        if (keys['d'])
        {
                xpos   += float(cos(yrotrad)) * 0.5;
                zpos   += float(sin(yrotrad)) * 0.5;
        }

        if (keys['a'])
        {
                xpos   -= float(cos(yrotrad)) * 0.5;
                zpos   -= float(sin(yrotrad)) * 0.5;
        }

        if (keys[27])
        {
                glutLeaveGameMode();
                exit(0);
        }
}
void desenhaChao()
{
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(-1000,-1,-1000);
        glVertex3f(-1000,-1,-1);
        glVertex3f(-1,-1,-1);
        glVertex3f(-1,-1,-1000);
        glEnd();
}

void mouseMovement(int x, int y)
{
        int diffx=(float)(x-lastx);
        int diffy=(float)(y-lasty);
        lastx=x;
        lasty=y;
        xrot+=  diffy*0.1;
        yrot+=  diffx;
}

int main (int argc, char **argv)
{
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
        glutEnterGameMode();
        start = clock();
        init();
        glutDisplayFunc(display);
        glutIdleFunc(display);
        glutReshapeFunc(reshape);
        glutPassiveMotionFunc(mouseMovement);
        glutKeyboardFunc(keyPress);
        glutKeyboardUpFunc(keyUp);
        glutMainLoop();
        return 0;
}
