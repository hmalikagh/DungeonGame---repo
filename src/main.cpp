#include  "SFML/Graphics.hpp" // replace quotes with less than and greater than symbols

#include "Entity.h"
#include "Setup.h"

#include<iostream>
#include<random>
#include<vector>

sf::Texture mob;




void check(std::vector<Entity>& Enemie)
{
    for (auto i = 0; i < Enemie.size(); ++i)
    {
        if (Enemie[i].getTexture() != Enemie[0].getTexture())
            Enemie[i].rect.setTexture(&mob);            
       
    }
}

void drawEnemies(std::vector<Entity>& Enemie, sf::RenderWindow& window)
{
    for (auto i = 0; i < Enemie.size(); ++i) {
        Enemie[i].drawTo(window);

    }
}

void drawSheeep(std::vector<Entity>& Sheep, sf::RenderWindow& window)
{
    for (auto i = 0; i < Sheep.size(); ++i) {
       Sheep[i].drawTo(window);

    }
}

int main()
{

    mob.loadFromFile("Images\\angry_mob.png");



    sf::RenderWindow window(sf::VideoMode(1000, 600), "Dungeon");
    sf::Event event;

    //Background
    sf::Sprite background;
    sf::Texture background_texture;
    background_texture.loadFromFile("Images\\background.png");
    background.setTexture(background_texture);

    //POINTS
    sf::Font font;
    font.loadFromFile("Fonts\\MontserratThin-K7B6D.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("Points:");
    text.setCharacterSize(24);

    sf::Text actuall_points;
    actuall_points.setFont(font);
    actuall_points.setCharacterSize(24);
    std::string p;

    //PLAYER
    Entity player(50, 50, "images\\dragon.png");
    player.setPosition(0, 0);

    //SHEEPS
    std::vector<Entity> sheep;
    Entity sheeps(50, 50, "images\\sheep.png");

    for (auto i = 0; i < numOfSheeps; ++i)
    {
        sheeps.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
        sheep.push_back(sheeps);
    }

    //ENEMIES
    std::vector<Entity> Enemie;
    Entity enemies(50, 50, "images\\angry_mob.png");

    for (auto i = 0; i < numOfEnemies; ++i)
    {
        enemies.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
        if (abs(enemies.getPositionX() - player.getPositionX()) < 100)
            enemies.getPositionX() + 100;
        else if (abs(enemies.getPositionY() - player.getPositionY()) < 100)
            enemies.getPositionY() + 100;
        Enemie.push_back(enemies);
    }
    //EVENTS
    while (window.isOpen()) {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                player.processEvent(event.key.code, true, window);
            if (event.type == sf::Event::KeyReleased)
                player.processEvent(event.key.code, false, window);
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            } 
            if (GameOver == true)
            {
                return points;

            }
        }

        text.setPosition(window.getSize().x - 130, window.getSize().y - 50);
        actuall_points.setPosition(window.getSize().x - 40, window.getSize().y - 50);

        p = std::to_string(points);
        actuall_points.setString(p);


        window.clear();
        window.draw(background);
        player.update(window, sheep, Enemie);
        //cheack(Enemie);

        drawSheeep(sheep, window);
        drawEnemies(Enemie, window);

       /* for (auto i = 0; i < Enemie.size(); ++i)
        {
            if(Enemie[i].rect.getTextureRect())
        }
        */
        check(Enemie); 
        window.draw(text);
        window.draw(actuall_points);

        player.drawTo(window);
        window.display();


    }
    

    return 0;
}