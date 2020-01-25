// JOEL

#include <iostream>
#include <stdlib.h>
#include <mysql_connection.h>
#include <mysql_driver.h>

#include <resultset.h>
#include <statement.h>

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
//#include "Map.h"


#include <wait.h>
#include <mutex>
#include <thread>
#include <pthread.h>



std::mutex mtx;

int actualTime = 0;
int oneSecond = 220;

enum class gameState {INIT, GAME, END};

int mapNumber = 0;

//int attackEnemy;

bool finishGame = false;
bool ended = false;

static std::vector<User> users;

void gemas(int ID) {
    //Buscar player con ID
    //Packet con gemas y numeros y tal
}

void viewUsers(std::vector<sf::TcpSocket*> sockets, int ID) {

//ACCEDER BASE DE DATOS Y MIRAR TODOS LOS USERS

}

void save(int ID) {

//WHERE ID TAL AAÑDIR GEMAS
}

void moveEnemies(sf::TcpSocket* sock, std::vector<Enemy>* enemies, User* user, Map* field){
   /// ENEMIGOS

                std::cout << enemies->size() << " enemies\n";
                sf::Packet p;
                p << (int)enemies->size();
                sock->send(p); // Enviar numero de enemigos
                p.clear();

                for(std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); it++) {
                    it->Move(user->player, sock, *field);



                    p << it->pos.x << it->pos.y << it->dead;

                    if(it->health <= 0) {
                        field->squares[it->pos.x][it->pos.y] = ' ';
                        std::cout << "COMBAT!\n";
                    }

                }

                sock->send(p);


                return;

            //}


}


