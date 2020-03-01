#pragma once

#include <iostream>
#include <glm/glm.hpp>

using namespace std;
const float BlockVertices[] = {
	//Back
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//1
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,//2
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//3
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//Facade
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//1
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//2
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//3
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//Left
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//1
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,//2
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//3
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

	//Right
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //1
	0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //2
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //3
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

	//Bottom
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//Top
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const float TorchThickness = 0.0625f; // Half Thickness
const float TorchHeight = 0.625f;

const float TorchVertices[] = {
	//Back
	
	 0.5f, -0.5f, -TorchThickness,  0.0f, 0.0f,//1
	-0.5f, -0.5f, -TorchThickness,  1.0f, 0.0f,//2 
	-0.5f,  0.5f, -TorchThickness,  1.0f, 1.0f,//3
	-0.5f,  0.5f, -TorchThickness,  1.0f, 1.0f,
	 0.5f,  0.5f, -TorchThickness,  0.0f, 1.0f,
	 0.5f, -0.5f, -TorchThickness,  0.0f, 0.0f,

	//Facade
	-0.5f, -0.5f,  TorchThickness,  1.0f, 0.0f,//2
	 0.5f, -0.5f,  TorchThickness,  0.0f, 0.0f,//1
	-0.5f,  0.5f,  TorchThickness,  1.0f, 1.0f,//3
	 0.5f,  0.5f,  TorchThickness,  0.0f, 1.0f,
	-0.5f,  0.5f,  TorchThickness,  1.0f, 1.0f, 
	 0.5f, -0.5f,  TorchThickness,  0.0f, 0.0f,

	//Left
	-TorchThickness,  0.5f,  0.5f,  1.0f, 1.0f,//1
	-TorchThickness,  0.5f, -0.5f,  0.0f, 1.0f,//2
	-TorchThickness, -0.5f, -0.5f,  0.0f, 0.0f,//3
	-TorchThickness, -0.5f, -0.5f,  0.0f, 0.0f,
	-TorchThickness, -0.5f,  0.5f,  1.0f, 0.0f,
	-TorchThickness,  0.5f,  0.5f,  1.0f, 1.0f,

	//Right
	TorchThickness,  0.5f, -0.5f,  0.0f, 1.0f, //2
	TorchThickness,  0.5f,  0.5f,  1.0f, 1.0f, //1
	TorchThickness, -0.5f, -0.5f,  0.0f, 0.0f, //3
	TorchThickness, -0.5f,  0.5f,  1.0f, 0.0f,
	TorchThickness, -0.5f, -0.5f,  0.0f, 0.0f,
	TorchThickness,  0.5f,  0.5f,  1.0f, 1.0f
};

const float SmallBlockVertices[] = {
	//Back
	0.1f, -0.1f, -0.1f,  1.0f, 0.0f,//2
	-0.1f, -0.1f, -0.1f,  0.0f, 0.0f,//1
	0.1f,  0.1f, -0.1f,  1.0f, 1.0f,//3
	-0.1f,  0.1f, -0.1f,  0.0f, 1.0f,
	0.1f,  0.1f, -0.1f,  1.0f, 1.0f,
	-0.1f, -0.1f, -0.1f,  0.0f, 0.0f,

	//Facade
	-0.1f, -0.1f,  0.1f,  0.0f, 0.0f,//1
	0.1f, -0.1f,  0.1f,  1.0f, 0.0f,//2
	0.1f,  0.1f,  0.1f,  1.0f, 1.0f,//3
	0.1f,  0.1f,  0.1f,  1.0f, 1.0f,
	-0.1f,  0.1f,  0.1f,  0.0f, 1.0f,
	-0.1f, -0.1f,  0.1f,  0.0f, 0.0f,

	//Left
	-0.1f,  0.1f,  0.1f,  1.0f, 1.0f,//1
	-0.1f,  0.1f, -0.1f,  0.0f, 1.0f,//2
	-0.1f, -0.1f, -0.1f,  0.0f, 0.0f,//3
	-0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
	-0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
	-0.1f,  0.1f,  0.1f,  1.0f, 1.0f,

	//Right
	0.1f,  0.1f, -0.1f,  0.0f, 1.0f, //2
	0.1f,  0.1f,  0.1f,  1.0f, 1.0f, //1
	0.1f, -0.1f, -0.1f,  0.0f, 0.0f, //3
	0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
	0.1f, -0.1f, -0.1f,  0.0f, 0.0f,
	0.1f,  0.1f,  0.1f,  1.0f, 1.0f,

	//Bottom
	-0.1f, -0.1f, -0.1f,  0.0f, 1.0f,
	0.1f, -0.1f, -0.1f,  1.0f, 1.0f,
	0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
	0.1f, -0.1f,  0.1f,  1.0f, 0.0f,
	-0.1f, -0.1f,  0.1f,  0.0f, 0.0f,
	-0.1f, -0.1f, -0.1f,  0.0f, 1.0f,

	//Top
	0.1f,  0.1f, -0.1f,  1.0f, 1.0f,
	-0.1f,  0.1f, -0.1f,  0.0f, 1.0f,
	0.1f,  0.1f,  0.1f,  1.0f, 0.0f,
	-0.1f,  0.1f,  0.1f,  0.0f, 0.0f,
	0.1f,  0.1f,  0.1f,  1.0f, 0.0f,
	-0.1f,  0.1f, -0.1f,  0.0f, 1.0f
};

const float StemSideVertices[] = {
	//Back
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//1
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,//2
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//3
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//Facade
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//1
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//2
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//3
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//Left
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//1
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,//2
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//3
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

	//Right
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //1
	0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //2
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //3
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f
};

const float StemEndVertices[] = {
	//Bottom
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//Top
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

const float LayerDefaultHeight = 0.01f;

const float LayerBottomVertices[] = {
	 0.5f,  -0.49f, -0.5f,  1.0f, 1.0f,
	-0.5f,  -0.49f, -0.5f,  0.0f, 1.0f,
	 0.5f,  -0.49f,  0.5f,  1.0f, 0.0f,
	-0.5f,  -0.49f,  0.5f,  0.0f, 0.0f,
	 0.5f,  -0.49f,  0.5f,  1.0f, 0.0f,
	-0.5f,  -0.49f, -0.5f,  0.0f, 1.0f
};

const float LayerSideDownVertices[] = {
	-0.51f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.51f, -0.5f, -0.5f,  1.0f, 1.0f,
	-0.51f, -1.5f, -0.5f,  0.0f, 1.0f,
	-0.51f, -1.5f, -0.5f,  0.0f, 1.0f,
	-0.51f, -1.5f,  0.5f,  0.0f, 0.0f,
	-0.51f, -0.5f,  0.5f,  1.0f, 0.0f
};

const float LayerSideUpVertices[] = {
	0.49f,  0.5f, -0.5f,  1.0f, 1.0f,	
	0.49f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.49f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.49f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.49f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.49f, -0.5f, -0.5f,  0.0f, 1.0f
};

const float QuadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

struct AABB{
	glm::vec3 aabb_min,aabb_max;
};

const AABB BlockAABB = { glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.5f,0.5f,0.5f) };

const AABB LayerAABB = { glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.5f,-0.4f,0.5f) };

const AABB TorchAABB = { glm::vec3(-TorchThickness,-0.5f,-TorchThickness), glm::vec3(TorchThickness,TorchHeight - 0.5f,TorchThickness) };

const AABB IOAABB = { glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.5f,-0.375f,0.5f) };
