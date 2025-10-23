#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D bgTexture;

void main()
{
    vec4 tex = texture(bgTexture, TexCoords);
    FragColor = tex;
}