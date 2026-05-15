#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<GL/glut.h>

#define PI 3.1416

/* ------------ WINDOW ------------ */
int width = 600, height = 650;

/* ------------ GAME STATE ------------ */
int page = 0;
// 0 = menu
// 1 = game
// 2 = game over

int pauseGame = 0;

/* ------------ GAME DATA ------------ */
int basket_x = 250;
int basket_w = 60;

float obj_x, obj_y;
float speed = 2;

int score = 0;
int highScore = 0;
int timeLeft = 60;

/* ------------ INIT ------------ */
void init(){
    glClearColor(0,0,0,1);
    gluOrtho2D(0,width,0,height);
}

/* ------------ TEXT ------------ */
void drawText(float x,float y,const char *str){
    glRasterPos2f(x,y);
    for(int i=0;str[i]!='\0';i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}

/* ------------ CIRCLE ------------ */
void circle(float x,float y,float r){
    glBegin(GL_POLYGON);
    for(int i=0;i<360;i++){
        float theta=i*PI/180;
        glVertex2f(x+r*cos(theta), y+r*sin(theta));
    }
    glEnd();
}

/* ------------ NEW OBJECT ------------ */
void newObject(){
    obj_x = rand()%550;
    obj_y = 380;
}

/* ------------ COLLISION ------------ */
void collision(){
    if(obj_y<=50){
        if(obj_x>=basket_x && obj_x<=basket_x+basket_w)
            score++;
        newObject();
    }
}

/* ------------ DISPLAY ------------ */
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    if(page==0){  // MENU
        glColor3f(1,1,0);
        drawText(220,400,"CATCH THE EGGS");
        drawText(220,350,"1. Start Game");
        drawText(220,320,"2. Resume Game");
        drawText(220,290,"3. High Score");
        drawText(220,260,"4. Exit");
    }

    else if(page==1){ // GAME

        if(!pauseGame){
            obj_y -= speed;
            collision();
        }

        glColor3f(1,1,1);
        circle(obj_x,obj_y,10);

        glColor3f(1,0,0);
        glBegin(GL_QUADS);
        glVertex2f(basket_x,40);
        glVertex2f(basket_x+basket_w,40);
        glVertex2f(basket_x+basket_w-10,10);
        glVertex2f(basket_x+10,10);
        glEnd();

        char s[30], t[30];
        sprintf(s,"Score: %d",score);
        sprintf(t,"Time: %d",timeLeft);

        glColor3f(1,1,1);
        drawText(20,620,s);
        drawText(500,620,t);

        if(pauseGame)
            drawText(260,350,"PAUSED (Press P)");
    }

    else if(page==2){ // GAME OVER
        glColor3f(1,0,0);
        drawText(250,400,"GAME OVER");

        char s[40];
        sprintf(s,"Final Score: %d",score);
        drawText(230,350,s);

        drawText(200,300,"Press M for Menu");
    }

    glutSwapBuffers();
}

/* ------------ TIMER ------------ */
void timer(int v){
    if(page==1 && !pauseGame){
        timeLeft--;
        if(timeLeft<=0){
            if(score>highScore)
                highScore=score;
            page=2;
        }
    }
    glutTimerFunc(1000,timer,0);
}

/* ------------ UPDATE ------------ */
void update(int v){
    if(page==1 && !pauseGame)
        glutPostRedisplay();
    glutTimerFunc(16,update,0);
}

/* ------------ KEYBOARD ------------ */
void keyboard(unsigned char key,int x,int y){

    if(page==0){ // MENU
        if(key=='1'){
            score=0;
            timeLeft=60;
            newObject();
            page=1;
        }
        if(key=='2') page=1;
        if(key=='3'){
            char hs[40];
            sprintf(hs,"High Score: %d",highScore);
            glClear(GL_COLOR_BUFFER_BIT);
            drawText(230,350,hs);
            drawText(200,300,"Press M for Menu");
            glutSwapBuffers();
        }
        if(key=='4') exit(0);
    }

    else if(page==1){ // GAME
        if(key=='a') basket_x-=20;
        if(key=='d') basket_x+=20;
        if(key=='p') pauseGame=!pauseGame;
        if(key=='q') exit(0);

        if(basket_x<0) basket_x=0;
        if(basket_x>540) basket_x=540;
    }

    else if(page==2){ // GAME OVER
        if(key=='m' || key=='M')
            page=0;
    }
}

/* ------------ MAIN ------------ */
int main(int argc,char** argv){
    srand(time(0));

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width,height);
    glutCreateWindow("Catch The Eggs");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(1000,timer,0);
    glutTimerFunc(16,update,0);

    glutMainLoop();
}
