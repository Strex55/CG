//rmdir / s / q build
//cd D:\Nikita\3_COURSE\CG\Lab_5
//cmake -B build -S . -G "Visual Studio 17 2022" -A x64
//cmake --build build --config Debug
//build\Debug\Lab_5.exe

#define FREEGLUT_LIB_PRAGMAS 0
#define NOMINMAX

#include <windows.h>
#include <gdiplus.h>

#include <GL/freeglut.h>

#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

ULONG_PTR g_gdiplusToken = 0;

GLuint wallTextures[7] = {};
GLuint floorTexture = 0;
GLuint ceilingTexture = 0;
GLuint exitTexture = 0;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 700;

const float CELL_SIZE = 2.0f;
const float WALL_HEIGHT = 2.0f;

const float MOVE_SPEED = 3.5f;
const float KEY_ROTATE_SPEED = 2.2f;
const float MOUSE_ROTATE_SPEED = 0.0015f;

float cameraX = 1.5f * CELL_SIZE;
float cameraY = 0.8f;
float cameraZ = 1.5f * CELL_SIZE;

float cameraAngle = 1.5708f;
float cameraPitch = 0.0f;
int lastFrameTime = 0;

int currentWindowWidth = WINDOW_WIDTH;
int currentWindowHeight = WINDOW_HEIGHT;

bool keyForward = false;
bool keyBackward = false;
bool keyLeft = false;
bool keyRight = false;

bool arrowLeft = false;
bool arrowRight = false;

bool ignoreNextMouseMove = false;

bool finishReached = false;

// 0 - проход
// 1..6 - разные типы стен
//7 - выход
const int MAZE_HEIGHT = 12;
const int MAZE_WIDTH = 12;

int maze[MAZE_HEIGHT][MAZE_WIDTH] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,2,0,0,0,0,0,0,1},
    {1,0,3,0,2,0,4,4,4,4,0,1},
    {1,0,3,0,0,0,0,0,0,4,0,1},
    {1,0,3,3,3,3,3,3,0,4,0,1},
    {1,0,0,0,0,0,0,3,0,0,0,1},
    {1,5,5,5,0,6,0,3,3,3,0,1},
    {1,0,0,5,0,6,0,0,0,3,0,1},
    {1,0,0,5,0,6,6,6,0,3,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,2,2,2,2,0,5,5,7,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
};

bool IsWall(float x, float z)
{
    int cellX = static_cast<int>(x / CELL_SIZE);
    int cellZ = static_cast<int>(z / CELL_SIZE);

    if (cellX < 0 || cellX >= MAZE_WIDTH || cellZ < 0 || cellZ >= MAZE_HEIGHT)
    {
        return true;
    }

    return maze[cellZ][cellX] >= 1 && maze[cellZ][cellX] <= 6;
}

bool IsWallCell(int x, int z)
{
    if (x < 0 || x >= MAZE_WIDTH || z < 0 || z >= MAZE_HEIGHT)
    {
        return true;
    }

    return maze[z][x] >= 1 && maze[z][x] <= 6;
}

bool IsExitCell(float x, float z)
{
    int cellX = static_cast<int>(x / CELL_SIZE);
    int cellZ = static_cast<int>(z / CELL_SIZE);

    if (cellX < 0 || cellX >= MAZE_WIDTH || cellZ < 0 || cellZ >= MAZE_HEIGHT)
    {
        return false;
    }

    return maze[cellZ][cellX] == 7;
}

void SetWallColor(int wallType)
{
    switch (wallType)
    {
    case 1:
        glColor3f(0.55f, 0.55f, 0.55f);
        break;
    case 2:
        glColor3f(0.65f, 0.25f, 0.15f);
        break;
    case 3:
        glColor3f(0.35f, 0.35f, 0.7f);
        break;
    case 4:
        glColor3f(0.25f, 0.55f, 0.25f);
        break;
    case 5:
        glColor3f(0.6f, 0.45f, 0.25f);
        break;
    case 6:
        glColor3f(0.45f, 0.25f, 0.55f);
        break;
    default:
        glColor3f(1.0f, 1.0f, 1.0f);
        break;
    }
}

