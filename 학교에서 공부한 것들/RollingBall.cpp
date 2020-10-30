#include <glut.h>
#include <GL.H>
#include<GLU.H>

GLfloat Rollx = 0, Rollz = 0, Movex = 0, Movez = 0;
GLfloat Green[] = { 0, 1, 0 }, White[] = { 1, 1, 1 };
bool Hit = false;

class Ball
{
	double radius;
	double x, y, z;
	double deltaR;
	double deltaT;
	GLfloat* color;

public :
	Ball(double radius, GLfloat* color, double x, double z)
	{
		this->radius = radius;
		this->x = x;
		this->y = 0;
		this->z = z;
		this->deltaR = 5;
		this->deltaT = 0.1;
		this->color = color;
	}

	double getRadius() { return radius; }
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	double getDeltaT() { return deltaT; }
	
	void rollLeft() { Rollz += deltaR; }
	void rollRight() { Rollz -= deltaR; }
	void rollForward() { Rollx += deltaR; }
	void rollBack() { Rollx -= deltaR; }
	void moveLeft() { Movex += deltaT; }
	void moveRight() { Movex -= deltaT; }
	void moveForward() { Movez += deltaT; }
	void moveBack() { Movez -= deltaT; }

	void update()
	{
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslatef(x + Movex, y, z + Movez);
		glRotatef(Rollx, 1.0, 0, 0);
		glRotatef(Rollz, 0, 0, 1.0);
		glutWireSphere(radius, 30, 30);
		glPopMatrix();
	}
};

class Cube
{
	double size;
	GLfloat* color;
	double x, y, z;
public :
	Cube(double size, GLfloat* color, double x, double y, double z)
	{
		this->size = size;
		this->color = color;
		this->x = x;
		this->z = z;
	}
	double getSize() { return size; }
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	void update()
	{
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslatef(x, y, z);
		glutSolidCube(size);
		glPopMatrix();
	}
};

class Collision
{
	double size;
	double x, y, z;
	double deltaT;
public :
	Collision(double size, double x, double y, double z)
	{
		this->size = size;
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Collision(double size, double x, double y, double z, double deltaT)
	{
		this->size = size;
		this->x = x;
		this->y = y;
		this->z = z;
		this->deltaT = deltaT;
	}

	double getSize() { return size; }
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	void moveLeft() { Movex += deltaT; }
	void moveRight() { Movex -= deltaT; }
	void moveForward() { Movez += deltaT; }
	void moveBack() { Movez -= deltaT; }
	double getMinX() { return x - (size / 2); }
	double getMaxX() { return x + (size / 2); }
	double getMinZ() { return z - (size / 2); }
	double getMaxZ() { return z + (size / 2); }
	double getMinXBall() { return x - (size / 2) + Movex; }
	double getMaxXBall() { return x + (size / 2) + Movex; }
	double getMinZBall() { return z - (size / 2) + Movez; }
	double getMaxZBall() { return z + (size / 2) + Movez; }

	void updateForCube()
	{
		glPushMatrix();
		glTranslatef(x, y, z);
		glutWireCube(size);
		glPopMatrix();
	}
	void updateForBall()
	{
		glPushMatrix();
		glTranslatef(x + Movex, y, z + Movez);
		glutWireCube(size);
		glPopMatrix();
	}
};

Ball one(0.5, Green, 0, 0);
Cube cube(1, White, 2, 0, 0);
Collision collisionCube(cube.getSize(), cube.getX(), cube.getY(), cube.getZ());
Collision collisionBall(2 * one.getRadius(), one.getX(), one.getY(), one.getZ(), one.getDeltaT());

void Collider(Collision cube, Collision ball)
{
	if (((cube.getMaxX() < ball.getMinXBall()) && (cube.getMaxZ() < ball.getMinZBall())))
		Hit = true;
	else if ((cube.getMinX() < ball.getMaxXBall()) && (cube.getMinZ() < ball.getMaxZBall()))
		Hit = true;
	else
		Hit = false;
}

void Init()
{
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, White);
	glLightfv(GL_LIGHT0, GL_SPECULAR, White);
	glMaterialfv(GL_FRONT, GL_SPECULAR, White);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	collisionBall.updateForBall();
	collisionCube.updateForCube();
}

void MyKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w' :
		if (Hit == false)
		{
			one.moveForward();
			one.rollForward();
			collisionBall.moveForward();
			break;
		}
		else
		{
			switch (key)
			{
			case 'w':
				one.rollForward();
				break;
			case 's':
				Hit = false;
				one.moveBack();
				one.rollBack();
				collisionBall.moveBack();
				break;
			case 'd':
				Hit = false;
				one.moveRight();
				one.rollRight();
				collisionBall.moveRight();
				break;
			case 'a':
				Hit = false;
				one.moveLeft();
				one.rollLeft();
				collisionBall.moveLeft();
				break;
			}
		}
	case 's' :
		if (Hit == false)
		{
			one.moveBack();
			one.rollBack();
			collisionBall.moveBack();
			break;
		}
		else 
		{
			switch (key)
			{
			case 'w':
				Hit = false;
				one.moveForward();
				one.rollForward();
				collisionBall.moveForward();
				break;
			case 's':
				one.rollBack();
				break;
			case 'd':
				Hit = false;
				one.moveRight();
				one.rollRight();
				collisionBall.moveRight();
				break;
			case 'a':
				Hit = false;
				one.moveLeft();
				one.rollLeft();
				collisionBall.moveLeft();
				break;
			}
		}
	case 'd' :
		if (Hit == false)
		{
			one.moveRight();
			one.rollRight();
			collisionBall.moveRight();
			break;
		}
		else
		{
			switch (key)
			{
			case 'w':
				Hit = false;
				one.moveForward();
				one.rollForward();
				collisionBall.moveForward();
				break;
			case 's':
				Hit = false;
				one.moveBack();
				one.rollBack();
				collisionBall.moveBack();
				break;
			case 'd':
				one.rollRight();
				break;
			case 'a':
				Hit = false;
				one.moveLeft();
				one.rollLeft();
				collisionBall.moveLeft();
				break;
			}
		}
	case 'a' :
		if (Hit == false)
		{
			one.moveLeft();
			one.rollLeft();
			collisionBall.moveLeft();
			break;
		}
		else
		{
			switch (key)
			{
			case 'w':
				Hit = false;
				one.moveForward();
				one.rollForward();
				collisionBall.moveForward();
				break;
			case 's':
				Hit = false;
				one.moveBack();
				one.rollBack();
				collisionBall.moveBack();
				break;
			case 'd':
				Hit = false;
				one.moveRight();
				one.rollRight();
				collisionBall.moveRight();
				break;
			case 'a':
				one.rollLeft();
				break;
			}
		}
	}
	glutPostRedisplay();
}

void MyDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(10, 3, 10, 0, 0, 0, 0, 1, 0);
	Collider(collisionCube, collisionBall);
	one.update();
	cube.update();
	glFlush();
	glutSwapBuffers();
}

void MyReshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Rolling Ball");
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutReshapeFunc(MyReshape);
	Init();
	glutMainLoop();
	return 0;
}