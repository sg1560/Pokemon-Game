#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include <endian.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>

#include "heap.h"

#define malloc(size) ({          \
  void *_tmp;                    \
  assert((_tmp = malloc(size))); \
  _tmp;                          \
})
//Contains a pointer to heap node
//Contains a 1-D array pos of size 2; contains x and y coor of array cell location (row and col)
//Contains a 1-D array from of size 2
//contains a cost int
typedef struct path {
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
} path_t;

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef uint8_t pair_t[num_dims];

#define MAP_X              80
#define MAP_Y              21
#define MIN_TREES          10
#define MIN_BOULDERS       10
#define TREE_PROB          95
#define BOULDER_PROB       95

#define WORLD_X 401
#define WORLD_Y 401

#define mappair(pair) (m->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (m->map[y][x])
#define heightpair(pair) (m->height[pair[dim_y]][pair[dim_x]])
#define heightxy(x, y) (m->height[y][x])

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_boulder,
  ter_tree,
  ter_path,
  ter_mart,
  ter_center,
  ter_grass,
  ter_clearing,
  ter_mountain,
  ter_forest,
  ter_water, 
  ter_pc
} terrain_type_t;

typedef struct map {
  terrain_type_t map[MAP_Y][MAP_X];
  uint8_t height[MAP_Y][MAP_X];
  uint8_t n, s, e, w;
} map_t;

typedef struct playerChar{
  map_t mapLocation;
  int x, y;
}playerChar;

//Creates the world which is 401x401 maps
map_t *world [WORLD_X][WORLD_Y]; 
//Creates a player character
playerChar pc; 
//creates the cost map
int hikerCostMap [MAP_Y][MAP_X];

void initializeWorld(){
  int i, j;
  for(i = 0; i < WORLD_Y; i++){
    for(j = 0; j < WORLD_X; j++){
      world[i][j] = NULL;
    }
  }
}
typedef struct queue_node {
  int x, y;
  struct queue_node *next;
} queue_node_t;

static int32_t path_cmp(const void *key, const void *with) {
  return ((path_t *) key)->cost - ((path_t *) with)->cost;
}
int getCost(int yCoor, int xCoor, map_t *m){
  switch (m->map[yCoor][xCoor]){
    case ter_path:
      return 10;
    case ter_mart:
      return 50;
    case ter_center:
      return 50;
    case ter_grass:
      return 15;
    case ter_clearing:
      return 10;
    case ter_mountain:
      return 15;
    case ter_forest:
      return 15;
    case ter_water:
      return INT_MAX;
    case ter_tree:
      return INT_MAX;
    case ter_boulder:
      return INT_MAX;
    case ter_debug:
      return INT_MAX;
    case ter_pc:
      return INT_MAX;
  }
}

void printHikerMap(path_t hikerM[MAP_Y][MAP_X]){
  for(int y = 0; y < MAP_Y; y++){
    for(int x = 0; x < MAP_X; x++){
      printf("%d", hikerM[y][x].cost);
    }
    printf("\n");
  }
}

static int32_t edge_penalty(uint8_t x, uint8_t y)
{
  return (x == 1 || y == 1 || x == MAP_X - 2 || y == MAP_Y - 2) ? 2 : 1;
}

