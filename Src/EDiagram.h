#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdio>
#include <windows.h>
#include "FlowIDSystem.h"
#include "Elements.h"
#include "Greetings.h"
#include "Map.h"
using namespace std;

#define IDMAX 4100
//ID: Sub-electron flows numbered as 1,2,4,8,16,32,64,128,256,512,1024,2048,etc. 12 Sub-electron flow maximum.

struct Point {
	int x;
	int y; // y represents altitude. 
	int z;
};

struct DistPoint {
	int x;
	int y;
	int z;
	int dist;
	DistPoint(int a, int b, int c, int d) :x(a), y(b), z(c), dist(d) {}
};

bool operator < (DistPoint a, DistPoint b) {
	return a.dist > b.dist; //Wrote invertedly for priority_queue is sorted from greater to less...
}

class EDiagram {
public:
	char map[MAPMAX][WORLDTOP][MAPMAX]; //The basic symbolic elec-map.
	int par[MAPMAX][WORLDTOP][MAPMAX]; //The direction of it's parent -> itself, this array is updated every time when a new path is found.
									   // When running dijkstra, used as multi-parent storage...
	int electDir[MAPMAX][WORLDTOP][MAPMAX]; //The direction of electrons, fixed. (Current-Output-map)
	int curDir[MAPMAX][WORLDTOP][MAPMAX]; //The direction of the current, fixed. (Current-Input-map)
	int idmap[MAPMAX][WORLDTOP][MAPMAX]; //The ID of points in the graph.
	double resistmap[IDMAX]; //The resistance of the electron flow with matched ID.
	double flowmap[IDMAX]; //The electric flow with a current ID.
	int width;
	int length;
	int height;

	double Power = 6.0;//Volts

	Point End;

	void Initial() {
		width = 0;
		length = 0;
		for (int x = 0; x < MAPMAX; x++) //x
			for (int z = 0; z < MAPMAX; z++) //z
				for (int y = 0; y < WORLDTOP; y++) //y
					map[x][y][z] = ' ';
	}
	void Input() {
		printf("Input--[W]or[L]d-[H]ello!--\n");
		Initial();
		int w, l, h;
		char ch;
		scanf("%d%d%d", &w, &l, &h);
		if (w > 0 && l > 0 && h > 0) {
			width = w;
			length = l;
			height = h;
			for (int y = 0; y < height; y++) { //y
				for (int z = 0; z < length; z++) //z
					for (int x = 0; x < width; x++) { //x
						while ((ch = getchar()) == '\n') {}
						map[x][y][z] = ch;
					}
			}
		}
	}
	void Print() {
		printf("Print----------------------\n");
		for (int y = 0; y < height; y++) { //y
										   // The next layer
			printf("h=%d\n", y);
			for (int z = 0; z < length; z++) {//z
				for (int x = 0; x < width; x++) //x
					putchar(map[x][y][z]);
				putchar('\n');
			}
		}
		putchar('\n');
	}

