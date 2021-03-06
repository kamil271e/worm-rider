#include "../lib/shaderprogram.h"
#include "../lib/object.h"
#include "../lib/OBJ_loader.h"
#include <string>

Object::Object(std::string path, GLuint tex, GLuint tex_reflect, bool inside){
    objl::Loader loader;
    bool loadout = loader.LoadFile(path);

    if (loadout){
        //std::cout << vertexCount << std::endl;
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

			if (inside){ //For skybox
				myObjNormals[i*4] *= -1;
				myObjNormals[i*4+1] *= -1;
				myObjNormals[i*4+2] *= -1;
			}
			myObjNormals[i*4+3] = 0.0f;

			myObjTexCoords[i*2] = loader.LoadedVertices[i].TextureCoordinate.X;
			myObjTexCoords[i*2+1] = loader.LoadedVertices[i].TextureCoordinate.Y;
		}
        this->vertices = myObjVertices;
        this->normals = myObjNormals;
        this->texCoords = myObjTexCoords;
        this->tex = tex;
		this->tex_reflect = tex_reflect;
    }
}

Object::~Object(){}

void Object::drawObject(glm::vec3 eye, glm::vec3 center, glm::vec3 up, glm::vec3 coords, glm::vec3 rot, glm::vec3 scal,
						ShaderProgram* sp, bool strip){
    glm::mat4 V=glm::lookAt(eye, center, up); //Compute view matrix
    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, 1.0f, 0.01f, 50.0f); //Compute projection matrix
	glm::mat4 M=glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(coords.x, coords.y, coords.z));
	M = glm::rotate(M, rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	M = glm::rotate(M, rot.y, glm::vec3(0.0f, 1.0, 0.0f));
	M = glm::rotate(M, rot.z, glm::vec3(0.0f, 0.0f, 1.0));
	M = glm::scale(M, glm::vec3(scal.x, scal.y, scal.z));
	sp->use(); //Activate shader program
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));
	glUniform1i(sp->u("textureMap0"),0);
	glUniform1i(sp->u("textureMap1"),1);

    glEnableVertexAttribArray(sp->a("vertex"));  //Enable sending data to the attribute vertex
    glVertexAttribPointer(sp->a("vertex"),4,GL_FLOAT,false,0,vertices); //Specify source of the data for the attribute vertex

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); 

	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"),2,GL_FLOAT,false,0,texCoords);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,tex_reflect);

	if (!strip) {glDrawArrays(GL_TRIANGLES,0,vertexCount);} //Draw the object
    else {glDrawArrays(GL_TRIANGLE_STRIP,0,vertexCount);}

    glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));
}
