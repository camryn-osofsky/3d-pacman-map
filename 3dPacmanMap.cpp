
#include <GL/freeglut.h>

// Material properties
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

// Globals
GLuint wallList;
const int mapHeight = 22;
const int mapWidth = 19;
// 0 = empty, 5 = wall, 1 = coin, 2 = powerup, 3 = ghost, 4 = pacman
int map[mapHeight][mapWidth] = {
	{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
	{5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5},
	{5, 2, 5, 5, 1, 5, 5, 5, 1, 5, 1, 5, 5, 5, 1, 5, 5, 2, 5},
	{5, 1, 5, 5, 1, 5, 5, 5, 1, 5, 1, 5, 5, 5, 1, 5, 5, 1, 5},
	{5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 5},
	{5, 1, 5, 5, 1, 5, 1, 5, 5, 5, 5, 5, 1, 5, 1, 5, 5, 1, 5},
	{5, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 5},
	{5, 5, 5, 5, 1, 5, 5, 5, 0, 5, 0, 5, 5, 5, 1, 5, 5, 5, 5},
	{0, 0, 0, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 5, 1, 5, 0, 0, 0},
	{5, 5, 5, 5, 1, 5, 0, 5, 5, 3, 5, 5, 0, 5, 1, 5, 5, 5, 5},
	{0, 0, 0, 0, 1, 0, 0, 5, 3, 3, 3, 5, 0, 0, 1, 0, 0, 0, 0},
	{5, 5, 5, 5, 1, 5, 0, 5, 5, 5, 5, 5, 0, 5, 1, 5, 5, 5, 5},
	{0, 0, 0, 5, 1, 5, 0, 0, 0, 0, 0, 0, 0, 5, 1, 5, 0, 0, 0},
	{5, 5, 5, 5, 1, 5, 0, 5, 5, 5, 5, 5, 0, 5, 1, 5, 5, 5, 5},
	{5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5},
	{5, 1, 5, 5, 1, 5, 5, 5, 1, 5, 1, 5, 5, 5, 1, 5, 5, 1, 5},
	{5, 2, 1, 5, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 5, 1, 2, 5},
	{5, 5, 1, 5, 1, 5, 1, 5, 5, 5, 5, 5, 1, 5, 1, 5, 1, 5, 5},
	{5, 1, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 5, 1, 1, 1, 1, 5},
	{5, 1, 5, 5, 5, 5, 5, 5, 1, 5, 1, 5, 5, 5, 5, 5, 5, 1, 5},
	{5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5},
	{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}
};

// Initialize OpenGL
void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

// pacman class
class ECE_Pacman {
public:
	void drawPacman(float x, float y, float z) {
		glPushMatrix();
		glColor3f(1.0, 1.0, 0.0); // yellow
		glTranslatef(x - 9, y, z - 11);
		glutSolidSphere(0.5, 20.0, 20.0);
		glPopMatrix();
	}
};

// ghost class
class ECE_Ghost {
public:
	float r, g, b;
	ECE_Ghost(float red, float green, float blue) : r(red), g(green), b(blue) {}

	void drawGhost(float x, float y, float z) {
		// head
		glPushMatrix();
		glColor3f(r, g, b);
		glTranslatef(x - 9, y, z - 11);
		glutSolidSphere(0.5, 20, 20);
		glPopMatrix();

		// body
		glPushMatrix();
		glTranslatef(x - 9, y, z - 11);
		glRotatef(90.0, 1.0, 0, 0);
		GLUquadric* quad = gluNewQuadric();
		gluCylinder(quad, 0.5, 0.5, 1.0, 20.0, 20.0);
		gluDeleteQuadric(quad);
		glPopMatrix();
	}
};


void drawCoin(float x, float y, float z) {
	glPushMatrix();
	glColor3f(0.75, 0.75, 0.75); // silver
	glTranslatef(x - 9, y, z - 11);
	glutSolidSphere(0.1, 20.0, 20.0);
	glPopMatrix();
}

void drawWalls() {
	GLUquadric* quad = gluNewQuadric();
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (map[y][x] == 5) {
				if ((x < mapWidth - 1) && map[y][x + 1] == 5) {
					// draw horizontal wall
					glPushMatrix();
					glColor3f(0.0, 0.0, 1.0);
					glTranslatef(x - 9, 0, y - 11);
					glRotatef(90, 0.0, 1.0, 0.0);
					gluCylinder(quad, 0.05, 0.05, 1.0, 25, 25);
					glPopMatrix();
				}
				if ((y < mapHeight - 1) && map[y + 1][x] == 5 && !((y == 2 && x == 6) || (y == 2 && x == 12))) {
					// draw vertical wall
					glPushMatrix();
					glColor3f(0.0, 0.0, 1.0);
					glTranslatef(x - 9, 0, y - 11);
					gluCylinder(quad, 0.05, 0.05, 1.0, 25, 25);
					glPopMatrix();
				}
			}
			else if (map[y][x] == 2) {
				// draw powerup
				glPushMatrix();
				glColor3f(1.0, 0.84, 0.34);
				glTranslatef(x - 9, 0, y - 10.8);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				gluDisk(quad, 0.0, 0.35, 50, 50);
				glPopMatrix();
			}
		}
	}
	gluDeleteQuadric(quad);
}

// Display callback
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ECE_Pacman pacman;
	ECE_Ghost blinky(1.0f, 0.0f, 0.0f); // red
	ECE_Ghost pinky(1.0f, 0.75f, 0.8f); // pink
	ECE_Ghost inky(0.0f, 0.5f, 0.5f); // orange
	ECE_Ghost clyde(1.0f, 0.5f, 0.0f); // teal

	pacman.drawPacman(9, 0, 16);

	blinky.drawGhost(9, 0, 9);
	pinky.drawGhost(9, 0, 10);
	inky.drawGhost(8, 0, 10);
	clyde.drawGhost(10, 0, 10);

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (map[y][x] == 1) {
				drawCoin(x, 0, y);
			}
		}
	}

	drawWalls();

	glutSwapBuffers();
}

// rotate the maze by 5 degrees when "R" is pressed
void rotateMaze(unsigned char key, int x, int y) {
	if (key == 'R' || key == 'r') {
		glRotatef(5, 0, 1, 0);
		glutPostRedisplay();
	}
}

// Reshape callback
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.01, 200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(15.0, 20.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// Main function
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("3D Pacman Maze");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(rotateMaze);
	glutMainLoop();
	return 0;
}
