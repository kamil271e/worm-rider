#include "../lib/shaderprogram.h"
#include "../lib/object.h"
#include "../lib/OBJ_loader.h"
#include <string>

Object::Object(std::string path, GLuint tex){
    objl::Loader loader;
    bool loadout = loader.LoadFile(path);

    if (loadout){
        std::cout << vertexCount << std::endl;
        this->vertexCount = loader.LoadedVertices.size();
        float* myObjVertices = new float[vertexCount*4];
        float* myObjNormals = new float[vertexCount*4];
        float* myObjTexCoords = new float[vertexCount*2];
        for(int i = 0; i < vertexCount; i++){
			
			myObjVertices[i*4] = loader.LoadedVertices[i].Position.X;
			myObjVertices[i*4+1] = loader.LoadedVertices[i].Position.Y;
			myObjVertices[i*4+2] = loader.LoadedVertices[i].Position.Z;
			myObjVertices[i*4+3] = 1.0f;

			myObjNormals[i*4] = loader.LoadedVertices[i].Normal.X;
			myObjNormals[i*4+1] = loader.LoadedVertices[i].Normal.Y;
			myObjNormals[i*4+2] = loader.LoadedVertices[i].Normal.Z;
			myObjNormals[i*4+3] = 0.0f;

			myObjTexCoords[i*2] = loader.LoadedVertices[i].TextureCoordinate.X;
			myObjTexCoords[i*2+1] = loader.LoadedVertices[i].TextureCoordinate.Y;
		}
        this->vertices = myObjVertices;
        this->normals = myObjNormals;
        this->texCoords = myObjTexCoords;
        this->tex = tex;
    }
}

Object::~Object(){}

void Object::drawObject(glm::vec3 eye, glm::vec3 center, glm::vec3 up, glm::vec3 coords, glm::vec3 rot, glm::vec3 scal){
    glm::mat4 V=glm::lookAt(eye, center, up);
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, 1.0f, 0.01f, 50.0f); //Wylicz macierz rzutowania
	glm::mat4 M=glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(coords.x, coords.y, coords.z));
	M = glm::rotate(M, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, rot.y, glm::vec3(0.0f, 1.0, 0.0f));
	M = glm::rotate(M, rot.z, glm::vec3(0.0f, 0.0f, 1.0));
	M = glm::scale(M, glm::vec3(scal.x, scal.y, scal.z));
	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(spLambertTextured->u("M"),1,false,glm::value_ptr(M));

    glEnableVertexAttribArray(spLambertTextured->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
    glVertexAttribPointer(spLambertTextured->a("vertex"),4,GL_FLOAT,false,0,vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(spLambertTextured->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
	glVertexAttribPointer(spLambertTextured->a("texCoord"),2,GL_FLOAT,false,0,texCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex);
	glUniform1i(spLambertTextured->u("tex"), 0);

    glDrawArrays(GL_TRIANGLES,0,vertexCount); //Narysuj obiekt

    glDisableVertexAttribArray(spLambertTextured->a("vertex"));
	glDisableVertexAttribArray(spLambertTextured->a("normal"));
	glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
}