#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexturePoin;

out vec2 TexturePoin;

void main()
{
	gl_Position = vec4(aPos.xyz, 1.0);
	TexturePoin = vec2(aTexturePoin.x, 1.0f - aTexturePoin.y);
	//TexturePoin = aTexturePoin;
}