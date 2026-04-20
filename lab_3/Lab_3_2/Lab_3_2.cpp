#include <GL/freeglut.h>
#include <cmath>

void DrawCircle(float centerX, float centerY, float radius, int segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawCircleOutline(float centerX, float centerY, float radius, int segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = centerX + radius * cosf(angle);
        float y = centerY + radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawEllipse(float centerX, float centerY, float radiusX, float radiusY, int segments)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = centerX + radiusX * cosf(angle);
        float y = centerY + radiusY * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawEllipseOutline(float centerX, float centerY, float radiusX, float radiusY, int segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * 3.1415926f * i / segments;
        float x = centerX + radiusX * cosf(angle);
        float y = centerY + radiusY * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void DrawRotatedEllipse(float centerX, float centerY, float radiusX, float radiusY, float angle, int segments)
{
    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++)
    {
        float a = 2.0f * 3.1415926f * i / segments;
        float x = radiusX * cosf(a);
        float y = radiusY * sinf(a);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

void DrawRotatedEllipseOutline(float centerX, float centerY, float radiusX, float radiusY, float angle, int segments)
{
    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        float a = 2.0f * 3.1415926f * i / segments;
        float x = radiusX * cosf(a);
        float y = radiusY * sinf(a);
        glVertex2f(x, y);
    }
    glEnd();

    glPopMatrix();
}

void DrawKrosh(float x, float y)
{
    float bodyR = 0.50f;
    float bodyG = 0.84f;
    float bodyB = 0.88f;

    float outlineR = 0.00f;
    float outlineG = 0.45f;
    float outlineB = 0.70f;

    // УШИ
    glColor3f(bodyR, bodyG, bodyB);
    DrawRotatedEllipse(x - 14.0f, y + 47.0f, 10.0f, 34.0f, 6.0f, 100);
    DrawRotatedEllipse(x + 14.0f, y + 47.0f, 10.0f, 34.0f, -6.0f, 100);

    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(3.0f);
    DrawRotatedEllipseOutline(x - 14.0f, y + 47.0f, 10.0f, 34.0f, 6.0f, 100);
    DrawRotatedEllipseOutline(x + 14.0f, y + 47.0f, 10.0f, 34.0f, -6.0f, 100);

    // РУКИ
    glColor3f(bodyR, bodyG, bodyB);
    DrawRotatedEllipse(x - 43.0f, y - 2.0f, 10.0f, 15.0f, 50.0f, 100);
    DrawRotatedEllipse(x + 43.0f, y - 2.0f, 10.0f, 15.0f, -50.0f, 100);

    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(3.0f);
    DrawRotatedEllipseOutline(x - 43.0f, y - 2.0f, 10.0f, 15.0f, 50.0f, 100);
    DrawRotatedEllipseOutline(x + 43.0f, y - 2.0f, 10.0f, 15.0f, -50.0f, 100);

    // НОГИ
    glColor3f(bodyR, bodyG, bodyB);
    DrawRotatedEllipse(x - 18.0f, y - 47.0f, 11.0f, 16.0f, -35.0f, 100);
    DrawRotatedEllipse(x + 18.0f, y - 47.0f, 11.0f, 16.0f, 35.0f, 100);

    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(3.0f);
    DrawRotatedEllipseOutline(x - 18.0f, y - 47.0f, 11.0f, 16.0f, -35.0f, 100);
    DrawRotatedEllipseOutline(x + 18.0f, y - 47.0f, 11.0f, 16.0f, 35.0f, 100);

    // ТЕЛО
    glColor3f(bodyR, bodyG, bodyB);
    DrawCircle(x, y, 40.0f, 120);

    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(3.0f);
    DrawCircleOutline(x, y, 40.0f, 120);

    // ГЛАЗА
    glColor3f(1.0f, 1.0f, 1.0f);
    DrawCircle(x - 11.0f, y + 10.0f, 11.0f, 100);
    DrawCircle(x + 11.0f, y + 10.0f, 11.0f, 100);

    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(2.0f);
    DrawCircleOutline(x - 11.0f, y + 10.0f, 11.0f, 100);
    DrawCircleOutline(x + 11.0f, y + 10.0f, 11.0f, 100);

    // ЗРАЧКИ
    glColor3f(0.0f, 0.0f, 0.0f);
    DrawCircle(x - 5.5f, y + 9.0f, 3.2f, 60);
    DrawCircle(x + 5.5f, y + 9.0f, 3.2f, 60);

    // НОС
    glColor3f(1.0f, 0.20f, 0.35f);
    DrawCircle(x, y, 4.5f, 80);

    // РОТ
    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(2.5f);

    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 13.0f, y - 2.0f);
    glVertex2f(x - 10.0f, y - 5.0f);
    glVertex2f(x - 6.0f, y - 7.0f);
    glVertex2f(x - 2.0f, y - 8.0f);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(x + 2.0f, y - 8.0f);
    glVertex2f(x + 6.0f, y - 7.0f);
    glVertex2f(x + 10.0f, y - 5.0f);
    glVertex2f(x + 13.0f, y - 2.0f);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(x, y - 4.0f);
    glVertex2f(x, y - 10.0f);
    glEnd();

    // ЗУБЫ
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(x - 4.5f, y - 8.0f);
    glVertex2f(x - 0.7f, y - 8.0f);
    glVertex2f(x - 0.7f, y - 17.0f);
    glVertex2f(x - 4.5f, y - 17.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(x + 0.7f, y - 8.0f);
    glVertex2f(x + 4.5f, y - 8.0f);
    glVertex2f(x + 4.5f, y - 17.0f);
    glVertex2f(x + 0.7f, y - 17.0f);
    glEnd();

    glColor3f(outlineR, outlineG, outlineB);
    glLineWidth(2.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x - 4.5f, y - 8.0f);
    glVertex2f(x - 0.7f, y - 8.0f);
    glVertex2f(x - 0.7f, y - 17.0f);
    glVertex2f(x - 4.5f, y - 17.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 0.7f, y - 8.0f);
    glVertex2f(x + 4.5f, y - 8.0f);
    glVertex2f(x + 4.5f, y - 17.0f);
    glVertex2f(x + 0.7f, y - 17.0f);
    glEnd();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    DrawKrosh(-50.0f, -30.0f);
    DrawKrosh(70.0f, 15.0f);
    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width / (float)height;

    if (aspect >= 1.0f)
    {
        glOrtho(-100.0f * aspect, 100.0f * aspect, -100.0f, 100.0f, -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-100.0f, 100.0f, -100.0f / aspect, 100.0f / aspect, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Kikoriki");

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glutMainLoop();
    return 0;
}