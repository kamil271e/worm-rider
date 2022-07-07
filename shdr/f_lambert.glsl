#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;

out vec4 pixelColor; //Output variable. Almost final pixel color.

in vec4 ic; 
in vec4 n;
in vec4 l;
in vec4 v;
in vec2 iTexCoord0;
in vec2 iTexCoord1;

in float layer;

void main(void) {

	//Normalized, interpolated vectors
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	//vec4 mv = normalize(v);
	//Reflected vector
	//vec4 mr = reflect(-ml, mn);

	//Surface parameters
	//vec4 kd = mix(texture(textureMap0,iTexCoord0),texture(textureMap1, iTexCoord1),0.3);
	//vec4 ks = vec4(1,1,1,1);

	//Lighting model computation
	float nl = clamp(dot(mn, ml), 0, 1);
	//float rv = pow(clamp(dot(mr, mv), 0, 1), 50);

	vec4 fur=texture(textureMap0,iTexCoord0); //Fur (is hair?)
	vec4 furCol=texture(textureMap1,iTexCoord0); //Fur color
	vec4 col=furCol;                          //Default color
	//If not the first layer then add alpha with fur texture
	if (layer!=0) col=vec4(furCol.rgb,fur.a);
	// Remove invisible fragment, Z Bufor will remain unchanged
	if (fur.a==0 && layer!=0) discard; 
	pixelColor=col*vec4(nl,nl,nl,1);       //Resulting color

}
