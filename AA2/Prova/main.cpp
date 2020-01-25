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
#include "databaseFunctions.h"
//#include "Map.h"


#include <wait.h>
#include <mutex>
#include <thread>
#include <pthread.h>



std::mutex mtx;

int actualTime = 0;
int oneSecond = 60;

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

                //std::cout << enemies->size() << " enemies\n";
                sf::Packet p;
                p << (int)enemies->size();
                sock->send(p); // Enviar numero de enemigos
                p.clear();

                for(std::vector<Enemy>::iterator it = enemies->begin(); it != enemies->end(); it++) {
                    it->Move(user->player, sock, *field);
                    p << it->pos.x << it->pos.y;

                    if(it->health <= 0) {
                        field->squares[it->pos.x][it->pos.y] = ' ';
                        std::cout << "dead\n";
                        enemies->erase(it);
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
    int *playerID = new int(0); // PILLAR EL PLAYER ID

    std::vector<Enemy> enemies; // GUARDAR TODOS LOS ENEMIGOS DE LA BASE DE DATOS EN ESTE VECTOR

    Map field; //

    int mapNum = 0;

    bool loggedIn = false;
    bool endgame = false;
    bool combatBlock = false;
    bool yes = false;
    bool replaying = false;

    User us;


    std::string mes;

    while (!finishGame) { // este loop no hace falta diria

    std::cout << "CLIENTE " << i << "\n";

        switch(state) {

        case gameState::INIT:
            if(replaying){
                p.clear();
                field.Reinit();
                mapNum = -1;
                combatBlock = false;
                yes = false;
                endgame = false;
            }
            else{
                replaying = true;

                // FUNCION ACCEDER A BASE DE DATOS Y MIRAR name Y password.
                try
                {
                    sql::Connection* conn = GetConnection();

                    int* playerID = new int(0);

                    //AL USAR LOGIN LE PASAMOS UN PUNTERO A INT PARA QUE ESTA FUNCION NOS DE EL ID DEL PLAYER EN ESE PUNTERO A INT
                    while(!loggedIn){
                        sockets[i]->receive(p);
                        p >> name >> password;
                        std::cout << name << " - " << password << " - "<< playerID << std::endl;
                        loggedIn = Login(conn, sockets[i], name, password, playerID);

                    }

                    /*if(canThrowRoulette(conn, *playerID))
                    {
                        setTimeStamp(conn, *playerID);
                    }

                    std::cout << getCurrentScore(conn, *playerID) << std::endl;

                    clearPlayerGems(conn, *playerID);

                    std::cout << getCurrentScore(conn, *playerID);*/

                    //addGem(conn, 2, *playerID);

                    delete(conn);

                }
                catch(sql::SQLException &e)
                {
                    std::cout << "Se produce el error " << e.getErrorCode()<<std::endl;
                }
                // HAY QUE ENVIAR UN PACKET CON UN 0 SI NO ESTÁ EL USUARIO
                // O CON UN 1 SI SI QUE ESTÁ Y

                // GUARDAR EN us (Clase User) todos los datos del usuario que hay guardados en la Base de Datos


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
                sockets[i]->receive(p);

            }



            std::cout << "Asking map" << std::endl;
            while(!yes) {
                p.clear();
                p << "Do you want to play on map 1 or 2? ";
                sockets[i]->send(p);

                p.clear();
                sockets[i]->receive(p);// numero de mapa
                p>>mapNum;

                std::cout << "El mapa que voy a cargar es el " << mapNum << std::endl;

                p.clear();
                std::string mapStr = "";

                if(mapNum == 1) { // COGER UN PATH O LO QUE SEA DE LA BASE DE DATOS
                    std::cout<<"Mapa 1"<<std::endl;
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

                    std::cout << "xml read\n";
                    p.clear();
                    p << mapStr;
                    sockets[i]->send(p);

                    p.clear();
                    p << 1;
                    std::cout << "confirmation 1 map 1 sent\n";
                    sockets[i]->send(p); // enviar confirmación
                    yes = true;
                }
                else if(mapNum == 2) { // COGER EL SEGUNDO PATH
                    std::cout<<"Mapa 2"<<std::endl;
                    field.ReadXML(2);

                    for(int j = 0; j < FIELD_MAX_SIZE; j++){
                        for(int k = 0; k < FIELD_MAX_SIZE; k++){
                            if(field.squares[j][k] == 'E') {
                                enemies.push_back(Enemy(sf::Vector2i(j, k)));
                                std::cout << "Enemy " << j << ", " << k << std::endl;
                            }
                            else if(field.squares[j][k] == 'P') {
                                users[i].player.pos = sf::Vector2i(j, k);
                                std::cout << "Player " << j << ", " << k << std::endl;
                            }

                            if(field.squares[j][k] == 'W') {
                                mapStr += 'W';
                                std::cout << "Wall " << j << ", " << k << std::endl;

                            } else {
                                mapStr += ' ';
                            std::cout << "Nothin " << j << ", " << k << std::endl;

                            }

                        }
                            std::cout << "next" << std::endl;
                    }

                    std::cout << "end\n";

                    p.clear();
                    p << mapStr;
                    sockets[i]->send(p);

                    std::cout << "enviado mapa 2\n";

                    p.clear();
                    p << 1;
                    std::cout << "confirmation 1 map 2 sent\n";
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
            std::cout << "changed to GAME, still in Map asking\n";
            break;


        case gameState::GAME:
            std::cout << "state game\n";

            while(!endgame) {

                std::string input;
                p.clear();
                sockets[i]->setBlocking(true);

                //std::cout <<"gonna receive input\n";

                sockets[i]->receive(p); // Recibir inputs
                p >> input;
                //std::cout << input << std::endl;
                p.clear();

                if(input == "I") {
                std::cout << "estoy en I\n";
                // FUNCIÓN
                    p.clear();
                    p << 1; sockets[i]->send(p);
                    // EL UNO HAY QUE ENVIARSELO SI O SI PARA QUE EN EL CLIENTE
                    // EN UN IF EN SU MAIN, ALLI RECIBIRA TODOS LOS DATOS Y LOS IMPRIMIRÁ POR CONSOLA
                    p << (int)users[i].player.gems.size();
                    sockets[i]->send(p);
                    p.clear();
                    for(std::vector<Gem>::iterator it = users[i].player.gems.begin(); it != users[i].player.gems.end(); it++)
                        p << it->name;

                    sockets[i]->send(p);
                    p.clear();

                }
                else if(input == "U") {
                std::cout << "estoy en U\n";
                    p.clear();
                    p << 2; sockets[i]->send(p);
                    p.clear();
                    p << (int)users.size(); sockets[i]->send(p);
                    p.clear();
                    // ENVIAR UN PACKET CON LOS DOATOS DE LOS USUARIOS QUE HAY CONECTADOS
                    for(std::vector<User>::iterator it = users.begin(); it != users.end(); it++)
                        p << it->name;

                    sockets[i]->send(p);
                    p.clear();

                    std::cout << "In U\n";

                }
                else if(input == "G") {
                    // FUNCION GUARDAR GEMAS EN BASE DE DATOS
                    std::cout << "estoy en G\n";
                    p.clear();
                    p << 3; sockets[i]->send(p); p.clear();

                    //try{
                    //
                    //}

                    // RECIBIR GEMAS DEL PLAYER EN UN PACKET
                // ACCEDER A BASE DE DATOS Y GUARDAR LAS GEMAS DEL JUGADOR
                }
                else if(input == "ESC") {
                std::cout << "estoy en ESC\n";
                    p.clear();
                    p << 0;
                    sockets[i]->send(p);

                    p.clear();
                    std::cout << "In ESC\n";

                    int restart;
                    sockets[i]->receive(p);
                    p >> restart; p.clear();
                    endgame = true;
                    if(restart == 1)    //true
                        state = gameState::INIT;
                    else {  //false
                        state = gameState::END;
                        ///Treure el user i el socket dels vectors i desconnectar-los, no ho he posat prq crec que es millor fer-ho al gameState::END
                    }

                    //Necesari?
                    /*mes = "User disconnected: ";
                    p << i << mes;

                    //ENVIAR DESCONEXIÓN A LOS DEMÁS USERS
                    for(int u = 0; u<sockets.size(); u++) {
                        if(u!=i) {
                            sockets[u]->send(p);
                        }

                    }
                    endgame = true;*/
                }
                else {
                    p.clear();
                    p << 4; sockets[i]->send(p);


                    /// PLAYER ///
                    users[i].player.Move(input, field);

                    //std::cout << users[i].player.pos.x << " " << users[i].player.pos.y << "\n";

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
                        //std::cout << "enemies sent: " << enemies.size() << std::endl;
                        p.clear();

                        for(std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); it++) {
                            p << it->pos.x << it->pos.y;

                            if(it->health <= 0) {
                                field.squares[it->pos.x][it->pos.y] = ' ';
                                enemies.erase(it);
                            }

                        }
                            sockets[i]->send(p);

                    } //else

                } // if de input recibido 0-1-2-3-4

            } // while endgame

            //state = gameState::END;

            break;

            case gameState::END:
                std::cout << "case end" << std::endl;
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
        status = dispatcher.accept(*lastSocket);
        if(status != sf::Socket::Done){
            std::cout << "Error al conectar\n";
        } else {
            std::cout << "Se ha podido conectar!\n";

            sockets.push_back(lastSocket);
            ///std::cout << &lastSocket <<  " = " << sockets.back() << std::endl;

            //THREAD
            ///DataBase db;

            std::thread t(&Play, sockets, sockets.size()-1/*, db*/);
            t.detach();
            std::cout << "done";
        }

    mtx.unlock();

    }


    //incoming.disconnect();
    //dispatcher.close();

    return 0;
}
