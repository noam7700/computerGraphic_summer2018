#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "PrimMeshModel.h"
#define GLM_SWIZZLE

//for checking filename
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;
void Scene::LoadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
}


void Scene::Draw()
{
	// 1. Send the renderer the current camera transform and the projection
	glm::mat4x4 default = glm::mat4x4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	//renderer->SetCameraTransform(default);
	//renderer->SetProjection(default);
	// 2. Tell all models to draw themselves
	
	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm

	//draw first triangle :O
	//std::cout << models.at(ActiveModel)->getVertexPosNum()<<"  ";
	renderer->DrawTriangles(models.at(ActiveModel)->Draw(),
	models.at(ActiveModel)->getVertexPosNum());
	renderer->SwapBuffers();
}

void Scene::DrawDemo()
{
	MeshModel* primitive = new PrimMeshModel(); //testing

	string fileName = "C:/Users/nir blagovsky/Documents/Noam/TEXTFILE.txt";
	//LoadFile of camera instead?
	MeshModel* testOBJ = new MeshModel(fileName); //a cube?

	const glm::vec4* verPos = testOBJ->Draw();
	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	
	//draw first triangle :O
	glm::vec2 a(0.0f, 0.0f), b(0.0f, 0.0f), c(0.0f, 0.0f);
	for (int face = 0; face < testOBJ->getVertexPosNum() - 2; face = face + 3)
	{
		a.x = verPos[face].x;
		a.y = verPos[face].y;

		b.x = verPos[face + 1].x;
		b.y = verPos[face + 1].y;

		c.x = verPos[face + 2].x;
		c.y = verPos[face + 2].y;

		renderer->drawLine(a, b);
		renderer->drawLine(b, c);
		renderer->drawLine(c, a);
	}

	
	renderer->SwapBuffers();
}
void Scene::drawf()
{
	glm::vec4 *c =new glm::vec4;
	c[0] = (glm::vec4(100, 100, 0,0));
	c[1] = (glm::vec4(200, 100, 0,0));
	c[2] = (glm::vec4(100, 200, 0,0));
	renderer->DrawTriangles(c,3); //Bresenham algorithm
	renderer->SwapBuffers();
}
