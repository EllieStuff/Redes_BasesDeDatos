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

void ActualizeMap(char tmpMap[FIELD_MAX_SIZE][FIELD_MAX_SIZE], std::string &myMap){
    for(int j = 0; j < FIELD_MAX_SIZE; j++){
        for(int k = 0; k < FIELD_MAX_SIZE; k++){
            tmpMap[j][k] = myMap[j + k * FIELD_MAX_SIZE];

        }
    }

}


void Initialize(sf::TcpSocket &socket, sf::Packet &packet, Data &data, int &confirmation, std::string &serverQuote, int &mapNum, std::string &myMap){
    while(confirmation!=1) {
        data.AskNameAndPassword(socket, data.user.name, data.user.password);
        packet << data.user.name << data.user.password; // RECIBIR NAME Y PASSWORD
        socket.send(packet);
        socket.setBlocking(true);

        socket.receive(packet);
        packet >> confirmation;
        packet.clear();
        //std::cout << "Packet recibido " << confirmation << std::endl;

        // recibir otro packet con la confirmación de la contraseña
        // SE PUEDE HACER OTRO LOOP PARA ESTO

    }


    int ok = 0;
    myMap = " ";

    do {
        packet.clear();
        socket.receive(packet); // "Que mapa quieres, 1 o 2?
        packet >> serverQuote;
        std::cout << serverQuote << std::endl;

        std::cin >> mapNum;
        std::cout << "Your map will be the map n" << mapNum << std::endl;
        packet.clear();
        packet<<mapNum;
        socket.send(packet); // enviar num mapa a server

        packet.clear();
        socket.receive(packet); // pillar el mapa del server
        packet >> myMap;

        packet.clear();
        socket.receive(packet); // recibir confirmación
        packet >> ok;
    } while (ok != 1);

}


void setEnemies(sf::TcpSocket* socket, char tmpMap[FIELD_MAX_SIZE][FIELD_MAX_SIZE]) {

    int enemiesSize = 0;
    //std::cout << "im in\n";
    sf::Packet pack; pack.clear(); pack.clear();

    socket->receive(pack); // recibir num enemigos
    pack >> enemiesSize;
    //std::cout << "received enemeis!" << enemiesSize << "\n";
    /*pack.clear(); pack.clear();
    pack << enemiesSize;
    socket->send(pack);
    std::cout << "sent enemeis!" << enemiesSize << "\n";*/
    sf::Vector2i enemyPos;

    pack.clear();
    socket->receive(pack);

    for(int i = 0; i < enemiesSize; i++) {
            //std::cout << i << "\n";
            pack >> enemyPos.x >> enemyPos.y;
            //std::cout << "enemyPos.x " << enemyPos.x << "\n";
            tmpMap[enemyPos.y][enemyPos.x] = 'E';
    }

    return;

}


void DrawField(char squares[FIELD_MAX_SIZE][FIELD_MAX_SIZE], sf::RenderWindow *window, mapSquares myMapStructure){
    //sf::RectangleShape rectFondo;

    myMapStructure.wall = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));    //Posar-li als altres

    myMapStructure.player = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));

    myMapStructure.obstacle = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));

    myMapStructure.enemy = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));

    myMapStructure.gem = sf::CircleShape(CASILLA_HORIZONTAL / 2, 30);

    for(int i = 0; i < FIELD_MAX_SIZE; i++){
        for(int j = 0; j < FIELD_MAX_SIZE; j++){
            switch(squares[j][i]){
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

template<typename T>
void PrintMap(T myMap){
    for(auto it = myMap.begin(); it != myMap.end(); it++)
        std::cout << it->first << " -   " << it->second << std::endl;

}

template<typename T>
void PrintVector(T myVector){
    for(auto it = myVector.begin(); it != myVector.end(); it++)
        std::cout << *it << std::endl;

}


void GetGems(std::map<std::string, int> &gemsMap, sf::TcpSocket &socket, sf::Packet &packet){
        packet.clear();
        int gemNum = 0;
        socket.receive(packet);
        packet >> gemNum;
        packet.clear();
        socket.receive(packet);
        for(int i = 0; i < gemNum; i++){
            std::string gemName;
            packet >> gemName;
            if(gemsMap.find(gemName) == gemsMap.end())
                gemsMap.insert(std::make_pair(gemName, 1));
            else
                gemsMap[gemName]++;

        }

}

void GetUsersConn(std::vector<std::string> &usersConn, sf::TcpSocket &socket, sf::Packet &packet){
        packet.clear();
        int usersNum = 0;
        socket.receive(packet);
        packet >> usersNum;
        packet.clear();
        socket.receive(packet);
        for(int i = 0; i < usersNum; i++){
            std::string userName;
            packet >> userName;
            usersConn.push_back(userName);

        }

}



#endif // UTILS_H_INCLUDED