void DrawWallCell(int cellX, int cellZ)
{
    float x0 = cellX * CELL_SIZE;
    float x1 = x0 + CELL_SIZE;

    float z0 = cellZ * CELL_SIZE;
    float z1 = z0 + CELL_SIZE;

    float y0 = 0.0f;
    float y1 = WALL_HEIGHT;

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    // Передняя сторона стены, если спереди проход
    if (!IsWallCell(cellX, cellZ - 1))
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x0, y0, z0);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(x1, y0, z0);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(x1, y1, z0);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(x0, y1, z0);
    }

    // Задняя сторона стены, если сзади проход
    if (!IsWallCell(cellX, cellZ + 1))
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x1, y0, z1);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(x0, y0, z1);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(x0, y1, z1);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(x1, y1, z1);
    }

    // Левая сторона стены, если слева проход
    if (!IsWallCell(cellX - 1, cellZ))
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x0, y0, z1);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(x0, y0, z0);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(x0, y1, z0);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(x0, y1, z1);
    }

    // Правая сторона стены, если справа проход
    if (!IsWallCell(cellX + 1, cellZ))
    {
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(x1, y0, z0);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(x1, y0, z1);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(x1, y1, z1);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(x1, y1, z0);
    }

    glEnd();
}

void DrawFloor()
{
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    float width = MAZE_WIDTH * CELL_SIZE;
    float depth = MAZE_HEIGHT * CELL_SIZE;

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    glTexCoord2f(static_cast<float>(MAZE_WIDTH), 0.0f);
    glVertex3f(width, 0.0f, 0.0f);

    glTexCoord2f(static_cast<float>(MAZE_WIDTH), static_cast<float>(MAZE_HEIGHT));
    glVertex3f(width, 0.0f, depth);

    glTexCoord2f(0.0f, static_cast<float>(MAZE_HEIGHT));
    glVertex3f(0.0f, 0.0f, depth);

    glEnd();
}

void DrawCeiling()
{
    glBindTexture(GL_TEXTURE_2D, ceilingTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    float width = MAZE_WIDTH * CELL_SIZE;
    float depth = MAZE_HEIGHT * CELL_SIZE;

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, WALL_HEIGHT, 0.0f);

    glTexCoord2f(0.0f, static_cast<float>(MAZE_HEIGHT));
    glVertex3f(0.0f, WALL_HEIGHT, depth);

    glTexCoord2f(static_cast<float>(MAZE_WIDTH), static_cast<float>(MAZE_HEIGHT));
    glVertex3f(width, WALL_HEIGHT, depth);

    glTexCoord2f(static_cast<float>(MAZE_WIDTH), 0.0f);
    glVertex3f(width, WALL_HEIGHT, 0.0f);

    glEnd();
}

void DrawExitTile()
{
    glBindTexture(GL_TEXTURE_2D, exitTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int z = 0; z < MAZE_HEIGHT; ++z)
    {
        for (int x = 0; x < MAZE_WIDTH; ++x)
        {
            if (maze[z][x] == 7)
            {
                float x0 = x * CELL_SIZE;
                float x1 = x0 + CELL_SIZE;

                float z0 = z * CELL_SIZE;
                float z1 = z0 + CELL_SIZE;

                float y = 0.01f;

                glBegin(GL_QUADS);

                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(x0, y, z0);

                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(x1, y, z0);

                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(x1, y, z1);

                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(x0, y, z1);

                glEnd();
            }
        }
    }
}

void DrawMaze()
{
    glEnable(GL_TEXTURE_2D);

    DrawFloor();
    DrawExitTile();
    DrawCeiling();

    for (int z = 0; z < MAZE_HEIGHT; ++z)
    {
        for (int x = 0; x < MAZE_WIDTH; ++x)
        {
            int wallType = maze[z][x];

            if (wallType >= 1 && wallType <= 6)
            {
                glBindTexture(GL_TEXTURE_2D, wallTextures[wallType]);
                DrawWallCell(x, z);
            }
        }
    }
}

void InitGdiPlus()
{
    GdiplusStartupInput input;

    if (GdiplusStartup(&g_gdiplusToken, &input, nullptr) != Ok)
    {
        throw std::runtime_error("Failed to initialize GDI+");
    }
}

void ShutdownGdiPlus()
{
    if (g_gdiplusToken != 0)
    {
        GdiplusShutdown(g_gdiplusToken);
        g_gdiplusToken = 0;
    }
}

