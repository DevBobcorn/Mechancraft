#pragma once

#include <cstdlib>
#include <iostream>
#include "Map.h"
#include "BlockData.h"
#include "Position.h"

using namespace std;

namespace Mechanicraft{
	class Map;
	
	class Block {
		public:
			virtual BlockID GetID() { };
			virtual void RSUpdate(Map* map,position self) { };
			virtual void InitStr(string str) { };
			virtual string ToStr() { };
			void MarkSelf(Map* map,position self);
			void MarkNextSelf(Map* map,position self);
			void MarkAround(Map* map,position self);
			void MarkAllAround(Map* map,position self);
	};
	
	class AirBlock : public Block {
		public:
			void InitStr(string str) { };
			string ToStr() { };
			AirBlock();
			BlockID GetID();
			void RSUpdate(Map* map,position self);
	};
	
	class TorchBlock : public Block {
		public:
			TorchDir TorDir = TorchUpDir;
			bool IsOn;
			TorchBlock();
			BlockID GetID();
			void RSUpdate(Map* map,position self);
			void InitStr(string str);
			string ToStr();
	};
	
	class RedStoneTorchBlock : public TorchBlock {
		public:
			RedStoneTorchBlock();
			BlockID GetID();
			position GetRelyingBlock(position self);
			void RSUpdate(Map* map,position self);
			string ToStr();
			void InitStr(string str);
		private:
			long long LastUpdate = 0;
			bool prevStates[3] = { true, true, true};
	};
	
	class OpaqueBlock : public Block {
		public:
            int PowerLev = 0;
            bool IsStronglyPowered = false;
			void RSUpdate(Map* map,position self); //Only write once for all opacity blocks
			void InitStr(string str) { };
			string ToStr() { };
	};
	
	class IronBlock : public OpaqueBlock {
		public:
			IronBlock();
			BlockID GetID();
	};
	
	class BedrockBlock : public OpaqueBlock {
		public:
			BedrockBlock();
			BlockID GetID();
	};
	
	class DiamondBlock : public OpaqueBlock {
		public:
			DiamondBlock();
			BlockID GetID();
	};
	
	class GoldBlock : public OpaqueBlock {
		public:
			GoldBlock();
			BlockID GetID();
	};
	
	class IronArrowBlock : public OpaqueBlock {
		public:
			IronArrowBlock();
			BlockID GetID();
	};

	class RedStoneDustBlock : public Block {
		public:
			int PowerLev = 0;
			int ConnectDir = 0;
			int DiagDir = 0;
			RedStoneDustBlock();
			BlockID GetID();
			void RSUpdate(Map* map,position self);
			void InitStr(string str);
			string ToStr();
	};
	
	class RedStoneBlock : public Block {
		public:
			RedStoneBlock();
			BlockID GetID();
			void RSUpdate(Map* map,position self);
			void InitStr(string str) { };
			string ToStr() { };
	};
	
	class Interactable : public Block {
		public:
			virtual void Interact() { };
	};
	
	class IOBlock : public Interactable {
		public:
			IODir FacDir = IOPosXDir;
			int Delay = 1;
			int OutputLev = 0;
			void PrepInputs(Map* map,position self);
			void PrepPowers(Map* map,position self,bool directSideInputOnly);
			void Alternate();
			bool CheckUpdate();
			position GetOutputPos(position self);
			position GetBackPos(position self);
			Block* CurInputs[4]; // Front, Left, Back, Right
			int CurStrength[4] = { 0, 0, 0, 0 };
			long long LastUpdate = 0;
			int prevStates[5] = { 0, 0, 0, 0, 0 };
		private:
			int Offset;
	};
	
	class RedStoneRepeaterBlock : public IOBlock {
		public:
			bool Locked = false;
			RedStoneRepeaterBlock();
			BlockID GetID();
			void RSUpdate(Map* map,position self);
			void Interact();
			void InitStr(string str);
			string ToStr();
	};
	
	class RedStoneComparatorBlock : public IOBlock {
		public:
			bool FrontOn = false; // true - compare mode; false - subtract mode
			RedStoneComparatorBlock();
			BlockID GetID();
			void RSUpdate(Map* map,position self);
			void Interact();
			void InitStr(string str);
			string ToStr();
	};
	
	static Block* GetNewBlock(BlockID Wanted){
		switch (Wanted){
			case AirID:
				return new AirBlock();
			case IronID:
				return new IronBlock();
			case BedrockID:
				return new BedrockBlock();
			case DiamondID:
				return new DiamondBlock();
			case GoldID:
				return new GoldBlock();
			case RedStoneDustID:
				return new RedStoneDustBlock();
			case RedStoneCubeID:
				return new RedStoneBlock();
			case TorchID:
				return new TorchBlock();
			case RedStoneTorchID:
				return new RedStoneTorchBlock();
			case RedStoneRepeaterID:
				return new RedStoneRepeaterBlock();
			case RedStoneComparatorID:
				return new RedStoneComparatorBlock();
			case IronArrowID:
				return new IronArrowBlock();
			default:
				return nullptr;
		}
	}
}

