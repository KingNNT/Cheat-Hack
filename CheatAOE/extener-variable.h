#pragma once
#include "Header.h"

extern DWORD pid = 0;

extern DWORD WoodbaseAddress = 0;
extern DWORD FoodbaseAddress = 0;
extern DWORD GoldbaseAddress = 0;
extern DWORD StonebaseAddress = 0;

extern DWORD moduleAddress = 0;

extern DWORD WoodAddress = WoodbaseAddress + Woodoffset;
extern DWORD FoodAddress = FoodbaseAddress + Foodoffset;
extern DWORD GoldAddress = GoldbaseAddress + Goldoffset;
extern DWORD StoneAddress = StonebaseAddress + Stoneoffset;

extern float newWood = 9999;
extern float newFood = 9999;
extern float newGold = 9999;
extern float newStone = 9999;