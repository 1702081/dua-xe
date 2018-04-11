#include<Windows.h>
#include<time.h>
#include<iostream>
#include<conio.h>
using namespace std;

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

bool gameOver = false;
int height = 30;
int width = 19;

class Car {
public:
	Car() {
		size_ = 3;
		x_ = (width + 1) / 2;
		y_ = height - 1;
	}
	void DrawCar() {
		gotoxy(x_, y_); cout << "X";
		gotoxy(x_ - 1, y_ - 1); cout << "O";
		gotoxy(x_ + 1, y_ - 1); cout << "O";
		gotoxy(x_ - 1, y_ + 1); cout << "O";
		gotoxy(x_ + 1, y_ + 1); cout << "O";
		gotoxy(x_ - 1, y_); cout << "I";
		gotoxy(x_ + 1, y_); cout << "I";
		gotoxy(x_, y_ - 1); cout << "^";
		gotoxy(x_, y_ + 1); cout << "_";
	}
	void ControlCar() {
		//fflush(stdin); //xoa bo nho dem phim
		if (_kbhit()) {
			switch (_getch()) {
			case 'a':
				if (x_>2) x_ -= 4; // sang trai khong chay ra khoi map
				break;
			case 'd':
				if (x_<17) x_ += 4; // sang phai khong chay ra khoi map
				break;
			}
		}
	}

	int get_x() { return x_; }
	int get_y() { return y_; }
	int get_score() { return score_; };
	void set_score() { score_++; }
private:
	int size_;
	int x_, y_; // toa do x, y cua xe
	int score_ = 0;
};

class Threat {
public:
	Threat() {
		int random_wave = rand() % 5; // khoi tao vi tri ban dau
		x_ = 2 + random_wave * 4;
	}
	void ReInitThreat() {
		int random_wave = rand() % 5; // khoi tao lai vi tri cho vat can
		x_ = 2 + random_wave * 4;
		y_ = 0;
	}

	void DrawThreat() {
		gotoxy(x_, y_); cout << "X";
		gotoxy(x_ - 1, y_ - 1); cout << "X";
		gotoxy(x_ + 1, y_ - 1); cout << "X";
		gotoxy(x_ - 1, y_ + 1); cout << "X";
		gotoxy(x_ + 1, y_ + 1); cout << "X";
		gotoxy(x_ - 1, y_); cout << "X";
		gotoxy(x_ + 1, y_); cout << "X";
		gotoxy(x_, y_ - 1); cout << "X";
		gotoxy(x_, y_ + 1); cout << "X";
	}
	void ControlThreat(Car& car) {
		y_++; // luon di xuong
		if (y_ == height) {
			ReInitThreat(); // neu cham bien duoi thi tao lai vat can
			car.set_score();
		}
	}

	void set_x(int a) { x_ = a; }
	void set_y(int a) { y_ = a; }
	int get_x() { return x_; }
	int get_y() { return y_; }

private:
	int x_, y_;
};

void map() {
	gotoxy(0, 0);
	for (int i = 1; i <= height + 1; i++)
		cout << (char)186 << endl;
	for (int i = 0; i <= height; i++) {
		gotoxy(20, i);
		cout << (char)186;
	}
	gotoxy(1, 31);
	for (int i = 1; i <= width; i++)
		cout << (char)205;

	int distance = 4; // kich thuoc 1 lan xe
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= height; j++) {
			gotoxy(distance, j);
			cout << (char)179;
		}
		distance += 4;
	}
}

void logic_gameover(int car_x, int car_y, int threat_x, int threat_y) {
	if (abs(car_x - threat_x) < 3 && abs(car_y - threat_y) < 2) gameOver = true;
}

int main() {
	srand(time(NULL));

	Car car;

	Threat threat[6];
	threat[0].set_y(20);
	for (int i = 1; i < 5; i++) {
		threat[i].set_y(threat[i - 1].get_y() - 6);
	}

	map();
	gotoxy(0, 10);
	cout << "game will be start in 5 second" << endl;
	cout << "left: A \t\t right: D";
	Sleep(5000);

	while (!gameOver) {
		map();
		gotoxy(14, 0);
		cout << "SCORE: " << car.get_score();

		for (int i = 0; i < 5; i++) {
			if (threat[i].get_y() >= 0) threat[i].DrawThreat(); threat[i].ControlThreat(car);

			logic_gameover(car.get_x(), car.get_y(), threat[i].get_x(), threat[i].get_y());
		}

		car.DrawCar();
		car.ControlCar();

		Sleep(20);
		system("cls");
	}

	map();
	gotoxy(10, 15);
	cout << "SCORE: " << car.get_score();

	return 0;
}
