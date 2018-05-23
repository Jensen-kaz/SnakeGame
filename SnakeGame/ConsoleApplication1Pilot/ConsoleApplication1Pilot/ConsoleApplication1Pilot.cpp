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
#include <stdio.h>
#include "basslibrary\c\bass.h"
 
using namespace std;


HSTREAM streamGame;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool gameOver;
const int width = 30, height = 20;
int y, x, foodX, foodY, score = 0, countTailElements = 0, r[7];
string snakeElement = "*", yourName;
vector<int> tailX(100), tailY(100);
char key = '0';


void Setup();
void Draw();            //Рисуем поле
void StartSettingsGame();		// Логика
void SnakeLogic();			//Поведение змеи
void LeadersTable();
void PlayAudio();
void PlayAudioGameOver();
void Equalizer(int z, int j);


int main(int argc, const char * argv[])
{
	setlocale(LC_ALL, "Russian");
	
	

	Setup();
	StartSettingsGame();
	PlayAudio();
	
	while (!gameOver)
	{
		Draw();
		SnakeLogic();
		
		cout << endl;
	 Sleep(100);
	}

	BASS_ChannelStop(streamGame);
	BASS_StreamFree(streamGame);

    PlayAudioGameOver();


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
	char fieldElement = static_cast<char>(176), food = 'F';
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
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
				cout << fieldElement;
			}

			Equalizer(z, j);     // рисует эквалайзер
		
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	
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
			
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4));
				cout << food;
			}
			else
			{
		
				SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
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
				{
					cout << " ";
				}
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
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	}

	

	for (int i = 0; i < width; i++)
	{
		cout << fieldElement;

	}
	//cout << "X and Y: " << x << " " << y;// << "TailX " << tailX[1];

	cout << endl << endl;
	
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

void PlayAudio()
{
	BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);
	streamGame = BASS_StreamCreateFile(FALSE, PCHAR("media/Komiku_58_Universe_big_takedown.mp3"), 0, 0, BASS_SAMPLE_LOOP);
    BASS_ChannelPlay(streamGame, false);
}

void PlayAudioGameOver() {
	
	streamGame = BASS_StreamCreateFile(FALSE, PCHAR("media/gameOverVoice.wav"), 0, 0, 0);
	BASS_ChannelPlay(streamGame, false);

}

void Equalizer(int z,int j) {

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 14));

	string c1[9] = { "*******","*********","*********", "**********" ,"**************","***************","****************","*********************","********************************" };
	int i;
	i = 0 + rand() % 8;

	switch (z)
	{
	case 0: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 1: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 2: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 3: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 4: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 5: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 6: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 7: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 8: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 9: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 10: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 11: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 12: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 13: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 14: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 15: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 16: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 17: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 18: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	case 19: {
		if (j == width - 1)
			cout << c1[i];
		break;
	}
	}

	

}
