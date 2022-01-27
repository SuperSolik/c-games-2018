#include <GL/gl.h>
#include <Gl/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SIZE 10
#define SPEED 2
#define N2 25
#define N1 10
#define WINDOW_W 600
#define WINDOW_H 600



void timer();
void display();
void field();
void player();
void spikes();
void move(unsigned char key, int x, int y);
void start();
int logic();
void gameover();

int score = 0;
int w =  40;//half of the field side
int h1 = 0;//z1
int h2 = -1000;//z3
int r = 20;
int i = 1;

struct Spike{//препятствия
    float x1, x2;
    float y1, y2;
    float z1, z2;
}spike[N1][N2];

struct square{//горизонтальные квадраты
    float x1, x2, x3, x4;
    float y1, y2, y3, y4;
    float z1, z2, z3, z4;

}s[SIZE];

struct Position{//все положения игрока
    float px1, px2;
    float py1, py2;
    float pz1, pz2;
}pos[16];

void timer(){
    display();
    int j = 0, k = 0, x = 0;
    for(j = 0; j<SIZE; j++){
        s[j].z1+=SPEED;
        s[j].z2+=SPEED;
        s[j].z3+=SPEED;
        s[j].z4+=SPEED;
            if(s[j].z1>0){
                s[j].z1=h2;
                s[j].z2=h2;
                s[j].z3=h2;
                s[j].z4=h2;
            }
    }
    srand(time(NULL));
    for(k = 0; k<N1; k++){
    for(j = 0; j<N2; j++){
        spike[k][j].z1+=SPEED;
        spike[k][j].z2+=SPEED;
            if(spike[k][j].z1>0){
                spike[k][j].z1 = h2;
                spike[k][j].z2 = h2;
                x = rand()%N2;
                spike[k][j].x1 = pos[x].px1;
                spike[k][j].x2 = pos[x].px2;
                spike[k][j].y1 = pos[x].py1;
                spike[k][j].y2 = pos[x].py2;
            }
    }
    }

    if(!logic()){
        gameover();
    }
    else{
        glutTimerFunc(15, timer, 0);
    }
}

void field(){//отрисовка поля
	glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2);
    int i = 0;
	glBegin(GL_LINES);
    //------------------ ближний квадрат
    glVertex3f(w, w, h1);
    glVertex3f(-w, w, h1);

    glVertex3f(w, -w, h1);
    glVertex3f(-w, -w, h1);

    glVertex3f(w, w, h1);
    glVertex3f(w, -w, h1);

    glVertex3f(-w, w, h1);
    glVertex3f(-w, -w, h1);
    //---------------------дальний квардрат
    glVertex3f(w, w, h2);
    glVertex3f(-w, w, h2);

    glVertex3f(w, -w, h2);
    glVertex3f(-w, -w, h2);

    glVertex3f(w, w, h2);
    glVertex3f(w, -w, h2);

    glVertex3f(-w, w, h2);
    glVertex3f(-w, -w, h2);
    //--------------------линии туннеля
    for(i = -w; i<=w; i+=w/2){
        glVertex3f(i, w, h1);
        glVertex3f(i, w, h2);
    }

    for(i = -w; i<=w; i+=w/2){
        glVertex3f(w, i, h1);
        glVertex3f(w, i, h2);
    }

    for(i = -w; i<=w; i+=w/2){
        glVertex3f(-i, -w, h1);
        glVertex3f(-i, -w, h2);
    }

    for(i = -w; i<=w; i+=w/2){
        glVertex3f(-w, i, h1);
        glVertex3f(-w, i, h2);
    }
    //------------------------гор квадраты
    int j;
    for(j = 0; j<SIZE; j++){
        glVertex3f(s[j].x1, s[j].y1, s[j].z1);
        glVertex3f(s[j].x2, s[j].y2, s[j].z2);

        glVertex3f(s[j].x2, s[j].y2, s[j].z2);
        glVertex3f(s[j].x3, s[j].y3, s[j].z3);

        glVertex3f(s[j].x3, s[j].y3, s[j].z3);
        glVertex3f(s[j].x4, s[j].y4, s[j].z4);

        glVertex3f(s[j].x4, s[j].y4, s[j].z4);
        glVertex3f(s[j].x1, s[j].y1, s[j].z1);


    }
    glEnd();

}

void player(){//отрисовка игрока
    glColor3f(0.0, 1.0, 0.0);
    glLineWidth(8);
	glBegin(GL_LINES);
	glVertex3f(pos[i].px1, pos[i].py1, pos[i].pz1);
	glVertex3f(pos[i].px2, pos[i].py2, pos[i].pz2);
    glEnd();
}

void spikes(){//отрисовка препятствий
    int k, j;
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(4);
	glBegin(GL_LINES);
	for(k = 0; k<N1; k++){
        for(j = 0; j<N2; j++){
            glVertex3f(spike[k][j].x1, spike[k][j].y1, spike[k][j].z1);
            glVertex3f(spike[k][j].x2, spike[k][j].y2, spike[k][j].z2);
        }
	}
    glEnd();
}