GLuint LoadTexture(const std::wstring& fileName)
{
    Bitmap bitmap(fileName.c_str());

    if (bitmap.GetLastStatus() != Ok)
    {
        throw std::runtime_error("Failed to load texture");
    }

    const UINT width = bitmap.GetWidth();
    const UINT height = bitmap.GetHeight();

    Rect rect(0, 0, width, height);

    BitmapData bitmapData;
    bitmapData.Scan0 = nullptr;

    Status status = bitmap.LockBits(
        &rect,
        ImageLockModeRead,
        PixelFormat32bppARGB,
        &bitmapData
    );

    if (status != Ok)
    {
        throw std::runtime_error("Failed to lock texture pixels");
    }

    std::vector<unsigned char> pixels(width * height * 3);

    unsigned char* source = static_cast<unsigned char*>(bitmapData.Scan0);

    for (UINT y = 0; y < height; ++y)
    {
        unsigned char* sourceRow = source + y * bitmapData.Stride;

        for (UINT x = 0; x < width; ++x)
        {
            unsigned char b = sourceRow[x * 4 + 0];
            unsigned char g = sourceRow[x * 4 + 1];
            unsigned char r = sourceRow[x * 4 + 2];

            UINT flippedY = height - 1 - y;
            UINT targetIndex = (flippedY * width + x) * 3;

            pixels[targetIndex + 0] = r;
            pixels[targetIndex + 1] = g;
            pixels[targetIndex + 2] = b;
        }
    }

    bitmap.UnlockBits(&bitmapData);

    GLuint textureId = 0;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureId;
}

void LoadTextures()
{
    wallTextures[1] = LoadTexture(L"textures/wall1.jpg");
    wallTextures[2] = LoadTexture(L"textures/wall2.jpg");
    wallTextures[3] = LoadTexture(L"textures/wall3.jpg");
    wallTextures[4] = LoadTexture(L"textures/wall4.jpg");
    wallTextures[5] = LoadTexture(L"textures/wall5.jpg");
    wallTextures[6] = LoadTexture(L"textures/wall6.jpg");

    floorTexture = LoadTexture(L"textures/floor.jpg");
    ceilingTexture = LoadTexture(L"textures/ceiling.jpg");
    exitTexture = LoadTexture(L"textures/exit.jpg");
}

void SetupFog()
{
    glEnable(GL_FOG);

    GLfloat fogColor[4] = { 0.12f, 0.12f, 0.14f, 1.0f };

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);

    glFogf(GL_FOG_START, 2.0f);
    glFogf(GL_FOG_END, 18.0f);

    glHint(GL_FOG_HINT, GL_NICEST);
}

void SetupCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float lookX = cameraX + std::sin(cameraAngle) * std::cos(cameraPitch);
    float lookY = cameraY + std::sin(cameraPitch);
    float lookZ = cameraZ - std::cos(cameraAngle) * std::cos(cameraPitch);

    gluLookAt(
        cameraX, cameraY, cameraZ,
        lookX, lookY, lookZ,
        0.0f, 1.0f, 0.0f
    );
}

void DrawFinishScreen()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, currentWindowWidth, 0, currentWindowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glClearColor(0.05f, 0.05f, 0.07f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const char* text = "Congrats! You escaped the maze";

    void* font = GLUT_BITMAP_TIMES_ROMAN_24;

    int textWidth = 0;
    for (const char* ch = text; *ch != '\0'; ++ch)
    {
        textWidth += glutBitmapWidth(font, *ch);
    }

    int x = (currentWindowWidth - textWidth) / 2;
    int y = currentWindowHeight / 2;

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(x, y);

    for (const char* ch = text; *ch != '\0'; ++ch)
    {
        glutBitmapCharacter(font, *ch);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void Display()
{
    if (finishReached)
    {
        DrawFinishScreen();
        glutSwapBuffers();
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SetupCamera();
    SetupFog();

    DrawMaze();

    glutSwapBuffers();
}

void Reshape(int width, int height)
{
    currentWindowWidth = std::max(width, 1);
    currentWindowHeight = std::max(height, 1);

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(
        60.0,
        static_cast<double>(currentWindowWidth) / static_cast<double>(currentWindowHeight),
        0.1,
        100.0
    );

    glMatrixMode(GL_MODELVIEW);
}

void TryMove(float dx, float dz)
{
    float nextX = cameraX + dx;
    float nextZ = cameraZ + dz;

    const float radius = 0.25f;

    bool collision =
        IsWall(nextX - radius, nextZ - radius) ||
        IsWall(nextX + radius, nextZ - radius) ||
        IsWall(nextX - radius, nextZ + radius) ||
        IsWall(nextX + radius, nextZ + radius);

    if (!collision)
    {
        cameraX = nextX;
        cameraZ = nextZ;
    }

    if (!finishReached && IsExitCell(cameraX, cameraZ))
    {
        finishReached = true;
        glutSetWindowTitle("3D Maze - Congrats! Finish reached");
    }
}

void Keyboard(unsigned char key, int, int)
{
    if (finishReached)
    {
        if (key == 27)
        {
            glutLeaveMainLoop();
        }

        return;
    }

    switch (key)
    {
    case 'w':
    case 'W':
    case 246: // ц
    case 214: // Ц
        keyForward = true;
        break;

    case 's':
    case 'S':
    case 251: // ы
    case 219: // Ы
        keyBackward = true;
        break;

    case 'a':
    case 'A':
    case 244: // ф
    case 212: // Ф
        keyLeft = true;
        break;

    case 'd':
    case 'D':
    case 226: // в
    case 194: // В
        keyRight = true;
        break;

    case 27:
        glutLeaveMainLoop();
        break;
    }
}

void KeyboardUp(unsigned char key, int, int)
{
    switch (key)
    {
    case 'w':
    case 'W':
    case 246: // ц
    case 214: // Ц
        keyForward = false;
        break;

    case 's':
    case 'S':
    case 251: // ы
    case 219: // Ы
        keyBackward = false;
        break;

    case 'a':
    case 'A':
    case 244: // ф
    case 212: // Ф
        keyLeft = false;
        break;

    case 'd':
    case 'D':
    case 226: // в
    case 194: // В
        keyRight = false;
        break;
    }
}

void SpecialKeys(int key, int, int)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        keyForward = true;
        break;

    case GLUT_KEY_DOWN:
        keyBackward = true;
        break;

    case GLUT_KEY_LEFT:
        arrowLeft = true;
        break;

    case GLUT_KEY_RIGHT:
        arrowRight = true;
        break;
    }
}

void SpecialKeysUp(int key, int, int)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        keyForward = false;
        break;

    case GLUT_KEY_DOWN:
        keyBackward = false;
        break;

    case GLUT_KEY_LEFT:
        arrowLeft = false;
        break;

    case GLUT_KEY_RIGHT:
        arrowRight = false;
        break;
    }
}

