 #pragma once
#include "ImguiMenus.h"
#include <stdio.h>
#include <stdlib.h>
// open file dialog cross platform https://github.com/mlabbe/nativefiledialog
#include <nfd.h>
//my includes:
#include <iostream>
#include <string>
#include <algorithm>
# define PI 3.141592653589793238462643383279502884L /* pi */
int mod_counter=0;
vector<glm::vec3> zero;
float *f11, *f12, *f13;
float *f21, *f22, *f23;
float *f31, *f32, *f33;
float d1=0, d2 = 0, d3 = 0;
bool* rad = new bool;
bool* deg = new bool;
bool showDemoWindow = false;
bool in_place1 = false;
bool showAnotherWindow = false;
bool showFile = false;
bool first = TRUE;
bool modelsWindow = false;
bool camerasWindow = false;
bool* camewid = new bool;
bool* showcame = new bool;
bool* modwid = new bool;
int camewid_num=0;

int num=0;
glm::vec3 a;
vector<glm::vec3> rotation;
vector<glm::vec3> scale;
vector<glm::vec3> transformLIST;
glm::vec4 clearColor = glm::vec4(0.4f, 0.55f, 0.60f, 1.00f);

glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}
void loadOBJ(Scene *scene)
{
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
	if (result == NFD_OKAY) 
	{
		std::string filename(outPath);
		std::cout << filename << std::endl;
		//replace all '\' to '/' in the path
		replace(filename.begin(), filename.end(), '\\', '/');
		scene->LoadOBJModel(filename);
		free(outPath);
	}
	else if (result == NFD_CANCEL) {
	}
	else {
	std:cout << "why...?";
	}
}
void DrawImguiMenus(ImGuiIO& io, Scene* scene)
{
	if (first)
	{
		first = FALSE;
		modwid[0]=camewid[0]= showcame[0] = FALSE;
		rotation.clear();
		rotation.push_back(glm::vec3(0, 0, 0));
		scale.push_back(glm::vec3(1, 1, 1));
		transformLIST.push_back(glm::vec3(0, 0, 0));
		zero.push_back(glm::vec3(0, 0, 0));
		f11= new float; f12 = new float; f13 = new float;
		f21= new float; f22 = new float; f23 = new float;
		f31= new float; f32 = new float; f33 = new float;
		mod_counter = 0;
	}
	string str;
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Begin("Test Menu");
		static float f = 0.0f;
		static int counter = 0;
		// Display some text (you can use a format string too)
		ImGui::Text("Hello, world!\nHere you can change which model will be viewed and view stats about the model");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);
		ImGui::Checkbox("Models Window", &modelsWindow);
		ImGui::Checkbox("Cameras Window", &camerasWindow);
		if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (showAnotherWindow)
	{
		int val[2]; val[0] = io.MousePos.x; val[1] = io.MousePos.y;
		ImGui::Begin("Another Window", &showAnotherWindow);
		ImGui::InputInt2("(x,y)", val, 3);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (showDemoWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						ImGui::Text("Hello from another window!");
						std::cout << "success\n";
						
						std::string filename(outPath);
						std::cout << filename << std::endl;
						//replace all '\' to '/' in the path
						std::replace(filename.begin(), filename.end(), '\\', '/');
						scene->LoadOBJModel(filename);

						std::cout << "test LoadOBJ\n";
						std::cout << "active Model: " << scene->ActiveModel << std::endl;
						std::cout << "num of models: " << scene->getModels().size() << std::endl;
						std::cout << "Models[0] " << scene->getModels()[0]->getNameModel() << std::endl;
						std::cout << "Models[1] " << scene->getModels()[1]->getNameModel() << std::endl;


						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
						std:cout << "why...?";
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Demo Menu")) { showDemoWindow = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	if (modelsWindow)
	{
		ImGui::Begin("Models", &modelsWindow);

		if (ImGui::Button("ADD Model"))
		{
			loadOBJ(scene);
			modwid[mod_counter] = FALSE;
			rotation.push_back(glm::vec3(0, 0, 0));
			scale.push_back(glm::vec3(1, 1, 1));
			transformLIST.push_back(glm::vec3(0, 0, 0));
			zero.push_back(glm::vec3(0, 0, 0));
			f11[mod_counter] = 0.0f; f12[mod_counter] = 0.0f; f13[mod_counter] = 0.0f;
			f21[mod_counter] = 0.0f; f22[mod_counter] = 0.0f; f23[mod_counter] = 0.0f;
			f31[mod_counter] = 1.0f; f32[mod_counter] = 1.0f; f33[mod_counter] = 1.0f;
			deg[mod_counter] = TRUE;
			rad[mod_counter] = FALSE;
			mod_counter++;
		}
		if (scene->getModels().size() > 0)
		{
			str = "the active model is model " +
				scene->getModels()[scene->ActiveModel]->getNameModel() + " " +
				to_string(scene->ActiveModel);
			ImGui::Text(const_cast<char*>(str.c_str()));
		ImGui::Checkbox("show the normals of the vertices of the active MODEL : "
			, &scene->draw_norm_vertex);
		ImGui::Checkbox("show the normals of the faces of the active MODEL : "
			, &scene->draw_norm_face);
		}
		for (int i = 0; i < scene->getModels().size(); i++)
		{
			str = "show the window of MODEL : " 
				+ scene->getModels()[i]->getNameModel() +" "+ to_string(i);
			ImGui::Checkbox(const_cast<char*>(str.c_str()), &modwid[i]);
			str = "REMOVE MODEL : " + scene->getModels()[i]->getNameModel()
			+ " " + to_string(i);
			if (ImGui::Button(const_cast<char*>(str.c_str())))
			{
				scene->RemoveModel(i);
				rotation.erase(rotation.begin() + i);
				scale.erase(scale.begin() + i);
				transformLIST.erase(transformLIST.begin() + i);
				modwid[i] = FALSE;
			}
		}
		ImGui::End();
		
	}
	if (camerasWindow)
	{
		ImGui::Begin("Cameras", &modelsWindow);
		if (ImGui::Button("ADD CAMERA"))
		{
			scene->load_cam();
			camewid[scene->getCameras().size()-1] = FALSE;
			showcame[scene->getCameras().size() - 1] = FALSE;
		}
		str = "the active camera is camera number " + to_string(
			scene->getCameras()[scene ->ActiveModel]->num+1);
		ImGui::Text(const_cast<char*>(str.c_str()));
		for (int i = 0; i < scene->getCameras().size(); i++)
		{
			num = scene->getCameras()[i]->num + 1;
			str = "show the window of camera number "+ to_string(num);
			ImGui::Checkbox(const_cast<char*>(str.c_str()),&camewid[i]);
			str = "show camera number : " + to_string(num);
			ImGui::Checkbox(const_cast<char*>(str.c_str()), &showcame[i]);
			str = "REMOVE CAMERA NUMBER " + to_string(num);
			if (scene->getCameras().size()>1)
			if(ImGui::Button(const_cast<char*>(str.c_str())))
			{
				scene->remove_cam(i);
				camewid[i] = FALSE;
			}
		}
		ImGui::End();
		
	}
	for (int i = 0; i < scene->getModels().size(); i++)
	{
		if (modwid[i])
		{
			string str = "MODEL : " + scene->getModels()[i]->getNameModel()+to_string(i);
			ImGui::Begin(const_cast<char*>(str.c_str()), &modelsWindow);
			if (ImGui::Button("make active"))
				scene->ActiveModel = i;

			if(!scene->getModels()[i]->folow_the_mouse)
				if (ImGui::Button("folow the mouse"))
					scene->getModels()[i]->folow_the_mouse = TRUE;

			if(scene->getModels()[i]->folow_the_mouse)
				if (ImGui::Button("STOP FOLOWING THE MOUSE"))
					scene->getModels()[i]->folow_the_mouse = FALSE;
		
			a = scale[i];
			str = "SCALE: ratio of x:" + to_string(a[0]) + "to 1 , ratio of y : " 
			+ to_string(a[1])+ "to 1 ,ratio of z : " + to_string(a[2])+"to 1.";
			ImGui::Text(const_cast<char*>(str.c_str()));

			a = transformLIST[i];
			str = "TRANSPOSE: x:" + to_string(a[0]) + ", y : "
				+ to_string(a[1]) + ",  z : " + to_string(a[2])+" .";
			ImGui::Text(const_cast<char*>(str.c_str()));

			a = rotation[i];
			str = "ROTATE in degrees: x:" + to_string(a[0]) + " , y : "
				+ to_string(a[1]) + ",  z : " + to_string(a[2])+".";
			ImGui::Text(const_cast<char*>(str.c_str()));
			Color = scene->getColor(i,0);
			ImGui::ColorEdit3("color", (float*)&Color);
			scene->setColor(i, Color,0);
			Camera* cam = scene->getCameras()[0];
			if (rad[i])
				{
					if (ImGui::Button("deg"))
					{
					rad[i] = FALSE;
					deg[i] = TRUE;
					for (int i = 0; i < mod_counter; i++)
						f11[i] = f12[i] = f13[i] = 0;
					}
					ImGui::SliderFloat("rotation x rad", &f11[i], 0.0f, 2*PI);
					d1= a[0] - f11[i];
					ImGui::SliderFloat("rotation y rad", &f12[i], 0.0f, 2*PI);
					d2 = a[1] - f12[i];
					ImGui::SliderFloat("rotation z rad", &f13[i], 0.0f, 2*PI);
					d3 = a[2] - f13[i];
				}
			else
				{
					if (ImGui::Button("rad"))
					{
						deg[i] = FALSE;rad[i] = TRUE;
						for (int i = 0; i < mod_counter; i++)
							f11[i] =f12[i] =f13[i] = 0;
					}
					else
					ImGui::SliderFloat("rotation x degrees", &f11[i], 0.0f, 360.0f);
					d1= (a[0] - f11[i])*(PI / 180.0);
					ImGui::SliderFloat("rotation y degrees", &f12[i], 0.0f, 360.0f);
					d2 = (a[1] - f12[i])*(PI / 180.0);
					ImGui::SliderFloat("rotation z degrees", &f13[i], 0.0f, 360.0f);
					d3 = (a[2] - f13[i])*(PI / 180.0);
				}
			ImGui::Checkbox("rotate in place", &in_place1);
				if (in_place1)
				{

					scene->transformModel(
					cam->GetTranslateTransform(zero[i][0], zero[i][1], zero[i][2])*
					cam->GetrotationTransform(d1, 0)*
					cam->GetrotationTransform(d2,1)*
					cam->GetrotationTransform(d3, 2)*
					cam->GetTranslateTransform(-zero[i][0],-zero[i][1],-zero[i][2])
					);
					zero[i] = cam->GetTranslateTransform(zero[i][0], zero[i][1], zero[i][2])*
						cam->GetrotationTransform(d1, 0)*
						cam->GetrotationTransform(d2, 1)*
						cam->GetrotationTransform(d3, 2)*
						cam->GetTranslateTransform(-zero[i][0], -zero[i][1], -zero[i][2])*
						glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);
				}
				else
				{
					

					scene->transformModel(
						cam->GetrotationTransform(d1, 0)*
						cam->GetrotationTransform(d2, 1)*
						cam->GetrotationTransform(d3, 2)
					);
					zero[i] = cam->GetrotationTransform(d1, 0)*
						cam->GetrotationTransform(d2, 1)*
						cam->GetrotationTransform(d3, 2) *
						glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);
				}
		
			rotation[i] = glm::vec3(f11[i], f12[i], f13[i]);

			glm::vec3 auo=glm::vec3(f21[i], f22[i], f23[i]);
			ImGui::InputFloat("transpose x", &f21[i], 0.0f, 0.0f);
			
			ImGui::InputFloat("transpose y", &f22[i], 0.0f, 0.0f);
		
			ImGui::InputFloat("transpose z", &f23[i], 0.0f, 0.0f);

			zero[i] = glm::vec3(zero[i][0] + f21[i]-auo[0], 
				zero[i][1] + f22[i] - auo[1], zero[i][2] + f23[i] - auo[2]);

			scene->transformModel(cam->GetTranslateTransform
			(f21[i] - auo[0], f22[i] - auo[1], f23[i] - auo[2]));
			transformLIST[i] = glm::vec3(transformLIST[i][0]+ f21[i]-auo[0]
			, transformLIST[i][1] + f22[i]-auo[1], transformLIST[i][2] + f23[i] - auo[2]);
			
			auo = glm::vec3(f31[i], f32[i], f33[i]);

			ImGui::InputFloat("scale x", &f31[i], 0.0f, 0.0f);
			ImGui::InputFloat("scale y", &f32[i], 0.0f, 0.0f);
			ImGui::InputFloat("scale z", &f33[i], 0.0f, 0.0f);

			scene->transformModel(cam->GetScaleTransform(f31[i] / auo[0],
					f32[i] / auo[1], f33[i] / auo[2]));
			zero[i] = cam->GetScaleTransform(f31[i] / auo[0], f32[i] / auo[1],
					f33[i] / auo[2])*glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);
			
			ImGui::End();
		}
	}
	for (int i = 0; i < scene->getCameras().size(); i++)
	{
		if (camewid[i])
		{
			string str = "camera number : " + to_string(i);
			ImGui::Begin(const_cast<char*>(str.c_str()), &modelsWindow);
			if (ImGui::Button("make active"))
				scene->ActiveCamera = i;
			Color = scene->getColor(i, 1);
			ImGui::ColorEdit3("color", (float*)&Color);
			scene->setColor(i, Color,1);
			ImGui::Text("up :");
			ImGui::SliderFloat("up x :", &scene->getCameras()[i]->up[0], -1.0f, 1.0f);
			ImGui::SliderFloat("up y :", &scene->getCameras()[i]->up[1], -1.0f, 1.0f);
			ImGui::SliderFloat("up z :", &scene->getCameras()[i]->up[2], -1.0f, 1.0f);

			ImGui::Text( "position :");
			ImGui::InputFloat("position x :", &scene->getCameras()[i]->pos[0]);
			ImGui::InputFloat("position y :", &scene->getCameras()[i]->pos[1]);
			ImGui::InputFloat("position z :", &scene->getCameras()[i]->pos[2]);

			if(scene->getModels().size()>0)
			scene->getCameras()[i]->LookAt(scene->getCameras()[i]->pos,
				scene->GetVertexAvg(scene->ActiveModel) , scene->getCameras()[i]->up);
			ImGui::End();
		}
		if(showcame[i])
		{
			//scene->draw("../Data/cam.obj");
		}
	}
}