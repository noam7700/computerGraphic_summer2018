#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Model.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
class Scene;
using namespace std;

/*
* MeshModel class. Mesh model object represents a triangle mesh (loaded fron an obj file).
*
*/
class MeshModel : public Model
{
protected:
	long norm_num;
	glm::vec4 *vertexPositions;
	glm::vec4 *normalPositions; //normal per face per vertex
	glm::vec4 *normalPositions2; //normal per per vertex
	glm::vec4 *faceNormals;
	glm::vec4 *faceAvgs;
	glm::vec2 *textCoords; //texture coordinates
	// Add more attributes.
	int vertexPosNum; // 3 * #vertices
	
	glm::mat4x4 worldTransform; //transformations according to the world's origin
	glm::mat4x4 modelTransform; //transformations according to model's origin
	glm::mat4x4 normalTransform; //transformations on normals

	void setFaceNormals(); //build the faces' normals - in constructor
	void setBound(); //find max's and min's indices - in constructor 


	//openGL attributes
	unsigned int VAO; //this is what we're going to bind everytime we're drawing...

	//buffers[0] - vertexPositions
	//buffers[1] - normalPositions -- this will be our normal per vertex! >:)
	//(and not normal per face per vertex)
	//buffers[2] - textureCoordinates
	unsigned int buffers[3];
	//* I don't think it should be save as an attribute, but learn openGL does it... (to buffer[3])
	unsigned int texture;
	void load_normal_per_vertex();
public:
	void MeshModel::LoadTexture(const string& fileName, GLFWwindow* window);
	//openGL help functions
	void initVaoModel(); //this will create our vao with all the vertex buffers
	void bindVaoModel(); //this will just bind our vao :)
	
	MeshModel();//for PrimMeshModel()
	~MeshModel();
	bool window_open = 0;
	bool folow_the_mouse = 0;
	bool willDrawTexture = 0;
	bool willDrawBox=0;
	bool willDrawVertexNormal = 0; //normal per face per vertex
	bool willDrawVertexNormal2 = 0; //normal per vertex
	bool willDrawFaceNormal = 0;
	bool TEXTURE = 0;
	bool norm_as_color = 0;
	void setModeltransform(glm::mat4x4 transform);
	MeshModel(const string& fileName);
	void LoadFile(const string& fileName);
	int getVertexPosNum();
	void transformModel(glm::mat4x4 transform);
	void transformWorld(glm::mat4x4 transform);
	glm::vec4* Draw();
	void DrawOpenGL(unsigned int shaderProgram, int index, Scene* scene, glm::mat4 cameraTrans, glm::mat4 camProject);
	glm::vec4* GetVertex();
	glm::vec4* getNormalVertex();
	glm::vec4* getNormalFace();
	glm::vec4* getFaceAvgs();
	glm::mat4x4& getModelTransform();
	glm::mat4x4& getWorldTransform();
	glm::mat4x4& getNormalTransform();

	int Specularity_exponent = 1;
	//keep the vertices indices that bounds the object
	int xMax;
	int xMin;
	int yMax;
	int yMin;
	int zMax;
	int zMin;
};