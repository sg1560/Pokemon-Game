#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//build a map
    //which includes filling it up with the necessary elements
    //rectangle way: we generate a terrain type, and the locations and 
    //continue building rectangles until full terminal is occupied
    //then we build roads using the way schaeffer said
    //finally we build the buildings
    //once all this is finished, we use queues to generate the random terrains
    //remember we only call the random number once
//print the map


//2 regions of long grass (:)
//1 region of water (~);
//2 clearings(.)

//21 rows and 80 columns
char terrain[21][80];
//defines the region types in the terrain
char regionTypes [4] = {':', '~', '.', '^'};
int numWater = 0; 
int numGrass = 0; 
int numClearing = 0;
int numTrees = 0;
//Sets every cell to empty
void initializeMap(){
    int i, j;
    for(i = 0; i < 21; i++){
        for(j = 0; j < 80; j++){
            terrain[i][j] = 'e';
        }
    }
}

void buildRectangles(char regionType, int row, int col, int width, int height){
    for(int i = row; i < row + height; i++){

        //bounds checking
        if(i >= 20){
            //printf("inside i break, i = %d\n", i);
            break;
        }
        for(int j = col; j < col + width; j++){
            if(j >= 79){
                break;
            }

            if(terrain[i][j] == 'e' && terrain[i][j] != regionType){
                terrain[i][j] = regionType;
            }
            //printf("cur terrain = %c\n", terrain[i][j]);
        }
    }
}

//works
void buildBorder(){
    int i, j;
    for(i = 0; i < 80; i++){
        terrain[0][i] = '%';
        terrain[20][i] = '%';
    }

    for(j = 0; j < 21; j++){
        terrain[j][0] = '%';
        terrain[j][79] = '%';
    }
}
//Works 
void printMap(){
    int i, j;
    for(i = 0; i < 21; i++){
        for(j = 0; j < 80; j++){
            printf("%c", terrain[i][j]);
        }
        printf("\n");
	}
}


void buildRoads(){
    //Builds vertical road (from north to south)
    int firstX = rand() % 78 + 1; 
    int endX = rand() % 78 + 1;
    int height = rand() % 20 + 1;
    int i, j, k;
    for(i = 0; i < height; i++){
        terrain[i][firstX] = '#';
    }
    int diff = endX - firstX;
    int dir = (endX - firstX) / (abs(endX - firstX));
    for(j = 0; j < abs(diff); j++){
        if(endX < firstX){
             terrain[height][firstX - j] = '#';
        }else{
            terrain[height][firstX + j] = '#';
        }
       
    }
    for(k = height; k < 21; k++){
        terrain[k][endX] = '#';
    }

    //Builds horizontal road (from east to west)
    int firstY = rand() % 19 + 1;
    int endY = rand() % 19 + 1;
    int width = rand() % 78 + 1;
    for(i = 0; i < width; i++){
        terrain[firstY][i] = '#';
    }
    int Ydiff = endY - firstY;
    int Ydir = (endY - firstY) / (abs(endY - firstY));
    for(j = 0; j < abs(Ydiff); j++){
        if(endY < firstY){
            terrain[firstY - j][width] = '#';
        }else{
            terrain[firstY + j][width] = '#';
        }
    }
     
    for(k = width; k < 80; k++){
        terrain[endY][k] = '#';
    }

    //builds a center along the vertical road 
    // int centerX, centerY;
    // if(endX < firstX){
    //     centerX = (rand() % firstX) + endX + 1;
    // }else{
    //     centerX =  (rand() % endX) + firstX + 1;
    // }
    // centerY = rand() % 20 + 1;
    // terrain[centerX][centerY] = 'C';
    // printf("%c \n", terrain[centerX][centerY]);
}
//0 is true, 1 is false
int isEmpty(){
    int i, j;
    for(i = 1; i < 20; i++){
        for(j = 1; j < 79; j++){
            char curTerrain = terrain[i][j];
            //printf("Current terrain = '%c' | i = %d j = %d \n", curTerrain, i, j);
            //printf("Compare = %d\n", curTerrain == 'e');
            if(curTerrain == 'e'){
                //printf("i = %d, j = %d, terrain = %c\n", i, j, terrain[i][j]);
                //printMap();
                return 0;
            }
        }
    }
    return 1;
}


int main(int argc, char* argv[]){
    initializeMap();
    buildBorder();
    
    srand(time(NULL));//initializes random generator

    //choose a random character, random row, random col
    while(isEmpty() != 1){
        int randStart = rand();
        int randEnd = rand();
        int regionIndex;
        if(numTrees >= 3){
            regionIndex = rand() % 3;
        }else{
            regionIndex = rand() % 4;
        }

        if(regionIndex == 0){
            numGrass++;
        }else if(regionIndex == 1){
            numWater++;
        }else if(regionIndex == 2){
            numClearing++;
        }else{
            numTrees++;
        }
        //buildRectangles(regionTypes[rand() %3], rand() % 20 + 1, rand() % 78 + 1, rand() % 78 + 1, rand() % 20 + 1);
        buildRectangles(regionTypes[regionIndex], randStart % 20 + 1, randStart % 78 + 1, randEnd % 60 + 20, randEnd % 20 + 1);
    }

    if(numGrass < 2){
        buildRectangles(regionTypes[0], rand() % 20 + 1, rand() % 78 + 1, rand() % 78 + 1, rand() % 20 + 1);
    }

    if(numWater < 1){
        buildRectangles(regionTypes[1], rand() % 20 + 1, rand() % 78 + 1, rand() % 78 + 1, rand() % 20 + 1);
    }

    if(numClearing < 2){
        buildRectangles(regionTypes[2], rand() % 20 + 1, rand() % 78 + 1, rand() % 78 + 1, rand() % 20 + 1);
    }
    buildRoads();
    printMap();
    return 0;
}