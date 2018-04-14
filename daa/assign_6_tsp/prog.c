/*
 * Author - Vaishnav Mhetre
 * Created at - Sunday, 10th April - 3:37 am
 * Bellman Ford Algorithm - Find Shortest path from
 * some source to destination in a Graph
 */

#include <limits.h>
#include <stdio.h>

#define MAX 50              // Haystack Max size, any array

/*
 * Copy one matrix to other - backup
 *
 * @function void copy
 * @param int[][] res - Destination matrix
 * @param int[][] mat - Source matrix
 * @param int n - Amount of elements in haystack
 */

void copy(int res[MAX][MAX], int mat[MAX][MAX], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            res[i][j] = mat[i][j];
        }
    }
}

/*
 * Reduce matrix by subtracting min value in row or column
 *
 * @function void subtractReduce
 * @param int[][] mat - Operative matrix
 * @param int n - Amount of elements in haystack
 * @param int index - Index of row/column to be worked over
 * @param int r - Subtractive value
 * @param int decision - Reduce row/column (0 - row, 1 - column)
 */

void subtractReduce(int mat[MAX][MAX], int n, int index, int r, int decision){
    for (int i = 0; i < n; ++i) {
        if(decision == 0){
            if (mat[index][i] != INT_MAX)           // If infinite value, no need to subtract
                mat[index][i] -= r;
        } else if(decision == 1){
            if (mat[i][index] != INT_MAX)           // If infinite value, no need to subtract
                mat[i][index] -= r;
        }
    }
}

/*
 * Reduction of matrix and derivation of minimal lower bound value
 *
 * @function int reduce
 * @param int[][] mat - Operative matrix
 * @param int n - AMount of elements in haystack
 */

int reduce(int mat[MAX][MAX], int n) {
    int RMin = 0, CMin = 0, R = 0, min = INT_MAX;

    for (int i = 0; i < n; ++i) {                       // Iterate through each row
        for (int j = 0; j < n; ++j) {                   // For each row, iterate through each column
            if (mat[i][j] < min)                        // Min check for each value in row
                min = mat[i][j];
        }
        if (min != INT_MAX){
            subtractReduce(mat, n, i, min, 0);
            RMin += min;                                // If min not infinity of that row, add it to sum of Row min (RMin)
        }
        min = INT_MAX;                                  // Reset min to infinity for next row check
    }

    min = INT_MAX;                                      // Reset min to infinity for column check
    for (int i = 0; i < n; ++i) {                       // Iterate through each column
        for (int j = 0; j < n; ++j) {                   // For each column, iterate through each row
            if (mat[j][i] < min)                        // Min check for each value in column
                min = mat[j][i];
        }
        if (min != INT_MAX){
            subtractReduce(mat, n, i, min, 1);
            CMin += min;                                // If min not infinity of that row, add it to sum of Column min (CMin)
        }
        min = INT_MAX;                                  // Reset min to infinity for next column check
    }

    R = RMin + CMin;                                    // Add both sums of row min and column min to get minimal lower bound

    return R;       // Return lower bound
}

/*
 * Set infinity to every row to column instance and haystack[dest][src] as requirement
 *
 * @function void resolveInfinity
 * @param int[][] mat - Operative matrix
 * @param int n - Amount of elements in matrix
 * @param int[] path - Relative path/tree of vertex connections
 * @param int paramCount - Amount of vertices in relative tree/path
 */

void resolveInfinity(int mat[MAX][MAX], int n, int path[MAX], int pathCount) {
    for (int i = 0; i < pathCount - 1; ++i) {
        int src = path[i], dest = path[i + 1];
        for (int j = 0; j < n; ++j) {
            mat[src][j] = mat[j][dest] = INT_MAX;
        }
        mat[dest][src] = INT_MAX;
    }
}

/*
 * Calculation of Cost "C(S)" for an operative matrix
 *
 * @function int calculateCost
 * @param int[][] mat - Operative matrix
 * @param int n - Amount of elements in haystack
 * @param int[] path - Relative path/tree of vertex connections
 * @param int pathCount - Amount of vertices in relative tree/path
 * @param int parentRVal - Lower bound of Parent Vertex in relative tree/path
 */

int calculateCost(int mat[MAX][MAX], int n, int path[MAX], int pathCount, int parentRVal) {
    int cost, R, reducedMat[MAX][MAX];
    int src = path[pathCount - 2];
    int dest = path[pathCount - 1];

    copy(reducedMat, mat, n);                               // Get a backup
    resolveInfinity(reducedMat, n, path, pathCount);        // Resolve inifinity
    R = reduce(reducedMat, n);                              // Get minimal lower bound
    cost = parentRVal + mat[src][dest] + R;                 // Derive cost => C(S) = C(parent) + weight[src][dest] + minimal lower bound (R)

    return cost;        // Return lower bound/cost
}

/*
 * Get minimum lower bound from haystack
 *
 * @function int getMinR
 * @param int[] haystack - Haystack for search
 * @param int n - Amount of elements in haystack
 */

