#include <stdio.h>
#include <stdlib.h>
#define MAX_BOARD = 5;
//defines a board array of size 25
int pathNum = 0;
int board[5][5];
int possibleX[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int possibleY[8] = {1, 2, 2, 1, -1, -2, -2, -1};

void initialBoard(){
	int num = 1;
	int i, j;
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			board[i][j] = num;
			num++;
		}
	}
}
//0 is for true, 1 is for false
int isSafe(int x, int y){
	if(x >= 0 && x <= 4 && y >= 0 && y <= 4){
		return 0;
	}else{
		return 1;
	}
}
void printPath(int finalPath[25]){
  		pathNum++;
		int i;
		for(i = 0; i < 25; i++){
			printf("%d,", finalPath[i]);
		}
		printf("\n");
	}

void DFSFromRec(int x, int y, int path [25], int visited [5][5]){
	
	if(path[24] != -1){
		printPath(path);
		return;
	}
	int i, k, j, row, col;
	for(i = 0; i < 8; i++){
		int nextX = x + possibleX[i];
		int nextY = y + possibleY[i];
		if(isSafe(nextX, nextY) == 0 && visited[nextX][nextY] == 1){
			//make a deep copy of the path
			int newPath [25];
			for(k = 0; k <= 24; k++){
				newPath[k] = path[k];
			}
			//updates the value of the new path
			for(j = 0; j <= 24; j++){
				if(newPath[j] == -1){
					newPath[j] = board[nextX][nextY];
					break;
				}
			}
			//makes a deep copy of visited
			int newVisited [5][5];
			for(row = 0; row < 5; row++){
				for(col = 0; col < 5; col++){
					newVisited[row][col] = visited[row][col];
				}
			}
			newVisited[nextX][nextY] = 0;
			DFSFromRec(nextX, nextY, newPath, newVisited);
		}
	}
}
void DFSFrom(){
	int path[25];
	int visited[5][5];
	int x, y, i, j, k;
	for(x = 0; x < 5; x++){
		for(y = 0; y < 5; y++){
			for(i = 0; i < 5; i++){
				for(j = 0; j < 5; j++){
					visited[i][j] = 1;
				}
			}
			for(k = 0; k < 25; k++){
				path[k] = -1;
			}
			visited[x][y] = 0;
			path[0] = board[x][y];
			DFSFromRec(x, y, path, visited);
		}
	}
}

	int main(int argc, char *argv[]){
		initialBoard();
		DFSFrom();
		return 0;
	}