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


Color brighten(Color color)
{
    int b=color.getBlue();
    int r=color.getRed();
    int g=color.getGreen();
    (r <= 235) ? r += 20 : r = 255;
    (g <= 235) ? g += 20 : g = 255;
    (b <= 235) ? b += 20 : b = 255;
    Color ncolor(r, g, b);
    return ncolor;
}

int main() {
    // Initialize Bridges
    Bridges bridges(1, "shawnwang", "1204195056134");

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
    for (int i = 0;i < 1000;i++) {
        Game game = games[i];
        graph.addVertex(game.getTitle(), game.getPlatformType());
        graph.addVertex(game.getPlatformType());
        Color color;
        color.setBlue(rand() % 255);
        color.setGreen(rand() % 255);
        color.setRed(rand() % 255);
        platforms.emplace((string)game.getPlatformType(), color);
        graph.addEdge(game.getTitle(), game.getPlatformType());
        graph.getEdge(game.getTitle(), game.getPlatformType()).setColor(platforms.at(game.getPlatformType()));
        graph.getVertex(game.getPlatformType())->setShape(STAR);
        graph.getVertex(game.getPlatformType())->setColor(brighten(color));
        graph.getVertex(game.getTitle())->setColor(brighten(platforms.at(game.getPlatformType())));

    }



    // Visualize the graph
    bridges.setDataStructure(&graph);
    bridges.visualize();

    return 0;
}
