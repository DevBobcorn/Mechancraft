#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include "BlockData.h"
#include "Block.h"

using namespace std;

namespace Mechanicraft{
	void Block::MarkAround(Map* map,position self){
		map -> MarkUpdateAround(self);
	}
	
	void Block::MarkAllAround(Map* map,position self){
		map -> MarkUpdateAllAround(self);
	}
	
	void Block::MarkSelf(Map* map,position self){
		map -> MarkUpdate(self);
	}
	
	void Block::MarkNextSelf(Map* map,position self){
		map -> MarkNextUpdate(self);
	}
	
	AirBlock::AirBlock() { }
	
	BlockID AirBlock::GetID() { return AirID; }
	
	void AirBlock::RSUpdate(Map* map,position self) { }
	
	TorchBlock::TorchBlock() { IsOn = true; }
	
	BlockID TorchBlock::GetID() { return TorchID; }

	void TorchBlock::InitStr(string Str) {
		if (Str[0] >= '0' && Str[0] <= '4') {
			TorDir = (TorchDir)(Str[0] - '0');
		}
	}
	
	string TorchBlock::ToStr() {
		char num[1];
		string tmpStr = "";
		num[0] = char(TorDir + '0');
		tmpStr += num;
		return tmpStr;
	}
	
	void TorchBlock::RSUpdate(Map* map,position self) {  }
	
	RedStoneTorchBlock::RedStoneTorchBlock() { IsOn = true; }
	
	BlockID RedStoneTorchBlock::GetID() { return RedStoneTorchID; }
	
	position RedStoneTorchBlock::GetRelyingBlock(position self){
		switch (TorDir){
			case TorchPosXDir:
				return self + position(-1,0,0);
			case TorchNegXDir:
				return self + position( 1,0,0);
			case TorchPosZDir:
				return self + position(0,0,-1);
			case TorchNegZDir:
				return self + position(0,0, 1);
			case TorchUpDir:
				return self + position(0,-1,0);
		}
	}
	
	void RedStoneTorchBlock::InitStr(string Str) {
		if (Str[0] >= '0' && Str[0] <= '4') {
			TorDir = (TorchDir)(Str[0] - '0');
		}
		if (Str[1] == '0' || Str[1] == '1') {
			IsOn = bool(Str[1] - '0');
		}
	}
	
	string RedStoneTorchBlock::ToStr() {
		char num[2];
		string tmpStr = "";
		num[0] = char(TorDir + '0');
		num[1] = char(IsOn + '0');
		tmpStr += num;
		return tmpStr;
	}
	
	void RedStoneTorchBlock::RSUpdate(Map* map,position self) {
		//printf("Torch:  %d %d %d\n",prevStates[2],prevStates[1],prevStates[0]);
		Block* curBlock;
		bool Changed = false;
		
		if (LastUpdate != map->TotRSTick){
			prevStates[2] = prevStates[1];
			prevStates[1] = prevStates[0];
			LastUpdate = map->TotRSTick;
		}

		curBlock = map->GetBlock(GetRelyingBlock(self));

		//init
		prevStates[0] = true;
		if (curBlock != nullptr) {
			if (GetRedType(curBlock->GetID()) == RSOpacity) {
				if (dynamic_cast<OpaqueBlock*>(curBlock)->PowerLev > 0) prevStates[0] = false;
				else prevStates[0] = true;
			} else if (curBlock->GetID() == RedStoneCubeID) prevStates[0] = false;
			if (prevStates[0] != IsOn) {
				if (prevStates[0]) { //If it wants to turn on
					if (prevStates[1]) IsOn = true,Changed = true;
					else MarkNextSelf(map,self); //Try Again Next Tick!
				} else { //willing to put itself off
					if (!prevStates[1]) IsOn = false,Changed = true;
					else MarkNextSelf(map,self); //Try Again Next Tick,Stay tuned!
				}
			}
			//Update others...
			if (Changed) MarkAround(map,self);
		}
	}
	
