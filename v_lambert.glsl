#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform float maxFurLength;
uniform float maxLayer;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec4 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 ic;
out vec4 l;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
out vec2 iTexCoord1;

out float layer;

void main(void) {
    vec4 lp = vec4(0, 0, -6, 1); //przestrzeń świata
    l = normalize(V * lp - V*M*vertex); //wektor do światła w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normal); //wektor normalny w przestrzeni oka
    iTexCoord0 = texCoord0;
    iTexCoord1 = (n.xy + 1) / 2;
    ic = color;

    vec4 vGravity=vec4(0,-0.2,0,0);
    vGravity = inverse(M)*vGravity;

    //gl_Position=P*V*M*vertex;

	layer=gl_InstanceID;
	vec4 nv=vertex+(layer*maxFurLength/maxLayer)*normalize(normal);
    nv = nv + vGravity*pow(layer/maxLayer,3);
	gl_Position=P*V*M*nv;
}
