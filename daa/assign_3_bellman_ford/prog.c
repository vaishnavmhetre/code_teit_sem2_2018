/*
 * Author - Vaishnav Mhetre
 * Created at - Sunday, 10th April - 3:37 am
 * Bellman Ford Algorithm - Find Shortest path from
 * some source to destination in a Graph
 */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50          // Max array (haystack) size limit - any array

typedef struct Vertex {     // Vertex in Graph
    char name[MAX];
} Vertex;

typedef struct Edge {       // Edge in Graph
    Vertex *src, *dest;
    int weight;
} Edge;

typedef struct Graph {      // Complete Graph
    int V, E;               // V - No. of Vertices, E - No. of Edges (in Graph)
    Edge **edges;           // Array of pointer to edges (can allocate dynamically)
    Vertex **vertices;      // Array of pointer to vertices (can allocate dynamically)
} Graph;

typedef struct Map {        // Resultant mapping of distances and parents
    int *distances;         // Distance from Parent Vertex
    Vertex **parents;       // Array of Parent Vertices of all Vertices (Parent - Closest way possible to approach from some source Vertex)
} Map;

/*
 * Get Index of Some Vertex stored in all Vertices of Graph
 *
 * @function int getVertexIndex
 * @param Vertex **vertices - Haystack
 * @param Vertex *vertex - Needle
 * @param int vertexCount - Amount of elements in Haystack
 */

int getVertexIndex(Vertex **vertices, Vertex *vertex, int vertexCount) {

    int i;

    for (i = 0; i < vertexCount; ++i) {         // Iterate through all vertices in Haystack
        if (vertices[i] == vertex)              // If needle vertex address same as in haystack vertex
            return i;                           // Return Index
    }
    if (i >= vertexCount)                       // Fallback - Not found, hence iterator 'i' goes out of bound
        return -1;                              // Hence, return -1 - invalid index
}

/*
 * Find Vertex using Data of Vertex
 *
 * @function Vertex *getVertexByName
 * @param Vertex **vertices - Haystack
 * @param char name[MAX] - Needle
 * @param int vertexCount - Amount of elements in Haystack
 */

Vertex *getVertexByName(Vertex **vertices, char name[MAX], int vertexCount) {

    int i;

    for (i = 0; i < vertexCount; ++i) {                 // Iterate through all vertices in Haystack
        if (strcmp(vertices[i]->name, name) == 0)       // If needle data same as haystack vertex data
            return vertices[i];                         // Return Reference to Vertex
    }
    if (i >= vertexCount) {                             // Fallback - Not found, hence iterator 'i' goes out of bound
        return NULL;                                    // Hence, return NULL - No address
    }

}

/*
 * Creates a Graph instance and return reference to it
 *
 * @function Graph *graph
 * @param int V - No. of Vertices
 * @param int E - No. of Edges
 */

Graph *createGraph(int V, int E) {

    Graph *graph = (struct Graph *) malloc(sizeof(struct Graph));               // Create instance of Graph

    graph->V = V;                                                               // Assign total vertex count
    graph->E = E;                                                               // Assign total edges count

    graph->edges = (Edge **) malloc(graph->E * sizeof(Edge *));                 // Create instance of Edges (Amount of edges * size of Edge)
    for (int i = 0; i < E; ++i)
        graph->edges[i] = (Edge *) malloc(graph->E * sizeof(Edge));             // Create instance of Individual empty Edges

    graph->vertices = (Vertex **) malloc(graph->V * sizeof(Vertex *));          // Create instance of Vertices (Amount of vertices * size of Vertex)
    for (int i = 0; i < V; ++i)
        graph->vertices[i] = (Vertex *) malloc(graph->V * sizeof(Vertex));      // Create instance of Individual empty Vertices

    return graph;       // Return reference to Graph instance
}

/*
 * Creates Map instance and returns reference to it
 *
 * @function Map *createMap
 * @param int V - No. of Vertices
 */

Map *createMap(int V) {

    Map *map = (struct Map *) malloc(sizeof(struct Map));           // Create instance of Map

    map->distances = (int *) malloc(V * sizeof(int));               // Instantiate distances to No. of Vertices * size of int instances (array)
    map->parents = (Vertex **) malloc(V * sizeof(Vertex *));        // Instantiate distances to No. of Vertices * size of Vertex instances (array)

    return map;     // Return reference to Map instance
}

/*
 * Display map in Table Plot
 *
 * @function void viewMap
 * @param Map *map
 * @param Graph *map
 */

