#ifndef POLL_H_INCLUDED
#define POLL_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#include "Data.h"
#include "Utils.h"


void Poll(sf::RenderWindow *window, Data &data, sf::TcpSocket &socket, sf::Socket::Status &status){
    sf::Event event;
    while(window->pollEvent(event)){
        sf::Packet packet;
        sf::TcpListener dispatcher;
        sf::String message = "";
        switch(event.type){
            case sf::Event::Closed:
                window->close();

            break;

            case sf::Event::KeyPressed:
            //Esc
                if(event.key.code == sf::Keyboard::Escape)
                    window->close();

            //Movement
                if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                    message = "UP";
                    packet << message;
                    status = socket.send(packet);
                    std::cout << "Up\n\n";

                }
                if(event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down){
                    packet << "DOWN";
                    std::cout << "Down\n\n";

                }
                if(event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left){
                    packet << "LEFT";
                    std::cout << "Left\n\n";

                }
                if(event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right){
                    packet << "RIGHT";
                    std::cout << "Right\n\n";

                }

            break;

            /*case sf::Event::Closed:
                window.close();

            break;

            case sf::Event::Closed:
                window.close();

            break;*/

            default:
            break;

        }

    }
    //socket.setBlocking(true);

};


#endif // POLL_H_INCLUDED
