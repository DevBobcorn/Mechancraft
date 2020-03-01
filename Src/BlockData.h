#pragma once

#include <cstring>
#include "Textures.h"

using namespace std;
using namespace Mechanicraft;

enum BlockType { Normal, Stem, SixSide, Layer, Plant, TorchType, RepeaterType, ComparatorType };
enum RSType { RSNone, RSOpacity, RSGlass, RSDust, RSSwitch, RSPower, RSTorch, RSIO };
enum BlockDir { PosXDir = 0, NegXDir, PosZDir, NegZDir, PosYDir, NegYDir };
enum TorchDir { TorchPosXDir = 0, TorchNegXDir, TorchPosZDir, TorchNegZDir, TorchUpDir };
enum IODir { IOPosXDir = 0, IONegXDir, IOPosZDir, IONegZDir };
enum BlockID { AirID, IronID, BedrockID, DiamondID, GoldID, RedStoneDustID, RedStoneCubeID, TorchID, RedStoneTorchID, RedStoneRepeaterID, RedStoneComparatorID,
			   IronArrowID };
	
namespace Mechanicraft{
	static TorchDir DirToTorchDir(BlockDir dir){
		switch (dir){
			case PosXDir:
				return TorchPosXDir;
			case NegXDir:
				return TorchNegXDir;
			case PosZDir:
				return TorchPosZDir;
			case NegZDir:
				return TorchNegZDir;
			case PosYDir:
			case NegYDir:
				return TorchUpDir;
		}
	}
	
	static int GetNumID(BlockID Wanted){   //Enum -> Numeral ID
		switch (Wanted){
			case AirID:
				return 0;
			case IronID:
				return 1;
			case BedrockID:
				return 2;
			case DiamondID:
				return 3;
			case GoldID:
				return 4;
			case RedStoneDustID:
				return 5;
			case RedStoneCubeID:
				return 6;
			case TorchID:
				return 7;
			case RedStoneTorchID:
				return 8;
			case RedStoneRepeaterID:
				return 9;
			case RedStoneComparatorID:
				return 10;
			case IronArrowID:
				return 11;
		}
	}
	
	struct BlockData {
		public:
			BlockID Id;
			RSType RedType;
			bool IsSolid;
			float Hardness;
			BlockType CubeType;
			string tex[6];
			int texNum;
			BlockData(BlockID id,RSType a,bool b,float c,BlockType d) : Id(id),RedType(a),IsSolid(b),Hardness(c),CubeType(d){ 
				texNum = 0;
			}
			BlockData(BlockID id,RSType a,bool b,float c,BlockType d,string texs[6],int num) : Id(id),RedType(a),IsSolid(b),Hardness(c),CubeType(d),texNum(num){
				for (int i = 0;i < num;i++){
					tex[i] = texs[i];
				}
			}
	};
	
	static BlockData Air = BlockData(AirID, RSNone, false, 0.0f, Normal); //0th
	static BlockData Iron = BlockData(IronID, RSOpacity, true, 5.0f, Normal, IronTex, 1);
	static BlockData Bedrock = BlockData(BedrockID, RSOpacity, true, 100000.0f, Normal, BedrockTex, 1);
	static BlockData Diamond = BlockData(DiamondID, RSOpacity, true, 20.0f, Normal, DiamondTex, 1);
	static BlockData Gold = BlockData(GoldID, RSOpacity, true, 4.0f, Normal, GoldTex, 1);
	static BlockData RedStoneDust = BlockData(RedStoneDustID, RSDust, true, 4.0f, Layer, RedStoneDustTex, 5); //5th,5 texs
	static BlockData RedStoneCube = BlockData(RedStoneCubeID, RSPower, true, 4.0f, Normal, RedStoneBlockTex, 1);
	static BlockData Torch = BlockData(TorchID, RSNone, false, 1.0f, TorchType, TorchTex, 2);
	static BlockData RedStoneTorch = BlockData(RedStoneTorchID, RSTorch, false, 1.0f, TorchType, RedStoneTorchTex, 3);
	static BlockData RedStoneRepeater = BlockData(RedStoneRepeaterID, RSIO, false, 1.0f,  RepeaterType, RedStoneRepeaterTex, 3);
	static BlockData RedStoneComparator = BlockData(RedStoneComparatorID, RSIO, false, 1.0f,  ComparatorType, RedStoneComparatorTex, 2);
	static BlockData IronArrow = BlockData(IronArrowID, RSOpacity, true, 3.0f, Stem, IronArrowTex, 2);
	
	
	static BlockData BlockDatas[] { Air, Iron, Bedrock, Diamond, Gold, RedStoneDust, RedStoneCube, Torch, RedStoneTorch, RedStoneRepeater, RedStoneComparator, IronArrow };
	
	static int GetBlockNum(){
		return sizeof(BlockDatas) / sizeof(BlockDatas[0]);
	}
	
	static BlockID GetBlockID(int ID){     //Numeral -> Enum ID
		return BlockDatas[ID].Id;
	}
	
	static bool IsSolid(BlockID ID){
		return BlockDatas[GetNumID(ID)].IsSolid;
	}
	
	static float GetHardness(BlockID ID){
		return BlockDatas[GetNumID(ID)].Hardness;
	}
	
	static BlockType GetBlockType(BlockID ID){
		return BlockDatas[GetNumID(ID)].CubeType;
	}
	
	static RSType GetRedType(BlockID ID){
		return BlockDatas[GetNumID(ID)].RedType;
	}
}
