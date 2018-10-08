#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 n;


uniform mat4 transformPos;


out vec3 norm;


void main()
{
    gl_Position = aPos;

	gl_Position = transformPos * gl_Position;
	gl_Position = gl_Position / gl_Position.w; // now he's NDC!

	vec4 transNorm = vec4(n, 1.0);
	norm = vec3(transNorm.x, transNorm.y, transNorm.z);

}
