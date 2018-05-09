#include <iostream>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;

#define consoleWidth  80
#define consoleHeight  20
#define roadWidth      20

#define ColorCode_Blue			9
#define ColorCode_Cyan			11
#define ColorCode_Red			12
#define default_ColorCode		7
#define key_none	-1

int inputKey()
{
	if (_kbhit())
	{
		int key = _getch();
		if (key == 224)	// special key
		{
			key = _getch();
			return key + 1000;
		}
		return key;
	}
	else
	{
		return key_none;
	}
	return key_none;
}

void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = { 0, 0 };
	DWORD	dummy;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}

void gotoXY(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void TextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

struct ToaDo
{
	int y, x;
};

struct HinhDang
{
	char a[3][3];
};

struct Xe
{
	ToaDo td; // toa do tai vi tri hd.a[1][1]
	HinhDang hd;
	int diem;
};

struct VatCan
{
	ToaDo td;
	HinhDang hd;
};

void KhoiTao(Xe &xe, VatCan &vc)
{
#define pHD xe.hd.a
	xe.diem = 0;
	// vị trí xe
	xe.td.y = consoleHeight - 2;
	xe.td.x = roadWidth / 2;
	// hình dạng xe
	pHD[0][0] = pHD[0][2] = pHD[2][0] = pHD[2][2] = 'o';
	pHD[0][1] = '^';
	pHD[1][0] = pHD[1][2] = '|';
	pHD[2][1] = '-';
	pHD[1][1] = 'X';
	// vị trí vật cản
	vc.td.x = 1 + rand() % (roadWidth - 4);
	vc.td.y = -1;
	// hình dáng vật cản
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vc.hd.a[i][j] = 173;
		}
	}
}

void HienThi(Xe xe, VatCan vc)
{
	clrscr();
// bien
	TextColor(default_ColorCode);
	for (int i = 0; i < consoleHeight; i++)
	{
		putchar(179);
		cout << endl;
	}
	for (int i = 0; i < consoleHeight; i++)
	{
		gotoXY(roadWidth, i);
		putchar(179);
	}
// xe
	TextColor(ColorCode_Cyan);
	for (int kDong = -1; kDong <= 1; kDong++)
	for (int kCot = -1; kCot <= 1; kCot++)
	{
		int x = kCot + xe.td.x;
		int y = kDong + xe.td.y;

		gotoXY(x, y);
		putchar(xe.hd.a[kDong + 1][kCot + 1]);	
	}
// vat can
	TextColor(ColorCode_Red);
	for (int kDong = -1; kDong <= 1; kDong++)
	for (int kCot = -1; kCot <= 1; kCot++)
	{
		int x = kCot + vc.td.x;
		int y = kDong + vc.td.y;

		if (y >= 0 && y < consoleHeight)
		{
			gotoXY(x, y);
			putchar(vc.hd.a[kDong + 1][kCot + 1]);
		}
	}
	// trang thai 
	gotoXY(roadWidth + 2, 2);
	cout << "diem = " << xe.diem << endl;
}


void DieuKhien(Xe &xe)
{
	if (_kbhit())
	{
		int key = _getch();
		if (key == 'A' || key == 'a'&& xe.td.x > 2)
			xe.td.x--;
		else if (key == 'D' || key == 'd'&& xe.td.x < roadWidth - 2)
			xe.td.x++;
	}
}

int XuLy(Xe &xe, VatCan &vc, int &ThoiGianSleep)
{
	//xe di chuyen
	vc.td.y++;

	if (vc.td.y > consoleHeight)
	{
		xe.diem++;
		vc.td.x = 2 + rand() % (roadWidth - 4);
		vc.td.y = -2;

		if (ThoiGianSleep > 39)
			ThoiGianSleep -= 20;
	}
	// xe va cham
	int dX = abs(xe.td.x - vc.td.x);
	int dY = abs(xe.td.y - vc.td.y);

	if (dX < 3 && dY < 3)
	{
		return -1;
	}

	return 0;

}

int main()
{
	srand (time_t(NULL)); // khoi tao bo sinh so ngau nhien
	Xe xe;
	VatCan vc;
	KhoiTao(xe, vc);
	int ma = 0;
	int ThoiGianSleep = 75;

	while (1)
	{
		HienThi(xe, vc);
		DieuKhien(xe);
		ma=XuLy(xe, vc, ThoiGianSleep);
		//game over
		if (ma == -1)
		{
			clrscr();
			gotoXY(0, 0);
			TextColor(default_ColorCode);
			cout << "diem cua ban la: " << xe.diem << endl;
			TextColor(ColorCode_Blue);
			cout << "  ####    ####   #    #  ######   ####   #    #  ######  #####   " << endl;
			cout << " #    #  #    #  ##  ##  #       #    #  #    #  #       #    #  " << endl;
			cout << " #       #    #  # ## #  ######  #    #  #    #  ######  #####   " << endl;
			cout << " #   ##  ######  #    #  #       #    #  #    #  #       #  #    " << endl;
			cout << " #    #  #    #  #    #  #       #    #   #  #   #       #   #   " << endl;
			cout << "  ####   #    #  #    #  ######   ####     ##    ######  #    #  " << endl;
			while (_getch() != 13);
			break;
			
		}
		Sleep(ThoiGianSleep);

	}
	

}