void MouseMotion(int x, int y)
{
    if (ignoreNextMouseMove)
    {
        ignoreNextMouseMove = false;
        return;
    }

    int centerX = currentWindowWidth / 2;
    int centerY = currentWindowHeight / 2;

    int dx = x - centerX;
    int dy = y - centerY;

    cameraAngle += dx * MOUSE_ROTATE_SPEED;
    cameraPitch -= dy * MOUSE_ROTATE_SPEED;

    const float maxPitch = 1.2f;

    if (cameraPitch > maxPitch)
    {
        cameraPitch = maxPitch;
    }

    if (cameraPitch < -maxPitch)
    {
        cameraPitch = -maxPitch;
    }

    ignoreNextMouseMove = true;
    glutWarpPointer(centerX, centerY);

    glutPostRedisplay();
}

void Idle()
{
    if (finishReached)
    {
        glutPostRedisplay();
        return;
    }

    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (lastFrameTime == 0)
    {
        lastFrameTime = currentTime;
    }

    float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
    lastFrameTime = currentTime;

    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    if (arrowLeft)
    {
        cameraAngle -= KEY_ROTATE_SPEED * deltaTime;
    }

    if (arrowRight)
    {
        cameraAngle += KEY_ROTATE_SPEED * deltaTime;
    }

    float forwardX = std::sin(cameraAngle);
    float forwardZ = -std::cos(cameraAngle);

    float rightX = std::cos(cameraAngle);
    float rightZ = std::sin(cameraAngle);

    float step = MOVE_SPEED * deltaTime;

    if (keyForward)
    {
        TryMove(forwardX * step, forwardZ * step);
    }

    if (keyBackward)
    {
        TryMove(-forwardX * step, -forwardZ * step);
    }

    if (keyLeft)
    {
        TryMove(-rightX * step, -rightZ * step);
    }

    if (keyRight)
    {
        TryMove(rightX * step, rightZ * step);
    }

    glutPostRedisplay();
}

void InitOpenGL()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.08f, 0.08f, 0.10f, 1.0f);

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    LoadTextures();

    glutSetCursor(GLUT_CURSOR_NONE);
}

int main(int argc, char* argv[])
{
    InitGdiPlus();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("3D Maze");
    glutFullScreen();

    InitOpenGL();

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glutKeyboardFunc(Keyboard);
    glutKeyboardUpFunc(KeyboardUp);

    glutSpecialFunc(SpecialKeys);
    glutSpecialUpFunc(SpecialKeysUp);

    glutPassiveMotionFunc(MouseMotion);
    glutIdleFunc(Idle);

    glutMainLoop();

    ShutdownGdiPlus();

    return 0;
}