//by Dmitry Kolontay

#include "imgui.h"
#include "backends\imgui_impl_glut.h"
#include "backends\imgui_impl_opengl3.h"
#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <cmath>

GLfloat directionalLightDiffuse[] = { 0.9, 0.9, 0.6, 1.0 };
GLfloat directionalLightPosition[] = { 100.0, 100.0, 50.0, 0.0 };

GLfloat pointLightDiffuse[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat pointLightPosition[] = { 0.0, 0.25, 0.0, 1.0 };
GLfloat pointLightConstantAttenuation[] = { 0.1 };
GLfloat pointLightLinearAttenuation[] = { 0.0 };
GLfloat pointLightQuadraticAttenuation[] = { 0.1 };

bool bIsDirectionalLightOn{ true };
bool bIsPointLightOn{ true };
float prismRotationAngle{ 0.f };
float prismRotationSpeed{ 0.35f };
int prismCount{ 3 };
float prismRadius{ 2.9f };
float currentPrismsRotationAngle{ 0.f };
int oldTimeSinceStart{ 0 };

float desiredCameraXAngle{ 0.f };
float desiredCameraYAngle{ 0.f };
float desiredCameraScale{ 1.f };
#define RENDER_WIDTH 800
#define RENDER_HEIGHT 800

void controlPanelInit()
{
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos, ImGuiCond_FirstUseEver);
    ImGui::Begin("Control panel");
    ImGui::Text("by Dmitry Kolontay");
    ImGui::SameLine();
    if (ImGui::Button("Set scene to default")) {
        prismCount = 3;
        prismRotationSpeed = 0.35f;
        prismRotationAngle = 0.f;
        desiredCameraXAngle = 0.f;
        desiredCameraYAngle = 0.f;
        desiredCameraScale = 1.f;
        if (!bIsDirectionalLightOn) {
            bIsDirectionalLightOn = true;
            glEnable(GL_LIGHT0);
        }
        if (!bIsPointLightOn) {
            bIsPointLightOn = true;
            glEnable(GL_LIGHT1);
        }
    }
    if (ImGui::Checkbox("Enable directional lights", &bIsDirectionalLightOn)) {
        if (bIsDirectionalLightOn)
            glEnable(GL_LIGHT0);
        else
            glDisable(GL_LIGHT0);
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Enable point lights", &bIsPointLightOn)) {
        if (bIsPointLightOn)
            glEnable(GL_LIGHT1);
        else
            glDisable(GL_LIGHT1);
    }
    ImGui::SliderInt("Number of prisms", &prismCount, 0, 7, "%d", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderAngle("Prism rotation angle", &prismRotationAngle);
    ImGui::SliderFloat("Prism rotation speed", &prismRotationSpeed, 0.f, 1.f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::SliderAngle("Camera horizontal angle", &desiredCameraXAngle);
    ImGui::SliderAngle("Camera vertical angle", &desiredCameraYAngle);
    ImGui::SliderFloat("Camera distance", &desiredCameraScale, 0.5f, 2.5f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
    ImGui::End();
}

void DrawConeAndTorus() {
    glPushMatrix();
    glutSolidCone(0.7, 1.5, 32, 32);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.95);
    glutSolidTorus(0.1, 0.335, 32, 64);
    glPopMatrix();
}

void DrawPrisms(int n = 3) {
    for (int i{ 0 }; i < n; ++i) {
        glPushMatrix();
        glScalef(0.15, 0.15, 0.15);
        float currentAngle{ currentPrismsRotationAngle + static_cast<float>(i) / n * static_cast<float>(M_PI) * 2 };
        glTranslatef(prismRadius * cos(currentAngle), prismRadius * sin(currentAngle), 10);
        glRotatef(prismRotationAngle * 180 / static_cast<float>(M_PI), 0.0, 0.0, 1.0);
        glutSolidTetrahedron();
        glPopMatrix();
    }
}

void glutDisplay()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    controlPanelInit();
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();

    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(desiredCameraXAngle * 180 / static_cast<float>(M_PI), 0.0, 1.0, 0.0);
    glRotatef(desiredCameraYAngle * 180 / static_cast<float>(M_PI), 1.0, 0.0, 0.0);
    glScalef(desiredCameraScale, desiredCameraScale, desiredCameraScale);
    glLightfv(GL_LIGHT0, GL_POSITION, directionalLightPosition);
    glLightfv(GL_LIGHT1, GL_POSITION, pointLightPosition);
    glPushMatrix();
    glRotatef(-90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, -1.3);
    DrawConeAndTorus();
    DrawPrisms(prismCount);
    glPopMatrix();
    glPopMatrix();

    int newTimeSinceStart{ glutGet(GLUT_ELAPSED_TIME) };
    currentPrismsRotationAngle += (newTimeSinceStart - oldTimeSinceStart) * prismRotationSpeed / 200;
    oldTimeSinceStart = newTimeSinceStart;

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glutSwapBuffers();
    glutPostRedisplay();
    glFlush();
}

void glInitialize()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    //glEnable(GL_POLYGON_STIPPLE);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(40.0, 1.0, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 0.0, 5.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, directionalLightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, directionalLightPosition);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, pointLightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, pointLightPosition);
    glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, pointLightConstantAttenuation);
    glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, pointLightLinearAttenuation);
    glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, pointLightQuadraticAttenuation);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE /*| GLUT_ALPHA*/);
    glutCreateWindow("dimkKy");
    glutDisplayFunc(glutDisplay);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();   
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGLUT_InstallFuncs();

    glInitialize();

    glutReshapeWindow(RENDER_WIDTH, RENDER_HEIGHT);
    glutMainLoop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
    return 0;
}