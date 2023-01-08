#include  "SFML/Graphics.hpp" 

#include "Entity.h"
#include "Setup.h"

#include<iostream>
#include<random>
#include<vector>

sf::Texture mob;  //Miejsce w którym bêdzie przetrzymywana textura wœciek³ego t³umu

///////////////////////////////////////////////////////////
// Funkcja sprawdzajaca tekstury
///////////////////////////////////////////////////////////
//		
// INPUT:
//		Enemie -  wektor w którym znajduje siê "wœciek³y t³um" (przeciwnicy)
// REMARKS:
//      Jeœli jakiœ przeciwnik nie ma tekstury to jes ona mu dodawana
//      Bez tej funkcji nowi przeciwnicy nie maj¹ tekstury
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
// Funkcja wyœwietlaj¹ca przeciwników 
///////////////////////////////////////////////////////////
//		
// INPUT:
//		Enemie - wektor w którym znajduje siê "wœciek³y t³um" (przeciwnicy)
//      window - okno na którym bêdziemy wyœwietlaæ dane
//
void drawEnemies(std::vector<Entity>& Enemie, sf::RenderWindow& window)
{
    for (auto i = 0; i < Enemie.size(); ++i) {
        Enemie[i].drawTo(window);
    }
}

///////////////////////////////////////////////////////////
// Funkcja wyœwietlaj¹ca nagrody
///////////////////////////////////////////////////////////
//		
// INPUT:
//		sheep -  wektor w, którym znajduj¹ siê "owce" (nagrody)
//      window - okno na którym bêdziemy wyœwietlaæ dane
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

    mob.loadFromFile("Images\\angry_mob.png");                          // £adujê nasz¹ texture do zmiennej mob



    sf::RenderWindow window(sf::VideoMode(1000, 600), "Dungeon");       //tworze i otwieram nasze okno w którym bêdzie siê gra³o
    sf::Event event;                                                    //Tworze zmmienna evnt która bêdzie obs³ugiwaæ szczególne przypadki takie jak ruch postaci

    //Background
    sf::Sprite background;                                              //Tworze spirt'a który bêdziê naszym t³em
    sf::Texture background_texture;
    background_texture.loadFromFile("Images\\background.png");
    background.setTexture(background_texture);

    //POINTS
    sf::Font font;                                                      //Tworze text któy bêdzie wyœwietlany w rogu ekranu i dodaje mu Font
    font.loadFromFile("Fonts\\MontserratThin-K7B6D.ttf");
    sf::Text text;
    text.setFont(font);
    text.setString("Points:");
    text.setCharacterSize(24);

    sf::Text actuall_points;
    actuall_points.setFont(font);
    actuall_points.setCharacterSize(24);
    std::string p;
        
    //PLAYER                                                            //Tworze postaæ któr¹ bedziemy siê poruszaæ
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
    std::vector<Entity> Enemie;                                          //Tworze przeciwników (wœciek³y t³um) i losuje ich pozycje i sprawdzam czy nie nak³¹da sie z pozycja gracza
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
            if (event.type == sf::Event::Closed)                        //Sprawdzenie czy okno jest wci¹¿ otwarte
                window.close();
            if (event.type == sf::Event::KeyPressed)                    //Sprawdzenie jakikoliwek przycisk zosta³ wciœniety jeœli tak to ustawiam temu przyciskowi true
                player.processEvent(event.key.code, true);
            if (event.type == sf::Event::KeyReleased)                   //Sprawdzenie jakikoliwek przycisk zosta³ odpuszczony jeœli tak to ustawiam temu przyciskowi false
                player.processEvent(event.key.code, false);              
            if (event.type == sf::Event::Resized)                       //Sprawdzenie czy wielkoœc okna zosta³a zmieniona, jeœli tak to zmieniam szerokoœæ i wysokoœæ okna aby nagrody i przeciwnicy mog³y siê pojawiac na wiêkszym obszarze
            {                                                           //aby nagrody i przeciwnicy mog³y siê pojawiac na wiêkszym obszarze
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            } 
            if (GameOver == true)                                       //Sprawdzam czy gra zosta³a skoñczona 
            {
                return points;

            }
        }

        text.setPosition(window.getSize().x - 130, window.getSize().y - 50);                //Ustawiam pozycje tekst "Points:" w oknie oraz zaraz za nim nasze punkty 
        actuall_points.setPosition(window.getSize().x - 40, window.getSize().y - 50);      
        p = std::to_string(points);
        actuall_points.setString(p);


        window.clear();                                                                     //Czyszcze ca³e oko
        window.draw(background);                                                            //Wysy³am t³o do okna
        player.update(window, sheep, Enemie);                                               //Aktualizuje pozycje ka¿dej jednostki i sprawdzam kolizje

        drawSheeep(sheep, window);                                                          //Wysy³am pozycje nagród (owce) do okna
        drawEnemies(Enemie, window);                                                        //Wysy³am pozycje przeciwników (wœciek³y t³um) do okna

        check(Enemie);                                                                      //Sprawdzam czy przeciwnicy maj¹ textury

        window.draw(text);                                                                  //Wysy³am pozycje tekstu "Points" do okna        
        window.draw(actuall_points);                                                        //Wysy³am pozycje punktów do okna

        player.drawTo(window);                                                              //Wysy³am pozycje gracza do okna
        window.display();                                                                   //Wyœwietlam ca³oœæ


    }
    return 0;
}