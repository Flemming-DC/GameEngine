#include "Renderer.h"
#include <GL/glew.h>
#include "Camera.h"
#include "OpenGlSetup.h"
#include "Renderable.h"
#include "Gizmo.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "OpenGLidChecker.h"
#include "EngineMode.h"
#include "Screen.h"

Shorts
uuid Renderer::horizontalGridID;
uuid Renderer::verticalGridID;
FrameBuffer Renderer::frameBuffer;


Renderer::RenderResult Renderer::DrawToFrameBuffer(vec3 cameraPos, quat cameraRot, vec3 cameraScale, bool drawGizmos)
{ 
    mat4 projView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);
    return DrawToFrameBuffer(projView, drawGizmos); 
}
Renderer::RenderResult Renderer::DrawToFrameBuffer() 
{ 
    return DrawToFrameBuffer(Camera::Current().ProjectionView(), false); 
}
void Renderer::DrawToScreen() 
{ 
    DrawToScreen(Camera::Current().ProjectionView()); 
}


Renderer::RenderResult Renderer::DrawToFrameBuffer(mat4 projectionView, bool drawGizmos)
{
    if (!UuidCreator::IsInitialized(frameBuffer.GetID()))
        frameBuffer = FrameBuffer::register_.Add(OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    else if (OpenGlSetup::WindowIsResized())
    {
        frameBuffer.ShutDown();
        frameBuffer = FrameBuffer::register_.Add(OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    }

    frameBuffer.Bind();
    glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    Renderer::DrawToScreen(projectionView, drawGizmos);
    frameBuffer.UnBind();
    return { frameBuffer.GetTextureOpenGLid(), OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight() };
}


void Renderer::DrawToScreen(mat4 projectionView, bool drawGizmos)
{
    glCall(glClear(GL_COLOR_BUFFER_BIT)); // same as renderer.Clear()
    Renderable::UnBind(); //  evt. add framebuffer unbind
    Gizmo::CleanupDeadGizmos();

    for (const uuids::uuid& renderableID : Renderable::allRenderables)
        Entity::GetComponent<Renderable>(renderableID).Draw(projectionView);
    if (drawGizmos)
    {
        for (Gizmo& gizmo : Gizmo::GetData())
            gizmo.Draw(projectionView);
    }
    Screen::ApplyCursorState();
}


void Renderer::SetupGrid2D(float gridScale)
{
    float brightness = 0.2f;
    vec4 color = vec4(brightness, brightness, brightness, 1);
    
    float gridSize = 50;
    vector<vec2> horizontallyOrganizedPosition2Ds;
    vector<vec2> verticallyOrganizedPosition2Ds;

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


    horizontalGridID = Gizmo::Make(
        horizontallyOrganizedPosition2Ds, nullptr, color, false, false);
    verticalGridID = Gizmo::Make(
        verticallyOrganizedPosition2Ds, nullptr, color, false, false);

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
    auto window = OpenGlSetup::GetWindow();
    if (show)
    {
        glCall(glfwShowWindow(window));
    }
    else
    {
        glCall(glfwHideWindow(window));
    }
}

bool Renderer::IsWindowVisible()
{
    auto window = OpenGlSetup::GetWindow();
    glCall(bool isVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE));
    return isVisible;
}
