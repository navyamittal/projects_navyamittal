#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    //loadlevel: check for inputs, invalid character check
    char** map=nullptr;
    int door=0;
    int exit=0;
    ifstream fin(fileName);
    if (!fin.is_open()){
        return nullptr;
    }
    fin >> maxRow;
    if (fin.fail() || maxRow<=0){
        return nullptr;
    }
    fin >> maxCol;
    if (fin.fail() || maxCol <=0){
        return nullptr;
    }
    //checking for overflow
    if(maxCol>INT32_MAX/maxRow){      
        return nullptr;
    }
    if (maxRow>INT32_MAX/maxCol){
        return nullptr;
    }
    fin >> player.row;
    if (fin.fail() || player.row >=maxRow || player.row<0){
        return nullptr;
    }
    fin >> player.col;
    if (fin.fail() || player.col >= maxCol || player.col<0){
        return nullptr;
    }    
    map = createMap(maxRow,maxCol);
    

    for (int i =0; i<maxRow; i++){
        for (int j =0; j<maxCol; j++){
            fin>> map[i][j];
            if (fin.fail()){ //not enough values
                //deleteMap(map,maxRow);
                return nullptr;
            }
            if (map[i][j]==TILE_PLAYER){ //there cannot be 2 players
                //deleteMap(map,maxRow);
                return nullptr;
            }
            if (map[i][j]!= TILE_OPEN && map[i][j]!=TILE_TREASURE && map[i][j]!=TILE_AMULET && map[i][j]!=TILE_MONSTER && map[i][j]!= TILE_PILLAR && map[i][j]!= TILE_DOOR && map[i][j]!=TILE_EXIT){
                //deleteMap(map,maxRow);
                return nullptr;
            }            
            if (map[i][j]==TILE_DOOR){
                door+=1;
            }
            if (map[i][j]==TILE_EXIT){
                exit+=1;
            }
        }
    }
    int temp = 0;
    fin>>temp;
    if (!fin.eof()){ //more values
        //deleteMap(map,maxRow);
        return nullptr;
    }
    if (door==0 && exit ==0){ //checking if it has an exit or a door
        //deleteMap(map,maxRow);
        return nullptr;
    }
    
    /*if(map[player.row][player.col]==TILE_OPEN){ //player can only be on an open tile 
        deleteMap(map,maxRow);
        return nullptr;
    }*/
    map[player.row][player.col] = TILE_PLAYER;
    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if (input == 's'){
        nextRow+=1;
    }
    if (input == 'd'){
        nextCol+=1;
    }
    if (input == 'a'){
        nextCol-=1;
    }
    if (input=='w'){
        nextRow-=1;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    if (maxRow<=0|| maxCol<=0){
        return nullptr;
    }
    char** map=new char*[maxRow];
    for (int y =0; y < maxRow; y++){
        map[y]= new char[maxCol];
    }
    for (int i =0; i<maxRow; i++){
        for (int j = 0; j<maxCol; j++){
            map[i][j]=TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    //maxrow is 0<0, map is null
    /*if (maxRow <=0){
        return;
    }*/
    if (map==nullptr){
        maxRow = 0;
        return;
    }
    if(map!=nullptr){
        for (int i =0; i<maxRow; i++){
            delete[] map[i];
        }
        delete[] map;
        map = nullptr;
        maxRow=0;
    }
    
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    int updatedrow = 0;
    int updatedcol = 0;
    updatedrow = 2*maxRow;
    updatedcol = 2*maxCol;
    char** temp=new char*[updatedrow];
    if (maxRow<=0 || maxCol<=0){
        return nullptr;
    }
    if (map==NULL){
        return nullptr;
    }
    for (int y =0; y < updatedrow; y++){
        temp[y]= new char[updatedcol];
    }
    for (int i =0; i<maxRow; i++){ //subsection A
        for (int j =0; j<maxCol; j++){
            temp[i][j] = map[i][j];
        }
    }

    for (int i=0; i<maxRow; i++){ //subsection B
        for (int j = maxCol; j<updatedcol; j++){
            temp[i][j]= map[i][abs(maxCol-j)]; //since i, j start at maxrow,maxcol abs(maxcol-i) gives values 0 to maxcol
        }
    }

    for (int i=maxRow; i<updatedrow; i++){ //subsection C
        for (int j = 0; j<maxCol; j++){
            temp[i][j] = map[abs(maxRow-i)][j];
        }
    }
    for (int i=maxRow; i<updatedrow; i++){ //sub D
        for (int j = maxCol; j<updatedcol; j++){
            temp[i][j] = map[abs(maxRow-i)][abs(maxCol-j)];
        }
    }
    for (int i=0; i<maxRow; i++){ //deleting player from subsection B
        for (int j = maxCol; j<updatedcol; j++){
            if (temp[i][j] == TILE_PLAYER){
                temp[i][j] = TILE_OPEN;
            }
            
        }
    }
    for (int i=maxRow; i<updatedrow; i++){ //deleting player from subsection C
        for (int j = 0; j<updatedcol; j++){
            if (temp[i][j] == TILE_PLAYER){
                temp[i][j] = TILE_OPEN;
            }
        }
    }

    for (int i=maxRow; i<updatedrow; i++){ //deleting player from sub D
        for (int j = maxCol; j<updatedcol; j++){
            if (temp[i][j] == TILE_PLAYER){
                temp[i][j] = TILE_OPEN;
            }
        }
    }
    for (int i =0; i<maxRow; i++){
        delete[] map[i];
    }
    delete[] map;
    map = nullptr;
    maxRow*=2; //doubling maxrow and maxcol
    maxCol*=2;
    return temp;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    int status=0;
    map[player.row][player.col]=TILE_PLAYER;
            if (nextRow >=maxRow || nextCol >=maxCol || nextRow <0 || nextCol <0){
                nextRow= player.row;
                nextCol = player.col;
                status=STATUS_STAY;
                return 0;
            }
            else if (map[nextRow][nextCol]==TILE_MONSTER || map[nextRow][nextCol]== TILE_PILLAR){
                status=STATUS_STAY;
                nextRow= player.row;
                nextCol = player.col;
                return 0;
            }

            else if(map[nextRow][nextCol]==TILE_TREASURE){
                player.treasure+=1;
                map[player.row][player.col]=TILE_OPEN; //old position
                player.row = nextRow;
                player.col = nextCol;
                map[player.row][player.col]=TILE_PLAYER; //new position
                status = STATUS_TREASURE;
            }
            else if(map[nextRow][nextCol]==TILE_AMULET){
                map[player.row][player.col]=TILE_OPEN; //old position
                player.row = nextRow;
                player.col = nextCol;
                map[player.row][player.col]=TILE_PLAYER; //new position
                status=STATUS_AMULET;
            }
            else if (map[nextRow][nextCol]==TILE_DOOR){
                map[player.row][player.col]=TILE_OPEN;
                player.row = nextRow;
                player.col = nextCol;
                map[player.row][player.col]=TILE_PLAYER; //new position
                status = STATUS_LEAVE;
            }
            else if (map[nextRow][nextCol]==TILE_EXIT){
                if (player.treasure>0){
                    map[player.row][player.col]=TILE_OPEN; //old position
                    player.row = nextRow;
                    player.col = nextCol;
                    map[player.row][player.col]=TILE_PLAYER; //new position
                    status=STATUS_ESCAPE;
                }
                else if (player.treasure==0){
                    status=STATUS_STAY;
                    nextRow= player.row;
                    nextCol = player.col;

                }                
            }
            else if (map[nextRow][nextCol]== TILE_OPEN){
                map[player.row][player.col]=TILE_OPEN; //old position
                player.row = nextRow;
                player.col = nextCol;
                map[player.row][player.col]=TILE_PLAYER;
                status = STATUS_MOVE;

            }
            

    return status;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    bool final = false;
    if (maxRow <=0){
        return false;
    }
    if (maxCol<=0){
        return false;
    }
    if (player.row>maxRow){
        return false;
    }
    if(player.col>maxCol){
        return false;
    }
    //map[player.row][player.col] = TILE_PLAYER;
    for (int j=0; j<player.row+1; j++){ //monster above player
        if (map[player.row-j][player.col]==TILE_PILLAR){ //checking till pillar
            break;
        }
        if(map[player.row-j][player.col]==TILE_MONSTER){
            map[player.row-j][player.col]=TILE_OPEN;
            if (map[player.row-j+1][player.col]==TILE_PLAYER){
                map[player.row-j+1][player.col]=TILE_MONSTER;
                final = true;
            }
            map[player.row-j+1][player.col]=TILE_MONSTER;
        }
    }

    for (int j=player.row; j<maxRow;j++){ //checking M down or player below monster
        if (map[j][player.col]==TILE_PILLAR){ //checking till pillar
            break;
        }
        if (map[j][player.col]== TILE_MONSTER){
            map[j][player.col]=TILE_OPEN;
            if (map[j-1][player.col]==TILE_PLAYER){
                map[j-1][player.col]=TILE_MONSTER;
                final = true;
            }
            map[j-1][player.col]=TILE_MONSTER;
        }
    }
    for (int j=0; j<player.col+1; j++){ // M      P (first monster, then player)
        if(map[player.row][player.col-j]==TILE_PILLAR){
            break;
        }
        if (map[player.row][player.col-j]==TILE_MONSTER){
            map[player.row][player.col-j] = TILE_OPEN;
            if (map[player.row][player.col-j+1] == TILE_PLAYER){
                map[player.row][player.col-j+1] =TILE_MONSTER;
                final = true;
            }
            map[player.row][player.col-j+1] =TILE_MONSTER;
        }
    }

    for (int j =player.col; j<maxCol; j++){ //  P       M (first player, then monster)
        if (map[player.row][j]==TILE_PILLAR){
            break;
        }
        if (map[player.row][j]==TILE_MONSTER){
            map[player.row][j] =TILE_OPEN;
            if (map[player.row][j-1]==TILE_PLAYER){
                map[player.row][j-1] =TILE_MONSTER; 
                final = true;
            }
            map[player.row][j-1] =TILE_MONSTER; 
        }
    }

    return final;
}
