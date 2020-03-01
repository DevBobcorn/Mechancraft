#pragma once

#define PI 3.1415927

namespace Mechanicraft{
	// settings
	const int TitleHeight = 30;
	const int BorderWidth = 8;
	const int SCR_WIDTH = 870 - TitleHeight - BorderWidth; //Screen
	const int SCR_HEIGHT = 518 - 2 * BorderWidth;
	int CUR_WIDTH; //Cursor
	int CUR_HEIGHT;
	const float BuildCoolDownTime = 0.1f;
	bool IsCreativeMode = true;
	const float CreativeModeBreakTime = 0.03f;
	float cursorScale = 1.0f;
	
	const float RedStoneUpdateTime = 0.1f;
	float RedStoneRemainTime = 0.0f;
	
	//Inventory
	int IDInHand = 1;
	bool ShowInventory = false;
	
	//Tex Num
	const int TexMax = 1000;
	const int BlockMax = 200;
	const int DamageMax = 15;
	int BlockNum;
	int DamageNum;
}
