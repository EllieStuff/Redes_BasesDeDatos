#ifndef POLL_H_INCLUDED
#define POLL_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>

#include "Data.h"
#include "Utils.h"

#include <mutex>


void Poll(sf::RenderWindow *window, Data &data, sf::TcpSocket &socket, sf::Socket::Status &status, bool* game, std::mutex* mtx/*, int playerX, int playerY*/){
    int playerX, playerY;
    std::string mes;
    sf::Event event;

    sf::Packet packet;
    sf::TcpListener dispatcher;
    std::string message = " ";

        packet << message;

    while(window->pollEvent(event)) {


        switch(event.type){
            case sf::Event::Closed:
                window->close();

            break;

            case sf::Event::KeyPressed:

            //Movement
                if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                    message = "UP";
                    packet.clear();
                    packet << message;
                    //socket.send(packet);

                    //packet.clear();
                    //socket.receive(packet);
                    //packet >> playerX >> playerY;
                    //std::cout << "playerX: " << playerX << " playerY: " << playerY << "\n";

                }
                if(event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down){
                    message = "DOWN";
                    packet.clear();
                    packet << message;
                    //socket.send(packet);

                    //packet.clear();
                    //socket.receive(packet);
                    //packet >> playerX >> playerY;
                    //std::cout << "playerX: " << playerX << " playerY: " << playerY << "\n";

                }
                if(event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left){
                    message ="LEFT";
                    packet.clear();
                    packet << message;
                    //status = socket.send(packet);

                    //packet.clear();
                    //socket.receive(packet);
                    //packet >> playerX >> playerY;
                    //std::cout << "playerX: " << playerX << " playerY: " << playerY << "\n";

                }
                if(event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right){
                    message ="RIGHT";
                    packet.clear();
                    packet << message;
                    //status = socket.send(packet);

                    //packet.clear();
                    //socket.receive(packet);
                    //packet >> playerX >> playerY;
                    //std::cout << "playerX: " << playerX << " playerY: " << playerY << "\n";

                }

                if(event.key.code == sf::Keyboard::I){
                    message ="I";
                    packet.clear();
                    packet << message;
                    //status = socket.send(packet);

                    /*packet.clear();
                    socket.receive(packet);
                    packet>>mes;
                    std::cout << mes << std::endl;*/

                }

                if(event.key.code == sf::Keyboard::U){
                    message ="U";
                    packet.clear();
                    packet << message;
                    //status = socket.send(packet);

                    /*packet.clear();
                    socket.receive(packet);
                    packet>>mes;
                    std::cout << mes << std::endl;*/

                }

                if(event.key.code == sf::Keyboard::G){
                    message ="G";
                    packet.clear();
                    packet << message;
                    //status = socket.send(packet);

                    /*packet.clear();
                    socket.receive(packet);
                    packet>>mes;
                    std::cout << mes << std::endl;*/

                }

                if(event.key.code == sf::Keyboard::Escape) {
                    message = "ESC";
                    packet.clear();
                    packet << message;
                    std::cout << "Exiting game...\n";
                    *game = false;
                }

            break;

            default:
                message = " ";

            break;

        }

    }

    //std::cout << "a punto de enviar" << message << "pene\n";
    mtx->lock();
        socket.send(packet);
    mtx->unlock();

};

/*
void Receive(sf::TcpSocket &socket, int &playerX, int &playerY) {
sf::Packet packet;
int inputType;
socket.receive(packet);
        packet>>inputType;
        switch(inputType) {
            case 0: //INPUT
                socket.receive(packet);
                packet >> playerX >> playerY;
                std::cout << playerX << " " << playerY << "\n";
            break;

            case 1:
                //mostrar Gemas
                std::cout << "estass son tus gemas \n";
            break;

            case 2:
                //mostrar usuarios
                std::cout<< "estos son los usuarios";
            break;

            case 3:
                std::cout << "se han guardado las gemas \n";
            break;

            case 4:
                std::cout << "quieres salir \n";
            break;

            case 5:
                std::cout<<"nada \n";
            break;

            default: break;


        }
    }
*/

#endif // POLL_H_INCLUDED