static void dijkstra_path(map_t *m, pair_t from, pair_t to)
{
  //path is a static variable; it's a 2D array that contains path_t
  //*p is a pointer to type path_t; 
  static path_t path[MAP_Y][MAP_X], *p;
  static uint32_t initialized = 0;
  //h is basically a heap; only exists within this function
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < MAP_Y; y++) {
      for (x = 0; x < MAP_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  //every cell in path is of type path_t; cost is set to infinity
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }
  //COST OF SOURCE NODE IS 0
  //path is a 2D array; from is type pair which is a 1D array of size 2
  //from contains the x and y coordinates of the starting location/cell/source node; from[1] contains y coor, from[0] is x coor
  //path[ycoor][xcoor].cost = 0;
  path[from[dim_y]][from[dim_x]].cost = 0;

  //creates empty heap where we pass our own comparator
  heap_init(&h, path_cmp, NULL);

  //puts everything in heap
  //this for loop excludes the borders by starting at 1 and ending a dimension earlier
  for (y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      //the heap node of every path_t cell is set to a pointer heap node
      //this pointer heap node contains data, which is the address of the path cell; this heap node is inserted in heap
      //so basically, every cell's heap node is in the heap as well, and the heap node contains the address of cell
      path[y][x].hn = heap_insert(&h, &path[y][x]);
    }
  }

  //removes the root of the heap; p is a pointer to path_t;
  //p is a pointer which holds the datum which is address of cell (which is of type path_t)
  while ((p = heap_remove_min(&h))) {
    //dereferences pointer p to get access to cell, sets its heap node to null
    p->hn = NULL;

    //IGNORE THIS STATEMENT
    if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x]) {
      for (x = to[dim_x], y = to[dim_y];
           (x != from[dim_x]) || (y != from[dim_y]);
           p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y]) {
        mapxy(x, y) = ter_path;
        heightxy(x, y) = 0;
      }
      heap_delete(&h);
      return;
    }
    
    //4 if statements
    //first part gets the path_t stored in neighboring cell of current cell
    //if neighboring cell's heap node is not null
    //AND the neighboring cell's cost is greater than our current cost plus the weight of our edge
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1)))) {
            //update neighboring cell cost to current cell cost plus weight of edge
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x], p->pos[dim_y] - 1));
         //DISREGARD BECAUSE ORANGE ARROWS
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      //put neighboring cell's heap node back in heap
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y])))) {
      path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x] - 1, p->pos[dim_y]));
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y])))) {
      path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x] + 1, p->pos[dim_y]));
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos)) *
          edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1)))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)) *
         edge_penalty(p->pos[dim_x], p->pos[dim_y] + 1));
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }
  }
}