	void PrintParent() {
		printf("PrintParent----------------\n");
		for (int y = 0; y < height; y++) { //y
										   // The next layer
			printf("h=%d\n", y);
			for (int z = 0; z < length; z++) {//z
				for (int x = 0; x < width; x++) //x
					switch (par[x][y][z]) {
					case -1:
						printf(" ");
						break;
					case 0:
						printf("<");
						break;
					case 1:
						printf(">");
						break;
					case 2:
						printf("A");
						break;
					case 3:
						printf("V");
						break;
					case 4:
						printf("^");
						break;
					case 5:
						printf("D");
						break;
					case 9:
						printf("@");
						break;
					default:
						printf("?");
						break;
					}
				putchar('\n');
			}
		}
		putchar('\n');
	}
	/*
	void PrintDir(char Mode) {
	if (Mode == 'C') printf("PrintCurrent---------------\n");
	else if (Mode == 'E') printf("PrintElectron--------------\n");
	else if (Mode == 'M') printf("PrintMultiParent-----------\n");
	else if (Mode == 'T') printf("PrintTempCurrent-----------\n");
	int dat;
	for (int y = 0; y < length; y++) { //y
	for (int x = 0; x < width; x++) {//x
	if (Mode == 'C') dat = curDir[x][y][z];
	else if (Mode == 'E') dat = electDir[x][y][z];
	else if (Mode == 'M') dat = par[x][y][z];
	else if (Mode == 'T') dat = TempCurDir[x][y][z];
	switch (dat) {
	case 0:
	printf(" ");
	break;
	case 1:
	printf("%c",27); // <
	break;
	case 2:
	printf("%c",26); // >
	break;
	case 4:
	printf("%c",24); // A
	break;
	case 8:
	printf("%c",25); // V
	break;
	//2 directions:
	case 3:
	printf("%c",29); // <>
	break;
	case 12:
	printf("%c",18); // AV
	break;
	case 5:
	printf("%c",30); // <A, represent as A.
	break;
	case 9:
	printf("%c",17); // V<, represent as <.
	break;
	case 6:
	printf("%c",16); // A>, represent as >.
	break;
	case 10:
	printf("%c",31); // AV, represent as V.
	break;
	//3 directions
	case 14:
	case 13:
	case 11:
	case 7:
	printf("*");
	break;
	case 15:
	printf("%c",15); //AV<>, Must be incorrect...
	break;
	case 99:
	printf("@");
	break;
	default:
	if (BitTest(dat,4)) //0001****, The 'Need recalculating pos is 1'
	printf("N");
	else printf("?");
	break;
	}
	}
	printf("\n");
	}
	}

	void PrintIDMap(char Mode){
	printf("PrintIDMap--------Mode:--%c-\n",Mode);
	int maxl = nXtPathID - 1;
	for (int y = 0; y < length; y++) { //y
	for (int x = 0; x < width; x++) //x
	switch (Mode){
	case 'N': //Normal
	if (maxl >= 10000) printf("RLBG|"); //REALLY BIG!!!
	else if (maxl >= 1000) printf("%4d|",idmap[x][y][z]);
	else if (maxl >= 100) printf("%3d|",idmap[x][y][z]);
	else if (maxl >= 10) printf("%2d|",idmap[x][y][z]);
	else printf("%d",idmap[x][y][z]);
	break;
	case 'R': //Resistance
	if (resistmap[idmap[x][y][z]] >= 10000.0) printf("RLBG|"); //REALLY BIG!!!
	else if (resistmap[idmap[x][y][z]] >= 1000.0) printf("%4d|",resistmap[idmap[x][y][z]]);
	else if (resistmap[idmap[x][y][z]] >= 100.0) printf("%.0f|",resistmap[idmap[x][y][z]]);
	else if (resistmap[idmap[x][y][z]] >= 10.0) printf("%.1f|",resistmap[idmap[x][y][z]]);
	else printf("%.2f|",resistmap[idmap[x][y][z]]);
	break;
	case 'I': //Current Flow
	if (flowmap[idmap[x][y][z]] >= 10000.0) printf("RLBG|"); //REALLY BIG!!!
	else if (flowmap[idmap[x][y][z]] >= 1000.0) printf("%4d|",flowmap[idmap[x][y][z]]);
	else if (flowmap[idmap[x][y][z]] >= 100.0) printf("%.0f|",flowmap[idmap[x][y][z]]);
	else if (flowmap[idmap[x][y][z]] >= 10.0) printf("%.1f|",flowmap[idmap[x][y][z]]);
	else printf("%.2f|",flowmap[idmap[x][y][z]]);
	break;
	case 'D': //Dijkstra
	if (ddist[x][y][z] >= 10000) printf("RLBG|"); //REALLY BIG!!!
	else printf("%4d|",ddist[x][y][z]);
	break;
	case 'A': //Ancestor
	printf("%2d|",ancestor[x][y][z]);
	break;
	}
	printf("\n");
	}
	}

	int TempCurDir[MAPMAX][WORLDTOP][MAPMAX];
	*/
	void CalcCurrentDir() {
		Point Power;
		//All Initializations...
		for (int x = 0; x < MAPMAX; x++) //x
			for (int z = 0; z < MAPMAX; z++) //z
				for (int y = 0; y < WORLDTOP; y++) { //y 
					vis_DFS[x][y][z] = false;
					par[x][y][z] = -1;
					electDir[x][y][z] = 0;
					curDir[x][y][z] = 0;
					if (map[x][y][z] == '#') {
						Power.z = z;
						Power.y = y;
						Power.x = x;
					}
					else if (map[x][y][z] == '=') {
						End.y = y;
						End.x = x;
						End.z = z;
					}
				}
		par[Power.x][Power.y][Power.z] = 9;
		// DFS Search
		NodeDFS(Power);

		/*
		int Uncon[MAPMAX][WORLDTOP][MAPMAX];
		memset(Uncon,0,MAPMAX * MAPMAX * sizeof(int));

		bool HasIllegal = false;
		//Then Find illegal paths...
		for (int y = 0; y < MAPMAX; y++)
		for (int x = 0; x < MAPMAX; x++)
		if (HasIntersection(electDir[x][y][z],curDir[x][y][z])) {
		//Reserve Uncontroversial Directions...
		Uncon[x][y][z] = (~(electDir[x][y][z] & curDir[x][y][z])) & curDir[x][y][z];
		HasIllegal = true;
		BitSet(&curDir[x][y][z],4,true);
		}

		PrintDir('C');

		if (HasIllegal) {
		//Use Dijkstra...
		NodeDijkstra(End.x,End.y);
		//Re-initialize...
		memcpy(TempCurDir, curDir, MAPMAX * MAPMAX * sizeof(int));

		for (int y = 0; y < MAPMAX; y++)
		for (int x = 0; x < MAPMAX; x++){
		if (BitTest(curDir[x][y][z],4)) {
		TempCurDir[x][y][z] = (Uncon[x][y][z] | par[x][y][z]) & 15; //00001111
		}
		vis_DFS[x][y][z] = false;
		par[x][y][z] = -1;
		curDir[x][y][z] = 0;
		idmap[x][y][z] = 0;
		}
		nXtPathID = 1;
		par[Power.x][Power.y][Power.z] = 9;
		TempCurDir[Power.x][Power.y][Power.z] = 15;
		NewDFS(Power);
		}
		PrintDir('C');
		*/
	}
	/*
	vector<IDData> SubsAndCombsID;

	void CalcResist(){
	if (nXtPathID <= 1) return;
	//Init...
	bool Calculated[IDMAX];
	for (int z = 0; z < IDMAX; z++) {
	resistmap[z] = 0.0;
	Calculated[z] = false;
	}
	Calculated[0] = true;

	bool Recorded[IDMAX];
	Recorded[0] = true; //Ignore ID = 0...
	memset(Recorded, false, IDMAX * sizeof(bool));
	//Calculate Sub-Flow Resistance & Add the resistance of those Elements on Combined flows, and by the way, record all combinations that need calculating...
	for (int y = 0; y < MAPMAX; y++)
	for (int x = 0; x < MAPMAX; x++) { //Iterate every point...
	if (!Recorded[idmap[x][y][z]]) {
	SubsAndCombsID.push_back((IDData){idmap[x][y][z],GetSubFlowNum(idmap[x][y][z])});
	cout << idmap[x][y][z];
	Recorded[idmap[x][y][z]] = true;
	}
	if (IsElement(map[x][y][z]))
	resistmap[idmap[x][y][z]] += GetResist(map[x][y][z]);
	if (IsMinSubFlow(idmap[x][y][z])) {
	//cout << idmap[x][y][z] << "Modified." << endl;
	Calculated[idmap[x][y][z]] = true;
	}
	}

	//Sort them from smaller combinations to larger ones...
	sort(SubsAndCombsID.begin(),SubsAndCombsID.end(),comp);

	//Calculate Combined-Flow Resistance, from simple ones to complicated ones...
	vector<int> avalbcombs;
	for (int idx = 0; idx < SubsAndCombsID.size(); idx++) { //Iterate every index...
	if (Calculated[SubsAndCombsID[idx].id]) continue;
	//Then calculate...
	int TempID = SubsAndCombsID[idx].id;
	double childresist = 0.0;
	//Consider available combination first...
	for (int ic = 0;ic < avalbcombs.size();ic++){
	cout << "Available Comb: " << avalbcombs[ic] << endl;
	if (DoesAContainB(TempID,avalbcombs[ic],false)) {
	//Then use the combination, and record it...
	PrintBitSet(TempID);
	cout << " Contains ";
	PrintBitSet(avalbcombs[ic]);
	cout << endl;

	SubsAndCombsID[idx].components.push_back(avalbcombs[ic]);

	TempID -= avalbcombs[ic];
	if (resistmap[avalbcombs[ic]] != 0.0) childresist += (1 / resistmap[avalbcombs[ic]]);
	else childresist += INFINITY; //There's an unblocked way(combination)...
	}
	}
	//Then single Sub-Flows...
	while (TempID > 0){
	int newSub = GetMaximumSubFlow(TempID);
	SubsAndCombsID[idx].components.push_back(newSub);
	//cout << TempID << "   Child ID: " << newSub << "  Resist: " << resistmap[newSub] << endl;
	TempID -= newSub;
	//cout << "TempID" << TempID << endl;
	if (resistmap[newSub] != 0.0) childresist += (1 / resistmap[newSub]);
	else childresist += INFINITY; //There's an empty way(sub-flow)...
	}
	if (childresist != 0) resistmap[SubsAndCombsID[idx].id] += (1 / childresist);
	//cout << SubsAndCombsID[idx].id << "Modified." << endl;
	Calculated[SubsAndCombsID[idx].id] = true;
	avalbcombs.push_back(SubsAndCombsID[idx].id);
	//cout << "Add Comb: " << SubsAndCombsID[idx].id <<  "  Combed by " << combnum << endl;
	}
	resistmap[0] = 0.0; //Ignore Those Isolated Elements, whose resistance is calculated in ID = 0;
	}

	void CalcFlow(){ //Called ONLY When Resistance Is Known...
	if (nXtPathID <= 1) return;
	//Init...
	bool Calculated[IDMAX];
	for (int z = 0; z < IDMAX; z++) {
	flowmap[z] = 0.0;
	Calculated[z] = false;
	}

	flowmap[SubsAndCombsID[SubsAndCombsID.size() - 1].id] = Power / resistmap[SubsAndCombsID[SubsAndCombsID.size() - 1].id];
	for (int idx1 = SubsAndCombsID.size() - 1; idx1 >= 0; idx1--) {
	cout << SubsAndCombsID[idx1].id << endl;
	}

	queue<int> mainstream;
	mainstream.push(SubsAndCombsID[SubsAndCombsID.size() - 1].id);
	while (!mainstream.empty()){
	int newID = mainstream.front();
	mainstream.pop();
	for (int idx = 0; idx < SubsAndCombsID.size(); idx++){
	if (SubsAndCombsID[idx].id == newID) {
	//cout << "Comb: " << SubsAndCombsID[idx].id << endl;
	double RSum = 0.0; //A sum of the Reciprocal of the Resistance of each line...
	int cpidx;
	for (cpidx = 0;cpidx < SubsAndCombsID[idx].components.size();cpidx++){
	if (resistmap[SubsAndCombsID[idx].components[cpidx]] != 0.0)
	RSum += 1.0 / resistmap[SubsAndCombsID[idx].components[cpidx]];
	else RSum += _INFINITY;
	}
	//cout << "RSum: " << RSum << endl;
	for (cpidx = 0;cpidx < SubsAndCombsID[idx].components.size();cpidx++){ //Iterate again, this time, distribute flow...
	//cout << "Son: " << SubsAndCombsID[idx].components[cpidx] << endl;
	if (Calculated[SubsAndCombsID[idx].components[cpidx]]) continue;
	//cout << "Calc: " << SubsAndCombsID[idx].components[cpidx] << endl;
	if (resistmap[SubsAndCombsID[idx].components[cpidx]] != 0.0)
	flowmap[SubsAndCombsID[idx].components[cpidx]] = (1.0 / resistmap[SubsAndCombsID[idx].components[cpidx]]) / RSum * flowmap[SubsAndCombsID[idx].id];
	else flowmap[SubsAndCombsID[idx].components[cpidx]] = _INFINITY / RSum * flowmap[SubsAndCombsID[idx].id];
	Calculated[SubsAndCombsID[idx].components[cpidx]] = true;
	//cout << "Res: " << flowmap[SubsAndCombsID[idx].components[cpidx]] << endl;
	if (!IsMinSubFlow(SubsAndCombsID[idx].components[cpidx])) mainstream.push(SubsAndCombsID[idx].components[cpidx]);
	}
	Calculated[newID] = true;
	}
	}
	}
	for (int idx = SubsAndCombsID.size() - 1; idx >= 0; idx--) {//Iterate every index...
	if (Calculated[SubsAndCombsID[idx].id]) continue;
	int curID = SubsAndCombsID[idx].id;
	while (curID > 0) {
	int sub = GetMaximumSubFlow(curID);
	flowmap[SubsAndCombsID[idx].id] += flowmap[sub];
	curID -= sub;
	}
	Calculated[SubsAndCombsID[idx].id] = true;
	}
	}
	*/
private:
	int nXtPathID = 1;