	void OpaqueBlock::RSUpdate(Map* map,position self) {
		IOBlock *curIOBlock;
		//cout << "Solid Update!" << endl;
		int OldLev = PowerLev;
		bool OldIsStrPow = IsStronglyPowered;
		PowerLev = 0;
		IsStronglyPowered = false;
		Block* curBlock;
		OpaqueBlock* curOpaqBlock;
		RedStoneDustBlock* curRedDust;
		for (int i = 0;i < 6;i++){
			curBlock = map->GetBlock(self + around[i]);
			if (curBlock == nullptr) continue;
			if (GetRedType(curBlock->GetID()) == RSOpacity){
				curOpaqBlock = dynamic_cast<OpaqueBlock*>(curBlock);
				if (curOpaqBlock->IsStronglyPowered) PowerLev = max(PowerLev,curOpaqBlock->PowerLev - 1); //but not strongly powered.
			} else if (curBlock->GetID() == RedStoneDustID){
				curRedDust = dynamic_cast<RedStoneDustBlock*>(curBlock);
				PowerLev = max(PowerLev,curRedDust->PowerLev - 1); //but not strongly powered.
			} else if (curBlock->GetID() == RedStoneCubeID) {
				PowerLev = max(PowerLev,15);
			} else if (curBlock->GetID() == RedStoneTorchID) { //Downside is RSTorch
				RedStoneTorchBlock* curRSTorch = dynamic_cast<RedStoneTorchBlock*>(curBlock);
				if (curRSTorch -> IsOn) {
					if (i == 1) {
						PowerLev = max(PowerLev,15); //strongly powered.
						IsStronglyPowered = true;
					}	
				}
			} else if ((curIOBlock = dynamic_cast<IOBlock*>(curBlock)) != nullptr && i != 0 && i != 1 && curIOBlock->GetOutputPos(self + adjacent[i - 2]) == self) {
					PowerLev = max(PowerLev,curIOBlock -> OutputLev);
					IsStronglyPowered = true;
			}
		}
		if (PowerLev != OldLev || OldIsStrPow != IsStronglyPowered) {
			MarkAround(map,self);
		}
	}
	
	IronBlock::IronBlock() { }
	
	BlockID IronBlock::GetID() { return IronID; }

	BedrockBlock::BedrockBlock() { }
	
	BlockID BedrockBlock::GetID() { return BedrockID; }

	DiamondBlock::DiamondBlock() { }
	
	BlockID DiamondBlock::GetID() { return DiamondID; }

	GoldBlock::GoldBlock() { }
	
	BlockID GoldBlock::GetID() { return GoldID; }
	
	IronArrowBlock::IronArrowBlock() { }
	
	BlockID IronArrowBlock::GetID() { return IronArrowID; }
	
	RedStoneDustBlock::RedStoneDustBlock() { PowerLev = 0; ConnectDir = 0; DiagDir = 0; }
	
	BlockID RedStoneDustBlock::GetID() { return RedStoneDustID; }
	
