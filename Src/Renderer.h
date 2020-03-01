#pragma once

#include <bitset>
#include <cmath>
#include <cstdlib> // Header file needed to use srand and rand
#include <ctime> // Header file needed to use time
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader_s.h"
#include "Map.h"
#include "Loader.h"
#include "Block.h"
#include "BlockData.h"

using namespace std;

namespace Mechanicraft{
	static void RenderVoxel(int* VoxelPointer,unsigned int TexID,glm::mat4 model,Shader* blockShader){
		glm::mat4 tmpModel;
		blockShader->setIntArr("mask", 256, VoxelPointer);
		glBindTexture(GL_TEXTURE_2D, TexID);
		glBindVertexArray(LayerBottomVAO);
		for (int fac = 1;fac <= 6;fac++){
			blockShader->setInt("masked", fac); //Top
			switch (fac){
				case 1:
					tmpModel = model; /* Top */ break;
				case 2:
					tmpModel = glm::rotate(model, (float)(PI), glm::vec3(1.0f, 0.0f,0.0f)); /* Bottom */ break;
				case 3:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(-1.0f, 0.0f,0.0f)); /* Neg Z */ break;
				case 4:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3( 1.0f, 0.0f,0.0f)); /* Pos Z */ break;
				case 5:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 0.0f,-1.0f)); /* Neg X */ break;
				case 6:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f)); /* Pos X */ break; }
			for (int curLayer = 0;curLayer < 16;curLayer++){
				blockShader->setMat4("model", glm::translate(tmpModel, glm::vec3(0.0,(curLayer + 1) * 0.0625 - LayerDefaultHeight,0.0)));
				blockShader->setInt("layer", curLayer);
				glDrawArrays(GL_TRIANGLES, 0, 6); }
		}
		blockShader->setInt("masked", 0);
	}
	
	int RdrTempVoxelArr[256] = { 0 };
	
	static void RenderCuboid(int top,int bottom,int posX,int negX,int posZ,int negZ,unsigned int TexID,glm::mat4 model,Shader* blockShader){ //ALL BETWEEN 0 TO 15
		glm::mat4 tmpModel;
		memset(RdrTempVoxelArr,0,sizeof(RdrTempVoxelArr));
		int xl = 15 - negX,xr = posX;
		int yl = 15 - bottom,yr = top;
		int zl = 15 - negZ,zr = posZ;
		if (xl > xr) swap(xl,xr);
		if (yl > yr) swap(yl,yr);
		if (zl > zr) swap(zl,zr);
		for (int ly = yl;ly <= yr;ly++)
			for (int x = xl;x <= xr;x++){
				for (int z = 0;z <= 15;z++){ // z must ..., for there might be empty slots...
					if (z >= zl && z <= zr) 
						RdrTempVoxelArr[ly * 16 + x] |= 1;
					RdrTempVoxelArr[ly * 16 + x] <<= 1;
				}
				RdrTempVoxelArr[ly * 16 + x] >>= 1;
			}
		/*
		for (int ly = 0;ly < 16;ly++){
			for (int x = 0;x < 16;x++){
				cout << bitset<16>(RdrTempVoxelArr[ly * 16 + x]) << endl;
			}
			cout << endl;
		}
		*/
		blockShader->setIntArr("mask", 256, &RdrTempVoxelArr[0]);
		glBindTexture(GL_TEXTURE_2D, TexID);
		glBindVertexArray(LayerBottomVAO);
		int curLayer;
		for (int fac = 1;fac <= 6;fac++){
			blockShader->setInt("masked", fac); //Top
			switch (fac){
				case 1:
					tmpModel = model; curLayer = top; /* Top */ break;
				case 2:
					tmpModel = glm::rotate(model, (float)(PI), glm::vec3(1.0f, 0.0f,0.0f)); curLayer = bottom; /* Bottom */ break;
				case 3:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(-1.0f, 0.0f,0.0f)); curLayer = negZ; /* Neg Z */ break;
				case 4:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3( 1.0f, 0.0f,0.0f)); curLayer = posZ; /* Pos Z */ break;
				case 5:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 0.0f,-1.0f)); curLayer = posX; /* Neg X */ break;
				case 6:
					tmpModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 0.0f, 1.0f)); curLayer = negX; /* Pos X */ break; }
			blockShader->setMat4("model", glm::translate(tmpModel, glm::vec3(0.0,(curLayer + 1) * 0.0625 - LayerDefaultHeight,0.0)));
			blockShader->setInt("layer", curLayer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		blockShader->setInt("masked", 0);
	}
	
	static void RenderBlock(Block* BlockPointer,bool selected,int DamageIndex,glm::mat4 model,Shader* blockShader){
		if (selected){
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, DamageTexID[DamageIndex]);
		}
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		TorchBlock* curTorch;//Must be placed outside.
		RedStoneRepeaterBlock* curReptr;
		RedStoneComparatorBlock* curComptr;
		
		int BottomDir = 0,DiagDir = 0;
		bool curIsOn;
		switch (GetBlockType(BlockPointer->GetID())) {
			case Normal:
				if (selected) blockShader->setBool("selected", true);
				blockShader->setMat4("model", model);
				glBindVertexArray(BlockVAO);
				glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()]]);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				break;
			case Stem:
				if (selected) blockShader->setBool("selected", true);
				blockShader->setMat4("model", model);
				glBindVertexArray(StemSideVAO);
				glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()]]);
				glDrawArrays(GL_TRIANGLES, 0, 24);
				glBindVertexArray(StemEndVAO);
				glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 1]);
				glDrawArrays(GL_TRIANGLES, 0, 12);
				break;
			case TorchType:
				curTorch = dynamic_cast<TorchBlock*>(BlockPointer);
				if (curTorch->TorDir != TorchUpDir){
					switch (curTorch->TorDir){
						case TorchNegZDir:
							model = glm::translate(model, glm::vec3(0.0,0.1875,0.31));
							model = glm::rotate(model, (float)(PI / 8.0f), glm::vec3(-1.0f, 0.0f,0.0f));
							break;
						case TorchPosZDir:
							model = glm::translate(model, glm::vec3(0.0,0.1875,-0.31));
							model = glm::rotate(model, (float)(PI / 8.0f), glm::vec3( 1.0f, 0.0f,0.0f));
							break;
						case TorchNegXDir:
							model = glm::translate(model, glm::vec3(0.31,0.1875,0.0));
							model = glm::rotate(model, (float)(PI / 8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
							break;
						case TorchPosXDir:
							model = glm::translate(model, glm::vec3(-0.31,0.1875,0.0));
							model = glm::rotate(model, (float)(PI / 8.0f), glm::vec3(0.0f, 0.0f,-1.0f));
							break;
					}
				}
				blockShader->setMat4("model", model);
				glBindVertexArray(TorchVAO);
				curIsOn = curTorch->IsOn;
				if (curIsOn) {
					if (BlockPointer->GetID() == TorchID)
						glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[TorchID]]);
					else {
						glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID]]);
					}					
				} else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 2]);

				glDrawArrays(GL_TRIANGLES, 0, 24);
				blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,TorchHeight - LayerDefaultHeight,0.0)));
				if (curIsOn){
					if (BlockPointer->GetID() == TorchID)
						glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[TorchID] + 1]);
					else {
						glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[TorchID] + 1]);
					}					
				} else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 1]);

				glBindVertexArray(LayerBottomVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				model = glm::scale(model, glm::vec3(0.25,0.65,0.25));
				blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-0.175,0.0)));
				if (selected) blockShader->setBool("selected", true);
				glBindVertexArray(BlockVAO);
				glBindTexture(GL_TEXTURE_2D, airTexID);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				break;
			case Layer:
				if (selected) blockShader->setBool("selected", true);
				if (GetRedType(BlockPointer->GetID()) == RSDust) {
					if (BlockPointer->GetID() == RedStoneDustID){
						RedStoneDustBlock* powder = dynamic_cast<RedStoneDustBlock*>(BlockPointer);
						blockShader->setInt("PowerLevel", powder->PowerLev);
						BottomDir = powder -> ConnectDir;
						DiagDir = powder -> DiagDir;
						glm::mat4 BottomModel = model;
						//glm::mat4 DiagModel = model;
						switch (BottomDir) {
							case 0:
								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 4]);//dot
								break;
							case 15:
								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()]]);//cross
								break;
							case 1:
							case 4:
							case 5: //x
								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 2]);//wire -> x
 								break;
 							case 2:
							case 8:
							case 10: //z
								BottomModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 2]);//wire -> x
 								break;
 							case 12:
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 3]);//corner -> +x,+z
 								break;
 							case 9:
 								BottomModel = glm::rotate(model,-(float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 3]);//corner -> -x,+z
 								break;
 							case 3:
 								BottomModel = glm::rotate(model, (float)(PI), glm::vec3(0.0f, 1.0f, 0.0f));
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 3]);//corner -> -x,-z
 								break;
 							case 6:
 								BottomModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 3]);//corner -> +x,-z
 								break;
 							case 7:
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 1]);//triCross -> x,-z
 								break;
 							case 14:
 								BottomModel = glm::rotate(model,-(float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 1]);//triCross -> +x,z
 								break;
 							case 13:
 								BottomModel = glm::rotate(model, (float)(PI), glm::vec3(0.0f, 1.0f, 0.0f));
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 1]);//triCross -> x,+z
 								break;
 							case 11:
 								BottomModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 								glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 1]);//triCross -> -x,z
 								break;
						}
						blockShader->setMat4("model", BottomModel);
						blockShader->setBool("RedstoneRetint", true);
						glBindVertexArray(LayerBottomVAO);
						glDrawArrays(GL_TRIANGLES, 0, 6);
						
						glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 2]);
						if (DiagDir & 1) {
							//BottomModel = model;
							BottomModel = glm::rotate(model, (float)(PI), glm::vec3(0.0f, 1.0f, 0.0f));
							blockShader->setMat4("model", BottomModel);
							glBindVertexArray(LayerSideUpVAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (DiagDir & 2) {
							BottomModel = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
							blockShader->setMat4("model", BottomModel);
							glBindVertexArray(LayerSideUpVAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (DiagDir & 4) {
							//BottomModel = glm::rotate(model, (float)(PI), glm::vec3(0.0f, 1.0f, 0.0f));
							BottomModel = model;
							blockShader->setMat4("model", BottomModel);
							glBindVertexArray(LayerSideUpVAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
						if (DiagDir & 8) {
							BottomModel = glm::rotate(model,-(float)(PI / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
							blockShader->setMat4("model", BottomModel);
							glBindVertexArray(LayerSideUpVAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
					}
				} else {
					glBindVertexArray(LayerBottomVAO);
					glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[BlockPointer->GetID()] + 2]);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				break;
			case RepeaterType:
				curReptr = dynamic_cast<RedStoneRepeaterBlock*>(BlockPointer);
				switch (curReptr->FacDir){
					case IOPosXDir:
						model = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f,-1.0f,0.0f));
						break;
					case IONegXDir:
						model = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 1.0f,0.0f));
						break;
					case IOPosZDir:
						model = glm::rotate(model, (float)(PI), glm::vec3(0.0f, 1.0f,0.0f));
						break;
				}
				blockShader->setMat4("model", model);
				glBindVertexArray(StemSideVAO);
				glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneRepeaterID] + 2]);
				glDrawArrays(GL_TRIANGLES, 0, 24);
				
				glBindVertexArray(TorchVAO);
				if (curReptr->OutputLev) glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID]]);
				else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 2]);
				if (!curReptr->Locked) {
					blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-0.1875,-0.1875 + curReptr->Delay * 0.125)));
					glDrawArrays(GL_TRIANGLES, 0, 24);					
				}
				blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-0.1875,-0.3125)));
				glDrawArrays(GL_TRIANGLES, 0, 24);
				
				glBindVertexArray(LayerBottomVAO);
				if (curReptr->OutputLev) glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[TorchID] + 1]);
				else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 1]);
				
				blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-LayerDefaultHeight + 0.4375,-0.3125)));
				glDrawArrays(GL_TRIANGLES, 0, 6);
				
				if (!curReptr->Locked) {
					blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-LayerDefaultHeight + 0.4375,-0.1875 + curReptr->Delay * 0.125)));
					glDrawArrays(GL_TRIANGLES, 0, 6);					
				} else { //Draw the bedrock lock
					RenderCuboid(3,15,13,13,7,9,texID[BlockIDToTexIndex[BedrockID]],model,blockShader);
				}
				
				if (selected) blockShader->setBool("selected", true);
				blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-LayerDefaultHeight + 0.125,0.0)));
				if (curReptr->OutputLev) glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneRepeaterID]]);
				else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneRepeaterID] + 1]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				break;
			case ComparatorType:
				curComptr = dynamic_cast<RedStoneComparatorBlock*>(BlockPointer);
				switch (curComptr->FacDir){
					case IOPosXDir:
						model = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f,-1.0f,0.0f));
						break;
					case IONegXDir:
						model = glm::rotate(model, (float)(PI / 2.0f), glm::vec3(0.0f, 1.0f,0.0f));
						break;
					case IOPosZDir:
						model = glm::rotate(model, (float)(PI), glm::vec3(0.0f, 1.0f,0.0f));
						break;
				}
				blockShader->setMat4("model", model);
				glBindVertexArray(StemSideVAO);
				glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneRepeaterID] + 2]);
				glDrawArrays(GL_TRIANGLES, 0, 24);

				glBindVertexArray(TorchVAO);
				if (curComptr->OutputLev) {
					glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID]]);
				} else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 2]);
				blockShader->setMat4("model", glm::translate(model, glm::vec3(-0.1875,-0.1875, 0.25)));
				glDrawArrays(GL_TRIANGLES, 0, 24);
				blockShader->setMat4("model", glm::translate(model, glm::vec3( 0.1875,-0.1875, 0.25)));
				glDrawArrays(GL_TRIANGLES, 0, 24);
				
				if (curComptr->FrontOn) {
					glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID]]);
					blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-0.1875,-0.3125)));
				} else {
					glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 2]);
					blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-0.375,-0.3125)));
				}
				glDrawArrays(GL_TRIANGLES, 0, 24);
				
				glBindVertexArray(LayerBottomVAO);
				if (curComptr->OutputLev) glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[TorchID] + 1]);
				else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 1]);
				blockShader->setMat4("model", glm::translate(model, glm::vec3(-0.1875,-LayerDefaultHeight + 0.4375, 0.25)));
				glDrawArrays(GL_TRIANGLES, 0, 6);
				blockShader->setMat4("model", glm::translate(model, glm::vec3( 0.1875,-LayerDefaultHeight + 0.4375, 0.25)));
				glDrawArrays(GL_TRIANGLES, 0, 6);
				
				if (curComptr->FrontOn) {
					glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[TorchID] + 1]);
					blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-LayerDefaultHeight + 0.4375,-0.3125)));
				} else {
					glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneTorchID] + 1]);
					blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-LayerDefaultHeight + 0.25,-0.3125)));
				}
				glDrawArrays(GL_TRIANGLES, 0, 6);
				
				if (selected) blockShader->setBool("selected", true);
				blockShader->setMat4("model", glm::translate(model, glm::vec3(0.0,-LayerDefaultHeight + 0.125,0.0)));
				if (curComptr->OutputLev) glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneComparatorID]]);
				else glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[RedStoneComparatorID] + 1]);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				
				//RenderVoxel(&tempVoxelArr[0],texID[BlockIDToTexIndex[GoldID]],model,blockShader);
				//RenderCuboid(15,14,13,12,11,10,texID[BlockIDToTexIndex[GoldID]],model,blockShader);
				break;
			default:
				if (selected) blockShader->setBool("selected", true);
				blockShader->setMat4("model", model);
				glBindVertexArray(BlockVAO);
				glBindTexture(GL_TEXTURE_2D, missingTexID);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				break;
		}
		if (selected) blockShader->setBool("selected", false);
		if (GetRedType(BlockPointer->GetID()) == RSDust) blockShader->setBool("RedstoneRetint", false);
	}
}
