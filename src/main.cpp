#include  "SFML/Graphics.hpp" 

#include "Entity.h"
#include "Setup.h"

#include<iostream>
#include<random>
#include<vector>

sf::Texture mob;  //Miejsce w kt�rym b�dzie przetrzymywana textura w�ciek�ego t�umu

///////////////////////////////////////////////////////////
// Funkcja sprawdzajaca tekstury
///////////////////////////////////////////////////////////
//		
// INPUT:
//		Enemie -  wektor w kt�rym znajduje si� "w�ciek�y t�um" (przeciwnicy)
// REMARKS:
//      Je�li jaki� przeciwnik nie ma tekstury to jes ona mu dodawana
//      Bez tej funkcji nowi przeciwnicy nie maj� tekstury
//
void check(std::vector<Entity>& Enemie)
{
    for (auto i = 0; i < Enemie.size(); ++i)
    {
        if (Enemie[i].getTexture() != Enemie[0].getTexture())
            Enemie[i].setTexture(mob);
    }
}


///////////////////////////////////////////////////////////
// Funkcja wy�wietlaj�ca przeciwnik�w 
///////////////////////////////////////////////////////////
//		
// INPUT:
//		Enemie - wektor w kt�rym znajduje si� "w�ciek�y t�um" (przeciwnicy)
//      window - okno na kt�rym b�dziemy wy�wietla� dane
//
void drawEnemies(std::vector<Entity>& Enemie, sf::RenderWindow& window)
{
    for (auto i = 0; i < Enemie.size(); ++i) {
        Enemie[i].drawTo(window);
    }
}

///////////////////////////////////////////////////////////
// Funkcja wy�wietlaj�ca nagrody
///////////////////////////////////////////////////////////
//		
// INPUT:
//		sheep -  wektor w, kt�rym znajduj� si� "owce" (nagrody)
//      window - okno na kt�rym b�dziemy wy�wietla� dane
//
void drawSheeep(std::vector<Entity>& Sheep, sf::RenderWindow& window)
{
    for (auto i = 0; i < Sheep.size(); ++i) {
       Sheep[i].drawTo(window);
    }
}


///////////////////////////////////////////////////////////
// Funkcja main
///////////////////////////////////////////////////////////
int main()
{

    mob.loadFromFile("Images\\angry_mob.png");                          // �aduj� nasz� texture do zmiennej mob



    sf::RenderWindow window(sf::VideoMode(1000, 600), "Dungeon");       //tworze i otwieram nasze okno w kt�rym b�dzie si� gra�o
    sf::Event event;                                                    //Tworze zmmienna evnt kt�ra b�dzie obs�ugiwa� szczeg�lne przypadki takie jak ruch postaci

    //Background
    sf::Sprite background;                                              //Tworze spirt'a kt�ry b�dzi� naszym t�em
    sf::Texture background_texture;
    background_texture.loadFromFile("Images\\background.png");
    background.setTexture(background_texture);

    //POINTS
    sf::Font font;                                                      //Tworze text kt�y b�dzie wy�wietlany w rogu ekranu i dodaje mu Font
    font.loadFromFile("Fonts\\MontserratThin-K7B6D.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("Points:");
    text.setCharacterSize(24);

    sf::Text actuall_points;
    actuall_points.setFont(font);
    actuall_points.setCharacterSize(24);
    std::string p;
        
    //PLAYER                                                            //Tworze posta� kt�r� bedziemy si� porusza�
    Entity player(50, 50, "images\\dragon.png");
    player.setPosition(0, 0);

    //SHEEPS                                                            //Tworze nagrody (owce) i losuje ich pozycje
    std::vector<Entity> sheep;
    Entity sheeps(50, 50, "images\\sheep.png");

    for (auto i = 0; i < numOfSheeps; ++i)
    {
        sheeps.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
        sheep.push_back(sheeps);
    }

    //ENEMIES
    std::vector<Entity> Enemie;                                          //Tworze przeciwnik�w (w�ciek�y t�um) i losuje ich pozycje i sprawdzam czy nie nak��da sie z pozycja gracza
    Entity enemies(50, 50, "images\\angry_mob.png");

    for (auto i = 0; i < numOfEnemies; ++i)
    {
        enemies.setPosition(random(window.getSize().x - 50), random(window.getSize().y - 50));
        if (enemies.getPositionX() == player.getPositionX())
            enemies.setPosition(enemies.getPositionX() + 100, enemies.getPositionY());
        else if (enemies.getPositionY() == player.getPositionY())
            enemies.setPosition(enemies.getPositionX() , enemies.getPositionY() + 100);
        Enemie.push_back(enemies);
    }
    //EVENTS
    while (window.isOpen()) {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)                        //Sprawdzenie czy okno jest wci�� otwarte
                window.close();
            if (event.type == sf::Event::KeyPressed)                    //Sprawdzenie jakikoliwek przycisk zosta� wci�niety je�li tak to ustawiam temu przyciskowi true
                player.processEvent(event.key.code, true);
            if (event.type == sf::Event::KeyReleased)                   //Sprawdzenie jakikoliwek przycisk zosta� odpuszczony je�li tak to ustawiam temu przyciskowi false
                player.processEvent(event.key.code, false);              
            if (event.type == sf::Event::Resized)                       //Sprawdzenie czy wielko�c okna zosta�a zmieniona, je�li tak to zmieniam szeroko�� i wysoko�� okna aby nagrody i przeciwnicy mog�y si� pojawiac na wi�kszym obszarze
            {                                                           //aby nagrody i przeciwnicy mog�y si� pojawiac na wi�kszym obszarze
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            } 
            if (GameOver == true)                                       //Sprawdzam czy gra zosta�a sko�czona 
            {
                return points;

            }
        }

        text.setPosition(window.getSize().x - 130, window.getSize().y - 50);                //Ustawiam pozycje tekst "Points:" w oknie oraz zaraz za nim nasze punkty 
        actuall_points.setPosition(window.getSize().x - 40, window.getSize().y - 50);      
        p = std::to_string(points);
        actuall_points.setString(p);


        window.clear();                                                                     //Czyszcze ca�e oko
        window.draw(background);                                                            //Wysy�am t�o do okna
        player.update(window, sheep, Enemie);                                               //Aktualizuje pozycje ka�dej jednostki i sprawdzam kolizje

        drawSheeep(sheep, window);                                                          //Wysy�am pozycje nagr�d (owce) do okna
        drawEnemies(Enemie, window);                                                        //Wysy�am pozycje przeciwnik�w (w�ciek�y t�um) do okna

        check(Enemie);                                                                      //Sprawdzam czy przeciwnicy maj� textury

        window.draw(text);                                                                  //Wysy�am pozycje tekstu "Points" do okna        
        window.draw(actuall_points);                                                        //Wysy�am pozycje punkt�w do okna

        player.drawTo(window);                                                              //Wysy�am pozycje gracza do okna
        window.display();                                                                   //Wy�wietlam ca�o��


    }
    return 0;
}