#include "GLApplication.h"

class CMyApplication : public CGLApplication
{
public:
    CMyApplication(const char* title, int width, int height)
        : CGLApplication(title, width, height)
    {
    }

protected:
    void OnInit() override
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(1, 1, 1, 1);
    }

    void OnDisplay() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Тут будем рисовать октаэдр
    }

    void OnReshape(int width, int height) override
    {
        glViewport(0, 0, width, height);

        double aspect = double(width) / double(height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, aspect, 0.5, 10);
        glMatrixMode(GL_MODELVIEW);
    }
};

int main()
{
    CMyApplication app("Lab 4 - Octahedron", 800, 600);
    app.Run();
    return 0;
}