void viewMap(Map *map, Graph *graph) {

    printf("\n\nvertices:");
    for (int i = 0; i < graph->V; ++i)                 // Display Vertex Names
        printf("\t%s", graph->vertices[i]->name);


    printf("\ndistances:");
    for (int i = 0; i < graph->V; ++i)                 // Display Vertex Distances from Parent
        if(map->distances[i] != INT_MAX)                // If distance not infinity, show
            printf("\t%d", map->distances[i]);
        else                                            // else show Infinity symbol
            printf("\t%c", '∞');


    printf("\nparents:");
    for (int i = 0; i < graph->V; ++i)                 // Display Vertex Parent Names
        printf("\t%s", (map->parents[i]) ? map->parents[i]->name : "-");

}

/*
 * Show Route from particular Source to Destination Vertex
 *
 * @function void viewPath
 * @param Map *map
 * @param Graph *graph
 * @param Vertex *src
 * @param Vertex *dest
 */

void viewPath(Map *map, Graph *graph, Vertex *src, Vertex *dest) {

    int destIndex = getVertexIndex(graph->vertices, dest, graph->V);            // Preload vertex index for further usage

    Vertex *iter = dest;                                                        // Backup destination vertex for iterating

    printf("\n\nPath: %s => %s", src->name, iter->name);                        // Show path source and destination names of vertices
    printf("\nCost: %d", map->distances[destIndex]);                            // Show Cost of path

    printf("\nRoute: ");                                                        // Start route printing

    do {
        printf("%s <= ", iter->name);                                           // Show vertex name
        iter = map->parents[getVertexIndex(graph->vertices, iter, graph->V)];   // Get address stored in Parent of current iterating Vertex from map
    } while (iter != src);                                                      // Iterate until source vertex not reached

    printf("%s", src->name);                                                    // At last, print source vertex data(name)

}

/*
 * Show Paths to all Vertices from a Source Vertex
 *
 * @function void viewAllPaths
 * @param Map *map
 * @param Graph *graph
 * @param Vertex *src
 */

void viewAllPaths(Map *map, Graph *graph, Vertex *src) {

    int srcIndex = getVertexIndex(graph->vertices, src, graph->V);      // Preload vertex index for further usage

    for (int i = 0; i < graph->V; ++i) {                                // Iterate through all Vertices

        if (i == srcIndex)                                              // No use showing path to self, hence ignore and continue
            continue;

        viewPath(map, graph, src, graph->vertices[i]);                  // Until then, show path to the iterating destination vertex

    }

}

/*
 * Initial set up of Map to Infinity and Null Parents
 *
 * @function void initSingleSource
 * @param Map *map
 * @param Graph *graph
 * @param Vertex *src
 */

void initSingleSource(Map *map, Graph *graph, Vertex *src) {

    int srcIndex = getVertexIndex(graph->vertices, src, graph->V);      // Preload vertex index for further usage

    for (int i = 0; i < graph->V; ++i) {                                // For all vertices (actually using their indexes)
        map->distances[i] = INT_MAX;                                    // Set distance to Infinity
        map->parents[i] = NULL;                                         // Set parent to Nobody (NULL)
    }

    map->distances[srcIndex] = 0;                                       // Set self distance to '0'

}

/*
 * Relaxation of Vertices in Map - Finding closest backtrack parent vertex,
 * who can reach us with least distance from some source vertex
 *
 * @function void relax
 * @param Map *map
 * @param Graph *graph
 * @param Edge *edge
 */

void relax(Map *map, Graph *graph, Edge *edge) {

    int uIndex = getVertexIndex(graph->vertices, edge->src, graph->V);      // Get source vertex Index
    int vIndex = getVertexIndex(graph->vertices, edge->dest, graph->V);     // Get destination vertex Index

    if (
            map->distances[vIndex]                                          // If current parent reach distance >
            >
            map->distances[uIndex] + edge->weight                           // Currently iterating source + weight of edge connecting them
            ) {                                                             // Then take new source, as it has lesser distance, else ignore

        map->distances[vIndex] = map->distances[uIndex] + edge->weight;     // Set new distance from new adjacent source vertex
        map->parents[vIndex] = edge->src;                                   // Set new adjacent source vertex as parent

    }
}

/*
 * Bellman Ford - To find shortest path, from
 * some source to destination vertex
 *
 * Returns '0' if negative cycle exists else '1'
 *
 * @function int bellmanFord
 * @param Map *map
 * @param Graph *graph
 * @param Vertex *src
 */

