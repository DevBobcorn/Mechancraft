#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "shader_s.h"
#include <iostream>
#include "Controller.h"
#include "Raycast.h"
#include "Models.h"
#include "Textures.h"
#include "Loader.h"
#include "EDiagram.h"
#include "Block.h"
#include "BlockData.h"
#include "Map.h"
#include "Renderer.h"
#include "Settings.h"

using namespace Mechanicraft;

// window
GLFWwindow* window;

// map
Map* worldMap;

int main()
{
	window = InitGL();
	if (window == nullptr) return 0;
	
	CreateFrameBuffer();
	
	// configure global opengl state
	// -----------------------------
	glEnable(GL_LESS);
	glEnable(GL_CULL_FACE);

	//Shaders
	Shader blockShader("./res/block.vert", "./res/block.frag");
	Shader screenShader("./res/screen.vert", "./res/screen.frag");

	// prepare model data
	// ------------------
	LoadModelForBlocks();

	unsigned int QuadVBO, QuadVAO;
	LoadPlaneModel(QuadVertices, sizeof(QuadVertices), &QuadVAO, &QuadVBO);

	unsigned int cursorTexID;
	int cursorWidth,cursorHeight,cursorNrChannel;
	LoadTexture("./res/tex/cursor.png", &cursorTexID, &cursorHeight, &cursorWidth, &cursorNrChannel);

	//Load Block Texture
	LoadTexForBlocks();

	//Load Damage Texture
	LoadTexForDamage();

	worldMap = new Map();
	
	ProcVoxel();
	
	worldMap->Load();
	
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	screenShader.setInt("cursor", 1);	
	blockShader.use();
	blockShader.setInt("texture1", 0);
	blockShader.setInt("texture2", 1);
	blockShader.setInt("destroyTex", 2);
	blockShader.setBool("RedstoneRetint", false);
	blockShader.setInt("PowerLevel", 0);

	float FullHardness = 10.0f;
	float RemainingHardness = 10.0f;
	float BuildCoolDown = 0.0f;
	int LastTargetX = -1, LastTargetY = -1, LastTargetZ = -1;
	bool TargetChanged = false;
	int DamageIndex = 0;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		// redstone update
		if (RedStoneRemainTime >= RedStoneUpdateTime) {
			worldMap->RunATick();
			RedStoneRemainTime = fmod(RedStoneRemainTime, RedStoneUpdateTime);
		}
		
		RedStoneRemainTime += deltaTime;
		
		// input
		// -----
		processInput(window);
		
		if (NeedSaving) worldMap->SaveCurrent();
		
		float Ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
		// calculate matrices
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projectionUI = glm::ortho<float>(-Ratio, Ratio, -1.0f, 1.0f, -200.0f, 200.0f);
		glm::mat4 viewUI = glm::lookAt(glm::vec3(0.0,0.0,-30.0), glm::vec3(0.0,0.0,0.0), glm::vec3(0.0, 1.0, 0.0));

		float intersection_distance;  // Output of TestRayOBBIntersection()
		float min_distance = 100000.0f; // current found nearest block / face distance
		bool target_found = false,printReport = false;
		int target_x = 0, target_y = 0, target_z = 0;

		// pick
		// ----
		glm::vec3 ray_origin;
		glm::vec3 ray_direction;
		ScreenPosToWorldRay(
			SCR_WIDTH / 2, SCR_HEIGHT / 2,
			SCR_WIDTH, SCR_HEIGHT,
			view,
			projection,
			ray_origin,
			ray_direction
		);

		glm::vec3 aabb_min;
		glm::vec3 aabb_max;
		int armLen = 5;
		// Only calculate those cubes around the camera
		for (int z = (int)(cameraPos.z) - armLen; z < (int)(cameraPos.z) + armLen + 1; z++)
			for (int x = (int)(cameraPos.x) - armLen; x < (int)(cameraPos.x) + armLen + 1; x++)
				for (int y = (int)(cameraPos.y) - armLen; y < (int)(cameraPos.y) + armLen; y++) {
					if (!worldMap->IsInWorld(x,y,z)) continue;
					if (worldMap->GetBlock(x,y,z)->GetID() == AirID) continue;
					// calculate the model matrix for each object before raycasting
					glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
					model = glm::translate(model, glm::vec3(x, y, z));
					// raycast
					switch (GetBlockType((worldMap->GetBlock(x, y, z)->GetID()))){
						case Normal:
						case SixSide:
						case Stem:
							aabb_min = BlockAABB.aabb_min;
							aabb_max = BlockAABB.aabb_max;
							break;
						case Layer:
							aabb_min = LayerAABB.aabb_min;
							aabb_max = LayerAABB.aabb_max;
							break;
						case TorchType:
							aabb_min = TorchAABB.aabb_min;
							aabb_max = TorchAABB.aabb_max;
							break;
						case RepeaterType:
						case ComparatorType:
							aabb_min = IOAABB.aabb_min;
							aabb_max = IOAABB.aabb_max;
							break;
						default:
							aabb_min = BlockAABB.aabb_min;
							aabb_max = BlockAABB.aabb_max;
							break;
					}
					if (TestRayOBBIntersection(ray_origin, ray_direction, aabb_min, aabb_max, model, intersection_distance)) {
						target_found = true;
						if (intersection_distance < min_distance) {
							//It's a nearer cube
							target_x = x;
							target_y = y;
							target_z = z;
							min_distance = intersection_distance;
						}
					}
				}

		if (target_found) {
			if (LastTargetX == target_x && LastTargetY == target_y && LastTargetZ == target_z) {
				TargetChanged = false;
			}
			else {
				TargetChanged = true;
				FullHardness = (IsCreativeMode) ? CreativeModeBreakTime : GetHardness(worldMap->GetBlock(target_x,target_y,target_z)->GetID());
				RemainingHardness = FullHardness;
				LastTargetX = target_x;
				LastTargetY = target_y;
				LastTargetZ = target_z;
			}
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
			//Destroy Cube
			if (target_found && (RemainingHardness <= 0.0f)) {
				worldMap->SetBlock(new AirBlock(),target_x,target_y,target_z);
				RemainingHardness = 0.0f;
			} else {
				RemainingHardness -= deltaTime * 0.5f;
				if (RemainingHardness < 0.0f) {
					worldMap->SetBlock(new AirBlock(),target_x,target_y,target_z);
					RemainingHardness = 0.0f;
				}
			}
		} else if (target_found && !TargetChanged) {
			FullHardness = (IsCreativeMode) ? CreativeModeBreakTime : GetHardness(worldMap->GetBlock(target_x,target_y,target_z)->GetID());
			RemainingHardness = FullHardness;
		}
		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
			if (target_found) {
				IDInHand = GetNumID(worldMap->GetBlock(target_x,target_y,target_z)->GetID());
			}
		}
		
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
			if (target_found && (BuildCoolDown <= 0.0f)) {
				BlockType curType = GetBlockType((worldMap->GetBlock(target_x, target_y, target_z)->GetID()));
				if (curType == Normal || curType == Stem || curType == SixSide){ //Build Cube
					int face = -1;
					// raycast again, this time, get the face
					//printf("CAST STARTED FROM: %f %f %f TO: %d %d %d\n", ray_origin.x, ray_origin.y, ray_origin.z, target_x,target_y,target_z);
					min_distance = 100000.0f; // re-initialize
					intersection_distance = 100000.0f; // re-initialize
					//aabbs of faces...
					// Faces                       // Facing +-X                                               // Facing +-Z                                              // Facing +-Y
					glm::vec3 face_aabb_min[6] = { glm::vec3( 0.4f,-0.5f,-0.5f), glm::vec3(-0.6f,-0.5f,-0.5f), glm::vec3(-0.5f,-0.5f, 0.4f), glm::vec3(-0.5f,-0.5f,-0.6f), glm::vec3(-0.5f, 0.4f,-0.5f), glm::vec3(-0.5f,-0.6f,-0.5f) };
					glm::vec3 face_aabb_max[6] = { glm::vec3( 0.6f, 0.5f, 0.5f), glm::vec3(-0.4f, 0.5f, 0.5f), glm::vec3( 0.5f, 0.5f, 0.6f), glm::vec3( 0.5f, 0.5f,-0.4f), glm::vec3( 0.5f, 0.6f, 0.5f), glm::vec3( 0.5f,-0.4f, 0.5f) };
					// raycast
					glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
					model = glm::translate(model, glm::vec3(target_x, target_y, target_z));
					for (int faceidx = 0; faceidx < 6; faceidx++) {
						//face_model = model;
						if (TestRayOBBIntersection(ray_origin, ray_direction, face_aabb_min[faceidx], face_aabb_max[faceidx], model, intersection_distance)) {
							//printf("	%d CASTED %.4f\n", faceidx, intersection_distance);
							if (intersection_distance < min_distance) {
								//It's a nearer face
								min_distance = intersection_distance;
								face = faceidx;
							}
						}
					}
					BlockDir NewDir;
					if (face != -1) {
						//printf("	%d CHOSEN\n", face);
						switch (face) {
							case 0:
								target_x++; NewDir = PosXDir; break;
							case 1:
								target_x--; NewDir = NegXDir; break;
							case 2:
								target_z++; NewDir = PosZDir; break;
							case 3:
								target_z--; NewDir = NegZDir; break;
							case 4:
								target_y++; NewDir = PosYDir; break;
							case 5:
								target_y--; NewDir = PosYDir; break; }
						if (worldMap->IsInWorld(target_x,target_y,target_z)) {
							if (worldMap -> GetBlock(target_x,target_y,target_z)->GetID() == AirID) {
								worldMap->SetBlock(GetNewBlock(GetBlockID(IDInHand)),target_x,target_y,target_z);
								if (GetBlockID(IDInHand) == RedStoneTorchID || GetBlockID(IDInHand) == TorchID) {
									dynamic_cast<TorchBlock*>(worldMap->GetBlock(target_x,target_y,target_z)) -> TorDir = DirToTorchDir(NewDir);
								} else if (GetBlockID(IDInHand) == RedStoneRepeaterID || GetBlockID(IDInHand) == RedStoneComparatorID) {
									dynamic_cast<IOBlock*>(worldMap->GetBlock(target_x,target_y,target_z)) -> FacDir = (IODir)(PlayerDir);
								}
							}
						}
					}
					BuildCoolDown = BuildCoolDownTime;	
				} else if (dynamic_cast<Interactable*>(worldMap->GetBlock(target_x, target_y, target_z)) != nullptr) {
					dynamic_cast<Interactable*>(worldMap->GetBlock(target_x, target_y, target_z))->Interact();
					worldMap->GetBlock(target_x, target_y, target_z)->MarkSelf(worldMap,position(target_x, target_y, target_z));
					BuildCoolDown = BuildCoolDownTime;	
				}
			}
		}
		if (BuildCoolDown >= 0.0f) BuildCoolDown -= deltaTime;

		// render
		// ------
		glEnable(GL_DEPTH_TEST);//Re-active Depth Test
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		blockShader.use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		blockShader.setMat4("projection", projection);
		// camera/view transformation
		blockShader.setMat4("view", view);
		
		// not selected by default
		blockShader.setBool("selected", false);
		blockShader.setFloat("wireWidth", 0.02);
		blockShader.setInt("wireMode", 0);

		// render blocks
		for (int z = 0; z < MAPMAX; z++)
			for (int x = 0; x < MAPMAX; x++)
				for (int y = 0; y < WORLDTOP; y++) {
					if (worldMap->GetBlock(x,y,z)->GetID() == 0) continue;
					// calculate the model matrix for each object and pass it to shader before drawing
					glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
					model = glm::translate(model, glm::vec3(x,y,z));
					bool selected = false;
					Block* BlockPt = worldMap->GetBlock(x,y,z);
					if (target_found && x == target_x && y == target_y && z == target_z) {
						selected = true;
					}
					int damageIndex = (IsCreativeMode) ? 0 : floor((1 - (RemainingHardness / FullHardness)) * (float)(DamageNum - 1));
					RenderBlock(BlockPt,selected,damageIndex,model,&blockShader);
				}
		
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
								  // clear all relevant buffers
		
		if (ShowInventory) {
			// pass projection matrix to shader (note that in this case it could change every frame)
			blockShader.setMat4("projection", projectionUI);
			// camera/view transformation
			blockShader.setMat4("view", viewUI);
			//float TotWidth = ;
			for (int itemidx = 1; itemidx < BlockNum; itemidx++) {
				glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
				model = glm::translate(model, glm::vec3((float)(-BlockNum / 2.0 + itemidx) * 0.2f,-0.2f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, (float)(PI / 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, (float)(PI / -4.7f), glm::vec3(1.0f, 0.0f, 1.0f));
				if (itemidx == IDInHand) {
					blockShader.setBool("selected", true);
					blockShader.setFloat("wireWidth", 0.09);
					blockShader.setInt("wireMode", 1);
					model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
				}
				blockShader.setMat4("model", model);
				glBindVertexArray(SmallBlockVAO);
				glBindTexture(GL_TEXTURE_2D, texID[BlockIDToTexIndex[itemidx]]);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				if (itemidx == IDInHand) blockShader.setBool("selected", false);
				
				//RenderBlock(worldMap,x,y,z,selected,damageIndex,model,&blockShader);
			}			
		}

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		float fract = (float)SCR_HEIGHT / (float)SCR_WIDTH;
		//std::cout << cursorWidth << ' ' << cursorHeight << std::endl;
		screenShader.setFloat("cursorVert", ((float)cursorHeight / (float)SCR_HEIGHT) / 2.0f * cursorScale);
		screenShader.setFloat("cursorHorz", ((float)cursorWidth / (float)SCR_WIDTH) / 2.0f * cursorScale);
		glBindVertexArray(QuadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cursorTexID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &BlockVAO);
	glDeleteBuffers(1, &BlockVBO);
	glDeleteVertexArrays(1, &StemSideVAO);
	glDeleteBuffers(1, &StemSideVBO);
	glDeleteVertexArrays(1, &StemEndVAO);
	glDeleteBuffers(1, &StemEndVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

