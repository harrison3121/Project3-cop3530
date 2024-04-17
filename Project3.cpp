#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>

#include "GraphAdjList.h"
#include "DataSource.h"
#include "Bridges.h"
#include "Array1D.h"

using namespace std;
using namespace bridges;




int main() {
    // Initialize Bridges
    Bridges bridges(1, "hlucas", "1631129020485");

    // Set title for visualization
    bridges.setTitle("Video Game Graph");

    // Initialize data source
    DataSource ds(&bridges);

    // Get the video game data
    vector<Game> games = ds.getGameData();

    // Graph of video games
    GraphAdjList<string, string> graph;

    //set of all platforms
    map<string, Color> platforms;
    // Add vertices to the graph
    for (Game game : games) {
        graph.addVertex(game.getTitle(), game.getPlatformType());
        graph.addVertex(game.getPlatformType());
        Color color;
        color.setBlue(rand() % 255);
        color.setGreen(rand() % 255);
        color.setRed(rand() % 255);
        platforms.emplace((string)game.getPlatformType(), color);
        graph.addEdge(game.getTitle(), game.getPlatformType());
        graph.getEdge(game.getTitle(), game.getPlatformType()).setColor(platforms.at(game.getPlatformType()));
    }



    // Visualize the graph
    bridges.setDataStructure(&graph);
    bridges.visualize();

    return 0;
}
