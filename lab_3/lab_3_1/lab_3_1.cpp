#include <GL/freeglut.h>
#include <cmath>

const float X_MIN = -10.0f;
const float X_MAX = 10.0f;
const float Y_MIN = -1.0f;
const float Y_MAX = 1.5f;

const float EPS = 0.0001f;
const float GRAPH_STEP = 0.01f;
const float TICK_STEP_X = 1.0f;
const float TICK_STEP_Y = 0.2f;

float Function(float x)
{
    if (x > -EPS && x < EPS)
    {
        return 1.0f;
    }

    return sinf(x) / x;
}

void DrawAxes()
{
    glColor3f(0.f, 0.f, 0.f);
    glLineWidth(2.0f);

    glBegin(GL_LINES);

    // Ось X
    glVertex2f(X_MIN, 0.0f);
    glVertex2f(X_MAX, 0.0f);

    // Ось Y
    glVertex2f(0.0f, Y_MIN);
    glVertex2f(0.0f, Y_MAX);

    // Стрелка X
    glVertex2f(X_MAX, 0.0f);
    glVertex2f(X_MAX - 0.3f, 0.05f);

    glVertex2f(X_MAX, 0.0f);
    glVertex2f(X_MAX - 0.3f, -0.05f);

    // Стрелка Y
    glVertex2f(0.0f, Y_MAX);
    glVertex2f(-0.2f, Y_MAX - 0.1f);

    glVertex2f(0.0f, Y_MAX);
    glVertex2f(0.2f, Y_MAX - 0.1f);

    // Деления по X
    for (float x = X_MIN + 1.0f; x <= X_MAX - 1.0f; x += TICK_STEP_X)
    {
        if (x > -EPS && x < EPS)
        {
            continue;
        }

        glVertex2f(x, -0.03f);
        glVertex2f(x, 0.03f);
    }

    // Деления по Y
    for (float y = Y_MIN + TICK_STEP_Y; y <= Y_MAX - TICK_STEP_Y; y += TICK_STEP_Y)
    {
        if (y > -EPS && y < EPS)
        {
            continue;
        }

        glVertex2f(-0.15f, y);
        glVertex2f(0.15f, y);
    }
    glEnd();
}

void DrawGraph()
{
    glColor3f(1.f, 0.f, 0.f);
    glLineWidth(2.0f);

    glBegin(GL_LINE_STRIP);
    for (float x = X_MIN; x <= X_MAX; x += GRAPH_STEP)
    {
        glVertex2f(x, Function(x));
    }
    glEnd();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    DrawAxes();
    DrawGraph();

    glFlush();
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = static_cast<float>(width) / static_cast<float>(height);

    if (aspect >= 1.0f)
    {
        glOrtho(X_MIN * aspect, X_MAX * aspect, Y_MIN, Y_MAX, -1.0f, 1.0f);
    }
    else
    {
        glOrtho(X_MIN, X_MAX, Y_MIN / aspect, Y_MAX / aspect, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("График y = sin(x) / x [-10; 10]");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glClearColor(1.f, 1.f, 1.f, 1.f);

    glutMainLoop();
    return 0;
}