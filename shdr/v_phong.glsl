#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Attributes
in vec4 vertex; //Vertex coordinates in model space
in vec4 color; //Color associated with vertex
in vec4 normal; //Vertex normal in model space
in vec2 texCoord;

//Varying variables
out vec4 ic;
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;

void main(void) {
    vec4 lp = vec4(0, 0, -2, 1); //light position, world space
    l = normalize(V * lp - V*M*vertex); //vector towards the light in eye space
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //vector towards the viewer in eye space
    n = normalize(V * M * normal); //normal vector in eye space
    
    ic = color;
    iTexCoord0 = texCoord;
    
    gl_Position=P*V*M*vertex;
}