	void RedStoneDustBlock::RSUpdate(Map* map,position self) {
		int OldLev = PowerLev,OldConnectDir = ConnectDir;
		IOBlock* curIOBlock;
		bool AboveIsSolid = false;
		ConnectDir = 0;
		DiagDir = 0;
		PowerLev = 0;
		Block* curBlock,* curBlock2;
		OpaqueBlock* curOpaqBlock;
		//Up--------------------------------------------------------------
		curBlock = map->GetBlock(self + position(0, 1,0));
		if (curBlock != nullptr){
			if (curBlock->GetID() == RedStoneCubeID) {
				PowerLev = 15;
			}
			if (GetRedType(curBlock->GetID()) == RSOpacity){
				curOpaqBlock = dynamic_cast<OpaqueBlock*>(curBlock);
				if (curOpaqBlock->IsStronglyPowered) PowerLev = max(PowerLev,curOpaqBlock->PowerLev - 1);
				AboveIsSolid = true;
			}
		}
		//Down------------------------------------------------------------
		curBlock = map->GetBlock(self + position(0,-1,0));
		if (curBlock != nullptr){
			if (curBlock->GetID() == RedStoneCubeID) {
				PowerLev = 15;
			}
			if (GetRedType(curBlock->GetID()) == RSOpacity){
				curOpaqBlock = dynamic_cast<OpaqueBlock*>(curBlock);
				if (curOpaqBlock->IsStronglyPowered) PowerLev = max(PowerLev,curOpaqBlock->PowerLev - 1);
			}
		}
		//Adjacent--------------------------------------------------------
		int curCon;
		for (int i = 0;i < 4;i++){
			switch (i) {
				case 0: //-x dir
					curCon = 4;
					break;
				case 1: //+x dir
					curCon = 1;
					break;
				case 2: //-z dir
					curCon = 8;
					break;
				case 3: //+z dir
					curCon = 2;
					break;
			}
			curBlock = map->GetBlock(self + adjacent[i]);
			if (curBlock != nullptr){
				if (curBlock->GetID() == RedStoneDustID) { //
					RedStoneDustBlock* curRedDust = dynamic_cast<RedStoneDustBlock*>(curBlock);
					PowerLev = max(PowerLev,curRedDust->PowerLev - 1);
					ConnectDir |= curCon;
				} else if (curBlock->GetID() == RedStoneCubeID) {
					PowerLev = 15, ConnectDir |= curCon;
				} else if ((curIOBlock = dynamic_cast<IOBlock*>(curBlock)) != nullptr) {
					if (curIOBlock->GetOutputPos(self + adjacent[i]) == self) {
						PowerLev = min(15,max(PowerLev,curIOBlock -> OutputLev));
						ConnectDir |= curCon;
					} else if (curBlock->GetID() == RedStoneComparatorID || curIOBlock->GetBackPos(self + adjacent[i]) == self) ConnectDir |= curCon;
				}
				if (GetRedType(curBlock->GetID()) == RSOpacity) {
					curOpaqBlock = dynamic_cast<OpaqueBlock*>(curBlock);
					if (curOpaqBlock->IsStronglyPowered) PowerLev = max(PowerLev,curOpaqBlock->PowerLev - 1);
					if (!AboveIsSolid) {
						curBlock2 = map->GetBlock(self + adjacent[i] + position(0,1,0));
						if (curBlock2->GetID() == RedStoneDustID) { //up
							RedStoneDustBlock* curRedDust = dynamic_cast<RedStoneDustBlock*>(curBlock2);
							PowerLev = max(PowerLev,curRedDust->PowerLev - 1);
							DiagDir |= curCon, ConnectDir |= curCon;
						}					
					}
				} else {
					if (curBlock->GetID() == RedStoneTorchID) {
						ConnectDir |= curCon;
						if (dynamic_cast<RedStoneTorchBlock*>(curBlock)->IsOn) PowerLev = 15;
					}
					curBlock2 = map->GetBlock(self + adjacent[i] + position(0,-1,0));
					if (curBlock2->GetID() == RedStoneDustID) { //down
						RedStoneDustBlock* curRedDust = dynamic_cast<RedStoneDustBlock*>(curBlock2);
						PowerLev = max(PowerLev,curRedDust->PowerLev - 1);
						ConnectDir |= curCon;
					}
				}
			}
		}
		if (PowerLev != OldLev || OldConnectDir != ConnectDir) {
			MarkAllAround(map,self);
		}
	}
	
	void RedStoneDustBlock::InitStr(string Str) {
		if (Str[0] >= 'A' && Str[0] <= 'P') {
			PowerLev = int(Str[0] - 'A');
		}
		if (Str[1] >= 'A' && Str[1] <= 'P') {
			ConnectDir = int(Str[1] - 'A');
		}
		if (Str[2] >= 'A' && Str[2] <= 'P') {
			DiagDir = int(Str[2] - 'A');
		}
	}
	
