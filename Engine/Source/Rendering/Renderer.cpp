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


Renderer::RenderResult Renderer::DrawToFrameBuffer(
    vec3 cameraPos, quat cameraRot, vec3 cameraScale, vec2 screenMinCorner, vec2 screenMaxCorner)
{
    vec2 upperLeft = Screen::ToWorldPosition(screenMinCorner,
        screenMinCorner, screenMaxCorner, cameraPos, cameraRot, cameraScale);
    vec2 lowerRight = Screen::ToWorldPosition(screenMaxCorner,
        screenMinCorner, screenMaxCorner, cameraPos, cameraRot, cameraScale);
    RenderBoundingBox viewBounds = { upperLeft.x, lowerRight.y, lowerRight.x, upperLeft.y, };

    mat4 projView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);
    return DrawToFrameBuffer(projView, viewBounds);
}
Renderer::RenderResult Renderer::DrawToFrameBuffer() 
{
    vec2 min = Screen::ToWorldPosition(vec2(0.0f, 0.0f));
    vec2 max = Screen::ToWorldPosition(vec2(OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight()));
    RenderBoundingBox viewBounds = { min.x, min.y, max.x, max.y, };

    return DrawToFrameBuffer(Camera::Current().ProjectionView(), viewBounds);
}
void Renderer::DrawToScreen() 
{
    vec2 min = Screen::ToWorldPosition(vec2(0.0f, 0.0f));
    vec2 max = Screen::ToWorldPosition(vec2(OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight()));
    RenderBoundingBox viewBounds = { min.x, min.y, max.x, max.y, };

    DrawToScreen(Camera::Current().ProjectionView(), viewBounds);
}


Renderer::RenderResult Renderer::DrawToFrameBuffer(const mat4& projectionView, const RenderBoundingBox& viewBounds)
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
    Renderer::DrawToScreen(projectionView, viewBounds);
    frameBuffer.UnBind();
    return { frameBuffer.GetTextureOpenGLid(), OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight() };
}


void Renderer::DrawToScreen(const mat4& projectionView, const RenderBoundingBox& viewBounds)
{
    glCall(glClear(GL_COLOR_BUFFER_BIT));
    Renderable::UnBind(); //  evt. add framebuffer unbind

    // get sorted renderables
    static vector<Renderable*> renderables;
    renderables.clear();
    for (const uuid& renderableID : Renderable::allRenderables)
        renderables.push_back(&Entity::GetComponent<Renderable>(renderableID));
    struct SortByOrder {
        bool operator() (Renderable* lhs, Renderable* rhs) const
            { return lhs->DrawOrder() < rhs->DrawOrder(); } };
    std::sort(renderables.begin(), renderables.end(), SortByOrder());
    
    // drawing
    for (Renderable* rend : renderables)
    {
        if (rend->IsInView(viewBounds))
            rend->Draw(projectionView);
    }
    
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
