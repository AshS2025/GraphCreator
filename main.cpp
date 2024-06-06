/*
Author: Ashvika Singhal
Turn-in Date: 6/6/24
Sources:
stack overflow
geeks for geeks
roger and vikram
 */

#include <iostream>
#include <cstring>
#include <limits>
#include <vector>
#include <iterator>
#include <iomanip>

using namespace std;

const int maxVertices = 20;
const int infiniteCost = numeric_limits<int>::max();

//vertex struct!
struct Vertex {
    char* label;

    Vertex(char* newLabel) {
        label = newLabel;
    }
};

//shortest path vertex struct
struct VertexProperties {
    Vertex* vert;
    int distance;
    vector<Vertex*> route;
    bool visited;

    VertexProperties() {
        vert = NULL;
        distance = infiniteCost;
        visited = false;
    }

    VertexProperties(Vertex* newVert) {
        vert = newVert;
        distance = infiniteCost;
        visited = false;
    }
};


int getVertexIndex(Vertex** verticesList, char* label);
void insertVertex(Vertex** verticesList);
void insertEdge(Vertex** verticesList, int** adjMatrix);
void deleteEdge(Vertex** verticesList, int** adjMatrix);
void deleteVertex(Vertex** verticesList, int** adjMatrix);
void computeShortestPath(Vertex** verticesList, int** adjMatrix);
void dijkstraAlgorithm(VertexProperties** propList, int** adjMatrix, Vertex** verticesList, int endIndex);

int main() {
    int* adjMatrix[maxVertices];
    Vertex* verticesList[maxVertices];
    char userCommand[50];
    bool isRunning = true;

    //initialize adjacency matrix and vertex list
    for (int i = 0; i < maxVertices; i++) {
        adjMatrix[i] = new int[maxVertices];
        for (int j = 0; j < maxVertices; j++) {
            adjMatrix[i][j] = -1;
        }
        verticesList[i] = NULL;
    }

    
    cout << "Welcome to Graph Creator" << endl;
    cout << "type a command: add vertex, add edge, remove edge, remove vertex, short path and quit" << endl;

    while (isRunning) {
      
        cin.get(userCommand, 50);
        cin.get();

      
        if (strcmp(userCommand, "add vertex") == 0) {
            insertVertex(verticesList);
	    
        }

	else if (strcmp(userCommand, "add edge") == 0) {
            insertEdge(verticesList, adjMatrix);
        }

	else if (strcmp(userCommand, "remove edge") == 0) {
            deleteEdge(verticesList, adjMatrix);
        }

	else if (strcmp(userCommand, "remove vertex") == 0) {
            deleteVertex(verticesList, adjMatrix);
        }

	else if (strcmp(userCommand, "short path") == 0) {
            computeShortestPath(verticesList, adjMatrix);
        }

	else if (strcmp(userCommand, "quit") == 0) {
            isRunning = false;
        }

	else {
            cout << "Invalid command" << endl;
        }
    }
}

//shortest path using Dijkstra's algorithm
void dijkstraAlgorithm(VertexProperties** propList, int** adjMatrix, Vertex** verticesList, int endIndex) {
    int lowestCost = infiniteCost;
    int startIndex = -1;

    //find the vertex with the lowest cost
    for (int i = 0; i < maxVertices; i++) {
        if (propList[i]->vert != NULL && propList[i]->distance < lowestCost && propList[i]->visited == false) {
            lowestCost = propList[i]->distance;
            startIndex = i;
        }
	
    }

    //if no path is found
    if (startIndex == -1) {
        return;
    }

    //if end vertex is reached
    if (startIndex == endIndex) {
        return;
    }

    //mark the vertex as visited
    propList[startIndex]->visited = true;

    
    //relaxation of edges
    for (int i = 0; i < maxVertices; i++) {
        if (adjMatrix[startIndex][i] != -1 && propList[startIndex]->distance + adjMatrix[startIndex][i] < propList[i]->distance) {
            propList[i]->distance = propList[startIndex]->distance + adjMatrix[startIndex][i];
            propList[i]->route.clear();
            for (vector<Vertex*>::iterator it = propList[startIndex]->route.begin(); it != propList[startIndex]->route.end(); it++) {
                propList[i]->route.push_back(*it);
            }
            propList[i]->route.push_back(verticesList[i]);
        }
	
    }

    //recursion
    dijkstraAlgorithm(propList, adjMatrix, verticesList, endIndex);
}

