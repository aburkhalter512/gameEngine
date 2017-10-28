#version 150 core
in vec4 vertex;

uniform mat3 mvp;
uniform mat3 size;

out vec2 imageCoordinate;

void main()
{
    gl_Position = vec4(mvp * size * vec3(vertex.xy, 1.0), 1.0);

    imageCoordinate = vertex.zw;
}
