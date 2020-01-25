#ifndef DATABASEFUNCTIONS_H_INCLUDED
#define DATABASEFUNCTIONS_H_INCLUDED

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <resultset.h>
#include <statement.h>

int rouletteDelayTime = 60;

sql::Connection* GetConnection()
{
    try {

        sql::Driver* driver = sql::mysql::get_driver_instance();
        std::cout << "despues del driver \n";
        sql::Connection* conn = driver->connect(DB_HOST, DB_USER, DB_PASSWORD);

        if(conn==nullptr)std::cout << "falla connection?\n";
        else{std::cout << conn << std::endl;}

        conn->setSchema("gamedb"); // NOMBRE DE LA BASE DE DATOS

        if(conn==nullptr)std::cout << "falla schema?\n";
        else{std::cout << conn << std::endl;}

        return conn;

    } catch(sql::SQLException& e) {std::cout << "Salto excepcion" << e.what() << std::endl;}
}

//SI DEVUELVE 0 ES QUE EL USUARIO NO EXISTE, SI DEVUELVE 1 EXISTE Y CONTRASEÑA CORRECTA, SI DEVUELVE 2 EXISTE PERO CONSTRASEÑA INCORRECTA
//AL USAR LOGIN LE PASAMOS UN PUNTERO A INT PARA QUE ESTA FUNCION NOS DE EL ID DEL PLAYER EN ESE PUNTERO A INT
bool Login( sql::Connection* conn, sf::TcpSocket *socket, std::string name, std::string password, int* playerID)
{
    sf::Packet p;
    try
    {
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res= stmt->executeQuery("SELECT PlayerID, PlayerName, PlayerPassword FROM Players WHERE PlayerName='"+ name +"'");
        if(res==nullptr) std::cout << "es nullptr\n";
        //std::cout << "despues del resultset" << std::endl << "SELECT PlayerName FROM Players WHERE PlayerName='"+ name +"' and PlayerPassword='"+ password +"'" << std::endl;
        bool noExist = true;
        int retValue = false;
        if(res->next())
        {
            noExist = false;
            std::cout << "Existe el usuario!" << res->getString("PlayerName");
            if(res->getString("PlayerPassword") == password)
            {
                //Contraseña está bien
                *playerID = res->getInt("PlayerID");
                std::cout << "Contraseña bien" << std::endl;
                p << 0; //Tot ok
                socket->send(p);
                retValue = true;
                // Mirar Ruleta
            }
            else
            {
                //Contraseña está mal !!
                std::cout << "Contraseña mal" << std::endl;
                p << 1; //Falla contrasenya
                socket->send(p);
            }
        }

        if(noExist)
        {
            std::cout << "No existe\n";
            p << 2; //L'usuari no existeix
            socket->send(p); p.clear();
            int createUser = 0;
            socket->receive(p); p >> createUser; p.clear();
            if(createUser == 1){    //true
                stmt->executeUpdate("INSERT INTO Players (PlayerID ,PlayerName , PlayerPassword) VALUE(NULL ,'" + name + "', '" + password + "')");
                std::cout << "1\n";
                res= stmt->executeQuery("SELECT PlayerID FROM Players WHERE PlayerName='" + name + "'");
                res->next();
                std::cout << "2\n";
                *playerID = res->getInt("PlayerID");
                //std::cout << "3\n";
                //std::cout << ", te hemos creado una cuenta con tus Username y Password!!" << std::endl;
                p << 1; socket->send(p); p.clear();
                retValue = true;
            }
        }
        stmt->close();
        res->close();
        return retValue;

    } catch(sql::SQLException& e) {std::cout << "Salto excepcion en Login" << e.what() << std::endl;}

    return false;
}

time_t datetimeConv(const std::string& dateTime)
{
    tm t;
    strptime(dateTime.c_str(), "%F %T", &t);
    return mktime(&t);
}

bool canThrowRoulette(sql::Connection* conn, int userID)
{
    sql::Statement* stmt = conn->createStatement();

    sql::ResultSet* result = stmt->executeQuery("SELECT PlayerID, LastPrizeTime from Players");

    while(result->next())
    {
        if(result->getInt("PlayerID") == userID)
        {
            time_t lastTime = datetimeConv((std::string)result->getString("LastPrizeTime"));

            std::cout << std::difftime(std::time(nullptr), lastTime) - 3600 << " | \n";
            std::cout << rouletteDelayTime << " | \n";

            if( std::difftime(std::time(nullptr), lastTime) > rouletteDelayTime)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

void setTimeStamp(sql::Connection* conn, int userID)
{
    sql::Statement* stmt = conn->createStatement();

    std::string tempStr = "UPDATE Players SET LastPrizeTime = NOW() where PlayerID = '";
    tempStr += std::to_string(userID);
    tempStr += "'";
    //std::cout << tempStr;
    stmt->executeUpdate(tempStr);

    delete(stmt);
}

void addGem(sql::Connection* conn, int gemScore, int userID)
{
    sql::Statement* stmt = conn->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT Gem_ID FROM Gems WHERE Puntuacion='" + std::to_string(gemScore) + "' ");
    if(res->next())
    {
        int gemID = res->getInt("Gem_ID");
        res = stmt->executeQuery("SELECT Gem_FK, Amount FROM Players_Gems_FK WHERE Player_FK='" + std::to_string(userID) + "'");
        if(res->next())
        {
            //stmt->executeUpdate("SET Amount = Amount + 1 FROM Players_Gems_FK WHERE Player_FK='" + std::to_string(userID) + "'");
            stmt->executeUpdate("UPDATE Players_Gems_FK SET Amount = Amount + 1 WHERE Player_FK='" + std::to_string(userID) + "'");
            res->close();
            stmt->close();
            return;
        }
        stmt->executeUpdate("INSERT INTO Players_Gems_FK (Player_FK, Gem_FK, Amount) VALUE( " + std::to_string(userID) + ", " + std::to_string(gemID) + ", 1)");
        res->close();
        stmt->close();
        return;
    }
}

void clearPlayerGems(sql::Connection* conn, int userID)
{
    try
    {
    sql::Statement* stmt = conn->createStatement();
    stmt->executeUpdate("DELETE FROM Players_Gems_FK WHERE Player_FK='" + std::to_string(userID) + "'");
    stmt->close();
    }catch(sql::SQLException& e){std::cout << "Salto de excepcion en clearPlayerGems" << e.what() << std::endl;}
}

int getGemScore(sql::Connection* conn, int gemID)
{
    int score = 0;
    try
    {
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT Puntuacion FROM Gems WHERE Gem_ID ='" + std::to_string(gemID) + "'");
        if(res->next())
        {
            score = res->getInt("Puntuacion");
        }
        res->close();
        stmt->close();

    } catch(sql::SQLException& e){std::cout << "Salto de excepcion en getGemScore" << e.what() << std::endl;}
    return score;
}

int getCurrentScore(sql::Connection* conn, int userID)
{
    int score = 0;
    try
    {
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT Gem_FK, Amount FROM Players_Gems_FK WHERE Player_FK='" + std::to_string(userID) + "'");

        while(res->next())
        {
            int gemScore = getGemScore(conn, res->getInt("Gem_FK"));
            gemScore *= res->getInt("Amount");
            score += gemScore;
        }
        res->close();
        stmt->close();

    } catch(sql::SQLException &e) {std::cout << "Excepción en getCurrentScore" << e.what() << std::endl;}

    return score;
}


#endif // DATABASEFUNCTIONS_H_INCLUDED
