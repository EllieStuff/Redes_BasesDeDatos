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
    sf::Vector2i pos;

    Gem(){};
    Gem(const Gem &g) : quirk(g.quirk), name(g.name), shape(g.shape) {};
    Gem(const int &_quirk, const std::string &_name, sf::CircleShape &_shape) : quirk(_quirk), name(_name), shape(_shape) {};

    void Init(int x, int y, const int &_quirk, const std::string &_name, sf::CircleShape &_shape){
        quirk = _quirk;
        name = _name;
        shape = _shape;
        pos.x = x; pos.y = y;

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

    void Init() {
        AskNameAndPassword();

    }

    void AskNameAndPassword() {
        bool done = false;
        bool exists = true; //Aquesta variable s'hauria de confirmar des del server
        std::cout << "Log In\n";
        while(!done){
            std::cout << "Name: ";
            std::cin >> user.name;
            std::cout << "Password: ";
            std::cin >> user.password;

            //1. Mirar si el nom i el jugador concorden amb el dalgu a la base de dades, si ho son, done = true;
            //2. Si no ho fa, li preguntes si es vol crear una compte o que si vol tornar a posar el nom i contrasenya
            //3. Si diu que vol crear-se una nova compte l'introdueixes a la base de dades des del server
            if(exists){
                CopyUserFromServer();
                done = true;

            }
            else{
                char confirmation;
                std::cout << "User and password doesn't match. Do you want to create a new user? (y/n) \n";
                std::cin >> confirmation;

                if(confirmation == 'y'){
                    //CreateNewUser();  //Segurament s'hagi de fer des del server
                    done = true;

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
