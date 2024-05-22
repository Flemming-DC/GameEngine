#shader vertex
#version 330 core

layout(location = 0) in vec4 v_position; // v for vertex
layout(location = 1) in vec2 v_textureCoordinates;
out vec2 textureCoordinates;
uniform mat4 u_MVP; // u for uniform

void main()
{
	gl_Position = u_MVP * v_position;
	textureCoordinates = v_textureCoordinates;
};


#shader fragment
#version 330 core
//#extension GL_ARB_separate_shader_objects : enable

in vec2 textureCoordinates;
layout(location = 0) out vec4 color;
uniform vec4 u_color; // u for uniform
uniform sampler2D u_textureSampler; // this is the same as the slot variable in the shader bind function

void main()
{
	vec4 textureColor = texture(u_textureSampler, textureCoordinates);
	color = textureColor * u_color;
};
















