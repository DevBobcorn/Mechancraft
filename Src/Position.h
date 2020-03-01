#pragma once

namespace Mechanicraft{
	struct position{
		int x,y,z;
		position(int _x,int _y,int _z) : x(_x),y(_y),z(_z){ }
		position operator+(const position& pos2){
	        position newpos(this->x,this->y,this->z);
	        newpos.x += pos2.x;
	        newpos.y += pos2.y;
	        newpos.z += pos2.z;
	        return newpos;
	    }
	    
	    bool operator==(const position& pos2){
	        return this->x == pos2.x && this->y == pos2.y && this->z == pos2.z;
	    }
	    
	    bool operator!=(const position& pos2){
	        return this->x != pos2.x || this->y != pos2.y || this->z != pos2.z;
	    }
	};

	static const position adjacent[4] = {
		position( 1,0,0),
		position(-1,0,0),
		position(0,0, 1),
		position(0,0,-1)
	};

	static const position around[6] = {
		position(0, 1,0),
		position(0,-1,0),
		position( 1,0,0),
		position(-1,0,0),
		position(0,0, 1),
		position(0,0,-1)
	};
	
	static const position diagonals[8] = {
		position( 1, 1,0),
		position(-1, 1,0),
		position(0, 1, 1),
		position(0, 1,-1),
		position( 1,-1,0),
		position(-1,-1,0),
		position(0,-1, 1),
		position(0,-1,-1)
	};
	
	static const position roundAdjacent[4] = {
		position( 1,0,0),
		position(0,0,-1),
		position(-1,0,0),
		position(0,0, 1)
	};
}
