#include <GL\glut.h>
#include <math.h>
#include <stdio.h>
#define TSIZE 6

const int w = 760;
const int h = 600;

float x, y;

float distance = 0.0;
float middle = 0.5;
float speed = 0.0;

float pos = 0.0;
int nsection = 0;
float cur_curve;
float res_curve = 0.0f;
float curve_diff;
float track_curvature = 0.0;
float player_curvature = 0.0;

float pixel_size = 2;

int tick;

float carpos_x;
float carpos_y;
float carpos;
float direction;


struct Track{
    float curve;
    float length;
}track[TSIZE];

int wPressed = 0;
int aPressed = 0;
int dPressed = 0;

void move1(unsigned char key, int x, int y){
    switch (key) {
		case 'w' : wPressed = 1; break;
		case 'a' : aPressed = 1; break;
		case 'd' : dPressed = 1; break;

	}

    if(key==27) exit(0);
}

void move2(unsigned char key, int x, int y){
    switch (key) {
		case 'w' : wPressed = 0; break;
		case 'a' : aPressed = 0; break;
		case 'd' : dPressed = 0; break;
	}
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	//glBegin(GL_POINTS);

    for(y = -h/2; y < 0; y+=pixel_size){
        for(x = 0; x < w; x+=pixel_size){
        if(y < -h/4)glColor3f(0.0, 0.1, 0.5);
        else glColor3f(0.0, 0.1, 0.6);
        glRectf(x, y, x+pixel_size, y+pixel_size);
        }
    }

    glColor3f(0.6, 0.5, 0.2);
    for(x = 0; x < w; x+=pixel_size){
        int m_height = -(int)fabs(sinf(x*0.005+track_curvature)*50);
        for(y = 0; y > m_height; y-=pixel_size){
            glRectf(x, y, x+pixel_size, y+pixel_size);
        }
    }

    for(y = 0; y < h/2; y+=pixel_size){
        float perspective = y/(h/2);

        //middle = 0.5f + res_curve*powf(1.0f - perspective, 3);
        middle = 0.5f + res_curve*powf(1.0f - perspective, 3)+carpos*y/100+carpos/20;

        float roadw = 0.01+0.5*perspective;
        float roadedge = roadw*0.2;
        float lGrass = (middle-roadw-roadedge)*w;
        float lEdge = (middle-roadw)*w;
        float rGrass = (middle+roadw+roadedge)*w;
        float rEdge = (middle+roadw)*w;
        float road_mark = roadw*0.03;
        float lMark = (middle+road_mark)*w;
        float rMark = (middle-road_mark)*w;
        float line_color = sinf(30.0f*powf(1.0f - perspective, 3) + distance*1.0f);
        for(x = 0; x < w; x+=pixel_size){;
            if(x>0&&x<=lGrass||x>rGrass&&x<w){
                if(line_color > 0) glColor3f(0.1, 0.7, 0.1);
                else glColor3f(0.1, 0.6, 0.1);
                //glVertex2f(x, y);
                glRectf(x, y, x+pixel_size, y+pixel_size);
                }
            if(x>lGrass&&x<=lEdge||x>rEdge&&x<=rGrass){
                if(line_color > 0) glColor3f(1.0, 1.0, 1.0);
                else glColor3f(1.0, 0.0, 0.0);
                //glVertex2f(x, y);
                glRectf(x, y, x+pixel_size, y+pixel_size);
                }
            if(x>lEdge&&x<=rEdge){
               if(line_color > 0) glColor3f(0.5, 0.5, 0.5);
                else glColor3f(0.45, 0.45, 0.45);
                //glVertex2f(x, y);
                glRectf(x, y, x+pixel_size, y+pixel_size);
                }
             if(x>=rMark&&x<=lMark){
                if(line_color > 0) glColor3f(0.5, 0.5, 0.5);
                else glColor3f(1.0, 1.0, 1.0);
                //glVertex2f(x, y);
                glRectf(x, y, x+pixel_size, y+pixel_size);
                }
        }

    }

	//glEnd();
	glFlush();
}

