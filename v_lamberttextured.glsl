#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


uniform vec4 lightDir=vec4(0,0,1,0);

//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space
layout (location=1) in vec4 normal; //vertex normal vector in model space
layout (location=2) in vec2 texCoord; //texturing coordinates


//varying variables
out vec2 i_tc;
out float i_nl;

//fog
out float visibility;
const float density = 0.03;
const float gradient = 3;

void main(void) {
    gl_Position=P*V*M*vertex;

    //fog
    vec4 positionToCam = V*M*vertex;
    float distance = length(positionToCam.xyz);
    visibility = exp(-pow((distance*density), gradient));
    visibility = clamp(visibility, 0.0, 1.0);

    mat4 G=mat4(inverse(transpose(mat3(M))));
    vec4 n=normalize(V*G*normal);

    i_nl=clamp(dot(n,lightDir),0,1);
    i_tc=texCoord;
}
