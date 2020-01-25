#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED


#define W_WINDOW_TITLE 1000
#define H_WINDOW_TITLE 800

#define CASILLA_HORIZONTAL 10
#define CASILLA_VERTICAL 9

#define NUM_OBSTACLES_2x2 10


enum class Direction {UP = 'U', DOWN = 'D', RIGHT = 'R', LEFT = 'L', NONE = 'N'};


const int PORT = 5005;

const std::string CONTRA = "NOMbre2000";

const int FIELD_MAX_SIZE = 100;
const int FIELD_TOTAL_SQUARES = 1000;


#endif // CONSTANTS_H_INCLUDED
