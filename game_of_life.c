#include <Gl/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int w = 800;
int h = 400;
int cell_size = 4;
int cols;
int rows;


int **arr;
int** next;
void timer();
int **make_array(int cols, int rows);
void display();
void setup();
int look_around(int **temp, int y, int x);
void control(unsigned char key, int x, int y);

int **make_array(int cols, int rows){
    int **temp = malloc(rows*sizeof(int*));
    for(int i = 0; i < rows; i++)
        temp[i] = malloc(cols*sizeof(int));
    return temp;
}

int look_around(int **temp, int y, int x){
    int sum = 0;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            int row = (y+i+rows)%rows;
            int col = (x+j+cols)%cols;
            sum += temp[row][col];
        }
    }

    sum -= temp[y][x];

    return sum;
}

void display(){

    int x = 0, y = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(arr[i][j] == 1) glColor3f(0.0, 0.5, 0.2);
            else glColor3f(0.0, 0.0, 0.0);
            glRectf(x, y, x+cell_size-1, y+cell_size-1);
            x += cell_size;
        }
        x = 0;
        y += cell_size;
    }
    glutSwapBuffers();
    glFlush();
}


void setup(){
    cols = w/cell_size;
    rows = h/cell_size;

    srand(time(NULL));

    arr = make_array(cols, rows);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            arr[i][j] = rand()%2;
        }
    }

}

void control(unsigned char key, int x, int y){
    switch (key){
        case 27: exit(0);
                break;
        case '1': glutFullScreen();
                break;
    }
}

void timer(){

    next = make_array(cols, rows);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            int life = arr[i][j];

                int near_cells = look_around(arr, i, j);


                if(life == 1 && (near_cells < 2 || near_cells > 3)){
                    next[i][j] = 0;
                }
                else if(life == 0 && near_cells == 3){
                    next[i][j] = 1;
                }
                else next[i][j] = life;
        }
    }

    for(int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);

    arr = next;
    display();

    glutTimerFunc(50, timer, 0);

}

int main(int argc, char** argv){

    setup();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(w-w/2, h-h/2);
	glutCreateWindow("Game of life");
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);

    //glutFullScreen();

	glutDisplayFunc(display);

	timer(0);
	glutKeyboardFunc(control);
	glutMainLoop();

    for(int i = 0; i < rows; i++)
        free(arr[i]);
    free(arr);

	return 0;
}

