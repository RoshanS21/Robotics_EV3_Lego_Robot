/*
* Compile:
* need to link math library
* gcc -o main test2.c -lm
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define X_WORLD 3.66 // IN METERS
#define Y_WORLD 3.05 // IN METERS
#define MOTOR_SPEED 20
#define SIZE 50 // maximum world size

// ----------------------------------------------------

#define MAX_OBSTACLES   25                      /* maximum number of obstacles */
int num_obstacles = 20;                         /* number of obstacles         */
double obstacle[MAX_OBSTACLES][2] =             /* obstacle locations          */
{{0.915, 0.305},{0.915, 0.61},{0.915, 0.915},{0.915, 1.219},
 {0.915, 1.524},{0.915, 1.829}, {1.829, 1.524},{1.829, 1.829},
 {1.829, 2.13},{1.829, 2.44},{1.829, 2.743},{1.829, 0.61},
 {2.13, 0.61}, {2.44, 0.61}, {2.743, 0.61},{2.743, 0.305},
 {2.743, 0.915},{2.743, 1.22},{2.743, 1.524},{2.743, 1.83},
 {-1,-1},{-1,-1},{-1,-1}, {-1,-1},{-1,-1}};
 
double start[2] = {0.305, 0.61};          /* start location              */
double goal[2] = {3.356, 1.524};           /* goal location               */

// ------------------------------------------------------

int x_world, y_world;
int x_start, y_start;
int x_goal, y_goal;
int v_goal = 10000;
char path[200];
int obstacles[MAX_OBSTACLES][2];

int world[SIZE][SIZE], visited[SIZE][SIZE];

int meterToFeet(double m){
    return ceil(m / 0.305);
}

int manhattanDistance(int r, int c){
    return abs(c-x_start) + abs(r-y_start);
}

// display world
void displayWorld(){
	int r,c;
	printf("\nWorld: Length: %d, Height: %d)\n", x_world, y_world);
    printf("Start: (%d, %d)\n", x_start, y_start);
    printf("Goal: (%d, %d)\n\n", x_goal, y_goal);
    printf("World Grid\n");
    printf("\t");
    for(c=1; c<=x_world; c++){
        printf("c%d\t",c);
    }
    printf("\n");
    for(r=1; r<=y_world; r++)
    {
        printf("r%d\t", r);
		for(c=1; c<=x_world; c++)
        {
			printf("%d\t", world[r][c]);
		}
		printf("\n\n");
	}
}

void displayVisited()
{
	printf("\n\nSolution\n");
	printf("Follow the 1's.\n\n");
    int r,c;
    printf("\t");
    for(c=1; c<=x_world; c++){
        printf("c%d\t",c);
    }
    for(r=1; r<=y_world; r++)
    {
        printf("r%d\t", r);
        for(c=1; c<=x_world; c++)
        {
            printf("%d\t",visited[r][c]);
        }
        printf("\n\n");
    }
}

void displayStats(){
    printf("\nWorld: Length: %d, Height: %d)\n", x_world, y_world);
    printf("Start: (%d, %d)\n", x_start, y_start);
    printf("Goal: (%d, %d)\n\n", x_goal, y_goal);
    printf("Obstacles: \n");
    int r;
    for(r=0; r<MAX_OBSTACLES; r++){
        printf("(%d, %d)\n", obstacles[r][0], obstacles[r][1]);
    }
    printf("\n");
}

int solveworld(int r, int c, int counter, char ch)
{
    path[counter] = ch;
    ++counter;
    // if destination is reached, world is solved
    if(world[r][c] == v_goal)
    {
        visited[r][c] = 1;
        path[counter] = '\0';
        return 1;
    }
	
    // checking if we can visit in this cell or not
    // and visited[r][c] == 0 is making sure that the cell is not already visited
    // world[r][c] >= counter is making sure that the cell is not blocked
	// since counter is never < 0
	// blocked states are -1 or lower
    if(r>0 && c>0 && r<y_world && c<=x_world && visited[r][c] == 0 && world[r][c] >= counter)
    {
        // if safe to visit then visit the cell
        visited[r][c] = 1;
		
        // going right
        if(solveworld(r, c+1, counter, 'R'))
            return 1;
        // going up
        if(solveworld(r-1, c, counter, 'U'))
            return 1;
        // going down
        if(solveworld(r+1, c, counter, 'D'))
            return 1;
        // going left
        if(solveworld(r, c-1, counter, 'L'))
            return 1;
        // backtracking
        visited[r][c] = 0;
		return 0;
    }
	--counter;
    return 0;
}


int main(void){
    int r,c,i,j;
    x_world = meterToFeet(X_WORLD);
	y_world = meterToFeet(Y_WORLD);

    x_start = meterToFeet(start[0]);
	y_start = y_world - meterToFeet(start[1]);

    x_goal = meterToFeet(goal[0]) - 1;
	y_goal = y_world - meterToFeet(goal[1]);

    for(r=0; r<MAX_OBSTACLES; r++){
        if( obstacle[r][0] < 0 ){
            obstacles[r][0] = obstacle[r][0];
            obstacles[r][1] = obstacle[r][1];
            continue;
        }
        obstacles[r][0] = meterToFeet(obstacle[r][0]);
        obstacles[r][1] = y_world - meterToFeet(obstacle[r][1]);
    }
    
    // initialize world grid and visited grid
    for(r=0; r<SIZE; r++){
		for(c=0; c<SIZE; c++){
			world[r][c] = 0;
			visited[r][c] = 0;
		}
	}

    world[y_start][x_start] = 1;
    world[y_goal][x_goal] = v_goal;

    for(r=0; r<num_obstacles; r++){
        i = obstacles[r][1]; // row is y-axis
        j = obstacles[r][0]; // column is x-axis
        world[i][j] = -1;
    }

    // path planning
    // Manhattan Distance
    // A* algorithm
    // heuristic has been added to the manhattan distance
    for(r=1; r<=y_world; r++){
        for(c=1; c<=x_world; c++){
            if(world[r][c] == -1) continue;
            if(world[r][c] == v_goal) continue;
            if(world[r][c] == 1) continue;
            if(r == y_world) continue;
            int dist = abs(r-y_goal) + abs(c-x_goal);
            int heuristic = ceil(1000/dist);
            world[r][c] = manhattanDistance(r,c) + heuristic;
        }
    }

    int counter = 0;
    char ch = 'S';	

	if (solveworld(y_start, x_start, counter, ch)){
        displayStats();
        displayWorld();
        displayVisited();
        printf("\nPath = %s\n", path);
    }
    else{
        printf("No solution\n");
    }
    
    return 0;
}