#include "Renderer.h"
#include <GL/glew.h>
#include "ErrorChecker.h"
#include "Renderable.h"
#include "Gizmo.h"


bool Renderer::showBlackScreenDebugInfo = true;
Gizmo* Renderer::horizontalGrid = nullptr;
Gizmo* Renderer::verticalGrid = nullptr;



void Renderer::Draw()
{
    Renderable::UnBind();
    for (Renderable* renderable : Renderable::allRenderables)
        renderable->Draw();
    for (Gizmo* gizmo : Gizmo::allGizmos)
        gizmo->Draw();
    
    
    if (showBlackScreenDebugInfo && ScreenIsBlack())
        Log("The screen is black. Here are some possible causes:\n"
            "   * You might have forgotten to bind a texture, shader or vertexArray\n"
            "   * You might have forgotten to set a uniform.\n"
            "   * You might have provided an incorrect type to a gl function. fx GL_INT instead of GL_UNSIGNED_INT.\n"
            "   * You might have set the z coordinate of the view and the models, such that they are outside the clipping planes.\n"
            );
}


bool Renderer::ScreenIsBlack()
{
    float threshold = 0.001f; // Define a threshold for considering a pixel as "black"

    // get data about screen color
    GLint viewport[4];
    glCall(glGetIntegerv(GL_VIEWPORT, viewport));
    int width = viewport[2];
    int height = viewport[3];
    unsigned char* pixelData = new unsigned char[width * height * 4];
    glCall(glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData));

    // check if it's essentially black
    bool isMostlyBlack = true;
    for (int i = 0; i < width * height; ++i)
    {
        unsigned char r = pixelData[i * 4];
        unsigned char g = pixelData[i * 4 + 1];
        unsigned char b = pixelData[i * 4 + 2];

        if (r > threshold || g > threshold || b > threshold)
        {
            isMostlyBlack = false;
            break;
        }
    }
    delete[] pixelData;
    showBlackScreenDebugInfo = false;
    return isMostlyBlack;
}


void Renderer::SetupGrid2D(float gridScale)
{
    if (verticalGrid)
        delete verticalGrid;

    float brightness = 0.2f;
    glm::vec4 color = glm::vec4(brightness, brightness, brightness, 1);
    
    float gridSize = 100;
    std::vector<glm::vec2> horizontallyOrganizedPosition2Ds;
    std::vector<glm::vec2> verticallyOrganizedPosition2Ds;
    for (float x = -gridSize; x < gridSize; x += gridScale)
    {
        for (float y = -gridSize; y < gridSize; y += gridScale)
        {
            horizontallyOrganizedPosition2Ds.push_back({ y, x }); // this swap x<->y effectively swaps columns and rows
            verticallyOrganizedPosition2Ds.push_back({ x, y });
        }

    }

    Log("Renderer: make gizmo");
    horizontalGrid = new Gizmo(horizontallyOrganizedPosition2Ds, nullptr, color);
    horizontalGrid->loop = false;
    horizontalGrid->showPoints = false;

    Log("Renderer: make gizmo");
    verticalGrid = new Gizmo(verticallyOrganizedPosition2Ds, nullptr, color);
    Log("Renderer: done making gizmos");
    verticalGrid->loop = false;
    verticalGrid->showPoints = false;
    Log("Renderer: exit codeblock ");
}



