#version 330 core
in vec2 texcoord;
out vec4 FragColor;
uniform sampler2D sky;
void main()
{
   FragColor = texture(sky, texcoord);
}

