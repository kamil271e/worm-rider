#version 330

uniform sampler2D tex;

out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.

//fog
in float visibility;

//Varying variables
in float i_nl;
in vec2 i_tc;

void main(void) {
    vec4 color=texture(tex,i_tc);
	pixelColor=vec4(color.rgb*i_nl,color.a);

    // fog
    vec3 skyColor = vec3(42/255,11/255,8/255);
    pixelColor=mix(vec4(skyColor,1.0), pixelColor, visibility);
}
