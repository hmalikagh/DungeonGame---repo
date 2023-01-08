#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>
#include<random>

#include "Setup.h"


///////////////////////////////////////////////////////////
// Funckja losuj�ca liczb�
///////////////////////////////////////////////////////////
//		
// INPUT:
//		size - maksymalna warto�� do jak� mo�na wylosowa�
//		
// OUTPUT:
//		losowa liczba od 0 do liczby podanej jako argument funkcji
//		
// REMARKS:
//		maksymalna liczba jak� mo�na wylosowa� jest o jeden mniejsza
//		ni� liczba podana jako argument size
//
int random(int size)
{
	std::random_device rd;
	std::mt19937 mtRandomEngine(rd());
	std::uniform_int_distribution uni_distr(0, size - 1);
	return uni_distr(mtRandomEngine);
}

///////////////////////////////////////////////////////////
// Klasa reprezentuj�ca jednostke w mojej grze
///////////////////////////////////////////////////////////
class Entity
{
public:
	
	
	///////////////////////////////////////////////////////////
	// Konsrtyktor klasy
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		x - rozmiar w osi X jednostki
	//		y - rozmiar w osi Y jednostki
	//		textu - textura jaka bedzie mia�a jednostka
	//
	Entity(float x, float y, std::string textu)
	{
		rect.setSize(sf::Vector2f(x, y));
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		texture.loadFromFile(textu);
		rect.setTexture(&texture);
	}

	
	///////////////////////////////////////////////////////////
	//Funkcja ustawiaj�ca wcisni�cie/odpuszczenie danego klawisza w,a,s,d
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		key - klawisz kt�ry ma by� wci�ni�ty/niewci�ni�ty
	//		checkPressed - czy klawisz jest wcisniety czy nie
	// REMARKS:
	//		Je�li checkPressed == true to klawisz jest wcisni�ty,
	//		Je�li checkPressed == false to kalwisz jest niewci�niety
	//
	void processEvent(sf::Keyboard::Key key, bool checkPressed)
	{
		if (checkPressed == true)
		{
			if (key == sf::Keyboard::W)
				up = true;
			if (key == sf::Keyboard::S)
				down = true;
			if (key == sf::Keyboard::A)
				left = true;
			if (key == sf::Keyboard::D)
				right = true;
		}
		if (checkPressed == false)
		{
			up = false;
			down = false;
			left = false;
			right = false;
		}
	}
	///////////////////////////////////////////////////////////
	// Funkcja aktualizuj�ca pozycje gracza oraz sprawdzaj�ca 
	// kolizje z przeciwnikami (Enemie) oraz nagrodami (sheep),
	// a tak�e dodaj�ca nowego przeciwnika 
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		window - okno na kt�rym b�dziemy wy�wietla� dane
	//		sheep - wektor w, kt�rym znajduj� si� "owce" (nagrody)
	//		Enemie - wektor w kt�rym znajduje si� "w�ciek�y t�um" (przeciwnicy)
	// REMARKS:
	//		Je�li kt�ry� z przycisk�w jest wcisni�ty odpowiadaj�ca mu warto�� up, 
	//		down, left, right spowoduje poruszenie si� naszej postaci w dan� strone z odpowiedni� pr�dko�ci�
	// 
	//		Je�li nasza posta� wejdzie w granice owcy to danej owcy zostanie wylosowana nowa pozycja
	// 
	//		Je�li nasza posta� wejdzie w granice enemie to nast�pi koniec gry
	// 
	//		Gdy liczba punkt�w b�dzie r�wna wilokrotno�ci whenNewEnemie to pojawi si� nowy przeciwnik
	//
	void update(sf::RenderWindow& window, std::vector<Entity>& sheep, std::vector<Entity>& Enemie)
	{
		sf::Vector2f movement{ 0,0 };
		float speed = 0.1f;
		if (up)
		{
			movement.y -= speed;
			if (rect.getPosition().y < 0)
				rect.setPosition(rect.getPosition().x, 0);
		}
		if (down)
		{
			movement.y += speed;
			if (rect.getPosition().y > window.getSize().y - 50)
				rect.setPosition(rect.getPosition().x, window.getSize().y - 50);
		}
		if (left)
		{
			movement.x -= speed;
			if (rect.getPosition().x < 0)
				rect.setPosition(0, rect.getPosition().y);

		}
		if (right)
		{
			movement.x += speed;
			if (rect.getPosition().x > window.getSize().x - 50)
				rect.setPosition(window.getSize().x - 50, rect.getPosition().y);
		}

		//Collisions with sheeps
		for (auto i = 0; i < sheep.size(); ++i)
		{
			sf::FloatRect playerBounds = rect.getGlobalBounds();
			sf::FloatRect sheepBounds = sheep[i].rect.getGlobalBounds();

			nextPosition = playerBounds;
			nextPosition.left += movement.x;
			nextPosition.top += movement.y;

			if (sheepBounds.intersects(nextPosition))
			{
				sheep[i].rect.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
				points++;
				COUNTER = 0;
			}
		}
		//Collison with enemies
		for (auto i = 0; i < Enemie.size(); ++i)
		{
			sf::FloatRect playerBounds = rect.getGlobalBounds();
			sf::FloatRect enemieBounds = Enemie[i].rect.getGlobalBounds();

			nextPosition = playerBounds;
			nextPosition.left += movement.x;
			nextPosition.top += movement.y;

			if (enemieBounds.intersects(nextPosition))
			{
				GameOver = true;
			}
		}


		//New enemie
		if (points % whenNewEnemie == 0 && COUNTER == 0)
		{
			
			Entity tmp(50, 50, "images\\angry_mob.png");
			tmp.rect.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
			Enemie.push_back(tmp);
			numOfEnemies++;
			COUNTER++;
		}

		rect.move(movement);
	}