int bellmanFord(Map *map, Graph *graph, Vertex *src) {

    initSingleSource(map, graph, src);              // Set up map for give source vertex

    for (int i = 0; i < graph->V; ++i)             // For all vertices,
        for (int j = 0; j < graph->E; ++j)         // Through edges,
            relax(map, graph, graph->edges[j]);     // Relax the Vertices in Map

    for (int j = 0; j < graph->E; ++j) {
        int uIndex = getVertexIndex(graph->vertices, graph->edges[j]->src, graph->V);       // Preload source vertex Index
        int vIndex = getVertexIndex(graph->vertices, graph->edges[j]->dest, graph->V);      // Preload destination vertex Index

        if (                                                        // Still can find shorter path, means infinite iteration exists
                map->distances[vIndex]                              // Hence, negative cycle exists
                >
                map->distances[uIndex] + graph->edges[j]->weight
                ) {

            return 0;                                               // Hence, return 0, as negative cycle exists
        }
    }
    return 1;                                                       // Else return 1, as no negative cycle exists
}

/*
 * Start of Execution
 */

int main() {

    /*
     * Prerequisites
     */
    int V, E;
    Map *map;
    Graph *graph;
    Vertex *src;
    char srcname[50];
    char destname[50];
    int weight;

    scanf("%d %d", &V, &E);             // Accept No. of Vertices and Edges

    graph = createGraph(V, E);          // Set up Graph
    map = createMap(V);                 // Set up Map

    for (int i = 0; i < V; ++i)
        scanf("%s", graph->vertices[i]->name);      // Accept Vertex Names



    for (int i = 0; i < E; ++i) {                   // Accept and create Edges
        scanf("%s %s %d", srcname, destname, &weight);
        graph->edges[i]->src = getVertexByName(graph->vertices, srcname, graph->V);
        graph->edges[i]->dest = getVertexByName(graph->vertices, destname, graph->V);
        graph->edges[i]->weight = weight;

    }

    src = graph->vertices[0];                       // Currently default source vertex set to 1st vertex

    int status = bellmanFord(map, graph, src);      // Receive status of Bellman Ford Algorithm for given source

    printf("\n%d", status);                         // Print status

    if (status == 1){                               // Is no negative cycle present
        viewMap(map, graph);                        // View Map to vertices from given source
        viewAllPaths(map, graph, src);              // View Paths to all Vertices from given source
    }

    return 0;       // End of line

}

/*
 * INPUT FORMAT
 *
 * (vertex count) (edges count)
 * (vertex name) [(vertex name)...]
 * ((source vertex name) (destination vertex name) (weight)) [((source vertex name) (destination vertex name) (weight))...]
 *
 */

/*
 * OUTPUT FORMAT
 *
 * (bellman ford status)
 *
 * <if status = 1>{
 *      (map){
 *          vertices:   (vertex name)       [(vertex name)...]
 *          distances:  (distance)          [(distance)...]
 *          parents:    (parent v. name)    [(parent v. name)...]
 *      }
 *
 *      (paths to destination vertices){
 *          (path){
 *              Path: (source v. name) => (destination v. name)
 *              Cost: (cost of path)
 *              Route: (destination v. name) <- [(mid adjacent v. name)...] <- (source v. name)
 *          }
 *          [(path)...]
 *      }
 * }
 *
 */

/*
 * INPUT
 *

8 16
0 1 2 3 4 5 6 7
0 1 5
0 4 9
0 7 8
1 2 12
1 3 15
1 7 4
2 3 3
2 6 11
3 6 9
4 5 4
4 6 20
4 7 5
5 2 1
5 6 13
7 5 6
7 2 7

 */

/*
 * OUTPUT
 *

1

vertices:	0	1	2	3	4	5	6	7
distances:	0	5	14	17	9	13	25	8
parents:	-	0	5	2	0	4	2	0

Path: 0 => 1
Cost: 5
Route: 1 <= 0

Path: 0 => 2
Cost: 14
Route: 2 <= 5 <= 4 <= 0

Path: 0 => 3
Cost: 17
Route: 3 <= 2 <= 5 <= 4 <= 0

Path: 0 => 4
Cost: 9
Route: 4 <= 0

Path: 0 => 5
Cost: 13
Route: 5 <= 4 <= 0

Path: 0 => 6
Cost: 25
Route: 6 <= 2 <= 5 <= 4 <= 0

Path: 0 => 7
Cost: 8
Route: 7 <= 0

 */