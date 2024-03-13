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
    
    /*
    if (showBlackScreenDebugInfo && ScreenIsBlack())
        info
    */
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



void Renderer::SetupGrid2D(float gridScale)
{
    float brightness = 0.2f;
    glm::vec4 color = glm::vec4(brightness, brightness, brightness, 1);
    
    float gridSize = 50;
    std::vector<glm::vec2> horizontallyOrganizedPosition2Ds;
    std::vector<glm::vec2> verticallyOrganizedPosition2Ds;

    for (float y = -gridSize; y < gridSize; y += gridScale)
    {
        horizontallyOrganizedPosition2Ds.push_back({ -gridSize, y });
        horizontallyOrganizedPosition2Ds.push_back({  gridSize, y });
    }
    for (float x = -gridSize; x < gridSize; x += gridScale)
    {
        verticallyOrganizedPosition2Ds.push_back({ x, -gridSize });
        verticallyOrganizedPosition2Ds.push_back({ x,  gridSize });
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