int getMinR(int haystack[MAX], int n) {
    int min = haystack[0];

    for (int i = 1; i < n; ++i) {
        if (haystack[i] < min)
            min = haystack[i];
    }

    return min;
}

/*
 * Processing each child over reduction and lower bound discovery for next vertex discovery
 *
 * @function int processor
 * @param int[][] mat - Operative matrix
 * @param int[] path - Resultant route
 * @param int n - Amount of elements in haystack
 * @param int[] relativePath - Relative path/tree of vertex connections
 * @param int relativeCount - Amount of vertices in relative tree/path
 * @param int[] visited - Tracking of unvisited vertices
 * @param int visitedCount - Count of unvisited vertices
 * @param int parent - Parent vertex
 * @param int parentRVal - Lower bound of Parent Vertex in relative tree/path
 */

int processor(int mat[MAX][MAX], int path[MAX], int n, int relativePath[MAX], int relativePathCount, int visited[MAX],
              int visitedCount, int parent, int parentRVal) {

    int minRVal, childR[n], temp[MAX][MAX], minIndex;

    for (int i = 0; i < n; ++i) {
        if (i != parent && visited[i] == 0) {       // Get lower bound of each unvisited node except parent, else set infinity
            copy(temp, mat, n);
            relativePath[relativePathCount] = i;
            childR[i] = calculateCost(temp, n, relativePath, relativePathCount+1, parentRVal);
        } else
            childR[i] = INT_MAX;
    }

    minRVal = getMinR(childR, n);           // Get minimum lower bound to proceed to

    for (int i = 0; i < n; ++i) {           // Find the vertex deriving minimum lower bound
        if (minRVal == childR[i])
            minIndex = i;
    }

    /* Proceed to that vertex
     * (path => which vertex to proceed from current [useful after derivation of whole tree],
     * relativePath => fixed vertex in the path to continue with)
     */
    path[parent] = relativePath[relativePathCount++] = minIndex;
    visited[minIndex] = 1;
    visitedCount++;

    if (visitedCount < n) {     // If not visited all vertices, continue the derivation
        return processor(mat, path, n, relativePath, relativePathCount, visited, visitedCount, minIndex, minRVal);
    }

    return minRVal;     // Return the cost of current matrix distance covered
}

/*
 * Kick start of the sequence
 *
 * @function int TSP
 * @param int[][] mat - Operative matrix
 * @param int[] path - Resultant route
 * @param int n - Amount of elements in Haystack
 * @param int src - Source vertex to start rote from
 */

int TSP(int mat[MAX][MAX], int path[MAX], int n, int src) {
    int R, reducedMat[MAX][MAX], relativePath[MAX], relativePathCount = 0, visited[MAX], visitedCount = 0;

    copy(reducedMat, mat, n);
    R = reduce(reducedMat, n);

    path[src]  = -1;
    relativePath[relativePathCount++] = src;
    visited[src] = 1;
    visitedCount++;

    return processor(reducedMat, path, n, relativePath, relativePathCount, visited, visitedCount, src, R);

}

/*
 * View complete path
 *
 * @function void viewPath
 * @param int[] path - Path to be displayed
 * @param int n - Amount of elments in haystack
 * @param int src - Source vertex
 */

void viewPath(int path[MAX], int n, int src){
    int iter = 0;

    do{
        printf("%d", src+1);            // Print index+1 as vertex name
        src = path[src];                // Goto next vertex
        iter++;
        if(iter != n)
            printf(" => ");             // If last vertex, don't print arrow
    }while (iter != n);                 // Run till all vertices covered

    printf("%d", src+1);
}

/*
 * Start of Execution
 */

int main() {
    int mat[MAX][MAX], path[MAX], n, src, minRouteDist;

    scanf("%d", &n);                            // Accept amount of vertices

    for (int i = 0; i < n; ++i) {               // Accept matrix, except same row col values are set to infinity
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                scanf("%d", &mat[i][j]);        // Accept weight of edge
            }
            else
                mat[i][j] = INT_MAX;
        }
    }

    scanf("%d", &src);                          // Accept source vertex

    minRouteDist = TSP(mat, path, n, --src);    // Derive minimum route distance

    printf("\n%d\n\n", minRouteDist);

    viewPath(path, n, src);                     // View route

    return 0;
}

/*
 * INPUT FORMAT
 *
 * (amount of vertices)
 * (vertex name) [(vertex name)...]
 * (source vertex)
 *
 */

/*
 * OUTPUT FORMAT
 *
 * (minimum route distance)
 *
 * (path){
 *      (src vertex name) => [(linking vertex name)...] => (destination vertex name)
 * }
 *
 */

/*
 * INPUT

5
20 30 10 11 15 16 4 2 3 5 2 4 19 6 18 3 16 4 7 16
1

 */

/*
 * OUTPUT


28

1 => 4 => 2 => 5 => 3

 */
