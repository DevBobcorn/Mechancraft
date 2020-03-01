#include "Block.h"
#include "Map.h"

#include <fstream>
#include <iostream>

class Map;

namespace Mechanicraft{
	Map::Map(){
		for (int y = 0; y < WORLDTOP; y++)
			for (int x = 0; x < MAPMAX; x++)
				for (int z = 0; z < MAPMAX; z++) {
					map[x][y][z] = new AirBlock();
				}
	}
	
	void Map::Load(){
		int tmpTop,tmpX,tmpZ;
		char blockChar;
		char tmpCharArr[1];
		string TmpStr;
		Block* tmpBlock;
		freopen("./sav/test.sav","r",stdin);
		cin >> tmpTop >> tmpX >> tmpZ;
		for (int y = 0; y < tmpTop; y++)
			for (int x = 0; x < tmpX; x++)
				for (int z = 0; z < tmpZ; z++) {
					if ((blockChar = getchar()) < 0) break;
					while (blockChar < 'A' || blockChar == ' ' || blockChar == '\t' || blockChar == '\n')
						if ((blockChar = getchar()) < 0) break;
					tmpBlock = GetNewBlock(GetBlockID(blockChar - 'A'));
					if (tmpBlock == nullptr) map[x][y][z] = new AirBlock();
					else {
						map[x][y][z] = tmpBlock;
						switch (GetBlockType(map[x][y][z]->GetID())){
							case Layer:
							case Stem:
							case SixSide:
							case TorchType:
							case RepeaterType:
							case ComparatorType:
								TmpStr = "";
								while ((tmpCharArr[0] = getchar()) != '~')
									TmpStr += tmpCharArr[0];
								//cout << TmpStr << endl;
								map[x][y][z]->InitStr(TmpStr);
								
								break;
						}
					}
					
				}
		freopen("CON","r",stdin);
		cout << "Save Loaded." << endl;
	}
	
	bool Map::IsInWorld(position pos) {
		return (pos.x >= 0 && pos.x < MAPMAX && pos.y >= 0 && pos.y < WORLDTOP && pos.z >= 0 && pos.z < MAPMAX);
	}
	bool Map::IsInWorld(int x, int y, int z) {
		return (x >= 0 && x < MAPMAX && y >= 0 && y < WORLDTOP && z >= 0 && z < MAPMAX); 
	}	
	void Map::SetBlock(Block* newBlock,const position pos){
		if (!IsInWorld(pos)) return;
		delete map[pos.x][pos.y][pos.z];
		map[pos.x][pos.y][pos.z] = newBlock;
		MarkUpdate(pos);
		MarkUpdateAllAround(pos);
		//cout << pos.x << ' ' << pos.y << ' ' << pos.z << " Placed!" << endl;
	}
	void Map::SetBlock(Block* newBlock,const int x,const int y,const int z){
		if (!IsInWorld(x,y,z)) return;
		delete map[x][y][z];
		map[x][y][z] = newBlock;
		MarkUpdate(position(x,y,z));
		MarkUpdateAllAround(position(x,y,z));
		//cout << x << ' ' << y << ' ' << z << " Placed!" << endl;
	}
	Block* Map::GetBlock(const position pos){
		if (!IsInWorld(pos)) return nullptr;
		return map[pos.x][pos.y][pos.z];
	}
	Block* Map::GetBlock(const int x,const int y,const int z){
		if (!IsInWorld(x,y,z)) return nullptr;
		return map[x][y][z];
	}
	
	void Map::MarkUpdate(position pos){
		if (!IsInWorld(pos.x,pos.y,pos.z)) return;
		Updates.push(pos);
		//cout << pos.x << ' ' << pos.y << ' ' << pos.z << " Marked!" << endl;
	}
	
	void Map::MarkUpdateAround(position pos){
		for (int i = 0;i < 6;i++){
			MarkUpdate(pos + around[i]);
		}
	}
	
	void Map::MarkUpdateAllAround(position pos){ //Diagonals Included
		MarkUpdateAround(pos);
		for (int i = 0;i < 8;i++){
			MarkUpdate(pos + diagonals[i]);
		}
	}
	
	void Map::MarkNextUpdate(position pos){
		if (!IsInWorld(pos)) return;
		NextUpdates.push(pos);
	}
	
	void Map::RunATick(){
		//Go through the updates
		while (!Updates.empty()){
			position cur = Updates.front();
			Updates.pop();
			map[cur.x][cur.y][cur.z]->RSUpdate(this,cur);
		}
		while (!NextUpdates.empty()){
			position cur = NextUpdates.front();
			NextUpdates.pop();
			Updates.push(cur);
		}
		++TotRSTick;
		TotRSTick %= TICKMAX;
	}
	
	void Map::SaveCurrent(){
		freopen("./sav/test.sav","w",stdout);
		cout << WORLDTOP << ' ' << MAPMAX << ' ' << MAPMAX << endl;
		for (int y = 0; y < WORLDTOP; y++) {
			for (int x = 0; x < MAPMAX; x++) {
				for (int z = 0; z < MAPMAX; z++) {
					//map[x][y][z] = new AirBlock();
					cout << char(GetNumID(map[x][y][z]->GetID()) + 'A');
					switch (GetBlockType(map[x][y][z]->GetID())){
						case Layer:
						case Stem:
						case SixSide:
						case TorchType:
						case RepeaterType:
						case ComparatorType:
							cout << map[x][y][z]->ToStr() << '~';
							break;
					}
				}
				cout << endl;
			}
			cout << endl;
		}
		freopen("CON", "w", stdout); 
		cout << "Map Saved!" << endl;
	}
}
