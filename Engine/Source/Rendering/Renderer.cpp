#include "Renderer.h"
#include <GL/glew.h>
#include "Camera.h"
#include "OpenGlSetup.h"
#include "Renderable.h"
#include "Shader.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "OpenGLidChecker.h"
#include "EngineMode.h"
#include "Screen.h"

Shorts
FrameBuffer Renderer::frameBuffer;


Renderer::RenderResult Renderer::DrawToFrameBuffer(vec3 cameraPos, quat cameraRot, vec3 cameraScale)
{ 
    mat4 projView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);
    return DrawToFrameBuffer(projView); 
}
Renderer::RenderResult Renderer::DrawToFrameBuffer() 
{ 
    return DrawToFrameBuffer(Camera::Current().ProjectionView()); 
}
void Renderer::DrawToScreen() 
{ 
    DrawToScreen(Camera::Current().ProjectionView()); 
}


Renderer::RenderResult Renderer::DrawToFrameBuffer(mat4 projectionView)
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
    Renderer::DrawToScreen(projectionView);
    frameBuffer.UnBind();
    return { frameBuffer.GetTextureOpenGLid(), OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight() };
}


void Renderer::DrawToScreen(mat4 projectionView)
{
    glCall(glClear(GL_COLOR_BUFFER_BIT)); // same as renderer.Clear()
    Renderable::UnBind(); //  evt. add framebuffer unbind

    for (const uuid& renderableID : Renderable::allRenderables)
        Entity::GetComponent<Renderable>(renderableID).Draw(projectionView);
    
    Screen::ApplyCursorState();
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