	///////////////////////////////////////////////////////////
	// Funkcja wy�wietlaj�ca aktualn� pozycj� jednostki w oknie
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		window - okno na kt�rym b�dziemy wy�wietla� dane
	//
	void drawTo(sf::RenderWindow& window)
	{
		window.draw(rect);
	}

	///////////////////////////////////////////////////////////
	// Funkcja ustawiaj�ca texture danej jednostki 
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		textu - textura jaka ma by� ustawiona
	//
	void setTexture(std::string textu) {
		texture.loadFromFile(textu);
		rect.setTexture(&texture);
	}

	///////////////////////////////////////////////////////////
	// Funkcja ustawiaj�ca pozycje danej jednostki
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		x - pixel na osi X gdzie ma by� ustawiona jednostka
	//		y - pixel na osi Y gdzie ma by� ustawiona jednostka
	//
	void setPosition(int x, int y)
	{
		rect.setPosition(x, y);
	}

	///////////////////////////////////////////////////////////
	// Funkcja zwracaj�ca pozycje jednostki w osi X
	///////////////////////////////////////////////////////////
	//		
	// OUTPUT:
	//		Pozycja na osi X jednostki
	//
	int getPositionX()
	{
		return rect.getPosition().x;
	}

	///////////////////////////////////////////////////////////
	// Funkcja zwracaj�ca pozycje jednostki w osi Y
	///////////////////////////////////////////////////////////
	//		
	// OUTPUT:
	//		Pozycja na osi Y jednostki
	//
	int getPositionY()
	{
		return rect.getPosition().y;
	}

	///////////////////////////////////////////////////////////
	// Funkcja zwracaj�ca teksture jednostki 
	///////////////////////////////////////////////////////////
	//		
	// OUTPUT:
	//		Textura jednostki w kodzie szesnatkowym
	//
	int getTexture()
	{
		return int(rect.getTexture());
	}

	///////////////////////////////////////////////////////////
	// Funkcja ustawiaj�ca texture
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		mob - textura jak� chcemy ustawi�
	//
	void setTexture(sf::Texture &mob)
	{
		rect.setTexture(&mob);
	}

private:
	bool up;
	bool down;
	bool left;
	bool right;

	
	sf::RectangleShape rect;
	sf::Texture texture;
	
	
	sf::FloatRect nextPosition;
};
