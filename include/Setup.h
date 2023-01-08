#pragma once

const int numOfSheeps = 3;		//Ilość nagród - owiec
int numOfEnemies = 3;			//Początkowa ilośc przeciwników - wściekły tłum

const int whenNewEnemie = 5;	//Kiedy ma się pojawić nowy przeciwnik

int points = 0;					//Początkowa ilość punktów

bool GameOver = false;			//Warunek działania gry

 int COUNTER = 1;				//Dodatkowa zmienna zapewniająca pojawienie się tylko jednego nowego przeciwnika

float speed = 0.1f;				//Prędkość poruszania się gracza - należy dostosować jeśli było by za szybko lub za wolno (zależy od sprzętu jaki się używa
