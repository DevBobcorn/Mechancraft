#pragma once

#include "Models.h"
#include <memory.h>
#include <bitset>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Settings.h"
#include "BlockData.h"

namespace Mechanicraft{
	//GL ID Storage
	unsigned int frameBuffer;
	unsigned int textureColorbuffer;
	unsigned int renderBufferObject;
	//Blocks
	unsigned int airTexID; //Not really air's just an empty one.
	unsigned int missingTexID;
	unsigned int texID[TexMax];
	int BlockIDToTexIndex[BlockMax];
	
	unsigned int SmallBlockVBO, SmallBlockVAO, BlockVBO, BlockVAO, StemSideVBO, StemSideVAO, StemEndVBO, StemEndVAO,
				 LayerBottomVBO, LayerBottomVAO, LayerSideUpVBO, LayerSideUpVAO, LayerSideDownVBO, LayerSideDownVAO,
				 TorchVBO, TorchVAO;
	//Damage
	unsigned int DamageTexID[DamageMax];
	
	int tempVoxelArr[256];
	int colorVoxelArr[4096];
	
	bool needsDrawing[16] = { false };
	
	unsigned char getRed(int x, int y, int n, int w, unsigned char *data) {
		return data[w * n * x + y * n + n - 4];
	}

	unsigned char getGreen(int x, int y, int n, int w, unsigned char *data) {
		return data[w * n * x + y * n + n - 3];
	}
	
	unsigned char getBlue(int x, int y, int n, int w, unsigned char *data) {
		return data[w * n * x + y * n + n - 2];
	}
	
	unsigned char getAlpha(int x, int y, int n, int w, unsigned char *data) {
		return data[w * n * x + y * n + n - 1];
	}
	
	static void LoadVoxel(){
		const char *filepath = "./res/tex/toxel.png";
		int w, h, n;
		unsigned char *data = stbi_load(filepath, &w, &h, &n, 0);
		if (data == NULL) {
			cout << "ERROE_FILE_NOT_LOAD" << endl;
			return;
		}
		else {
			if (w != 16 || h != 256) cout << "Incorrect Size: " << w << " " << h << " " << n << endl;
			for (int ly = 0;ly < 16;ly++)
				for (int x = 0; x < 16; x++) {
					for (int y = 0; y < 16; y++) {
						if (getRed(ly * 16 + x, y, n, w, data) == 0 && getGreen(ly * 16 + x, y, n, w, data) == 0 && getBlue(ly * 16 + x, y, n, w, data) == 0){
							tempVoxelArr[ly * 16 + x] |= 1;
						}
						tempVoxelArr[ly * 16 + x] <<= 1;
					}
					tempVoxelArr[ly * 16 + x] >>= 1;
				}
		}
		stbi_image_free(data);
	}
	
	static void LoadColorVoxel(){
		const char *filepath = "./res/tex/tulipOrange.png";
		int w, h, n;
		unsigned char *data = stbi_load(filepath, &w, &h, &n, 0);
		if (data == NULL) {
			cout << "ERROE_FILE_NOT_LOAD" << endl;
			return;
		}
		else {
			if (w != 16 || h != 256) cout << "Incorrect Size: " << w << " " << h << " " << n << endl;
			for (int ly = 0;ly < 16;ly++)
				for (int x = 0; x < 16; x++) {
					for (int y = 0; y < 16; y++) {
						if (getAlpha(ly * 16 + x, y, n, w, data) != 0){
							colorVoxelArr[ly * 256 + x * 16 + y] = getRed(ly * 16 + x, y, n, w, data) * 256 + getGreen(ly * 16 + x, y, n, w, data) * 16 + getBlue(ly * 16 + x, y, n, w, data);
						}
					}
				}
		}
		stbi_image_free(data);
	}
	
	static void ProcVoxel(){
		memset(tempVoxelArr,0,sizeof(tempVoxelArr));
		/*
		for (int ly = 0;ly < 16;ly++)
			for (int x = 0;x < 16;x++){
				for (int y = 0;y < 16;y++){
					if (x * x + y * y + ly * ly <= 128) {
						tempVoxelArr[ly * 16 + x] |= 1;
						//if (ly == 15 || !tempVoxelArr[(ly + 1) * 16 + x]) needsDrawing[ly] |= 1;
					}
					tempVoxelArr[ly * 16 + x] <<= 1;
				}
				tempVoxelArr[ly * 16 + x] >>= 1;
			}
		*/
		LoadVoxel();
		/*
		for (int ly = 0;ly < 16;ly++){
			for (int x = 0;x < 16;x++){
				cout << bitset<16>(tempVoxelArr[ly * 16 + x]) << endl;
			}
			cout << endl;
		}
		*/
	}
	
