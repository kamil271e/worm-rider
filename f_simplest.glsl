#version 330


out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 ic; 
in vec4 n;
in vec4 l;
in vec4 v;
in vec2 iTexCoord0;
uniform sampler2D textureMap0; // texturing unit
uniform sampler2D textureMap1;

void main(void) {

	//Znormalizowane interpolowane wektory
	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	//Wektor odbity
	vec4 mr = reflect(-ml, mn);

	//Parametry powierzchni
	//vec4 kd = ic; // normaly a color for an object
	//vec4 ks = vec4(1, 1, 1, 1); // color of surface of reflected light (white)
	//vec4 kd = texture(textureMap0, iTexCoord0);
	//vec4 ks = texture(textureMap1, iTexCoord1);
	
	vec4 ks = vec4(1, 1, 1, 1);
	vec2 skyCoord = (mn.xy+1)/2;
	vec4 texColor1 = texture(textureMap0, iTexCoord0);
	vec4 texColor2 = texture(textureMap1, skyCoord);
	vec4 kd = mix(texColor1, texColor2, 0.1); // standard linear interpolation function
	

	//Obliczenie modelu oświetlenia
	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);
	pixelColor= vec4(kd.rgb * nl, kd.a) + vec4(ks.rgb*rv, 0);
}