	string RedStoneDustBlock::ToStr() {
		char num[3];
		string tmpStr = "";
		num[0] = char(PowerLev + 'A');
		num[1] = char(ConnectDir + 'A');
		num[2] = char(DiagDir + 'A');
		tmpStr += num;
		return tmpStr;
	}
	
	RedStoneBlock::RedStoneBlock() { }
	
	BlockID RedStoneBlock::GetID() { return RedStoneCubeID; }
	
	void RedStoneBlock::RSUpdate(Map* map,position self) { }
	
	position IOBlock::GetOutputPos(position self) {
		switch (FacDir){
			case IOPosXDir:
				return (self + roundAdjacent[0]);
			case IONegZDir:
				return (self + roundAdjacent[1]);
			case IONegXDir:
				return (self + roundAdjacent[2]);
			case IOPosZDir:
				return (self + roundAdjacent[3]);
		}
	}
	
	position IOBlock::GetBackPos(position self) {
		switch (FacDir){
			case IOPosXDir:
				return (self + roundAdjacent[2]);
			case IONegZDir:
				return (self + roundAdjacent[3]);
			case IONegXDir:
				return (self + roundAdjacent[0]);
			case IOPosZDir:
				return (self + roundAdjacent[1]);
		}
	}
	
	void IOBlock::PrepInputs(Map* map,position self) {
		switch (FacDir){
			case IOPosXDir:
				Offset = 0;
				break;
			case IONegZDir:
				Offset = 1;
				break;
			case IONegXDir:
				Offset = 2;
				break;
			case IOPosZDir:
				Offset = 3;
				break;
		}
		//cout << endl << Offset << endl;
		for (int i = 0;i < 4;i++){
			CurInputs[i] = map->GetBlock(self + roundAdjacent[(Offset + i) % 4]);
		}
		/*
		cout << "Prepared" << endl;
		cout << "Front: " << GetNumID(CurInputs[0]->GetID());
		cout << "Back:  " << GetNumID(CurInputs[2]->GetID());
		cout << "Left:  " << GetNumID(CurInputs[1]->GetID());
		cout << "Right: " << GetNumID(CurInputs[3]->GetID()) << endl;
		*/
	}
	
	void IOBlock::PrepPowers(Map* map,position self,bool directSideInputOnly) {
		IOBlock* curIOBlock;
		for (int i = 1;i < 4;i++) {
			CurStrength[i] = 0;
			if (CurInputs[i] == nullptr) continue;
			if (CurInputs[i]->GetID() == RedStoneCubeID && (!directSideInputOnly || i == 2)) CurStrength[i] = 16;
			else if (CurInputs[i]->GetID() == RedStoneDustID && (!directSideInputOnly || i == 2)) CurStrength[i] = dynamic_cast<RedStoneDustBlock*>(CurInputs[i])->PowerLev;
			else if (CurInputs[i]->GetID() == RedStoneTorchID && (!directSideInputOnly || i == 2)) CurStrength[i] = dynamic_cast<RedStoneTorchBlock*>(CurInputs[i])->IsOn ? 15 : 0;
			else if ((curIOBlock = dynamic_cast<IOBlock*>(CurInputs[i])) != nullptr && curIOBlock->GetOutputPos(self + roundAdjacent[(Offset + i) % 4]) == self) {
				CurStrength[i] = curIOBlock->OutputLev;
			}
		}
	}
	
	void IOBlock::Alternate(){
		for (int i = 3;i >= 0;i--) prevStates[i + 1] = prevStates[i];
	}
	
	bool IOBlock::CheckUpdate(){
		bool result;
		for (int i = 3;i >= 0;i--) result |= (prevStates[i + 1] == prevStates[i]);
		return result;
	}
	
	RedStoneRepeaterBlock::RedStoneRepeaterBlock() { }
	
	BlockID RedStoneRepeaterBlock::GetID() { return RedStoneRepeaterID; }
	
