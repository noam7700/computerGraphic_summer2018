#pragma once
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include "Camera.h"
/*
 * Light class. Holds light source information and data.
 */
class Light
{
private:
	glm::vec4 Position;
	glm::mat4 transromMatrix;
public:
	glm::vec3 getPosition();
	void transformPosition(glm::mat4 transform);
	void resetPosition();
	Light();
	~Light();
	glm::vec3 ambient , difus , specalar, direction;
	float strengte_specalar, strengte_difus, strengte_ambient;
	glm::vec3 CalcDirLight(Light light, glm::vec3 normal, glm::vec3 viewDir);
	bool type=true;
};
