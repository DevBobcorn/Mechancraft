#pragma once
using namespace std;
#include <bitset>
#include <cmath>
#define BITLEN 8

struct IDData {
	int id;
	int weight;
	vector<int> components; //Maybe not its Subflows, maybe smaller combinations(IDs)...
};

bool comp(const IDData &a, const IDData &b) {
	return a.weight < b.weight;
}

bool IsMinSubFlow(int f) {
	//Quickly work out whether the number is a power of 2 or not...
	if (f != 0 && (f & f - 1) == 0) return true;
	return false;
}

int GetMaximumSubFlow(int f) {
	int ans = 1;
	while (ans <= f) ans <<= 1;
	return (ans >>= 1);
}

//To calculate the max path count held by a MAX PATH ID...
int GetPathNum(int MaxPathID) {
	int ans = 0;
	while (true) {
		if (MaxPathID >>= 1) ans++;
		else break;
	}
	return ans;
}

int GetSubFlowNum(int f) {
	int count = 0;
	while (f)
	{
		count++;
		f = (f - 1) & f;
	}
	return count;
}

//Indicates whether the binary code of a contains that of b...
/* Example:
011101 (a)-----+
-  010100 (b)     +=> 001001 (subs & a) Equals subs.
-------------      |      L=> Contains? True!
001001 (subs)<-+

Another Example:
110111 (a)-----+
-  101001 (b)     +=> 010100 (subs & a) Does NOT Equal subs.
-------------      |      L=> Contains? False!
011100 (subs)<-+
*/
bool DoesAContainB(int a, int b, bool allowContSelf) {
	if (!allowContSelf && (a == b)) return false;
	int subs = a - b;
	return (subs & a) == subs;
}

bool HasIntersection(int a, int b) {
	return (bool)(a & b);
}

bool BitTest(int num, int index) {
	return bitset<BITLEN>(num).test(index);
}

void BitSet(int *num, int index, bool val) {
	if ((!BitTest(*num, index)) && val) *num += pow(2, index);
	if (BitTest(*num, index) && (!val)) *num -= pow(2, index);
}

void PrintBitSet(int num) {
	cout << bitset<BITLEN>(num) << " (" << num << ")";
}
