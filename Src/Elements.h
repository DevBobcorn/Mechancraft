#pragma once

#define _INFINITY 1000000
#define IGNORABLE 0.000001

const int littleBulb = 5;
const int largeBulb = 10;

/* 	The Elements in the Current.
'1'-'9' represent resisters from 1 Ohm to 9 Ohm,
'X' represents large light bulb,
'x' represents little light bulb,
'A' represents current flow meter,
'V' represents voltage meter...
*/

bool IsElement(char c) {
	if (c >= 49 && c <= 57) return true; //Resister...
	switch (c) {
	case 'A':
	case 'V':
	case 'X':
	case 'x':
		return true;
	}
	return false;
}

double GetResist(char c) {
	if (c >= 49 && c <= 57) return c - 48.0; //Resister...
	switch (c) {
	case 'A':
		return IGNORABLE;
	case 'V':
		return _INFINITY;
	case 'X':
		return littleBulb;
	case 'x':
		return largeBulb;
	}
	return 0;
}
