// JOEL

#include <iostream>
#include <stdlib.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
//#include <resultset.h>
//#include <statement.h>
#include <unistd.h>
#include <wait.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "Utils.h"
#include "Constants.h"
#include "Data.h"
#include "Poll.h"


#include <wait.h>
#include <mutex>
#include <thread>
#include <pthread.h>

bool game = true;

//int actualTime = 0;
//int oneSecond = 60;

std::mutex mtx;


int main()
{

    sf::Vector2i playerPos;
    mapSquares myMapStructure;
    std::string myMap;
    char tmpMap[FIELD_MAX_SIZE][FIELD_MAX_SIZE];
    int enemyAtk, enemyDef, playerAtk, playerDef, combat = 0; // PARA IMPRIMIR
    int functionToCall = 4;

    char squares[FIELD_MAX_SIZE][FIELD_MAX_SIZE];

    sf::IpAddress ip = "192.168.1.29";
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect(ip, 5005, sf::seconds(15.f));

    sf::Font font;
    font.loadFromFile("Gameplay.ttf");

    std::string serverQuote;
    int mapNum;
    bool exist = false;
    int confirmation = 0;
    Data data;

    std::vector<Gem> gems;

    sf::Packet packet;
    Initialize(socket, packet, data, confirmation, serverQuote, mapNum, myMap);

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(W_WINDOW_TITLE, H_WINDOW_TITLE), "Window");
    std::cout << "Game Started!\n";
    /*socket.receive(packet);
    std::string tmpMap;
    packet >> tmpMap;
    DrawField(tmpMap.c_str(), window);
    window->display();*/



    while(window->isOpen()){
        game = window->isOpen();
        window->clear();

        ActualizeMap(tmpMap, myMap);

        //std::cout << "after map setting\n";

        ///socket.setBlocking(false);
        //std::cout << "poll antes \n";
        Poll(window, data, socket, status, &game, &mtx);
        //socket.receive(packet); // recibir posicion nueva del player
        ///socket.setBlocking(true);

        packet.clear();
        socket.setBlocking(true);
        socket.receive(packet);
        packet >> functionToCall;

        if(functionToCall == 1) {
            ///Inventory
            //std::cout << "Inside I\n";
            std::map<std::string, int> gemsMap;
            GetGems(gemsMap, socket, packet);

            std::cout << "Inventory:\n";
            PrintMap(gemsMap);

        }
        else if (functionToCall==2) {
            ///Users
            //std::cout << "Inside U\n";
            std::vector<std::string> usersVec;
            GetUsersConn(usersVec, socket, packet);

            std::cout << "Users:\n";
            PrintVector(usersVec);

        }
        else if (functionToCall==3) {
            ///Save gems

            ///Comprovar si shan guardat les gemes
            std::cout << "Your progress has been saved succesfully!\n";
            ///En cas contrari
            std::cout << "Some error ocurred! Please, try again later.\n";

        }
        else if(functionToCall == 0) {
            ///GAME OVER
            char ans;
            int replay = 0;
            window->close();
            packet.clear();
            std::cout << "Do you want to play again? (y/n) ";
            std::cin >> ans;
            if(ans == 'y' || ans == 'Y')
                replay = 1;    //true
            else
                replay = 0;    //false

            packet << replay;
            socket.send(packet);

            if(replay == 1){
                Initialize(socket, packet, data, confirmation, serverQuote, mapNum, myMap);
                window = new sf::RenderWindow(sf::VideoMode(W_WINDOW_TITLE, H_WINDOW_TITLE), "Window");
                std::cout << "Game Started!\n";

            }

        }
        else if (functionToCall==4) {


        /*if(combat==1) {
            std::cout << "On battle mode\n";
            //pausa (combate)
            //recibir todos los numeros de ataque/defensa
            //pintar gema

        }*/

        ///Posicion del jugador
            packet.clear();
            socket.receive(packet); // recibir player.x y player.y

            packet >> playerPos.x >> playerPos.y;
            //std::cout << "player X: " << playerPos.x << " player Y: " << playerPos.y << "\n";
            tmpMap[playerPos.y][playerPos.x] = 'P';

            packet.clear();

            /// Enemigos
            /*int enemiesSize = 0;
            socket.receive(packet);
            packet >> enemiesSize;
            packet.clear();
            socket.receive(packet);*/

            ///Posiciones de enemigos
            /*for(int i = 0; i < enemiesSize; i++){
                std::cout << i << "\n";
                sf::Vector2i enemyPos;
                packet >> enemyPos.x >> enemyPos.y;;
                std::cout << "enemyPos.x " << enemyPos.x << "\n";

                tmpMap[enemyPos.y][enemyPos.x] = 'E';
            }*/

            setEnemies(&socket, tmpMap);



        } // if (functionToCall==4)

        DrawField(tmpMap, window, myMapStructure); ///TODO: fer que rebi l'informacio per a omplir el squares del server
        ///tmpMap[playerPos.y][playerPos.x] = 'P';


        window->display();

    }

}

