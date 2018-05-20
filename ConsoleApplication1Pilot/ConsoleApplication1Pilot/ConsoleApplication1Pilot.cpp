// ConsoleApplication1Pilot.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <array>
#include<string>
#include<conio.h>
#include <ctime>
#include <Windows.h>
#include <vector>
#include <fstream>
 
using namespace std;

bool gameOver;
const int width = 30, height = 20;
int y, x, foodX, foodY, score = 0, countTailElements = 0;
string snakeElement = "*", yourName;
vector<int> tailX(100), tailY(100);
char key = '0';


void Setup();
void Draw();            //Рисуем поле
void StartSettingsGame();		// Логика
void SnakeLogic();			//Поведение змеи
void LeadersTable();



int main(int argc, const char * argv[])
{
	setlocale(LC_ALL, "Russian");
	
	
	Setup();
	StartSettingsGame();
	
	while (!gameOver)
	{
		Draw();
		SnakeLogic();
		
		cout << endl;
	 Sleep(100);
	}
	
	system("cls");
	cout << "Game Over "<<"\t"<<"YOUR SCORE "<<score<<endl;
	for (int i = 0; i < 30; i++)
	{
		cout << "-";
	}
	cout << endl << "Leaders Table" << endl<<endl;

	LeadersTable();
	

	system("pause");
    return 0;
}


void Draw() {
	system("cls");
	string fieldElement = "#", food = "F";
	int countTailElementsLocal;

	cout << "Snake Game   " <<"\t      "<< "SCORE: " << score <<endl;
	for (int i = 0; i < width; i++)
	{
		cout << fieldElement;
	}
	cout << endl;

	for (int z = 0; z < height; z++)
	{
		for (int j = 0; j < width; j++)
		{
			if (j == 0 || j == width-2)
			{
				cout << fieldElement;
			}

	
			if (z == y && j == x) // движения змеи
			{
				cout << snakeElement;
				countTailElementsLocal = countTailElements; // длина хвоста локально
				if (countTailElements > 0)
				{
					do
					{
						tailX[countTailElementsLocal] = tailX[countTailElementsLocal -1];
						tailY[countTailElementsLocal] = tailY[countTailElementsLocal -1];
					} while (--countTailElementsLocal);

						tailX[0]=x;
						tailY[0]=y;
				}
			}
			else if (z == foodY && j == foodX)
			{
				cout << food;
			}
			else
			{
				bool flag = false;
				for (int h = 1; h <= countTailElements; h++)
				{
					if (z == tailY[h] && j == tailX[h])
					{
						flag = true;
						cout << snakeElement;
					}
				}
				if (!flag)
					cout << " ";
			}

			if ((z == foodY && j == foodX) && (z == y && j == x))
			{
				foodX = 1 + rand() % (width - 3);
				foodY = 1 + rand() % (height - 1);
				score += 5;
				countTailElements++;
				tailX[0] = x;
				tailY[0] = y;
			}
		}
		cout << endl;
	}

	for (int i = 0; i < width; i++)
	{
		cout << fieldElement;

	}
	//cout << "X and Y: " << x << " " << y;// << "TailX " << tailX[1];
}

void Setup()
{
	cout << "Enter your name" << endl;
	cin >> yourName;
	score = 0;
	gameOver = false;
}

void StartSettingsGame()
{
	srand(time(0));
	y = 1+rand() % height;
	x = 1+rand() % (width-5);
	foodX = 1 + rand() % (width - 4);
	foodY = 1 + rand() % (height-1);
}


void SnakeLogic()
{
	int codeKey=0, codeKeyFlag;
	char keyFlag=-33;

	if (_kbhit())
	{
		while (keyFlag <= -32)
		{
			keyFlag = getch();
		}
		codeKeyFlag = static_cast<int>(keyFlag);
		codeKey = static_cast<int>(key);

		if (codeKey == 72 && (codeKeyFlag == -32 || codeKeyFlag == 80))
		{
			codeKey = 72;
		}
		else if (codeKey == 80 && (codeKeyFlag == -32 || codeKeyFlag == 72))
		{
			codeKey = 80;
		}
		else if (codeKey == 77 && (codeKeyFlag == -32 || codeKeyFlag == 75))
		{
			codeKey = 77;
		}
		else if (codeKey == 75 && (codeKeyFlag == -32 || codeKeyFlag == 77))
		{
			codeKey = 75;
		}
		else {
			key = keyFlag;
		}
	}

	codeKey = static_cast<int>(key);
    
		switch (codeKey) //управление
		{
		case 72: {
			y--;                   // UP
			break;
		}
		case 80: {
			y++;                  // DOWN
			break;
		}
		case 77: {
			x++;               // RIGHT
			break;
		}

		case 75: {
			x--;                 //LEFT
			break;
		}

		}
	
	for (int i = 1; i <= countTailElements; i++)
	{
		if (y == tailY[i] && x == tailX[i])
		{
			gameOver = true;
		}
	}

	if (y==-1 || y==height+1 || x == -1 || x == width-2)
	{
		gameOver = true;
	}

}

void LeadersTable() 
{
	char buff[50];
	string s;

	fstream fileLength("tableleader.txt");
	long fileSize;
	fileLength.seekg(0, ios::end);
	fileSize = fileLength.tellg();
	fileLength.close();


	if (fileSize == 0)
	{
		ofstream fileWrite("tableleader.txt");
		fileWrite << yourName << "\t" << " Score " << score << endl;
		fileWrite.close();
	}
	else
	{
		ofstream fileWrite("tableleader.txt", ios_base::app);
		fileWrite << yourName << "\t" << " Score " << score << endl;
		fileWrite.close();
	}
	

	ifstream fileRead("tableleader.txt");
	
	while (getline(fileRead, s))
	{
		cout << s<<endl;
	}
	fileRead.close();
}

