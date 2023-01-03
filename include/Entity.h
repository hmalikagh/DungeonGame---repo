#pragma once

#include <SFML/Graphics.hpp>
#include<iostream>
#include<random>

#include "Setup.h"



int random(int size)
{
	std::random_device rd;
	std::mt19937 mtRandomEngine(rd());
	std::uniform_int_distribution uni_distr(0, size - 1);
	return uni_distr(mtRandomEngine);
}

class Entity
{
public:
	sf::RectangleShape rect;
	sf::Texture texture;
	

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

	

	void processEvent(sf::Keyboard::Key key, bool checkPressed, sf::RenderWindow& window)
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


		
		if (points % 5 == 0 && COUNTER == 0)
		{
			
			Entity tmp(50, 50, "images\\angry_mob.png");
			tmp.rect.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
			Enemie.push_back(tmp);
			numOfEnemies++;
			COUNTER++;
		}

		rect.move(movement);
	}

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(rect);
	}

	void setTexture(std::string textu) {
		texture.loadFromFile(textu);
		rect.setTexture(&texture);
	}

	void setPosition(int x, int y)
	{
		rect.setPosition(x, y);
	}

	int getPositionX()
	{
		return rect.getPosition().x;
	}

	int getPositionY()
	{
		return rect.getPosition().y;
	}

	int getTexture()
	{
		return int(rect.getTexture());
	}

private:
	bool up;
	bool down;
	bool left;
	bool right;

	

	
	
	sf::FloatRect nextPosition;
};
