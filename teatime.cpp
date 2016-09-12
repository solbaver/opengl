#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>


int number_of_particles = 1000;
bool teapot = true; 

//variables for teapots
GLuint teapot_object;
float teapot_rotation;

//file for loading
char object_name[] = "teapot1.obj";

//variables for sliders
int a = -3.5;
int b = 1;
char ch='1';

//global variables for timer
clock_t t0;
clock_t t1;

//variable for teapots number
int tea_num = 1;
int tea_polygons_number = 1;

//classes that describe splinters and particles
class particle_info
{ public:
  float   position[3];
  float   speed[3];
  float   color[3];
};



//obj load function
void load_obj(char *fname)
{
    FILE *fp;
    int read;
    GLfloat x, y, z;
    char ch;
    teapot=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(teapot, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_POINTS);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    fclose(fp);
}



//fuction for printing to opengl screen
void
print_to_screen(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, string[i]);
	}
}

//maximum of particles 
particle_info particles[8000];


     

void
tea_speed (float speed[3])
{
	speed[0] = (4.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 2.0;
	speed[1] = (4.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 2.0;
	speed[2] = (4.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
}



void
blow_up_tea (int a, int b, int c)
{
  int i;

  for (i = 0; i < number_of_particles; i++)
    {
      particles[i].position[0] = a;
      particles[i].position[1] = b;
      particles[i].position[2] = c;

      particles[i].color[0] = 1.0;
      particles[i].color[1] = 0.0;
      particles[i].color[2] = 0.0;

      tea_speed (particles[i].speed);
    }

}




void
draw_me_sliders(void)
{
	//two sliders - 1 for teapots number, second for number of polygons
	//start drawing in a point suitable for the slider
	glTranslatef (a, -3.5, -10.0);
	//yes slider just a cube
	glutSolidCube (0.5);
	glTranslatef (-a-3.5, b-0.5, 0.0);
	
	
	glutSolidCube (0.5);
	//go back to previos point
	glTranslatef (3.5, -b+4, 0.0);	
}



void draw_me_custom_teapots()
{	
	int i = 1;
	int step = 6;
    glPushMatrix();
    //glTranslatef(0,-40.00,-105);
    glTranslatef(0,0,-10);
    glColor3f(1.0,0.23,0.27);
    glScalef(0.6,0.6,0.6);

    glRotatef(teapot_rotation,0,1,0);
    glCallList(teapot);
    while (i<tea_num)
    {	
    	//each teapot has his own rotation
		glRotatef(teapot_rotation,0,-1,0);
		
		//parity check for symmetry of teapot
		if (i % 2 == 0 )
			{
			glTranslatef(-step*i,0,0);
			}
		else
		{
			glTranslatef(step*i,0,0);
		}
		glRotatef(teapot_rotation,0,1,0);
    	glCallList(teapot);
    	i = i + 1;
    }
    
    glPopMatrix();
    teapot_rotation=teapot_rotation+0.5;
}



//test finction for opengl teapots
void
draw_me_standart_teapots(int tea_num)
{
	int i = 0;
	
	while (i<tea_num)
		{
 			glTranslatef ((-1+(i*1.5)), 0.0, 0.0);
			glutSolidTeapot (0.5);
			glTranslatef ((1-(i*1.5)), 0.0, 0.0);
			i = i + 1;

		}
	i = 0;
}

//function for drawing of explosion
void
draw_me_expl(void)
{	int    i;
	glPushMatrix ();
		glBegin (GL_POINTS);

			for (i = 0; i < number_of_particles; i++)
			{
				glColor3fv (particles[i].color);
				glVertex3fv (particles[i].position);
			}
		glEnd();
	glPopMatrix ();

	glEnable (GL_LIGHTING); 
	glEnable (GL_LIGHT0); 
	glEnable (GL_DEPTH_TEST);

}






void
keyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ':
		if (teapot == true)
			{
			blow_up_tea (0,0, -10);
			t0 = clock();
			teapot = false;
			}
		break;

    case 27:
      exit (0);
      break;
      
	case 'a':
		if (a>-2.5)
			{
			a = a - 1;
			tea_num = tea_num - 1;
			number_of_particles = number_of_particles - 2000;
     		}
      break;
      
 	case 'd':
		if (a<-1.5)
			{
			a = a + 1;
			number_of_particles = number_of_particles + 2000;
			tea_num = tea_num + 1;
			}
		break;
      
	case 'w':
		if (b<3)
			{
			tea_polygons_number = tea_polygons_number + 1;
			b = b + 1;
			object_name[11] = tea_polygons_number + '0';
			load_obj(object_name);
			}
 		break;
      
	case 's':
		if (b>1)
			{
			tea_polygons_number = tea_polygons_number - 1;
			b = b - 1;
			object_name[11] = tea_polygons_number + '0';
			load_obj(object_name);
			}
		break;
           
	case 'r':   
		t1 = clock();   
		if ((t0 - t1)<-100000)
			{
			teapot = true;
			}
		break;      
      
	}
}




void
idle (void)
{
	int i;
	clock_t t1 = clock();
	if ((t0 - t1)>-100000)
	{
		for (i = 0; i < number_of_particles; i++)
		{
			particles[i].position[0] += particles[i].speed[0] * 0.2;
			particles[i].position[1] += particles[i].speed[1] * 0.2;
			particles[i].position[2] += particles[i].speed[2] * 0.2;	      
			particles[i].color[0] -= 1.0 / 500.0;
			particles[i].color[1] -= 1.0 / 100.0;
			particles[i].color[2] -= 1.0 / 50.0;

	    }
	  

	}
      
	glutPostRedisplay ();
	
}



void 
initialization(void)
{
	GLfloat  light0Amb[4] =  { 1.0, 0.5, 0.1, 1.0 };
	GLfloat  light0Dif[4] =  { 1.0, 0.5, 0.1, 1.0 };   
	GLfloat  light0Spec[4] = { 0.0, 0.0, 0.0, 1.0 };   
	GLfloat  light0Pos[4] =  { 5.0, 5.0, 5.0, 10.0 };

	GLfloat  light1Amb[4] =  { 0.0, 0.0, 0.0, 1.0 };
	GLfloat  light1Dif[4] =  { 1.0, 1.0, 1.0, 1.0 };   
	GLfloat  light1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };   
	GLfloat  light1Pos[4] =  { 200.0, -100.0, -100.0, 0.0 };

	GLfloat  materialAmb[4] = { 0.25, 0.25, 0.25, 1.0 };
	GLfloat  materialDif[4] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat  materialSpec[4] = { 1.0, 1.0, 1.0, 1.0 };


	glEnable (GL_LIGHT0);
	glEnable (GL_LIGHT1);
	glLightfv (GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light0Dif);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light0Spec);
	glLightfv (GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv (GL_LIGHT1, GL_AMBIENT, light1Amb);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, light1Dif);
	glLightfv (GL_LIGHT1, GL_SPECULAR, light1Spec);
	glLightfv (GL_LIGHT1, GL_POSITION, light1Pos);
	glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb);
	glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, 10);
	glEnable (GL_NORMALIZE);	

}


void reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (45, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    //glOrtho(-25,25,-2,2,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}




void display(void)
{
	int    i;
	clock_t t1 = clock();
	glClearColor (255.0, 255.0, 255.0, 1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
	draw_me_sliders();
	//check that the teapot is not blown up now
	if ( teapot == true)
    {
    	draw_me_custom_teapots();
    }
    //a time limit for explosion
    if ((t0 - t1)>-100000)
    {
		draw_me_expl();
    }
    glutSwapBuffers(); //swap the buffers
}
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(450,450);
    glutInitWindowPosition(20,20);
    glutCreateWindow("princess celestia hates tea");
	glutKeyboardFunc (keyboard);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
	initialization();
    load_obj(object_name);
    glutMainLoop();
    return 0;
}