void Play(std::vector<sf::TcpSocket*> sockets, int i/*, DataBase db*/) {

std::cout << "CLIENTE " << i << "\n";

gameState state = gameState::INIT;
    sf::Packet p;
    std::string name, password;

    std::vector<Enemy> enemies; // GUARDAR TODOS LOS ENEMIGOS DE LA BASE DE DATOS EN ESTE VECTOR

    Map field; //

    int mapNum = -1;

    int playerID; // PILLAR EL PLAYER ID

    bool found = false;
    bool endgame = false;
    bool combatBlock = false;
    bool yes = false;

    User us;


    std::string mes;

    while (!finishGame) { // este loop no hace falta diria


    /// MIRAR SOCKET IF YES FINISHGAME = TURE

    std::cout << "CLIENTE " << i << "\n";

        switch(state) {

        case gameState::INIT:
            sockets[i]->receive(p);
            p >> name >> password;
            std::cout << name << " " << password << std::endl;

            // FUNCION ACCEDER A BASE DE DATOS Y MIRAR name Y password.

            // HAY QUE ENVIAR UN PACKET CON UN 0 SI NO ESTÁ EL USUARIO
            // O CON UN 1 SI SI QUE ESTÁ Y

            // GUARDAR EN us (Clase User) todos los datos del usuario que hay guardados en la Base de Datos


            /// mostrar GEMAS Y SINGULARIDAD en la consola del Cliente

            /// ¡¡¡¡ RULETA !!!! ///

            //if(!found) {
                us.name = name; us.password = password;
                users.push_back(us);
                std::cout << "Nuevo usuario: " << users.back().name << std::endl;
                //p.clear(); p << 1;
                //sockets[i]->send(p);
            //}

            p.clear();
            p << 1;
            sockets[i]->send(p);
            p.clear();



            std::cout << "Asking map" << std::endl;
            while(!yes) {
            p.clear();
                p << "Que mapa quieres, 1 o 2?";
                sockets[i]->send(p);

                p.clear();
                sockets[i]->receive(p); // numero de mapa
                p>>mapNum;

                p.clear();
                std::string mapStr = "";

                if(mapNum == 1) { // COGER UN PATH O LO QUE SEA DE LA BASE DE DATOS
                    std::cout<<"Cargando Mapa 1"<<std::endl;
                    field.ReadXML(1);

                    for(int j = 0; j < FIELD_MAX_SIZE; j++) {
                        for(int k = 0; k < FIELD_MAX_SIZE; k++) {
                            if(field.squares[j][k] == 'E') {
                                enemies.push_back(Enemy(sf::Vector2i(j, k)));

                                //std::cout << "Enemy " << j << ", " << k << std::endl;
                            }
                            else if(field.squares[j][k] == 'P') {
                                users[i].player.pos = sf::Vector2i(j, k);

                                //std::cout << "Player " << j << ", " << k << std::endl;
                            }

                            if(field.squares[j][k] == 'W')
                                mapStr += 'W';
                            else
                                mapStr += ' ';

                            }

                        }

                    p.clear();
                    p << mapStr;
                    sockets[i]->send(p);

                    p.clear();
                    p << 1;
                    std::cout << "Mapa 1 cargado\n";
                    sockets[i]->send(p); // enviar confirmación
                    yes = true;
                }
                else if(mapNum == 2) { // COGER EL SEGUNDO PATH
                    std::cout<<"Cargando Mapa 2"<<std::endl;
                    field.ReadXML(2);

                    for(int j = 0; j < FIELD_MAX_SIZE; j++){
                        for(int k = 0; k < FIELD_MAX_SIZE; k++){
                            if(field.squares[j][k] == 'E') {
                                enemies.push_back(Enemy(sf::Vector2i(j, k)));
                            }
                            else if(field.squares[j][k] == 'P') {
                                users[i].player.pos = sf::Vector2i(j, k);
                            }

                            if(field.squares[j][k] == 'W') {
                                mapStr += 'W';

                            } else {
                                mapStr += ' ';
                            }

                        }
                    }

                    p.clear();
                    p << mapStr;
                    sockets[i]->send(p);

                    p.clear();
                    p << 1;
                    std::cout << "Mapa 2 cargado\n";
                    sockets[i]->send(p);
                    yes = true;
                }
                else { // ESTO ES PARA CUANDO NO RECIBE NI UN 1 NI UN 2, VUELVE A ESPERAR UN NÚMERO VÁLIDO
                    p.clear();
                    p << ' ';
                    sockets[i]->send(p);

                    p.clear(); p << 0;
                    sockets[i]->send(p);
                    yes = false;
                }

                p.clear(); p.clear();
            }

            state = gameState::GAME;
            endgame = false;
            break;


        case gameState::GAME:
            std::cout << "Game started!\n";

            while(!endgame) {

                std::string input;

                p.clear();
                sockets[i]->setBlocking(true);
                sockets[i]->receive(p); // Recibir inputs
                p >> input;
                std::cout << input << std::endl;

                if(input == "I") {
                std::cout << "I pressed\n";
                // FUNCIÓN
                    p.clear();
                    p << 1; sockets[i]->send(p);
                    // EL UNO HAY QUE ENVIARSELO SI O SI PARA QUE EN EL CLIENTE
                    // EN UN IF EN SU MAIN, ALLI RECIBIRA TODOS LOS DATOS Y LOS IMPRIMIRÁ POR CONSOLA

                }
                else if(input == "U") {
                std::cout << "U pressed\n";
                    p.clear();
                    p << 2; sockets[i]->send(p);
                // ENVIAR UN PACKET CON LOS DOATOS DE LOS USUARIOS QUE HAY CONECTADOS

                }
                else if(input == "G") {
                    // FUNCION GUARDAR GEMAS EN BASE DE DATOS
                std::cout << "G pressed\n";
                    p.clear();
                    p << 3; sockets[i]->send(p);
                    // RECIBIR GEMAS DEL PLAYER EN UN PACKET
                // ACCEDER A BASE DE DATOS Y GUARDAR LAS GEMAS DEL JUGADOR
                }
                else if(input == "ESC") {
                std::cout << "ESC pressed\n";
                    p.clear();
                    p << 0;
                    sockets[i]->send(p);

                    p.clear();
                    mes = "User disconnected: ";
                    p << i << mes;

                    //ENVIAR DESCONEXIÓN A LOS DEMÁS USERS
                    for(int u = 0; u<sockets.size(); u++) {
                        if(u!=i) {
                            sockets[u]->send(p);
                        }

                    }
                    endgame = true;
                }
                else {
                    p.clear();
                    p << 4; sockets[i]->send(p);


                    /// PLAYER ///
                    users[i].player.Move(input, field);

                    std::cout << users[i].player.pos.x << " " << users[i].player.pos.y << "\n";

                    p.clear();

                    p << users[i].player.pos.x << users[i].player.pos.y; // BASE DE DATOS
                    sockets[i]->send(p);
                    p.clear();


                    /// ENEMIES ///

                    if(actualTime >= oneSecond) {
                        actualTime = 0;
                        moveEnemies(sockets[i], &enemies, &users[i], &field);

                    }
                    else {  actualTime++;

                        p << (int)enemies.size();

                        sockets[i]->send(p); // enviar num enemigos
                        p.clear();

                        for(std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
                            bool death = false;

                            p << it->pos.x << it->pos.y << death;
                            if(it->health <= 0) {
                                field.squares[it->pos.x][it->pos.y] = ' ';
                                enemies.erase(it);
                            }

                        }
                            sockets[i]->send(p);

                    } //else

                } // if de input recibido 0-1-2-3-4

            } // while endgame

            state = gameState::END;

            break;

            case gameState::END:
                std::cout << "Game Ended" << std::endl;
                //status = sf::Socket::Disconnected;
                finishGame = true;
            break;

            default:
            break;



    }


} // finishGame loop

} // END FUNCTION



int main() {

    std::vector<sf::TcpSocket*> sockets;
    std::vector<std::string> strings;
    sf::TcpListener dispatcher;
    //GameState state = GameState::INIT;

    sf::Socket::Status status = dispatcher.listen(PORT);
    if(status != sf::Socket::Done){
        std::cout << "No se puede vincular al puerto " << PORT << "\n"; //, pruebe a bajar su nivel de belleza

    }

    while(!ended)
    {

        std::cout << "esperando clientes\n";

    mtx.lock();

        sf::TcpSocket *lastSocket = new sf::TcpSocket;

            std::cout << "socket creado, listening\n";

        status = dispatcher.accept(*lastSocket);

            std::cout << "after dispatcher.accept\n";

        if(status != sf::Socket::Done){
            std::cout << "Error al conectar\n";
        } else {
            std::cout << "Se ha podido conectar!\n";

            sockets.push_back(lastSocket);

            //THREAD
            std::thread t(&Play, sockets, sockets.size()-1/*, db*/);
            t.detach();
        }

    mtx.unlock();

    }


    //incoming.disconnect();
    //dispatcher.close();

    return 0;
}
