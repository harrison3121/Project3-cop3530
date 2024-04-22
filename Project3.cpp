#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <chrono>
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

Redo:

    string name;
    cout << "Enter Game Name" << endl;
    getline(cin, name);
    string structure;
    cout << "1. Adjacency List" << endl << "2. Adjacency Matrix" << endl;
    getline(cin, structure);
    string search;
    cout << "3: Find Similar Games" << endl << "4: Find Best Platform" << endl;
    getline(cin, search);
    string data_size;
    cout << "Enter Data Size in Games (Max is 17534)" << endl;
    getline(cin, data_size);

    string b, c;
    cout << "Enter Bridges username and api key with one space inbetween" << endl;
    cin >> b >> c;
    // Initialize Bridges
    Bridges bridges(1, b, c);

    // Set title for visualization
    bridges.setTitle("Video Game Graph");

    // Initialize data source
    DataSource ds(&bridges);

    // Get the video game data
    vector<Game> games = ds.getGameData();

    auto start = std::chrono::high_resolution_clock::now();
    string type;
    if (structure == "1" && search == "3") {
        type = "Adjacency List";
        AdjacencyList g;
        for (int i = 0; i < stoi(data_size); i++) {
            Game game = games[i];
            //cout << games[i].getTitle() << endl;
            g.insertLink(game);

        }
        g.searchGame(name);
        GraphAdjList<string, string> graph = g.generateGraph(name);
        bridges.setDataStructure(&graph);
        bridges.visualize();
    }
    else if(structure == "2" && search == "3") {
        type = "Adjacency Matrix";
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

    else if (structure == "1" && search == "4") {

        type = "Adjacency List";
        AdjacencyMatrix g(stoi(data_size));
        for (int i = 0; i < stoi(data_size); i++) {
            Game game = games[i];
            //cout << games[i].getTitle() << endl;
            g.insertGame(game);
            //cout << i << endl;


        }
        if (!g.searchGameQuiet(name)) {
            cout << "Game Not Found in Dataset!!" << endl;
            goto Redo;
        }
        vector<string> v = g.TopThreePlatform(name);
        cout << "The top three platforms with the most games similar to yours are:" << endl;
        for (string s : v) {
            cout << s << endl;
        }
        GraphAdjList<string, string>* graph = g.generateGraph(name);
        bridges.setDataStructure(graph);
        bridges.visualize();



    }

    else if (structure == "2" && search == "4") {

        type = "Adjacency Matrix";
        AdjacencyMatrix g(stoi(data_size));
        for (int i = 0; i < stoi(data_size); i++) {
            Game game = games[i];
            //cout << games[i].getTitle() << endl;
            g.insertGame(game);
            //cout << i << endl;


        }
        if (!g.searchGameQuiet(name)) {
            cout << "Game Not Found in Dataset!!" << endl;
            goto Redo;
        }
        vector<string> v = g.TopThreePlatform(name);
        cout << "The top three platforms with the most games similar to yours are:" << endl;
        for (string s : v) {
            cout << s << endl;
        }
        GraphAdjList<string, string>* graph = g.generateGraph(name);
        bridges.setDataStructure(graph);
        bridges.visualize();


    }




    auto stop = std::chrono::high_resolution_clock::now(); // Stop timing
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    cout << "Time taken for " << type <<": " << duration.count() << " microseconds" << endl;

    system("pause");
    

    return 0;
}
