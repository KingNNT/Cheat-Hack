#include<iostream>
#include<windows.h>
#include"MemoryManager.h"
#include "DirectXM.h"
#include <tchar.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <Psapi.h>
#include<stdio.h>
#include <math.h>
#include<thread>
#include <cmath>
#include"ESPWork.h"


#pragma comment(lib,"Psapi.lib")
using namespace std;

//const TCHAR *szCaption = _T("Bluestacks");
const TCHAR* szCaption = _T("LDPlayer");
//const TCHAR *szCaption = _T("SmartGaGa");
//const TCHAR *szCaption = _T("Gameloop¡¾Turbo AOW Engine¡¿");


HANDLE hProcess;

int main()
{

	bool Play = false;

	DWORD processIid = FindProcessId(L"LdVBoxHeadless.exe");

	//auto processId = FindProcessId(L"HD-Player.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processIid);
	pMM = new MemoryManager(hProcess);
	handle(hProcess);
	if (hProcess != NULL)
	{
		cout << "Process: " << hProcess << endl;
		cout << "PID: " << processIid << endl;
		cout << "Searching for ViewMatrix.........." << endl;
		Play = cheak_ViewMatrix();


	}


	/*int data;
	ReadProcessMemory(hProcess, (LPCVOID)(0x7BFB6DD8 + 0x1B5), &data , sizeof(int), NULL);
	if(data == 2)
	cout <<hex<<data << endl;*/




	while (Play)
	{
		pDxm = new DirectXM();
		pDxm->SetupWindow(szCaption);

		MSG msg;
		while (1)
		{

			ZeroMemory(&msg, sizeof(MSG));
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			HWND hWnd = NULL;
			hWnd = FindWindow(0, szCaption);
			if (!hWnd)
			{
				DestroyWindow(pDxm->over_hWnd);
				delete pDxm;
				break;
			}

			hWnd = ::GetWindow(hWnd, GW_CHILD);

			RECT rc;
			ZeroMemory(&rc, sizeof(RECT));

			GetWindowRect(hWnd, &rc);

			pDxm->s_width = rc.right - rc.left;
			pDxm->s_height = rc.bottom - rc.top;

			MoveWindow(pDxm->over_hWnd, rc.left, rc.top, pDxm->s_width, pDxm->s_height, true);

			HWND hwnd2 = GetForegroundWindow();
			HWND hwnd3 = GetWindow(hwnd2, GW_HWNDPREV);



			SetWindowPos(pDxm->over_hWnd, hwnd3, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			UpdateWindow(pDxm->over_hWnd);
			pDxm->render();
			Sleep(20);
		}
	}

	cout << "Exit Game!" << endl;
	cin.get();
	return 0;
}

============================================================

ESPWork.cpp

#include<iostream>
#include<windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include<stdio.h>
#include <math.h>
#include "ESPWork.h"
using namespace std;


#define DARKORANGE  D3DCOLOR_ARGB(255,255,127,000)
#define TextWhite   D3DCOLOR_ARGB(255,255,255,255)
#define TextRed     D3DCOLOR_ARGB(255,255,0,0)
#define TextGreen   D3DCOLOR_ARGB(255,0,255,0)
#define TextBlue    D3DCOLOR_ARGB(255,0,0,255)
#define TextBlack   D3DCOLOR_ARGB(255,0,0,0)
#define TextPurple  D3DCOLOR_ARGB(255,125,0,255)
#define TextGrey    D3DCOLOR_ARGB(255,128,128,128)
#define TextYellow  D3DCOLOR_ARGB(255,255,255,0)
#define TextOrange  D3DCOLOR_ARGB(255,255,140,0)
#define TextCyan    D3DCOLOR_ARGB(255,0,139,200)
#define TextPink    D3DCOLOR_ARGB(255,255,192,203)



BYTE ViewPatt[] = { 0x00, 0x00, 0x80, 0x3F ,0x00 ,0x00 ,0x80 ,0x3F ,0x00 ,0x00 ,0x80 ,0x3F ,0x00 ,0x00 ,0x80 ,0x3F ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x3F ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x3F ,0x00 ,0x00 ,0x80 ,0x3F };
BYTE Players_Pattren[] = { '?', '?', '?', '?', 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', 0x00, 0x00, 0x00, '?','?', '?', '?', '?', '?', '?', '?', '?', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '?', '?', '?', '?', '?', '?', '?', '?', 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xA6, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
BYTE Players_Pattren2[] = { 0x78, 0x72, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x14, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
BYTE bCarsTag[] = { 0xEC,'?', '?', '?',0x00,0x00,0x20,0x00 };
//BYTE bCarsTag[] = { 0xEC, '?', '?', '?', 0x00, 0x00, 0x20, 0x00 };

std::vector<DWORD_PTR> ViewMatrix;
std::vector<DWORD_PTR> Players;

HWND game_hWnd;

DWORD Vaddr = 0;
DWORD Paddr[30];
DWORD GOODS_POS_OFFSET = 336;

int c;
int PlayerNum = 0;

DWORD dwCars[1000];
DWORD dwCarsCount;

DirectXM* pDxm = NULL;

HANDLE hPro;

byte d;
byte live;

DWORD Status;
DWORD Drive;
DWORD Jump;
DWORD Swmming;


void handle(HANDLE h)
{
	hPro = h;
}

int GetpNum()
{
	return PlayerNum;
}

bool check(DWORD vAddr)
{
	D3DMATRIX GameViewMatrix = pMM->RPM<D3DMATRIX>(vAddr + 0xD0, sizeof(D3DMATRIX));
	if (GameViewMatrix._41 != 0)
	{
		Vaddr = vAddr;
		return 1;
	}
	return 0;
}


bool cheak_ViewMatrix()
{
	pMM->MemSearch(ViewPatt, sizeof(ViewPatt), 0x00000000, 0xffffffff, FALSE, 1, ViewMatrix);
	std::vector<DWORD_PTR>::iterator i;

	for (i = ViewMatrix.begin(); i != ViewMatrix.end(); i++)
	{
		if (check(ViewMatrix[c]))
		{
			cout << "View Matrix Found at : " << "0x" << hex << ViewMatrix[c] << endl;
			CloseHandle(CreateThread(0, 0, TIMER, 0, 0, 0));
			return 1;
		}
		c++;
	}
	c = 0;
	return 0;
}



DWORD WINAPI TIMER(LPVOID p)
{

	while (true)
	{

		pMM->MemSearch(Players_Pattren2, sizeof(Players_Pattren2), 0x00000000, 0xffffffff, FALSE, 1, Players);
		PlayerNum = Players.size();
		cout << "Scan" << endl;
		for (int i = 0; i < 30; i++)
			Paddr[i] = 0;

		for (int i = 0; i < PlayerNum; i++)
			Paddr[i] = Players[i];
		Players.clear();
	}
	return 1;
}



BOOL WorldToScreen(D3DXVECTOR3 _Enemy_Point, D3DXVECTOR3& _Screen_Point, int& Distance)
{
	_Screen_Point = D3DXVECTOR3(0, 0, 0);
	float ScreenW = 0;
	D3DMATRIX GameViewMatrix = pMM->RPM<D3DMATRIX>(Vaddr + 0xD0, sizeof(D3DMATRIX));

	ScreenW = (GameViewMatrix._14 * _Enemy_Point.x) + (GameViewMatrix._24 * _Enemy_Point.y) + (GameViewMatrix._34 * _Enemy_Point.z + GameViewMatrix._44);
	Distance = ScreenW / 100;
	if (ScreenW < 0.0001f)
	{
		return FALSE;
	}


	float ScreenY = (GameViewMatrix._12 * _Enemy_Point.x) + (GameViewMatrix._22 * _Enemy_Point.y) + (GameViewMatrix._32 * (_Enemy_Point.z + 85) + GameViewMatrix._42);
	float ScreenX = (GameViewMatrix._11 * _Enemy_Point.x) + (GameViewMatrix._21 * _Enemy_Point.y) + (GameViewMatrix._31 * _Enemy_Point.z + GameViewMatrix._41);

	_Screen_Point.y = (pDxm->s_height / 2) - (pDxm->s_height / 2) * ScreenY / ScreenW;
	_Screen_Point.x = (pDxm->s_width / 2) + (pDxm->s_width / 2) * ScreenX / ScreenW;
	float y1 = (pDxm->s_height / 2) - (GameViewMatrix._12 * _Enemy_Point.x + GameViewMatrix._22 * _Enemy_Point.y + GameViewMatrix._32 * (_Enemy_Point.z - 95) + GameViewMatrix._42) * (pDxm->s_height / 2) / ScreenW;
	_Screen_Point.z = y1 - _Screen_Point.y;
	return TRUE;
}


void print()
{

	if (PlayerNum > 0)
		for (int i = 0; i != PlayerNum; i++)
		{
			ReadProcessMemory(hPro, (LPCVOID)(Paddr[i] + 0xA3), &d, sizeof(d), NULL);
			ReadProcessMemory(hPro, (LPCVOID)(Paddr[i] + 0x1A0), &live, sizeof(live), NULL);
			ReadProcessMemory(hPro, (LPCVOID)(Paddr[i] + 0x45A), &Status, sizeof(Status), NULL);
			ReadProcessMemory(hPro, (LPCVOID)(Paddr[i] - 0x110), &Drive, sizeof(Drive), NULL);
			ReadProcessMemory(hPro, (LPCVOID)(Paddr[i] - 0x30), &Jump, sizeof(Jump), NULL);
			ReadProcessMemory(hPro, (LPCVOID)(Paddr[i] + 0x174), &Swmming, sizeof(Swmming), NULL);


			//if (d == 288230376151711744 && live != 0)
			if (d != 68)
			{
				d = 0;

				D3DXVECTOR3 EnPos = pMM->RPM<D3DXVECTOR3>(Paddr[i] - 0x80 - 0x8, sizeof(D3DXVECTOR3));
				D3DXVECTOR3 vret;
				int EnDistance = 0;

				if (WorldToScreen(EnPos, vret, EnDistance) && EnPos != 0 && EnDistance < 501)

				{
					float x = vret.x;
					float y = vret.y;
					float h = vret.z;
					float w = vret.z / 2;
					//if ((Status == 17072 || Status == 4123214511) && (Drive == 0 && Jump == 0 && Swmming == 0) && EnDistance < 201) //stand
					//{
					//	pDxm->DrawString(x - vret.z / 4, y + vret.z, TextYellow, pDxm->pFont, "Stand", 1);
					//}
					//if (Status == 16968 && EnDistance < 201)//prone
					//{
					//	y = vret.y + vret.z / 4;
					//	h = h - vret.z / 4;
					//	pDxm->DrawString(x - vret.z / 4, y + vret.z, TextYellow, pDxm->pFont, "Prone", 1);
					//}
					//if ((Status == 17008 || Status == 4123214447) && EnDistance < 201) //crouch
					//{
					//	y = vret.y + vret.z / 5;
					//	h = h - vret.z / 5;
					//	pDxm->DrawString(x - vret.z / 4, y + vret.z, TextYellow, pDxm->pFont, "Crouch", 1);
					//}
					/*if (Drive != 0 && EnDistance < 201)
					{
						y = vret.y + vret.z / 4;
						h = h - vret.z / 4;
						pDxm->DrawString(x - vret.z / 4, y + vret.z, TextYellow, pDxm->pFont, "Drive", 1);
					}*/
					/*if (Jump != 0 && Swmming == 0 && EnDistance < 201)
					{
						pDxm->DrawString(x - vret.z / 4, y + vret.z, TextYellow, pDxm->pFont, "Jump", 1);
					}*/
					/*if (Swmming != 0 && EnDistance < 201)
					{
						y = vret.y + vret.z / 4;
						h = h - vret.z / 4;
						pDxm->DrawString(x - vret.z / 4, y + vret.z, TextYellow, pDxm->pFont, "Sweeming", 1);
					}*/

					pDxm->DrawLine(x, y, TextWhite);
					pDxm->DrawBox((x - vret.z / 4) - 3, y - 5, w + 3, h + 5, TextGreen);
					pDxm->DrawString(x, y, TextYellow, pDxm->pFont, "[%dm]", EnDistance - 1);
				}
			}


		}



}