#version 330 core
layout(location = 0) in vec3 aPos;

out vec2 TexturePoin;

void main()
{
	gl_Position = vec4(aPos.xyz, 1.0);
	TexturePoin = vec2(aPos.xy);
}