#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/freeglut.h>
#include <vector>
#include <array>

float rotateX = 25.0f;
float rotateZ = 35.0f;

bool isMousePressed = false;
int lastMouseX = 0;
int lastMouseY = 0;

void DrawFace(
    const std::vector<std::array<float, 3>>& vertices,
    const std::vector<int>& face,
    float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_POLYGON);
    for (int index : face)
    {
        glVertex3fv(vertices[index].data());
    }
    glEnd();

    glColor3f(0, 0, 0);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    for (int index : face)
    {
        glVertex3fv(vertices[index].data());
    }
    glEnd();
}

void DrawTruncatedCube()
{
    const float a = 0.45f;

    std::vector<std::array<float, 3>> vertices =
    {
        {  1,  a,  1 }, {  a,  1,  1 }, { -a,  1,  1 }, { -1,  a,  1 },
        { -1, -a,  1 }, { -a, -1,  1 }, {  a, -1,  1 }, {  1, -a,  1 },

        {  1,  a, -1 }, {  a,  1, -1 }, { -a,  1, -1 }, { -1,  a, -1 },
        { -1, -a, -1 }, { -a, -1, -1 }, {  a, -1, -1 }, {  1, -a, -1 },

        {  1,  1,  a }, { -1,  1,  a }, { -1, -1,  a }, {  1, -1,  a },
        {  1,  1, -a }, { -1,  1, -a }, { -1, -1, -a }, {  1, -1, -a }
    };

    // 6 восьмиугольных граней
    DrawFace(vertices, { 0, 1, 2, 3, 4, 5, 6, 7 }, 1, 0.3f, 0.3f);
    DrawFace(vertices, { 15, 14, 13, 12, 11, 10, 9, 8 }, 0.3f, 0.5f, 1);

    DrawFace(vertices, { 0, 7, 19, 23, 15, 8, 20, 16 }, 0.3f, 1, 0.4f);
    DrawFace(vertices, { 3, 17, 21, 11, 12, 22, 18, 4 }, 1, 0.8f, 0.2f);

    DrawFace(vertices, { 1, 16, 20, 9, 10, 21, 17, 2 }, 0.8f, 0.4f, 1);
    DrawFace(vertices, { 6, 5, 18, 22, 13, 14, 23, 19 }, 0.2f, 1, 1);

    // 8 треугольных срезанных углов
    DrawFace(vertices, { 0, 16, 1 }, 1, 0, 0);
    DrawFace(vertices, { 3, 2, 17 }, 0, 1, 0);
    DrawFace(vertices, { 4, 18, 5 }, 0, 0, 1);
    DrawFace(vertices, { 7, 6, 19 }, 1, 1, 0);

    DrawFace(vertices, { 8, 9, 20 }, 1, 0, 1);
    DrawFace(vertices, { 11, 21, 10 }, 0, 1, 1);
    DrawFace(vertices, { 12, 13, 22 }, 1, 0.5f, 0);
    DrawFace(vertices, { 15, 23, 14 }, 0.5f, 0, 1);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(
        3, 3, 3,
        0, 0, 0,
        0, 0, 1
    );

    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateZ, 0, 0, 1);

    DrawTruncatedCube();

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
    gluPerspective(60, aspect, 0.5, 20);

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
        rotateX += dy * 0.5f;

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

void Init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab 4.1 - Truncated Cube");

    Init();

    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMove);

    glutMainLoop();

    return 0;
}