void car(float middlex, float middley){
    float a = 50;
    float b = 160;

    float x, y;
    float wheel = 0.5;

    glColor3f(0.0, 0.0, 0.0);
    glRectf(middlex-b/3+direction*b/12, middley, middlex-b/6+direction*b/12, middley+a*0.4);
    glRectf(middlex+b/6+direction*b/12, middley, middlex+b/3+direction*b/12, middley+a*0.4);
    glRectf(middlex-b/2-direction*b/12, middley+a/2, middlex-b/4-direction*b/12, middley+a+4);
    glRectf(middlex+b/2-direction*b/12, middley+a/2, middlex+b/4-direction*b/12, middley+a+4);
    glColor3f(1.0, 1.0, 0.1);
    glRectf(middlex-b/2.5, middley+a*0.4, middlex+b/2.5, middley+a/2);
    glColor3f(1.0, 0.0, 0.0);
    glRectf(middlex-b/6+direction*b/12, middley, middlex+b/6+direction*b/12, middley+a*0.4);
    glRectf(middlex-b/4-direction*b/12, middley+a/2, middlex+b/4-direction*b/12, middley+a+2);
    if(tick%3==0) glColor3f(1.0, 1.0, 1.0);
    else glColor3f(0.0, 0.0, 0.0);
    if(speed == 0) glColor3f(0.0, 0.0, 0.0);
        glRectf(middlex-b/3+direction*b/12, middley, middlex-b/6+direction*b/12, middley+3);
        glRectf(middlex+b/6+direction*b/12, middley, middlex+b/3+direction*b/12, middley+3);
        glRectf(middlex-b/2-direction*b/12, middley+a/2, middlex-b/4-direction*b/12, middley+a/2+3);
        glRectf(middlex+b/2-direction*b/12, middley+a/2, middlex+b/4-direction*b/12, middley+a/2+3);
	glFlush();
}


void timer(){


    if(wPressed) speed+=0.05;
    else speed-=0.1;

    if(dPressed){
            player_curvature += 0.025;
            direction = 0.8;
    }
    else if(aPressed){
        player_curvature -= 0.025;
        direction = -0.8;
    }
    else direction = 0;

    if(fabs(track_curvature-player_curvature)>=0.18) speed -= 0.2;

    if(speed<0) speed = 0;
    if(speed>2) speed = 2;

    distance += speed;
    while(nsection<TSIZE && pos<=distance){
        pos += track[nsection].length;
        nsection++;
        if(nsection == TSIZE) nsection = 0;
    }
    tick++;
    cur_curve = track[nsection-1].curve;
    curve_diff = (cur_curve - res_curve)*0.04*speed;
    res_curve += curve_diff;
    track_curvature += (res_curve*speed)/55;

    //carpos = (player_curvature-track_curvature);
    carpos = track_curvature-player_curvature;
    //carpos_x = w/2 + (w*carpos)/2;
    carpos_x = w/2;
    carpos_y = h/3.5;



    display();
    car(carpos_x, carpos_y);
    //printf("%f %f %f %f\n", cur_curve, track_curvature, player_curvature, speed);
    glutTimerFunc(40, timer, 0);
}

void start(){
    track[0].curve = 0.0f;
    track[0].length = 10;
    track[1].curve = 0.0f;
    track[1].length = 200;
    track[2].curve = 1.0f;
    track[2].length = 100;
    track[3].curve = -1.0f;
    track[3].length = 100;
    track[4].curve = -0.5f;
    track[4].length = 200;
    track[5].curve = 0.0f;
    track[5].length = 200;
}

int main(int argc, char** argv){
    start();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("RETRO RACING");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h/2, -h/2, -1, 1);
	glutDisplayFunc(display);
	glutKeyboardFunc(move1);
	glutKeyboardUpFunc(move2);
	timer();
	glutMainLoop();
	return 0;
}

