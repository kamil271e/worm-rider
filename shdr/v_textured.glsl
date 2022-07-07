#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//fog
//out float visibility;
//const float density = 0.03;
//const float gradient = 1.5;


//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space
layout (location=2) in vec2 texCoord; //texturing coordinates


//varying variables
out vec2 i_tc;

void main(void) {
    //fog
    //vec4 positionToCam = V*M*vertex;
    //float distance = length(positionToCam.xyz);
    //visibility = exp(-pow((distance*density), gradient));
    //visibility = clamp(visibility, 0.0, 1.0);

    gl_Position=P*V*M*vertex;
    i_tc=texCoord;
}
