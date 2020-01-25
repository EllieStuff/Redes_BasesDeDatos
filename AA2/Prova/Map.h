//#ifndef MAP_H_INCLUDED
//#define MAP_H_INCLUDED
#ifndef DEBUG
#define PUGIXML_HEADER_ONLY

#include <sstream>
#include <iostream>
#include <fstream>
#include <pugixml.hpp>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <resultset.h>
#include <statement.h>

#include "Constants.h"


class Map {
public:
    enum Tiles { WALL = 'W', OBSTACLE = 'O', PLAYER = 'P', ENEMY = 'E', GEM = 'G', EMPTY = ' '};
    char squares[FIELD_MAX_SIZE][FIELD_MAX_SIZE];


    Map(){
        Reinit();

    };

    void Reinit(){
        for(int i = 0; i < FIELD_MAX_SIZE; i++){
            for(int j = 0; j < FIELD_MAX_SIZE; j++){
                squares[i][j] = ' ';

            }

        }

    }


    void ReadXML(short mapNum){
        ///Open XML
        pugi::xml_document doc;
        pugi::xml_parse_result result;
        if(mapNum == 1) {
            result = doc.load_file("mapa1_aa2.xml");
        } else {
            result = doc.load_file("mapa2_aa2.xml");
        std::cout << "Se ha cargado: " << result.description() << std::endl;
        }
        ///Read XML
        pugi::xml_node father = doc.child("Map");
        for(pugi::xml_node node = father.child("Player"); node; node = node.next_sibling("Player")){
            squares[std::stoi(node.attribute("y").value())][std::stoi(node.attribute("x").value())] = 'P';
            //std::cout << "Reading a Player child from node: x = " << node.attribute("x").value() << ", y = " << node.attribute("y").value() << std::endl;

        }
        std::cout << "\n ------------------\n\n";
        for(pugi::xml_node node = father.child("Enemy"); node; node = node.next_sibling("Enemy")){
            squares[std::stoi(node.attribute("y").value())][std::stoi(node.attribute("x").value())] = 'E';
            //std::cout << "Reading an Enemy child from node: x = " << node.attribute("x").value() << ", y = " << node.attribute("y").value() << std::endl;

        }
        std::cout << "\n ------------------\n\n";
        for(pugi::xml_node node = father.child("Wall"); node; node = node.next_sibling("Wall")){
            squares[std::stoi(node.attribute("y").value())][std::stoi(node.attribute("x").value())] = 'W';
            //std::cout << squares[std::stoi(node.attribute("x").value())][std::stoi(node.attribute("y").value())];
            //std::cout << "Reading a Wall child from node: x = " << node.attribute("x").value() << ", y = " << node.attribute("y").value() << std::endl;

        }
        std::cout << "\n ------------------\n\n";



        /*std::ofstream file;
        if(mapNum == 1)
            file.open("mapa1_aa2.xml");
        else
            file.open("mapa2_aa2.xml");   ///Revisar nom quan es tingui

        std::stringstream buffer;
        if(file.is_open()) buffer << file.rdbuf();
        else std::cout << "Cannot open xml file\n";

        file.close();

        std::string content(buffer.str);*/


    };

    void PrintMapOnTerminal(){
        for(int i = 0; i < FIELD_MAX_SIZE; i++){
            for(int j = 0; j < FIELD_MAX_SIZE; j++){
                std::cout << squares[i][j];

            }
            std::cout << std::endl;
        }

    }


};


#endif // MAP_H_INCLUDED
