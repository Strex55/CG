#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut.h>
#include <cmath>

float rotateX = 60.0f;
float rotateZ = -35.0f;

bool isMousePressed = false;
int lastMouseX = 0;
int lastMouseY = 0;

const float A = 1.4f;
const float B = 1.4f;
const float RANGE = 2.2f;
const int GRID_SIZE = 80;

float Surface(float x, float y)
{
    return (x * x) / (A * A) - (y * y) / (B * B);
}

void SetNormal(float x, float y)
{
    float dzdx = 2.0f * x / (A * A);
    float dzdy = -2.0f * y / (B * B);

    float nx = -dzdx;
    float ny = -dzdy;
    float nz = 1.0f;

    float length = std::sqrt(nx * nx + ny * ny + nz * nz);

    glNormal3f(nx / length, ny / length, nz / length);
}

void SetColorByHeight(float z)
{
    float t = (z + 2.5f) / 5.0f;

    if (t < 0) t = 0;
    if (t > 1) t = 1;

    glColor3f(0.2f + 0.7f * t, 0.4f + 0.4f * t, 1.0f - 0.6f * t);
}

void DrawSurface()
{
    float step = (2.0f * RANGE) / GRID_SIZE;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        float x1 = -RANGE + i * step;
        float x2 = x1 + step;

        glBegin(GL_TRIANGLE_STRIP);

        for (int j = 0; j <= GRID_SIZE; ++j)
        {
            float y = -RANGE + j * step;

            float z1 = Surface(x1, y);
            SetColorByHeight(z1);
            SetNormal(x1, y);
            glVertex3f(x1, y, z1);

            float z2 = Surface(x2, y);
            SetColorByHeight(z2);
            SetNormal(x2, y);
            glVertex3f(x2, y, z2);
        }

        glEnd();
    }
}

void DrawSurfaceGrid()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glColor3f(0, 0, 0);
    glLineWidth(1);

    float step = (2.0f * RANGE) / GRID_SIZE;

    const float offsets[2] = { 0.01f, -0.01f };

    for (int k = 0; k < 2; ++k)
    {
        float offset = offsets[k];

        // линии вдоль Y
        for (int i = 0; i <= GRID_SIZE; i += 4)
        {
            float x = -RANGE + i * step;

            glBegin(GL_LINE_STRIP);
            for (int j = 0; j <= GRID_SIZE; ++j)
            {
                float y = -RANGE + j * step;
                float z = Surface(x, y);

                glVertex3f(x, y, z + offset);
            }
            glEnd();
        }

        // линии вдоль X
        for (int j = 0; j <= GRID_SIZE; j += 4)
        {
            float y = -RANGE + j * step;

            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= GRID_SIZE; ++i)
            {
                float x = -RANGE + i * step;
                float z = Surface(x, y);

                glVertex3f(x, y, z + offset);
            }
            glEnd();
        }
    }

    glEnable(GL_LIGHTING);
}

void DrawAxes()
{
    glDisable(GL_LIGHTING);

    glLineWidth(2);

    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(-3, 0, 0);
    glVertex3f(3, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, -3, 0);
    glVertex3f(0, 3, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -3);
    glVertex3f(0, 0, 3);

    glEnd();

    glEnable(GL_LIGHTING);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(
        6, 6, 4,
        0, 0, 0,
        0, 0, 1
    );

    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateZ, 0, 0, 1);

    DrawAxes();
    DrawSurface();
    DrawSurfaceGrid();

    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    if (height == 0)
    {
        height = 1;
    }

    glViewport(0, 0, width, height);

    double aspect = static_cast<double>(width) / height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, aspect, 0.5, 50);

    glMatrixMode(GL_MODELVIEW);
}

void MouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        isMousePressed = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
}

void MouseMove(int x, int y)
{
    if (isMousePressed)
    {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        rotateZ += dx * 0.5f;
        rotateX -= dy * 0.5f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

void SpecialKeys(int key, int, int)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotateZ += 5;
        break;
    case GLUT_KEY_RIGHT:
        rotateZ -= 5;
        break;
    case GLUT_KEY_UP:
        rotateX -= 5;
        break;
    case GLUT_KEY_DOWN:
        rotateX += 5;
        break;
    }

    glutPostRedisplay();
}

void Keyboard(unsigned char key, int, int)
{
    if (key == 27)
    {
        exit(0);
    }
}

void Init()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat lightPosition[] = { 4.0f, 4.0f, 6.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glShadeModel(GL_SMOOTH);
    glClearColor(1, 1, 1, 1);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(900, 700);
    glutCreateWindow("Lab 4.2 - Hyperbolic Paraboloid");

    Init();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();

    return 0;
}