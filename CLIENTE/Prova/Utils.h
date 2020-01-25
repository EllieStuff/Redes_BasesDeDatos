#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#include "Data.h"
#include "Constants.h"

//mapSquare myMapStructure;


void DrawField(char squares[FIELD_MAX_SIZE][FIELD_MAX_SIZE], sf::RenderWindow *window, mapSquares myMapStructure){
    //sf::RectangleShape rectFondo;

    myMapStructure.wall = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));    //Posar-li als altres

    myMapStructure.player = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));

    myMapStructure.obstacle = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));

    myMapStructure.enemy = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));

    myMapStructure.gem = sf::CircleShape(CASILLA_HORIZONTAL / 2, 30);

    for(int i = 0; i < FIELD_MAX_SIZE; i++){
        for(int j = 0; j < FIELD_MAX_SIZE; j++){
            switch(squares[i][j]){
            case 'W':
                myMapStructure.wall.setPosition(i * CASILLA_HORIZONTAL, j * CASILLA_VERTICAL);
                myMapStructure.wall.setFillColor(sf::Color::Blue);
                window->draw(myMapStructure.wall);
                //Posar-li als altres

            break;

            case 'E':
                myMapStructure.enemy.setPosition(i * CASILLA_HORIZONTAL, j * CASILLA_VERTICAL);
                myMapStructure.enemy.setFillColor(sf::Color::Red);
                window->draw(myMapStructure.enemy);

            break;

            case 'G':
                myMapStructure.gem.setPosition(i * CASILLA_HORIZONTAL, j * CASILLA_VERTICAL);
                myMapStructure.gem.setFillColor(sf::Color::Yellow);
                window->draw(myMapStructure.gem);

            break;

            case 'P':
                myMapStructure.player.setPosition(i * CASILLA_HORIZONTAL, j * CASILLA_VERTICAL);
                myMapStructure.player.setFillColor(sf::Color::Green);
                window->draw(myMapStructure.player);

            break;

            case 'O':
                myMapStructure.obstacle.setPosition(i * CASILLA_HORIZONTAL, j * CASILLA_VERTICAL);
                myMapStructure.obstacle.setFillColor(sf::Color::Cyan);
                window->draw(myMapStructure.obstacle);

            break;


            default:;

            }

        }

    }

}

void ReceiveMoveEnemies() {

}


#endif // UTILS_H_INCLUDED
