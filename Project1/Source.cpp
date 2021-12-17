
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include"glm.h"

GLMmodel* m ;
GLfloat attenuation[3]={1,.01,.01};
//light w =1 為點光源
GLfloat lightP[4]={0,0,1,1};
GLfloat att[3]={0};

int ang=0;
int anim_index=1;


GLMmodel* myReadOBJ(char* objname){
	GLMmodel* obj =glmReadOBJ(objname);
	if(!obj)exit(0);
	//glmUnitize(obj);
	glmFacetNormals(obj);
	glmVertexNormals(obj, 90.0);
	return obj;
}

void init(){
	glClearColor(0.0,0.0,0.0,1.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_RESCALE_NORMAL);
	

	glLightfv(GL_LIGHT0,GL_CONSTANT_ATTENUATION,&attenuation[0]);
	glLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION,&attenuation[1]);
	glLightfv(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,&attenuation[2]);
	glLightfv(GL_LIGHT0,GL_POSITION,lightP);

	glGetLightfv(GL_LIGHT0,GL_CONSTANT_ATTENUATION,&att[0]);
	glGetLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION,&att[1]);
	glGetLightfv(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,&att[2]);
}

void printModelInfo(GLMmodel* m){
	printf("nummaterials:%d\n",m->nummaterials);
	printf("groups:%d\n",m->numgroups);
	printf("%f %f %f\n",att[0],att[1],att[2]);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(	0,3,5,
				0,0,0,
				0,1,0);
	
	//====================================
	
	//char filename[64]="anim/anim_cube_000001.obj";
	char filename[64]={0};
	sprintf(filename,"anim/anim_cube_%06d.obj",anim_index);
	m = myReadOBJ(filename);

	//框線
	glViewport(-150,0,800,400);
	glPolygonMode(GL_FRONT,GL_LINE);
	glPushMatrix();
	{
		glRotatef(ang,0,1,0);
		glmDraw(m,GLM_SMOOTH|GLM_MATERIAL);
	}
	glPopMatrix();

	//實體
	glViewport(350,0,800,400);
	glPolygonMode(GL_FRONT,GL_FILL);
	glPushMatrix();
	{
		glRotatef(ang,0,1,0);
		glmDraw(m,GLM_SMOOTH|GLM_MATERIAL);
	}
	glPopMatrix();

	glmDelete(m);

	//====================================

	glutSwapBuffers();
}

void reshape(int w,int h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)w/(GLfloat)h,1,50);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key,int x,int y){
	if(key==27){
		exit(0);
	}
	else if(key=='a'){
		//旋轉Y軸
		ang+=20;
		if(ang>=360) ang-=360;
		printModelInfo(m);
	}
	else if(key=='r'){
		//重新播放動畫
		anim_index=1;
	}
	glutPostRedisplay();
}
void timer(int id){
	if(id==0){
		//59為動畫
		if(anim_index<59){
			anim_index++;
			glutPostRedisplay();
		}
	}
	glutTimerFunc(100,timer,0);
}

int main(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowPosition(300,300);
	glutInitWindowSize(1000,500);
	glutCreateWindow("TITLE");

	init();

	GLenum err = glewInit();
	if( err != GLEW_OK){
		printf("GLEW_ERROR\n");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(100,timer,0);
	glutMainLoop();
	return 0;
}
