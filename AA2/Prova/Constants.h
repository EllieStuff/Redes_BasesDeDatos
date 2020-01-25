#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED


#define W_WINDOW_TITLE 1000
#define H_WINDOW_TITLE 800

#define CASILLA_HORIZONTAL 10
#define CASILLA_VERTICAL 10

#define NUM_OBSTACLES_2x2 10


enum class Direction {UP = 'U', DOWN = 'D', RIGHT = 'R', LEFT = 'L', NONE = 'N'};


const int PORT = 5005;
const std::string DB_HOST = "tcp://127.0.0.1";
const std::string DB_USER = "root";
const std::string DB_PASSWORD = "NOMbre2000";

const int FIELD_MAX_SIZE = 100;



#endif // CONSTANTS_H_INCLUDED