//shortest path calculation
void computeShortestPath(Vertex** verticesList, int** adjMatrix) {
    char* input = new char[50];
    int startIndex = 0;
    int endIndex = 0;

    cout << "Enter the starting vertex:" << endl;
    cin.get(input, 50);
    cin.get();
    startIndex = getVertexIndex(verticesList, input);

    if (startIndex == -1) {
        cout << "Vertex not found" << endl;
        return;
    }

    cout << "Enter the ending vertex:" << endl;
    cin.get(input, 50);
    cin.get();
    endIndex = getVertexIndex(verticesList, input);

    if (endIndex == -1) {
        cout << "Vertex not found" << endl;
        return;
    }

    //create an array of properties
    VertexProperties* propList[maxVertices];
    for (int i = 0; i < maxVertices; i++) {
        propList[i] = new VertexProperties(verticesList[i]);
    }

    //initialize the source vertex
    propList[startIndex]->distance = 0;
    propList[startIndex]->route.push_back(verticesList[startIndex]);
    dijkstraAlgorithm(propList, adjMatrix, verticesList, endIndex);

    //check if a path was found
    if (propList[endIndex]->distance == infiniteCost) {
        cout << "No path found between these vertices" << endl;
    }
    else {
        cout << "Shortest path: ";
        for (vector<Vertex*>::iterator it = propList[endIndex]->route.begin(); it != propList[endIndex]->route.end(); it++) {
            cout << (*it)->label << " ";
        }
        cout << endl;
        cout << "Cost of shortest path: " << propList[endIndex]->distance << endl;
    }
}

//removes a vertex from the graph
void deleteVertex(Vertex** verticesList, int** adjMatrix) {
    char* input = new char[50];
    int vertexIndex = 0;

    cout << "Enter the vertex to delete:" << endl;
    cin.get(input, 50);
    cin.get();
    vertexIndex = getVertexIndex(verticesList, input);

    if (vertexIndex == -1) {
        cout << "Vertex not found" << endl;
        return;
    }

    //remove all edges to and from the vertex
    for (int i = 0; i < maxVertices; i++) {
        adjMatrix[vertexIndex][i] = -1;
        adjMatrix[i][vertexIndex] = -1;
    }

    
    delete verticesList[vertexIndex];
    verticesList[vertexIndex] = NULL;
}


//removes an edge from the graph
void deleteEdge(Vertex** verticesList, int** adjMatrix) {
    char* input = new char[50];
    int startVertexIndex = 0;
    int endVertexIndex = 0;

    cout << "Enter the start vertex of the edge:" << endl;
    cin.get(input, 50);
    cin.get();
    startVertexIndex = getVertexIndex(verticesList, input);

    if (startVertexIndex == -1) {
        cout << "Vertex not found" << endl;
        return;
    }

    cout << "Enter the end vertex of the edge:" << endl;
    cin.get(input, 50);
    cin.get();
    endVertexIndex = getVertexIndex(verticesList, input);

    if (endVertexIndex == -1) {
        cout << "Vertex not found" << endl;
        return;
    }

    //remove the edge
    adjMatrix[startVertexIndex][endVertexIndex] = -1;
}

//gets the index of a vertex in the list
int getVertexIndex(Vertex** verticesList, char* label) {
    for (int i = 0; i < maxVertices; i++) {
        if (verticesList[i] != NULL && strcmp(verticesList[i]->label, label) == 0) {
            return i;
    }
   }
    return -1; //vertex not found
}

//inserts an edge between two vertices
void insertEdge(Vertex** verticesList, int** adjMatrix) {
    char* input = new char[50];
    int weight = 0;
    int startVertexIndex = 0;
    int endVertexIndex = 0;

    //get the vertices for the edge from user
    cout << "Enter the start vertex of the edge:" << endl;
    cin.get(input, 50);
    cin.get();
    startVertexIndex = getVertexIndex(verticesList, input);
    if (startVertexIndex == -1) {
        cout << "Vertex does not exist" << endl;
        return;
    }
    cout << "Enter the end vertex of the edge:" << endl;
    cin.get(input, 50);
    cin.get();
    endVertexIndex = getVertexIndex(verticesList, input);

    
    if (endVertexIndex == -1) {
        cout << "Vertex does not exist" << endl;
        return;
    }

    
   //check if vertices are the same
    if (startVertexIndex == endVertexIndex) {
        cout << "Cannot create an edge with the same vertex" << endl;
       return;
    }

    cout << "Enter the weight of the edge:" << endl;
    cin >> weight;
    cin.get();

    if (weight >= 0) {
        if (adjMatrix[startVertexIndex][endVertexIndex] == -1) {
            adjMatrix[startVertexIndex][endVertexIndex] = weight;
        } else {
            cout << "Edge already exists" << endl;
        }
    } else {
        cout << "Invalid weight" << endl;
    }
}

//adds a vertex to the graph
void insertVertex(Vertex** verticesList) {
    char* input = new char[50];
    int index = 0;

    //get the vertex label from user
    cout << "Enter the vertex label:" << endl;
    cin.get(input, 50);
    cin.get();
    
    //find the first available spot in the list
    while (index < maxVertices && verticesList[index] != NULL) {
        index++;
    }
    if (index == maxVertices) {
        cout << "No space available" << endl;
        return;
    }

    //add the new vertex
    verticesList[index] = new Vertex(input);
}
