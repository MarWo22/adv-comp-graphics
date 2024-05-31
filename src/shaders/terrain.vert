#version 430

struct Vertex
{
    vec4 position;
    vec4 normal;
};

layout(std430, binding = 5) buffer GeomBuffer {
    Vertex vertices[];
};

uniform mat4 viewProjectionMatrix;
uniform bool displayNormals;

out vec3 color;

void main()
{
    Vertex vertex = vertices[gl_VertexID];
    gl_Position = viewProjectionMatrix * vertex.position;
    if (displayNormals)
        color = (normalize(vertex.normal.xyz) * 0.5f ) + 0.5f;
    else
        color = vec3(1,1,1);
}