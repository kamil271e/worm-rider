#version 330


uniform sampler2D tex;

out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.

//Varying variables
in vec2 i_tc;

//fog
in float visibility;

void main(void) {
	pixelColor=texture(tex,i_tc);

	// fog
    //vec3 skyColor = vec3(0.2, 0.2, 0.2);
    //pixelColor=mix(vec4(skyColor,1.0), pixelColor, visibility);
}
