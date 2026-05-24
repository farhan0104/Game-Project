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

        float theta = i * PI / 180;

        glVertex2f(
            x + r*cos(theta),
            y + r*sin(theta)
        );
    }

    glEnd();
}

/* ------------ CHICKEN ------------ */
void drawChicken(float x,float y){

    // Body
    glColor3f(1,1,1);
    circle(x,y,25);

    // Head
    circle(x+20,y+18,12);

    // Eye
    glColor3f(0,0,0);
    circle(x+24,y+20,2);

    // Beak
    glColor3f(1,0.5,0);

    glBegin(GL_TRIANGLES);

    glVertex2f(x+32,y+18);
    glVertex2f(x+42,y+22);
    glVertex2f(x+32,y+14);

    glEnd();

    // Comb
    glColor3f(1,0,0);

    circle(x+15,y+32,4);
    circle(x+22,y+35,4);
    circle(x+29,y+32,4);

    // Wing
    glColor3f(0.9,0.9,0.9);
    circle(x-5,y,10);

    // Legs
    glColor3f(1,0.7,0);

    glBegin(GL_LINES);

    glVertex2f(x-8,y-25);
    glVertex2f(x-8,y-38);

    glVertex2f(x+8,y-25);
    glVertex2f(x+8,y-38);

    glEnd();
}

/* ------------ EGG ------------ */
void drawEgg(float x,float y){

    glColor3f(1,1,1);

    glBegin(GL_POLYGON);

    for(int i=0;i<360;i++){

        float theta = i * PI / 180;

        float ex = 10*cos(theta);
        float ey = 14*sin(theta);

        if(ey>0)
            ey *= 1.3;

        glVertex2f(x+ex,y+ey);
    }

    glEnd();

    // Highlight
    glColor3f(0.9,0.9,0.9);
    circle(x-3,y+5,2);
}

/* ------------ NEW OBJECT ------------ */
void newObject(){

    obj_x = rand()%520 + 30;

    obj_y = 550;
}

/* ------------ COLLISION ------------ */
void collision(){

    if(obj_y<=50){

        if(obj_x>=basket_x &&
           obj_x<=basket_x+basket_w)
        {
            score++;
        }

        newObject();
    }
}

/* ------------ DISPLAY ------------ */
void display(){

    glClear(GL_COLOR_BUFFER_BIT);

    /* ------------ MENU ------------ */
    if(page==0){

        glColor3f(1,1,0);

        drawText(220,420,"CATCH THE EGGS");

        drawText(220,360,"1. Start Game");
        drawText(220,330,"2. Resume Game");
        drawText(220,300,"3. Show High Score");
        drawText(220,270,"4. Exit");
    }

    /* ------------ GAME ------------ */
    else if(page==1){

        if(!pauseGame){

            obj_y -= speed;

            collision();
        }

        // Chicken
        drawChicken(obj_x,600);

        // Egg
        drawEgg(obj_x,obj_y);

        // Basket
        glColor3f(1,0,0);

        glBegin(GL_QUADS);

        glVertex2f(basket_x,40);
        glVertex2f(basket_x+basket_w,40);
        glVertex2f(basket_x+basket_w-10,10);
        glVertex2f(basket_x+10,10);

        glEnd();

        // Score & Time
        char s[30], t[30];

        sprintf(s,"Score: %d",score);
        sprintf(t,"Time: %d",timeLeft);

        glColor3f(1,1,1);

        drawText(20,620,s);
        drawText(500,620,t);

        // Controls
        drawText(20,590,"A = Left");
        drawText(20,565,"D = Right");
        drawText(20,540,"P = Pause");

        if(pauseGame)
            drawText(220,350,"GAME PAUSED");
    }

    /* ------------ GAME OVER ------------ */
    else if(page==2){

        glColor3f(1,0,0);

        drawText(230,430,"GAME OVER");

        char s[50];
        char h[50];

        sprintf(s,"Final Score: %d",score);
        sprintf(h,"High Score: %d",highScore);

        glColor3f(1,1,1);

        drawText(220,360,s);
        drawText(220,320,h);

        drawText(180,260,"Press M for Menu");
        drawText(180,230,"Press Q to Exit");
    }

    glutSwapBuffers();
}

/* ------------ TIMER ------------ */
void timer(int v){

    if(page==1 && !pauseGame){

        timeLeft--;

        if(timeLeft<=0){

            if(score>highScore)
                highScore = score;

            page = 2;

            glutPostRedisplay();
        }
    }

    glutTimerFunc(1000,timer,0);
}

/* ------------ UPDATE ------------ */
void update(int v){

    glutPostRedisplay();

    glutTimerFunc(16,update,0);
}

/* ------------ KEYBOARD ------------ */
void keyboard(unsigned char key,int x,int y){

    // Convert uppercase to lowercase
    if(key>='A' && key<='Z')
        key = key + 32;

    /* ------------ MENU ------------ */
    if(page==0){

        if(key=='1'){

            score = 0;
            timeLeft = 60;
            pauseGame = 0;

            newObject();

            page = 1;
        }

        else if(key=='2'){

            page = 1;
        }

        else if(key=='3'){

            printf("High Score: %d\n",highScore);
        }

        else if(key=='4'){

            exit(0);
        }
    }

    /* ------------ GAME ------------ */
    else if(page==1){

        if(key=='a')
            basket_x -= 20;

        else if(key=='d')
            basket_x += 20;

        else if(key=='p')
            pauseGame = !pauseGame;

        else if(key=='q')
            exit(0);

        // Basket limit
        if(basket_x < 0)
            basket_x = 0;

        if(basket_x > 540)
            basket_x = 540;
    }

    /* ------------ GAME OVER ------------ */
    else if(page==2){

        if(key=='m'){

            page = 0;
        }

        else if(key=='q'){

            exit(0);
        }
    }

    glutPostRedisplay();
}

/* ------------ MAIN ------------ */
int main(int argc,char** argv){

    srand(time(0));

    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(width,height);

    glutCreateWindow("Catch The Eggs");

    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

    init();

    newObject();

    glutDisplayFunc(display);

    glutKeyboardFunc(keyboard);

    glutTimerFunc(1000,timer,0);

    glutTimerFunc(16,update,0);

    glutMainLoop();

    return 0;
}