static void build_hiker_map (pair_t from, map_t *map){
  //path is a static variable; it's a 2D array that contains path_t
  //*p is a pointer to type path_t; 
  static path_t hikerMap[MAP_Y][MAP_X], *p;
  static uint32_t initialized = 0;

  //Declares a heap to be used, only exists in this function
  heap_t h;

  //Sets every path cell in hikerMap to hold ycoor and xcoor
  if (!initialized) {
    for (int y = 0; y < MAP_Y; y++) {
      for (int x = 0; x < MAP_X; x++) {
        hikerMap[y][x].pos[dim_y] = y;
        hikerMap[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }

  //sets every path cell's cost to infinity
  for (int y = 0; y < MAP_Y; y++) {
    for (int x = 0; x < MAP_X; x++) {
      hikerMap[y][x].cost = INT_MAX;
    }
  }
  //Sets the cost of source node (pc) to 0
  hikerMap[from[dim_y]][from[dim_x]].cost = 0;

  //creates empty heap; REFACTORING POSSIBLE
  heap_init(&h, path_cmp, NULL);

  //puts everything in heap
  //this for loop excludes the borders because they are impassable; therefore, cost of border is never updated
  //MAY NEED REFACTORING
  for (int y = 1; y < MAP_Y - 1; y++) {
    for (int x = 1; x < MAP_X - 1; x++) {
      //the heap node of every path_t cell is set to a pointer heap node
      //this pointer heap node contains data, which is the address of the path cell; this heap node is inserted in heap
      //so basically, every cell's heap node is in the heap as well, and the heap node contains the address of cell
      hikerMap[y][x].hn = heap_insert(&h, &hikerMap[y][x]);
    }
  }
  //while heap isn't empty; removes root of heap, which is min 
  while ((p = heap_remove_min(&h))) {
    //dereferences pointer p to get access to cell, sets its heap node to null
    p->hn = NULL;

  //8 if statements
    //first if statement: neighboring cell is exactly one above current
    //first part gets the path_t stored in neighboring cell of current cell
    //if neighboring cell's heap node is not null (if it's in our heap)
    //AND the neighboring cell's cost is greater than our current cost plus the weight of our edge
    if ((hikerMap[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (hikerMap[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         ((p->cost + getCost((p -> pos[dim_y] - 1), p->pos[dim_x], map))))) {
        //update neighboring cell cost to current cell cost plus weight of edge
        hikerMap[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        ((p->cost + getCost((p -> pos[dim_y] - 1), p->pos[dim_x], map)));
      //put neighboring cell's heap node back in heap
      heap_decrease_key_no_replace(&h, hikerMap[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }

  }

  heap_delete(&h);
  printHikerMap(hikerMap);
}


static int build_paths(map_t *m)
{
  pair_t from, to;

  from[dim_x] = 1;
  to[dim_x] = MAP_X - 2;
  from[dim_y] = m->w;
  to[dim_y] = m->e;

  dijkstra_path(m, from, to);

  from[dim_y] = 1;
  to[dim_y] = MAP_Y - 2;
  from[dim_x] = m->n;
  to[dim_x] = m->s;

  dijkstra_path(m, from, to);

  return 0;
}

static int gaussian[5][5] = {
  {  1,  4,  7,  4,  1 },
  {  4, 16, 26, 16,  4 },
  {  7, 26, 41, 26,  7 },
  {  4, 16, 26, 16,  4 },
  {  1,  4,  7,  4,  1 }
};
static int smooth_height(map_t *m)
{
  int32_t i, x, y;
  int32_t s, t, p, q;
  queue_node_t *head, *tail, *tmp;
  /*  FILE *out;*/
  uint8_t height[MAP_Y][MAP_X];

  memset(&height, 0, sizeof (height));

  /* Seed with some values */
  for (i = 1; i < 255; i += 20) {
    do {
      x = rand() % MAP_X;
      y = rand() % MAP_Y;
    } while (height[y][x]);
    height[y][x] = i;
    if (i == 1) {
      head = tail = malloc(sizeof (*tail));
    } else {
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  /*
  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&height, sizeof (height), 1, out);
  fclose(out);
  */
  
  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    i = height[y][x];

    if (x - 1 >= 0 && y - 1 >= 0 && !height[y - 1][x - 1]) {
      height[y - 1][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y - 1;
    }
    if (x - 1 >= 0 && !height[y][x - 1]) {
      height[y][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y;
    }
    if (x - 1 >= 0 && y + 1 < MAP_Y && !height[y + 1][x - 1]) {
      height[y + 1][x - 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x - 1;
      tail->y = y + 1;
    }
    if (y - 1 >= 0 && !height[y - 1][x]) {
      height[y - 1][x] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y - 1;
    }
    if (y + 1 < MAP_Y && !height[y + 1][x]) {
      height[y + 1][x] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x;
      tail->y = y + 1;
    }
    if (x + 1 < MAP_X && y - 1 >= 0 && !height[y - 1][x + 1]) {
      height[y - 1][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y - 1;
    }
    if (x + 1 < MAP_X && !height[y][x + 1]) {
      height[y][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y;
    }
    if (x + 1 < MAP_X && y + 1 < MAP_Y && !height[y + 1][x + 1]) {
      height[y + 1][x + 1] = i;
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
      tail->next = NULL;
      tail->x = x + 1;
      tail->y = y + 1;
    }

    tmp = head;
    head = head->next;
    free(tmp);
  }

  /* And smooth it a bit with a gaussian convolution */
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < MAP_Y &&
              x + (q - 2) >= 0 && x + (q - 2) < MAP_X) {
            s += gaussian[p][q];
            t += height[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      m->height[y][x] = t / s;
    }
  }
  /* Let's do it again, until it's smooth like Kenny G. */
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      for (s = t = p = 0; p < 5; p++) {
        for (q = 0; q < 5; q++) {
          if (y + (p - 2) >= 0 && y + (p - 2) < MAP_Y &&
              x + (q - 2) >= 0 && x + (q - 2) < MAP_X) {
            s += gaussian[p][q];
            t += height[y + (p - 2)][x + (q - 2)] * gaussian[p][q];
          }
        }
      }
      m->height[y][x] = t / s;
    }
  }

  /*
  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&height, sizeof (height), 1, out);
  fclose(out);

  out = fopen("smoothed.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->height, sizeof (m->height), 1, out);
  fclose(out);
  */

  return 0;
}

static void find_building_location(map_t *m, pair_t p)
{
  do {
    p[dim_x] = rand() % (MAP_X - 3) + 1;
    p[dim_y] = rand() % (MAP_Y - 3) + 1;

    if ((((mapxy(p[dim_x] - 1, p[dim_y]    ) == ter_path)     &&
          (mapxy(p[dim_x] - 1, p[dim_y] + 1) == ter_path))    ||
         ((mapxy(p[dim_x] + 2, p[dim_y]    ) == ter_path)     &&
          (mapxy(p[dim_x] + 2, p[dim_y] + 1) == ter_path))    ||
         ((mapxy(p[dim_x]    , p[dim_y] - 1) == ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] - 1) == ter_path))    ||
         ((mapxy(p[dim_x]    , p[dim_y] + 2) == ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 2) == ter_path)))   &&
        (((mapxy(p[dim_x]    , p[dim_y]    ) != ter_mart)     &&
          (mapxy(p[dim_x]    , p[dim_y]    ) != ter_center)   &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_mart)     &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_center)   &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_mart)     &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_center)   &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_mart)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_center))) &&
        (((mapxy(p[dim_x]    , p[dim_y]    ) != ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y]    ) != ter_path)     &&
          (mapxy(p[dim_x]    , p[dim_y] + 1) != ter_path)     &&
          (mapxy(p[dim_x] + 1, p[dim_y] + 1) != ter_path)))) {
          break;
    }
  } while (1);
}

static int place_pokemart(map_t *m)
{
  pair_t p;

  find_building_location(m, p);

  mapxy(p[dim_x]    , p[dim_y]    ) = ter_mart;
  mapxy(p[dim_x] + 1, p[dim_y]    ) = ter_mart;
  mapxy(p[dim_x]    , p[dim_y] + 1) = ter_mart;
  mapxy(p[dim_x] + 1, p[dim_y] + 1) = ter_mart;

  return 0;
}

static int place_center(map_t *m)
{  pair_t p;

  find_building_location(m, p);

  mapxy(p[dim_x]    , p[dim_y]    ) = ter_center;
  mapxy(p[dim_x] + 1, p[dim_y]    ) = ter_center;
  mapxy(p[dim_x]    , p[dim_y] + 1) = ter_center;
  mapxy(p[dim_x] + 1, p[dim_y] + 1) = ter_center;

  return 0;
}

/* Chooses tree or boulder for border cell.  Choice is biased by dominance *
 * of neighboring cells.                                                   */
static terrain_type_t border_type(map_t *m, int32_t x, int32_t y)
{
  int32_t p, q;
  int32_t r, t;
  int32_t miny, minx, maxy, maxx;
  
  r = t = 0;
  
  miny = y - 1 >= 0 ? y - 1 : 0;
  maxy = y + 1 <= MAP_Y ? y + 1: MAP_Y;
  minx = x - 1 >= 0 ? x - 1 : 0;
  maxx = x + 1 <= MAP_X ? x + 1: MAP_X;

  for (q = miny; q < maxy; q++) {
    for (p = minx; p < maxx; p++) {
      if (q != y || p != x) {
        if (m->map[q][p] == ter_mountain ||
            m->map[q][p] == ter_boulder) {
          r++;
        } else if (m->map[q][p] == ter_forest ||
                   m->map[q][p] == ter_tree) {
          t++;
        }
      }
    }
  }
  
  if (t == r) {
    return rand() & 1 ? ter_boulder : ter_tree;
  } else if (t > r) {
    if (rand() % 10) {
      return ter_tree;
    } else {
      return ter_boulder;
    }
  } else {
    if (rand() % 10) {
      return ter_boulder;
    } else {
      return ter_tree;
    }
  }
}

static int map_terrain(map_t *m, uint8_t n, uint8_t s, uint8_t e, uint8_t w)
{
  int32_t i, x, y;
  queue_node_t *head, *tail, *tmp;
  //  FILE *out;
  int num_grass, num_clearing, num_mountain, num_forest, num_water, num_total;
  terrain_type_t type;
  int added_current = 0;
  
  num_grass = rand() % 4 + 2;
  num_clearing = rand() % 4 + 2;
  num_mountain = rand() % 2 + 1;
  num_forest = rand() % 2 + 1;
  num_water = rand() % 2 + 1;
  num_total = num_grass + num_clearing + num_mountain + num_forest + num_water;

  memset(&m->map, 0, sizeof (m->map));

  /* Seed with some values */
  for (i = 0; i < num_total; i++) {
    do {
      x = rand() % MAP_X;
      y = rand() % MAP_Y;
    } while (m->map[y][x]);
    if (i == 0) {
      type = ter_grass;
    } else if (i == num_grass) {
      type = ter_clearing;
    } else if (i == num_grass + num_clearing) {
      type = ter_mountain;
    } else if (i == num_grass + num_clearing + num_mountain) {
      type = ter_forest;
    } else if (i == num_grass + num_clearing + num_mountain + num_forest) {
      type = ter_water;
    }
    m->map[y][x] = type;
    if (i == 0) {
      head = tail = malloc(sizeof (*tail));
    } else {
      tail->next = malloc(sizeof (*tail));
      tail = tail->next;
    }
    tail->next = NULL;
    tail->x = x;
    tail->y = y;
  }

  /*
  out = fopen("seeded.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->map, sizeof (m->map), 1, out);
  fclose(out);
  */
  
  /* Diffuse the vaules to fill the space */
  while (head) {
    x = head->x;
    y = head->y;
    i = m->map[y][x];
    
    if (x - 1 >= 0 && !m->map[y][x - 1]) {
      if ((rand() % 100) < 80) {
        m->map[y][x - 1] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x - 1;
        tail->y = y;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (y - 1 >= 0 && !m->map[y - 1][x]) {
      if ((rand() % 100) < 20) {
        m->map[y - 1][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y - 1;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (y + 1 < MAP_Y && !m->map[y + 1][x]) {
      if ((rand() % 100) < 20) {
        m->map[y + 1][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y + 1;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    if (x + 1 < MAP_X && !m->map[y][x + 1]) {
      if ((rand() % 100) < 80) {
        m->map[y][x + 1] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x + 1;
        tail->y = y;
      } else if (!added_current) {
        added_current = 1;
        m->map[y][x] = i;
        tail->next = malloc(sizeof (*tail));
        tail = tail->next;
        tail->next = NULL;
        tail->x = x;
        tail->y = y;
      }
    }

    added_current = 0;
    tmp = head;
    head = head->next;
    free(tmp);
  }

  /*
  out = fopen("diffused.pgm", "w");
  fprintf(out, "P5\n%u %u\n255\n", MAP_X, MAP_Y);
  fwrite(&m->map, sizeof (m->map), 1, out);
  fclose(out);
  */
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      if (y == 0 || y == MAP_Y - 1 ||
          x == 0 || x == MAP_X - 1) {
        mapxy(x, y) = type = border_type(m, x, y);
      }
    }
  }

  m->n = n;
  m->s = s;
  m->e = e;
  m->w = w;

  mapxy(n,         0        ) = ter_path;
  mapxy(n,         1        ) = ter_path;
  mapxy(s,         MAP_Y - 1) = ter_path;
  mapxy(s,         MAP_Y - 2) = ter_path;
  mapxy(0,         w        ) = ter_path;
  mapxy(1,         w        ) = ter_path;
  mapxy(MAP_X - 1, e        ) = ter_path;
  mapxy(MAP_X - 2, e        ) = ter_path;

  return 0;
}

static int place_boulders(map_t *m)
{
  int i;
  int x, y;

  for (i = 0; i < MIN_BOULDERS || rand() % 100 < BOULDER_PROB; i++) {
    y = rand() % (MAP_Y - 2) + 1;
    x = rand() % (MAP_X - 2) + 1;
    if (m->map[y][x] != ter_forest && m->map[y][x] != ter_path) {
      m->map[y][x] = ter_boulder;
    }
  }

  return 0;
}

static int place_trees(map_t *m)
{
  int i;
  int x, y;
  
  for (i = 0; i < MIN_TREES || rand() % 100 < TREE_PROB; i++) {
    y = rand() % (MAP_Y - 2) + 1;
    x = rand() % (MAP_X - 2) + 1;
    if (m->map[y][x] != ter_mountain &&
        m->map[y][x] != ter_path     &&
        m->map[y][x] != ter_water) {
      m->map[y][x] = ter_tree;
    }
  }

  return 0;
}
//0 is true, 1 is false
int inBounds(int xCoor, int yCoor){
    if(xCoor < -200 || xCoor > 200 || yCoor < -200 || yCoor > 200){
      printf("Cannot go to this location");
      return 1;
    }
    return 0;
}
static int new_map(map_t *m)

{
  smooth_height(m);
  map_terrain(m,
              1 + rand() % (MAP_X - 2), 1 + rand() % (MAP_X - 2),
              1 + rand() % (MAP_Y - 2), 1 + rand() % (MAP_Y - 2));
  place_boulders(m);
  place_trees(m);
  build_paths(m);
  place_pokemart(m);
  place_center(m);
  
  return 0;
}

int generateMap(int row, int col, int n, int s, int e, int w){
    map_t *new;
    new = (map_t*) (malloc(sizeof(map_t)));
    smooth_height(new);
    map_terrain(new, n, s, e, w);
    place_boulders(new);
    place_trees(new);
    build_paths(new);
    // //implementing the probability function:
    int distance = abs(row - 200) + abs(col - 200);
    float prob;
    if(distance >= 200){
      prob = 5.0;
    }else{
      prob = (((-45 * distance) / 200.0) + 50); 
    }
    // //gives a random number between 1 and 100
    int number = (rand() % 100) + 1; 
    if(number <= prob && number > 1){
      place_pokemart(new);
      place_center(new);
    }
    //bounds checking if you reach edge of world
    if(row == 0){
      new ->map[0][n] = ter_boulder;
    }
    if(row == 400){
      new -> map[20][s] = ter_boulder;
    }
    if(col == 0){
      new -> map[w][0] = ter_boulder;
    }
    if(col == 400){
      new -> map[e][79] = ter_boulder;
    }
    world[row][col] = new;
    return 0;
}
static void print_map(map_t *m, int xCoor, int yCoor)
{
  int x, y;
  int default_reached = 0;
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      switch (m->map[y][x]) {
      case ter_boulder:
      case ter_mountain:
        putchar('%');
        break;
      case ter_tree:
      case ter_forest:
        putchar('^');
        break;
      case ter_path:
        putchar('#');
        break;
      case ter_mart:
        putchar('M');
        break;
      case ter_center:
        putchar('C');
        break;
      case ter_grass:
        putchar(':');
        break;
      case ter_clearing:
        putchar('.');
        break;
      case ter_water:
        putchar('~');
        break;
      case ter_pc:
        putchar('@');
      default:
        default_reached = 1;
        break;
      }
    }
    putchar('\n');
  }

  if (default_reached) {
    fprintf(stderr, "Default reached in %s\n", __FUNCTION__);
  }
  printf("Coordinates: %d %d \n", xCoor, yCoor);
}

void freeWorld(){
  for(int i = 0; i < 401; i++){
    for(int j = 0; j < 401; j++){
      if(world[i][j] != NULL){
        if(i == 200 && j == 200){
          continue;
        }
        map_t *map = world[i][j];
        free(map);
      }
    }
  }
}
int main(int argc, char *argv[])
{
  map_t d;
  struct timeval tv;
  uint32_t seed;

  if (argc == 2) {
    seed = atoi(argv[1]);
  } else {
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Using seed: %u\n", seed);
  srand(seed);
  
  //initalizes world with center map at 200, 200
  initializeWorld();
  //generates the map
  new_map(&d);
  //sets the map of pc to be current map
  pc.mapLocation = d;
  int x = rand() & 80;
  int y = rand() % 21;
  pc.x = x;
  pc.y = y;
  d.map[y][x] = ter_pc;
  //sets pointer to the ADDRESS of the original/default map
  world[200][200] = &d;
  //logical coordinates
  int currX = 0; 
  int currY = 0; 
  //array coordinates
  int arrX = 200;
  int arrY = 200;
  //This is for case 'f'
  int getX;
  int getY;

  // print_map(&d);
  print_map(world[200][200], 0, 0);

  //TESING DIJKSTRA
  pair_t from;
  from[dim_y] = pc.y;
  from[dim_x] = pc.x;
  build_hiker_map(from, &d);


  int truth = 0; 
  char c; 
  printf("n, s, e, w, f, q \n");

  printf("xcoor: %d ycoor: %d", pc.x, pc.y);
  while((c = getchar()) && truth == 0){
    //printf("n, s, e, w, f, q \n");
    //current map we're in and their gates
    map_t *curr = world[arrY][arrX];
    int northGate = curr -> n;
    int southGate = curr -> s;
    int eastGate = curr -> e;
    int westGate = curr -> w;
    //if statement to address user input
    switch(c){
      case 'q':
        truth = 1;
        freeWorld();
        break;
      //north
      case 'n':
        if(inBounds(currX, currY - 1) == 0){
          currY--;
          arrY++;
          if(world[arrY][arrX] == NULL){
            generateMap(arrY, arrX, 1 + rand() % (MAP_X - 2), northGate,  1 + rand() % (MAP_Y - 2), 1 + rand() % (MAP_Y - 2));
          }
          print_map(world[arrY][arrX], currX, currY);
        }
        break;
      case 's':
        if(inBounds(currX, currY + 1) == 0){
          currY++;
          arrY--;
          if(world[arrY][arrX] == NULL){
            generateMap(arrY, arrX, southGate, 1 + rand() % (MAP_X - 2),  1 + rand() % (MAP_Y - 2), 1 + rand() % (MAP_Y - 2));
          }
          print_map(world[arrY][arrX], currX, currY);
        }
        break;
      case 'e':
        if(inBounds(currX + 1, currY) == 0){
            currX++;
            arrX++;
            if(world[arrY][arrX] == NULL){
            generateMap(arrY, arrX, 1 + rand() % (MAP_X - 2), 1 + rand() % (MAP_X - 2),  1 + rand() % (MAP_Y - 2), eastGate);
          }
          print_map(world[arrY][arrX], currX, currY);
        }
        break;
      case 'w':
        if(inBounds(currX - 1, currY) == 0){
          currX--;
          arrX--;
          if(world[arrY][arrX] == NULL){
             generateMap(arrY, arrX, 1 + rand() % (MAP_X - 2), 1 + rand() % (MAP_X - 2),  westGate, 1 + rand() % (MAP_Y - 2));
          }
          print_map(world[arrY][arrX], currX, currY);
        }
         break;
      case 'f':
        scanf("%d%d", &getX, &getY);
        if(inBounds(getX, getY) == 0){
          //set arrx and arry values
          int diffX = (currX - getX) * -1;
          int diffY = (currY - getY);
          arrX += diffX;
          arrY += diffY;
          currX = getX;
          currY = getY;
          if(world[arrY][arrX] == NULL){
            generateMap(arrY, arrX, 1 + rand() % (MAP_X - 2), 1 + rand() % (MAP_X - 2), 1 + rand() % (MAP_Y - 2), 1 + rand() % (MAP_Y - 2));
          }
          print_map(world[arrY][arrX], currX, currY);
        }
        break;
      }
    }
  return 0;
  }