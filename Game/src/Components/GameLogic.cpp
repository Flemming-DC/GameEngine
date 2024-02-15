#include "GameLogic.h"



void GameLogic::SetByInspector(const Material& material_)
{
    material = material_;
    color = material.GetUniform<glm::vec4>("u_color");
}

void GameLogic::OnUpdate()
{
    Get<Renderable>()->GetMaterial().SetUniform("u_color", color);
    //material.SetUniform("u_color", color);

    if (color.r > 1)
        increment = -0.1f;
    if (color.r < 0)
        increment = 0.1f;
    color.r += increment;
}






