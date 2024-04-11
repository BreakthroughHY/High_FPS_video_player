#version 330 core
out vec4 FragColor;

in vec2 TexturePoin;

uniform sampler2D videoFrame;


void main()
{
	FragColor = texture(videoFrame, TexturePoin);
	//FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);
}