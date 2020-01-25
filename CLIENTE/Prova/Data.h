#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Constants.h"

struct Gem{
    int quirk;
    std::string name;
    sf::CircleShape shape;

    Gem(){};
    Gem(const Gem &g) : quirk(g.quirk), name(g.name), shape(g.shape) {};
    Gem(const int &_quirk, const std::string &_name, sf::CircleShape &_shape) : quirk(_quirk), name(_name), shape(_shape) {};

    void Init(const int &_quirk, const std::string &_name, sf::CircleShape &_shape){
        quirk = _quirk;
        name = _name;
        shape = _shape;

    }

};


struct Player{
    int atk;
    int def;
    sf::Vector2i pos;
    std::vector<Gem> gems;
    sf::RectangleShape shape;
    sf::Color color = sf::Color::Blue;
    Direction dir = Direction::NONE;

    Player() {
        atk = (rand() % 11) + 5;
        def = (rand() % 11) + 5;
        pos = {0, 0};
        shape = sf::RectangleShape(sf::Vector2f(CASILLA_HORIZONTAL - 2,CASILLA_VERTICAL - 2));
        shape.setFillColor(color);

    };
    Player(const Player &p) : atk(p.atk), def(def), pos(p.pos), gems(p.gems), shape(p.shape) {};
    Player(const sf::Vector2i _pos, const std::vector<Gem> &_gems, sf::RectangleShape &_shape) :
        atk((rand() % 11) + 5), def((rand() % 11) + 5), pos(_pos), gems(_gems), shape(_shape) {};

    void Init(const sf::Vector2i _pos, const std::vector<Gem> &_gems, sf::RectangleShape &_shape){
        atk = (rand() % 11) + 5;
        def = (rand() % 11) + 5;
        pos = _pos;
        gems = _gems;
        shape = _shape;

    }
};


struct Enemy{
    int atk;
    int def;
    int health;
    sf::Vector2i pos;
    Gem gem;
    sf::RectangleShape shape;
    sf::Color color = sf::Color::Red;
    Direction dir = Direction::NONE;


    Enemy () {};

};


struct User {
    std::string name = "?";
    std::string password = "baka";
    int gemNum = 0;
    int lastConn = 0;
    Player player;

    User(){};
    User(const User &u) : name(u.name), password(u.password), gemNum(u.gemNum), lastConn(u.lastConn) {};
    User(const std::string &_name, const std::string _password, const int &_gemNum, const int &_lastConn) :
        name(_name), password(_password), gemNum(_gemNum), lastConn(_lastConn) {};

    void Init(const std::string &_name, const std::string _password, const int &_gemNum, const int &_lastConn){
        name = _name;
        password = _password;
        gemNum = _gemNum;
        lastConn = _lastConn;

    };

};



struct Data{
    std::string id;
    User user;

    Data(){};
    Data(const Data &d) : id(d.id), user(d.user) {};
    Data(const std::string &_id, User &_user) : id(_id), user(_user) {};

    void Init(const std::string &_id, const User &_user){
        id = _id;
        user = _user;

    };

    void Init(sf::TcpSocket &socket) {
        //AskNameAndPassword(socket);

    }

    void AskNameAndPassword(sf::TcpSocket &socket, std::string &name, std::string &password) {
        bool done = false;
        int exists = 2;  //0 = ok, 1 = falla contra, 2 = no exiteix user
        sf::Packet p;
        std::cout << "Log In\n";
        while(!done){
            std::cout << "Name: ";
            std::cin >> user.name;
            std::cout << "Password: ";
            std::cin >> user.password;

            p << name << password; socket.send(p); p.clear();
            socket.receive(p); p >> exists; p.clear();


            //1. Mirar si el nom i el jugador concorden amb el dalgu a la base de dades, si ho son, done = true;
            //2. Si no ho fa, li preguntes si es vol crear una compte o que si vol tornar a posar el nom i contrasenya
            //3. Si diu que vol crear-se una nova compte l'introdueixes a la base de dades des del server
            if(exists == 0){
                CopyUserFromServer();   ///Copia User de db a User de Server i/o Client
                std::cout << "Login successful!\n";
                done = true;

            }
            else if(exists == 1){
                std::cout << "Looks like the User and password doesn't match... Try Again!\n";

            }
            else if(exists == 2){
                char confirmation;
                std::cout << "This User doesn't exists. Do you want to create a new one? (y/n) ";
                std::cin >> confirmation;

                if(confirmation == 'y' || confirmation == 'Y'){
                    std::cout << "\nCreating yout new account...\n";
                    //CreateNewUser();  //Segurament s'hagi de fer des del server
                    p << 1; socket.send(p); p.clear();
                    int accCreated = 0;
                    socket.receive(p); p >> accCreated; p.clear();
                    if(accCreated == 1){
                        done = true;
                        std::cout << "Account created successfully!\n";

                    }
                    else{
                        std::cout << "Ups, something went wrong! Try again later...\n";

                    }


                }
                else{
                    p << 0; socket.send(p); p.clear();

                }

            }

        }

    }

    void CopyUserFromServer(){


    }

};

struct mapSquares {
sf::RectangleShape rectFondo;

sf::RectangleShape wall;

sf::RectangleShape player;

sf::RectangleShape obstacle;

sf::RectangleShape enemy;

sf::CircleShape gem;

mapSquares(){}
~mapSquares(){}
};


#endif // DATA_H_INCLUDED
