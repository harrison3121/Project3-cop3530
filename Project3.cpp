#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include "AdjacencyList.h"
#include "AdjacencyMatrix.h"
#include "GraphAdjList.h"
#include "GraphAdjMatrix.h"
#include "DataSource.h"
#include "Bridges.h"
#include "Array1D.h"

using namespace std;
using namespace bridges;

//brighten the color by adding 
Color brighten(Color color, int degree)
{
    int b = color.getBlue();
    int r = color.getRed();
    int g = color.getGreen();
    (r <= 255 - degree) ? r += degree : r = 255;
    (g <= 255 - degree) ? g += degree : g = 255;
    (b <= 255 - degree) ? b += degree : b = 255;
    Color ncolor(r, g, b);
    return ncolor;
}

int main() {
    //find top rating games on the same platform
    // find top rating games 
    //prompt for input game name
    string structure;
    cout << "1. Adjacency List" << endl << "2. Adjacency Matrix" << endl;
    getline(cin, structure);
    string name;
    cout << "Enter Game Name" << endl;
    getline(cin, name);
    string data_size;
    cout << "Enter Data Size in Games (Max is 17534)" << endl;
    getline(cin, data_size);


    // Initialize Bridges
    Bridges bridges(2, "hlucas", "1631129020485");

    // Set title for visualization
    bridges.setTitle("Video Game Graph");

    // Initialize data source
    DataSource ds(&bridges);

    // Get the video game data
    vector<Game> games = ds.getGameData();
    
    if (structure == "1") {
        AdjacencyList g;
        for (int i = 0; i < 1000; i++) {
            Game game = games[i];
            //cout << games[i].getTitle() << endl;
            g.insertLink(game);

        }
        g.searchGame(name);
        GraphAdjList<string, string> graph = g.generateGraph(name);
        bridges.setDataStructure(&graph);
        bridges.visualize();
    }
    else {
        AdjacencyMatrix g(stoi(data_size));
        for (int i = 0; i < stoi(data_size); i++) {
            Game game = games[i];
            //cout << games[i].getTitle() << endl;
            g.insertGame(game);
            //cout << i << endl;
            

        }
        g.searchGame(name);
        GraphAdjList<string, string>* graph = g.generateGraphOne(name);
        bridges.setDataStructure(graph);
        bridges.visualize();


    }


    system("pause");
    // Visualize the graph
    

    return 0;
}
