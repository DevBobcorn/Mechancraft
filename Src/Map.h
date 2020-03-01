#pragma once

#include <queue>
#include "Position.h"
#include "Block.h"
//#include "Shader_s.h"

#define WORLDTOP 32
#define MAPMAX 64
#define TICKMAX 1200
using namespace std;

namespace Mechanicraft{
	class Block;
	
	class Map{
		public:
			long long TotRSTick = 0;
			Block* map[MAPMAX][WORLDTOP][MAPMAX];
			Map();
			bool IsInWorld(position pos);
			bool IsInWorld(int x,int y,int z);
			void SetBlock(Block* newBlock,const position pos);
			void SetBlock(Block* newBlock,const int x,const int y,const int z);
			Block* GetBlock(const position pos);
			Block* GetBlock(const int x,const int y,const int z);
			void RunATick();
			void Load();
			void SaveCurrent();
		private:
			friend class Block;
			queue<position> Updates;
			queue<position> NextUpdates;
			
			void MarkUpdate(position pos);
			void MarkNextUpdate(position pos);
			void MarkUpdateAround(position pos);
			void MarkUpdateAllAround(position pos); //Diagonals Included
	};
}