void move(unsigned char key, int x, int y){//управление клавиатурой "A" и "D", на "esc" закрыть программу
	switch(key){
	    case 'a': i--;
                  if(i<0) i = 15;
                 break;
        case 'd': i++;
                  if(i>15) i = 0;
                 break;
        case 27: exit(0);
    }
}

void display(){//вывод на экран
    glClear(GL_COLOR_BUFFER_BIT);
    field();
    player();
    spikes();
    glFlush();
}

void gameover(){//вывод в случае проигрыша
    char str[14] = {'G', 'A', 'M', 'E', ' ', 'O', 'V', 'E', 'R', 'S', 'C','O', 'R', 'E'};
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    int x = -8, y = 0, z = 0;
    int i = 0;
    glRasterPos3f(x, y, z);
    for(i = 0; i < 9; i++){
        x+=2;
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
        glRasterPos3f(x, y, z);
    }
    x = -6;
    y = -5;
    z = 0;
    glRasterPos3f(x, y, z);
    for(i = 9; i < 14; i++){
        x+=2;
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
        glRasterPos3f(x, y, z);
    }
    score -= 4;
    char arr[10];
    sprintf(arr, "%d", score);
    x++;
    glRasterPos3f(x, y, z);
    for(i = 0; arr[i]; i++){
        x+=2;
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, arr[i]);
        glRasterPos3f(x, y, z);
    }
    glFlush();
}

void start(){//инициализация начального положения
    int j = 0, step = 0, k = 0, x = 0;
    for(j=0; j<SIZE; j++){//инициализация горизонтальных квадратов
        s[j].x1 = -w;
        s[j].x2 = w;
        s[j].x3 = w;
        s[j].x4 = -w;
        s[j].y1 = w;
        s[j].y2 = w;
        s[j].y3 = -w;
        s[j].y4 = -w;
        s[j].z1 = h2+step;
        s[j].z2 = h2+step;
        s[j].z3 = h2+step;
        s[j].z4 = h2+step;
        step += 100;
    }
    step = 0;
    for(j=0; j<4; j++){//позиции игрока на нижнем ребре
        pos[j].px1 = -w+step;
        pos[j].px2 = -w/2+step;
        pos[j].py1 = -w;
        pos[j].py2 = -w;
        pos[j].pz1 = 0;
        pos[j].pz2 = 0;
        step += w/2;
    }

    step = 0;
    for(j=4; j<8; j++){//позиции игрока на правом ребре
        pos[j].px1 = w;
        pos[j].px2 = w;
        pos[j].py1 = -w+step;
        pos[j].py2 = -w/2+step;
        pos[j].pz1 = 0;
        pos[j].pz2 = 0;
        step += w/2;
    }

    step = 0;
    for(j=8; j<12; j++){//позиции игрока на верхнем ребре
        pos[j].px1 = w-step;
        pos[j].px2 = w/2-step;
        pos[j].py1 = w;
        pos[j].py2 = w;
        pos[j].pz1 = 0;
        pos[j].pz2 = 0;
        step += w/2;
    }

    step = 0;
    for(j=12; j<16; j++){//позиции игрока на левом ребре
        pos[j].px1 = -w;
        pos[j].px2 = -w;
        pos[j].py1 = w-step;
        pos[j].py2 = w/2-step;
        pos[j].pz1 = 0;
        pos[j].pz2 = 0;
        step += w/2;
    }

    step = 0;
    srand(time(NULL));
    for(k = 0; k<N1; k++){//создание препятствий
        for(j = 0; j<N2; j++){
                x = rand()%N2;
                spike[k][j].x1 = pos[x].px1;
                spike[k][j].x2 = pos[x].px2;
                spike[k][j].y1 = pos[x].py1;
                spike[k][j].y2 = pos[x].py2;
                spike[k][j].z1 = h2+step-400;
                spike[k][j].z2 = h2+step-400;
        }
        step+=100;
    }
}

int logic(){//проверка на столкновения+подсчет очков
    int k, j;
	for(k = 0; k<N1; k++){
        for(j = 0; j<N2; j++){
            if(pos[i].px1==spike[k][j].x1&&pos[i].py1==spike[k][j].y1&&pos[i].pz1==spike[k][j].z1) return 0;
        }
    }
    for(j = 0; j<SIZE; j++){
              if(pos[i].pz1==s[j].z1) score++;
    }
	return 1;
}


int main(int argc, char** argv){
    start();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(WINDOW_W, WINDOW_H);
	glutInitWindowPosition(300, 50);
	glutCreateWindow("GAME_VER1");
	//glutFullScreen();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0, 1);//режим проекции
	glTranslatef(0.0, 0.0, -50);//отдаление камеры
	glutDisplayFunc(display);
	glutKeyboardFunc(move);
	timer();
	glutMainLoop();
	return 0;
}


