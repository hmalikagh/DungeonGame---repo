#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>
#include<random>

#include "Setup.h"


///////////////////////////////////////////////////////////
// Funckja losuj¹ca liczbê
///////////////////////////////////////////////////////////
//		
// INPUT:
//		size - maksymalna wartoœæ do jak¹ mo¿na wylosowaæ
//		
// OUTPUT:
//		losowa liczba od 0 do liczby podanej jako argument funkcji
//		
// REMARKS:
//		maksymalna liczba jak¹ mo¿na wylosowaæ jest o jeden mniejsza
//		ni¿ liczba podana jako argument size
//
int random(int size)
{
	std::random_device rd;
	std::mt19937 mtRandomEngine(rd());
	std::uniform_int_distribution uni_distr(0, size - 1);
	return uni_distr(mtRandomEngine);
}

///////////////////////////////////////////////////////////
// Klasa reprezentuj¹ca jednostke w mojej grze
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
	//		textu - textura jaka bedzie mia³a jednostka
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
	//Funkcja ustawiaj¹ca wcisniêcie/odpuszczenie danego klawisza w,a,s,d
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		key - klawisz który ma byæ wciœniêty/niewciœniêty
	//		checkPressed - czy klawisz jest wcisniety czy nie
	// REMARKS:
	//		Jeœli checkPressed == true to klawisz jest wcisniêty,
	//		Jeœli checkPressed == false to kalwisz jest niewciœniety
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
	// Funkcja aktualizuj¹ca pozycje gracza oraz sprawdzaj¹ca 
	// kolizje z przeciwnikami (Enemie) oraz nagrodami (sheep),
	// a tak¿e dodaj¹ca nowego przeciwnika 
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		window - okno na którym bêdziemy wyœwietlaæ dane
	//		sheep - wektor w, którym znajduj¹ siê "owce" (nagrody)
	//		Enemie - wektor w którym znajduje siê "wœciek³y t³um" (przeciwnicy)
	// REMARKS:
	//		Jeœli któryœ z przycisków jest wcisniêty odpowiadaj¹ca mu wartoœæ up, 
	//		down, left, right spowoduje poruszenie siê naszej postaci w dan¹ strone z odpowiedni¹ prêdkoœci¹
	// 
	//		Jeœli nasza postaæ wejdzie w granice owcy to danej owcy zostanie wylosowana nowa pozycja
	// 
	//		Jeœli nasza postaæ wejdzie w granice enemie to nast¹pi koniec gry
	// 
	//		Gdy liczba punktów bêdzie równa wilokrotnoœci whenNewEnemie to pojawi siê nowy przeciwnik
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
	// Funkcja wyœwietlaj¹ca aktualn¹ pozycjê jednostki w oknie
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		window - okno na którym bêdziemy wyœwietlaæ dane
	//
	void drawTo(sf::RenderWindow& window)
	{
		window.draw(rect);
	}

	///////////////////////////////////////////////////////////
	// Funkcja ustawiaj¹ca texture danej jednostki 
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		textu - textura jaka ma byæ ustawiona
	//
	void setTexture(std::string textu) {
		texture.loadFromFile(textu);
		rect.setTexture(&texture);
	}

	///////////////////////////////////////////////////////////
	// Funkcja ustawiaj¹ca pozycje danej jednostki
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		x - pixel na osi X gdzie ma byæ ustawiona jednostka
	//		y - pixel na osi Y gdzie ma byæ ustawiona jednostka
	//
	void setPosition(int x, int y)
	{
		rect.setPosition(x, y);
	}

	///////////////////////////////////////////////////////////
	// Funkcja zwracaj¹ca pozycje jednostki w osi X
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
	// Funkcja zwracaj¹ca pozycje jednostki w osi Y
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
	// Funkcja zwracaj¹ca teksture jednostki 
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
	// Funkcja ustawiaj¹ca texture
	///////////////////////////////////////////////////////////
	//		
	// INPUT:
	//		mob - textura jak¹ chcemy ustawiæ
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