	int dir[6][3] = { { 1,0,0 },{ -1,0,0 },{ 0,0,1 },{ 0,0,-1 },{ 0,1,0 },{ 0,-1,0 } };
	bool vis_DFS[MAPMAX][WORLDTOP][MAPMAX];
	bool dflag = false;

	bool IsInDiagram(Point p) {
		return (p.x >= 0 && p.y >= 0 && p.z >= 0 && p.x < width && p.y < height && p.z < length);
	}

	bool IsInDiagram(DistPoint p) {
		return (p.x >= 0 && p.y >= 0 && p.z >= 0 && p.x < width && p.y < height && p.z < length);
	}

	void NodeDFS(Point curp) {
		//cout << "DFS1" << endl;
		vis_DFS[curp.x][curp.y][curp.z] = true;
		//Find the following way(s)...
		for (int i = 0; i < 6; i++) {
			Point newp;
			newp.x = curp.x + dir[i][0];
			newp.y = curp.y + dir[i][1];
			newp.z = curp.z + dir[i][2];
			if (IsInDiagram(newp) && !vis_DFS[newp.x][newp.y][newp.z]) {
				if ((i <= 1 && map[newp.x][newp.y][newp.z] == '-') || (i > 1 && i < 4 && map[newp.x][newp.y][newp.z] == '|') || (i >= 4 && map[newp.x][newp.y][newp.z] == '*') || (map[newp.x][newp.y][newp.z] == '+') || IsElement(map[newp.x][newp.y][newp.z])) {
					//cout << "DFS i=" << i << " nxt: " << map[newp.x][newp.y][newp.z] << " x: " << newp.x << " y: " << newp.y << " vis: " << vis_DFS[newp.x][newp.y][newp.z] << endl;
					//A new point found, record where it came from...
					par[newp.x][newp.y][newp.z] = i;
					NodeDFS(newp);
					//map[newp.x][newp.y][newp.z] = '*';
				}
				else if (map[newp.x][newp.y][newp.z] == '=') {
					//Found...
					par[newp.x][newp.y][newp.z] = i;
					PrintParent();
					//Trace it's path...
					Point PathTracer = Point{ newp.x, newp.y, newp.z };
					Point ChildPoint;
					int childDir, parDir;

					while (par[PathTracer.x][PathTracer.y][PathTracer.z] != 9) {
						//printf("Dir: %d x: %d y: %d => \n",par[PathTracer.x][PathTracer.y][PathTracer.z],PathTracer.x,PatTracer.y);
						int dx = -dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][0];
						int dy = -dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][1];
						int dz = -dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][2];
						ChildPoint = PathTracer;        //You can't move me...
						PathTracer.x += dx;
						PathTracer.y += dy;
						PathTracer.z += dz;
						//printf("Move: x: %d y: %d \n",dx,dy);
						//printf("After: %d x: %d y: %d => \n",par[PathTracer.x][PathTracer.y][PathTracer.z],PathTracer.x,PathTracer.y);
						//cout << dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][0] << "  " << dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][1] << endl;
						parDir = par[PathTracer.x][PathTracer.y][PathTracer.z];

						if (parDir >= 0 && parDir < 6) {
							if (!BitTest(electDir[PathTracer.x][PathTracer.y][PathTracer.z], parDir)) {
								electDir[PathTracer.x][PathTracer.y][PathTracer.z] += pow(2, parDir);
							}
							childDir = (par[ChildPoint.x][ChildPoint.y][ChildPoint.z] ^ 1);

							if (!BitTest(curDir[PathTracer.x][PathTracer.y][PathTracer.z], (childDir))) {
								curDir[PathTracer.x][PathTracer.y][PathTracer.z] += pow(2, childDir);
							}
						}

						//Add ID...
						idmap[PathTracer.x][PathTracer.y][PathTracer.z] += nXtPathID;


					}
					nXtPathID *= 2;
					/*
					//Reset the par-map, not necessary...
					for (int x = 0; x < MAPMAX; x++) //x
					for (int z = 0; z < MAPMAX; z++) //z
					for (int y = 0; y < WORLDTOP; y++)  //y
					par[x][y][z] = -1;
					//PrintParent();
					*/
				}
			}
		}
		vis_DFS[curp.x][curp.y][curp.z] = false;
	}
	/*
	void NewDFS(Point curp) {
	vis_DFS[curp.x][curp.y][curp.z] = true;
	//Find the following way(s)...
	for (int i = 0; i < 4; i++) {
	Point newp;
	newp.x = curp.x + dir[i][0];
	newp.y = curp.y + dir[i][1];
	//cout << "DFS i=" << i << " nxt: " << map[newp.x][newp.y][newp.z] << " x: " << newp.x << " y: " << newp.y << " vis: " << vis_DFS[newp.x][newp.y][newp.z] << endl;
	if (IsInDiagram(newp) && !vis_DFS[newp.x][newp.y][newp.z]) {
	if (map[newp.x][newp.y][newp.z] == '=') {
	//Found...
	par[newp.x][newp.y][newp.z] = i;
	//PrintParent();
	//Trace it's path...
	Point PathTracer = (Point){newp.x, newp.y};
	Point ChildPoint;
	int childDir,parDir;

	while (par[PathTracer.x][PathTracer.y][PathTracer.z] != 9) {
	//printf("Dir: %d x: %d y: %d => \n",par[PathTracer.x][PathTracer.y][PathTracer.z],PathTracer.x,PatTracer.y);
	int dx = -dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][0];
	int dy = -dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][1];
	ChildPoint = PathTracer;        //You can't move me...
	PathTracer.x += dx;
	PathTracer.y += dy;
	//printf("Move: x: %d y: %d \n",dx,dy);
	//printf("After: %d x: %d y: %d => \n",par[PathTracer.x][PathTracer.y][PathTracer.z],PathTracer.x,PathTracer.y);
	//cout << dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][0] << "  " << dir[par[PathTracer.x][PathTracer.y][PathTracer.z]][1] << endl;
	parDir = par[PathTracer.x][PathTracer.y][PathTracer.z];

	if (parDir >= 0 && parDir < 4) {
	if (!BitTest(electDir[PathTracer.x][PathTracer.y][PathTracer.z],parDir)) {
	electDir[PathTracer.x][PathTracer.y][PathTracer.z] += pow(2,parDir);
	}
	childDir = (par[ChildPoint.x][ChildPoint.y][ChildPoint.z] ^ 1);

	if (!BitTest(curDir[PathTracer.x][PathTracer.y][PathTracer.z],(childDir))){
	curDir[PathTracer.x][PathTracer.y][PathTracer.z] += pow(2,childDir);
	}
	}

	//Add ID...
	idmap[PathTracer.x][PathTracer.y][PathTracer.z] += nXtPathID;
	}
	nXtPathID *= 2;
	} else if (BitTest(TempCurDir[curp.x][curp.y][curp.z],i ^ 1)) {
	//A new point found, record where it came from...
	par[newp.x][newp.y][newp.z] = i;
	//PrintParent();
	NewDFS(newp);
	//map[newp.x][newp.y][newp.z] = '*';
	}
	}
	}
	vis_DFS[curp.x][curp.y][curp.z] = false;
	}

	//Dijkstra, used to find the right direction...
	bool dopen[MAPMAX][WORLDTOP][MAPMAX];
	int ddist[MAPMAX][WORLDTOP][MAPMAX];
	int ancestor[MAPMAX][WORLDTOP][MAPMAX];

	priority_queue<DistPoint> sup;

	void NodeDijkstra(int ex,int ey){
	//Initialize...
	for (int y = 0; y < MAPMAX; y++)
	for (int x = 0; x < MAPMAX; x++) {
	dopen[x][y][z] = true;
	ddist[x][y][z] = _INFINITY;
	par[x][y][z] = 0; //When mult-parent, initial with 0...
	ancestor[x][y][z] = 0;
	}
	//Then Start...
	ddist[ey][ex] = 0;
	//dopen[ey][ex] = false;
	ancestor[ey][ex] = 15;// 1 + 2 + 4 + 8
	par[End.y][End.x] = 15;

	sup.push(DistPoint(ex,ey,0));

	DistPoint nXt = DistPoint(-1,-1,0);
	while(!sup.empty()) {
	//Choose Point...
	nXt = sup.top();
	sup.pop();
	if (!dopen[nXt.y][nXt.x]) continue;
	//Use the point to update the distances of points around...
	dopen[nXt.y][nXt.x] = false;
	for (int i = 0; i < 4; i++) {
	DistPoint newp = DistPoint(-1,-1,0);
	newp.x = nXt.x + dir[i][0];
	newp.y = nXt.y + dir[i][1];

	if (IsInDiagram(newp)) {
	if (BitTest(electDir[newp.x][newp.y][newp.z],4) || BitTest(curDir[newp.x][newp.y][newp.z],i)) { //'Not sure' or 'Direction Available'...
	if (nXt.x == ex && nXt.y == ey) {
	ancestor[newp.x][newp.y][newp.z] = pow(2,i);
	}

	if (GetResist(map[newp.x][newp.y][newp.z]) + ddist[nXt.y][nXt.x] < (double)ddist[newp.x][newp.y][newp.z])
	par[newp.x][newp.y][newp.z] = pow(2,i);
	else if ((GetResist(map[newp.x][newp.y][newp.z]) + ddist[nXt.y][nXt.x] == (double)ddist[newp.x][newp.y][newp.z] && !BitTest(par[newp.x][newp.y][newp.z],i))) //Another way...
	par[newp.x][newp.y][newp.z] += pow(2,i);

	ddist[newp.x][newp.y][newp.z] = min(GetResist(map[newp.x][newp.y][newp.z]) + ddist[nXt.y][nXt.x], (double)ddist[newp.x][newp.y][newp.z]);
	if (!(nXt.x == ex && nXt.y == ey)) ancestor[newp.x][newp.y][newp.z] = ancestor[newp.x][newp.y][newp.z] | ancestor[nXt.y][nXt.x];
	newp.dist = ddist[newp.x][newp.y][newp.z];
	sup.push(newp);
	}
	}
	}
	}

	//found, trace back...
	for (int y = 0; y < MAPMAX; y++)
	for (int x = 0; x < MAPMAX; x++) {
	vis_DFS[x][y][z] = false;
	}
	curDir[ey][ex] = 16; // binary(10000).....
	AvaDirDFS(End,(Point){ex,ey});
	BitSet(&curDir[ey][ex],4,false);

	PrintDir('M');
	}

	void AvaDirDFS(Point curp,Point target) {
	vis_DFS[curp.x][curp.y][curp.z] = true;

	//Find the following way(s)...
	for (int i = 0; i < 4; i++) {
	if (BitTest(par[curp.x][curp.y][curp.z],i ^ 1)) {
	//PrintBitSet(par[curp.x][curp.y][curp.z]);
	//cout << curp.x << " , " << curp.y << endl;
	Point newp;
	newp.x = curp.x + dir[i][0];
	newp.y = curp.y + dir[i][1];
	if (newp.x == target.x && newp.y == target.y) {
	//Found...
	//cout << "Approached from " << i << endl;
	BitSet(&curDir[target.y][target.x],i,true);
	} else if (IsInDiagram(newp) && !vis_DFS[newp.x][newp.y][newp.z])
	AvaDirDFS(newp,target);
	}
	}
	vis_DFS[curp.x][curp.y][curp.z] = false;
	}
	*/
};

/*
16 7
+--1----+------+
|.......|......|
|....+2-+----+.4
+----+.......|.|
|....+---3---+.|
|..............|
+----------=.#-+

16 9
+--X-----+-----+
|........|.....|
+--------+.....|
|..............|
|....+2---x--+.|
+-6--+.......+-+
|....+---3---+.|
|..............4
+----------=.#-+

17 5
..+-------+......
..|.......|......
#-+-3-+-3-+-3-+-=
......|.......|..
......+-------+..

//ERROR(Fixed In Referee Update, Jul 8th,2019):
17 5
..+---3---+......
..|.......|......
#-+-3-+-1-+-9-+-=
......|.......|..
......+---3---+..
*/


