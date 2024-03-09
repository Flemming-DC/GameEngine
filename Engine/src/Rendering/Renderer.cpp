#include "Renderer.h"
#include <GL/glew.h>
#include "ErrorChecker.h"
#include "Renderable.h"
#include "Gizmo.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "OpenGLidChecker.h"

bool Renderer::showBlackScreenDebugInfo = true;
uuids::uuid Renderer::horizontalGridID;
uuids::uuid Renderer::verticalGridID;
FrameBuffer Renderer::frameBuffer;


void Renderer::DrawToScreen()
{
    Renderable::UnBind(); //  evt. add framebuffer unbind
    for (const uuids::uuid& renderableID : Renderable::allRenderables)
        Entity::GetComponent<Renderable>(renderableID).Draw();
    for (Gizmo& gizmo : Gizmo::register_.GetData())
        gizmo.Draw();
    
    
    if (showBlackScreenDebugInfo && ScreenIsBlack())
        Log("The screen is black. Here are some possible causes:\n"
            "   * You might have forgotten to bind a texture, shader or vertexArray\n"
            "   * You might have forgotten to set a uniform.\n"
            "   * You might have provided an incorrect type to a gl function. fx GL_INT instead of GL_UNSIGNED_INT.\n"
            "   * You might have set the z coordinate of the view and the models, such that they are outside the clipping planes.\n"
            );
}


Renderer::RenderResult Renderer::DrawToFrameBuffer()
{
    if (!UuidCreator::IsInitialized(frameBuffer.GetID()))
        frameBuffer = FrameBuffer::register_.Add(Initializer::GetWidth(), Initializer::GetHeight());
    frameBuffer.Bind();
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    Renderer::DrawToScreen();
    frameBuffer.UnBind();
    return { frameBuffer.GetTextureOpenGLid(), Initializer::GetWidth(), Initializer::GetHeight() };
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
    horizontalGridID = Gizmo::register_.Add(
        horizontallyOrganizedPosition2Ds, nullptr, color, false, false).GetID();
    verticalGridID = Gizmo::register_.Add(
        verticallyOrganizedPosition2Ds, nullptr, color, false, false).GetID();

}


void Renderer::ShutDown()
{
    for (Shader& shader : Shader::register_.GetData())
        shader.ShutDown();
    for (Texture& texture : Texture::register_.GetData())
        texture.ShutDown();
    for (IndexBuffer& indexBuffer : IndexBuffer::register_.GetData())
        indexBuffer.ShutDown();
    for (VertexBuffer& vertexBuffer : VertexBuffer::register_.GetData())
        vertexBuffer.ShutDown();
    for (VertexArray& vertexArray : VertexArray::register_.GetData())
        vertexArray.ShutDown();
    for (FrameBuffer& frameBuffer : FrameBuffer::register_.GetData())
        frameBuffer.ShutDown();

    OpenGLidChecker::CheckCleanup();
}


void Renderer::ShowWindow(bool show)
{
    auto window = Initializer::GetWindow();
    if (IsWindowVisible())
    {
        glCall(glfwHideWindow(window));
    }
    else
    {
        glCall(glfwShowWindow(window));
    }
}

bool Renderer::IsWindowVisible()
{
    auto window = Initializer::GetWindow();
    glCall(bool isVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE));
    return isVisible;
}
