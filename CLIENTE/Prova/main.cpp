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
static bool combat = false;

static std::vector<sf::Vector2i> enemyToGem;

std::vector<Gem> gems;

//int actualTime = 0;
//int oneSecond = 60;

std::mutex mtx;

void setEnemies(sf::TcpSocket* socket, char tmpMap[FIELD_MAX_SIZE][FIELD_MAX_SIZE]) {

    int enemiesSize = 0;
    std::cout << "im in\n";
    sf::Packet pack; pack.clear(); pack.clear();

    socket->receive(pack); // recibir num enemigos
    pack >> enemiesSize;
    std::cout << "received enemeis! " << enemiesSize << "\n";
    /*pack.clear(); pack.clear();
    pack << enemiesSize;
    socket->send(pack);
    std::cout << "sent enemeis!" << enemiesSize << "\n";*/
    sf::Vector2i enemyPos;

        pack.clear();
        socket->receive(pack);

    for(int i = 0; i < enemiesSize; i++) {
            bool death = false;
            pack >> enemyPos.x >> enemyPos.y >> death;

            std::cout << enemyPos.x << " " << enemyPos.y << " " << death << std::endl;

            if(death) {
                tmpMap[enemyPos.y][enemyPos.x] = 'E';
                sf::Vector2i enemyDeathPosition(enemyPos.x, enemyPos.y);
                enemyToGem.push_back(enemyDeathPosition);

                combat = true;
            }
            else {
            //std::cout << "enemyPos.x " << enemyPos.x << "\n";
            tmpMap[enemyPos.y][enemyPos.x] = 'E';

            }
        }

    return;

}

int main()
{

    sf::Vector2i playerPos;
    mapSquares myMapStructure;
    std::string myMap;
    char tmpMap[FIELD_MAX_SIZE][FIELD_MAX_SIZE];
    int enemyAtk, enemyDef, playerAtk, playerDef; // PARA IMPRIMIR
    int functionToCall = 4;

    char squares[FIELD_MAX_SIZE][FIELD_MAX_SIZE];

    sf::IpAddress ip = "192.168.1.110";
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect(ip, 5005, sf::seconds(15.f));

    sf::Font font;
    font.loadFromFile("Gameplay.ttf");

    std::string serverQuote;
    int mapNum;
    bool exist = false;
    int confirmation = 0;
    Data data;
    data.Init();

    int enemiesConfirm = 1;

    sf::Packet packet;
    while(confirmation!=1) { // usuario hace login
        packet << data.user.name << data.user.password; // RECIBIR NAME Y PASSWORD
        socket.send(packet);
        socket.setBlocking(true);

        socket.receive(packet);
        packet >> confirmation;
        std::cout << "Packet recibido " << confirmation << std::endl;

        // recibir otro packet con la confirmación de la contraseña
        // SE PUEDE HACER OTRO LOOP PARA ESTO

        /// cuando haya hecho el login bien, recibir gemas y nivel de singularidad

        /// RULETA !!!

    }

    int ok = 0;

do {  // elegir mapa
    packet.clear();
    socket.receive(packet); // "Que mapa quieres, 1 o 2?
    packet >> serverQuote;
    std::cout << serverQuote << std::endl;

    std::cin >> mapNum;
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

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(W_WINDOW_TITLE, H_WINDOW_TITLE), "Window");
    /*socket.receive(packet);
    std::string tmpMap;
    packet >> tmpMap;
    DrawField(tmpMap.c_str(), window);
    window->display();*/

    while(window->isOpen()){
        game = window->isOpen();
        window->clear();


    for(int j = 0; j < FIELD_MAX_SIZE; j++){
            for(int k = 0; k < FIELD_MAX_SIZE; k++){
                tmpMap[j][k] = myMap[j + k * FIELD_MAX_SIZE];

            }
        }

        if(combat){

            for(int health = 15, attack = rand()%5; health > 0; health-=attack) {

                std::cout << "Enemy Health: " << health << std::endl;
                ///sleep(1);
                std::cout << "Attack: " << attack << std::endl;
                attack = rand()%(1,5);


            }

            combat = false;

        }else{ // partida normal

        ///socket.setBlocking(false);
        std::cout << "poll antes \n";
        Poll(window, data, socket, status, &game, &mtx);
        //socket.receive(packet); // recibir posicion nueva del player
        ///socket.setBlocking(true);

        packet.clear();
        socket.setBlocking(true);
        socket.receive(packet);
        packet >> functionToCall;

        if(functionToCall == 1) {

        // MIRAR GEMAS ACUTALES EN EL INVENTARIO (vector Gems)

        }
        else if (functionToCall==2) {

        // RECIBIR TODOS LOS USUARIOS Y IMPRIMIRLOS

        }
        else if (functionToCall==3) {

        // ENVIAR GEMAS RECOGIDAS EN UN PACKET AL SERVER

        }
        else if(functionToCall == 0) {
            //GAME OVER
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
            std::cout << "player X: " << playerPos.x << " player Y: " << playerPos.y << "\n";
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

            std::vector<sf::Vector2i>::iterator it = enemyToGem.begin();
            while(it!=enemyToGem.end()) {
                if(tmpMap[it->y][it->x]=='P') {
                    tmpMap[it->y][it->x] = ' ';
                    enemyToGem.erase(it);
                    it = enemyToGem.begin();
                }else{
                    tmpMap[it->y][it->x] = 'G';
                    it++;
                }
            }

            socket.receive(packet);
            packet >> enemiesConfirm; packet.clear();
            if(enemiesConfirm == 1){
                setEnemies(&socket, tmpMap); }

        } // if (functionToCall==4)

        }

        DrawField(tmpMap, window, myMapStructure); ///TODO: fer que rebi l'informacio per a omplir el squares del server
        ///tmpMap[playerPos.y][playerPos.x] = 'P';


        window->display();

    }

}