	void LoadModel(const float Verticesa[], int VertSize, GLuint* ModelVAOID, GLuint* ModelVBOID) {
		glGenVertexArrays(1, ModelVAOID);
		glGenBuffers(1, ModelVBOID);
	
		glBindVertexArray(*ModelVAOID);
	
		glBindBuffer(GL_ARRAY_BUFFER, *ModelVBOID);
		glBufferData(GL_ARRAY_BUFFER, VertSize, Verticesa, GL_STATIC_DRAW);
	
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	
	void LoadPlaneModel(const float Verticesa[], int VertSize, GLuint* ModelVAOID, GLuint* ModelVBOID) {
		glGenVertexArrays(1, ModelVAOID);
		glGenBuffers(1, ModelVBOID);
	
		glBindVertexArray(*ModelVAOID);
	
		glBindBuffer(GL_ARRAY_BUFFER, *ModelVBOID);
		glBufferData(GL_ARRAY_BUFFER, VertSize, Verticesa, GL_STATIC_DRAW);
	
		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	
	void LoadTexture(const char* TexFileName, GLuint* TexID, int* height, int* width, int* nrChannels) {
		// load and create a texture 
		// -------------------------
		glGenTextures(1, TexID);
		glBindTexture(GL_TEXTURE_2D, *TexID);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// load image, create texture and generate mipmaps
		//int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char *data = stbi_load(TexFileName, width, height, nrChannels, 0);
		//std::cout << *width << ' ' << *height << std::endl;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	
	void LoadTexture(const char* TexFileName, GLuint* TexID) {
		// load and create a texture 
		// -------------------------
		glGenTextures(1, TexID);
		glBindTexture(GL_TEXTURE_2D, *TexID);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		unsigned char *data = stbi_load(TexFileName, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	
	void LoadTexForBlocks(){
		LoadTexture(AirTex.c_str(), &airTexID);
		LoadTexture(MissingTex.c_str(), &missingTexID);
		BlockNum = GetBlockNum();
		int texidx = 0;
		for (int blockID = 1; blockID < BlockNum; blockID++) { // Skip Air
			BlockIDToTexIndex[blockID] = texidx; // record every block's first texture index...
			for (int ti = 0;ti < BlockDatas[blockID].texNum;ti++) {
				LoadTexture((BlockDatas[blockID].tex[ti]).c_str(), &texID[texidx]);
				texidx++;	
			}
		}
	}
	
	void LoadTexForDamage(){
		DamageNum = sizeof(DamageImages) / sizeof(DamageImages[0]);
		for (int dmgidx = 0;dmgidx < DamageNum;dmgidx++)
			LoadTexture((DamageImages[dmgidx]).c_str(), &DamageTexID[dmgidx]);
	}
	
	void LoadModelForBlocks(){
		LoadModel(SmallBlockVertices, sizeof(SmallBlockVertices), &SmallBlockVAO, &SmallBlockVBO);
		LoadModel(BlockVertices, sizeof(BlockVertices), &BlockVAO, &BlockVBO);
		LoadModel(StemSideVertices, sizeof(StemSideVertices), &StemSideVAO, &StemSideVBO);
		LoadModel(StemEndVertices, sizeof(StemEndVertices), &StemEndVAO, &StemEndVBO);
		LoadModel(LayerBottomVertices, sizeof(LayerBottomVertices), &LayerBottomVAO, &LayerBottomVBO);
		LoadModel(LayerSideDownVertices, sizeof(LayerSideDownVertices), &LayerSideDownVAO, &LayerSideDownVBO);
		LoadModel(LayerSideUpVertices, sizeof(LayerSideUpVertices), &LayerSideUpVAO, &LayerSideUpVBO);
		LoadModel(TorchVertices, sizeof(TorchVertices), &TorchVAO, &TorchVBO);
	}
	
	void CreateFrameBuffer() {
		// framebuffer configuration
		// -------------------------
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		// create a color attachment texture
	
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
		glGenRenderbuffers(1, &renderBufferObject);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject); // now actually attach it
																									  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}	
}
