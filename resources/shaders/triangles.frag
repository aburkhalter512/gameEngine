#version 150 core
in vec2 imageCoordinate;

uniform sampler2D image;
uniform vec3 color;

out vec4 outColor;

void main()
{
    outColor = vec4(color, 1.0) * texture(image, imageCoordinate);
}