	void RedStoneRepeaterBlock::RSUpdate(Map* map,position self) {
		int OldOutLev = OutputLev;
		
		IOBlock::PrepInputs(map,self);
		IOBlock::PrepPowers(map,self,true);
		
		if (CurStrength[1] || CurStrength[3]) {
			Locked = true;
			return;
		}
		else Locked = false;
		
		if (LastUpdate != map->TotRSTick){
			Alternate();
			LastUpdate = map->TotRSTick;
		}
		
		if (CurStrength[2]) prevStates[0] = CurStrength[2];
		else prevStates[0] = 0;
		
		OutputLev = prevStates[Delay];
		
		if (OldOutLev != OutputLev) MarkSelf(map, GetOutputPos(self));
		if (CheckUpdate()) MarkNextSelf(map, self);
	}
	
	void RedStoneRepeaterBlock::Interact() {
		Delay %= 4;
		Delay++;
		//cout << "You touched me! --Repeater" << endl;
	}
	
	void RedStoneRepeaterBlock::InitStr(string Str) {
		if (Str[0] >= '0' && Str[0] < '4') {
			FacDir = (IODir)(Str[0] - '0');
		}
		if (Str[1] >= '1' && Str[1] <= '4') {
			Delay = int(Str[1] - '0');
		}
		if (Str[2] >= 'A' && Str[2] <= 'P') {
			OutputLev = int(Str[2] - 'A');
		}
		if (Str[3] == '0' || Str[3] == '1') {
			Locked = bool(Str[3] - '0');
		}
	}
	
	string RedStoneRepeaterBlock::ToStr() {
		char num[4];
		string tmpStr = "";
		num[0] = char(FacDir + '0');
		num[1] = char(Delay + '0');
		num[2] = char(OutputLev + 'A');
		num[3] = char(Locked + '0');
		tmpStr += num;
		return tmpStr;
	}
	
	RedStoneComparatorBlock::RedStoneComparatorBlock() { }
	
	BlockID RedStoneComparatorBlock::GetID() { return RedStoneComparatorID; }
	
	void RedStoneComparatorBlock::RSUpdate(Map* map,position self) { 
		int OldOutLev = OutputLev;
		
		IOBlock::PrepInputs(map,self);
		IOBlock::PrepPowers(map,self,false);
		
		if (LastUpdate != map->TotRSTick){
			prevStates[1] = prevStates[0];
			LastUpdate = map->TotRSTick;
		}
		
		if (!CurStrength[1] && !CurStrength[3]) prevStates[0] = CurStrength[2];
		else {
			if (FrontOn) { // Subtract Mode
				prevStates[0] = max(0,CurStrength[2] - max(CurStrength[1],CurStrength[3]));
				//cout << CurStrength[2] << ' ' << CurStrength[1] << ' ' << CurStrength[3] << endl;
			} else { // Compare Mode
				if (CurStrength[1] > CurStrength[2] || CurStrength[3] > CurStrength[2])
					prevStates[0] = 0;
				else prevStates[0] = CurStrength[2];
			}
		}
		
		OutputLev = prevStates[1];
		
		if (OldOutLev != OutputLev) MarkSelf(map, GetOutputPos(self));
		if (prevStates[0] != prevStates[1]) MarkNextSelf(map, self);
	}
	
	void RedStoneComparatorBlock::Interact() {
		FrontOn = ((int)FrontOn + 1) % 2;
		//cout << "You touched me! --Comparator" << endl;
	}
	
	void RedStoneComparatorBlock::InitStr(string Str) {
		if (Str[0] >= '0' && Str[0] < '4') {
			FacDir = (IODir)(Str[0] - '0');
		}
		if (Str[1] >= 'A' && Str[1] <= 'P') {
			OutputLev = int(Str[1] - 'A');
		}
		if (Str[2] == '0' || Str[2] == '1') {
			FrontOn = bool(Str[2] - '0');
		}
	}
	
	string RedStoneComparatorBlock::ToStr() {
		char num[3];
		string tmpStr = "";
		num[0] = char(FacDir + '0');
		num[1] = char(OutputLev + 'A');
		num[2] = char(FrontOn + '0');
		tmpStr += num;
		return tmpStr;
	}
}

