#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include "AdjacencyList.h"
#include "GraphAdjList.h"
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
    string name;
    cout << "enter game name" << endl;
    cin >> name;

    // Initialize Bridges
    Bridges bridges(1, "shawnwang", "1204195056134");

    // Set title for visualization
    bridges.setTitle("Video Game Graph");

    // Initialize data source
    DataSource ds(&bridges);

    // Get the video game data
    vector<Game> games = ds.getGameData();

    // Graph of video games
    //GraphAdjList<string, string> graph;

    //set of all platforms
    //map<string, Color> platforms;
    //// Add vertices to the graph
    //vector<string> genres;
    //for (int i = 0;i < 1000;i++)
    //{
    //    if (games[i].getTitle() == name)
    //    {
    //        for (string genre : games[i].getGameGenre())
    //            genres.push_back(genre);
    //    }
    //}
    AdjacencyList test;
    for (int i = 0;i < 1000;i++) {
        Game game = games[i];
        test.insertLink(game);
        //for (string genre : game.getGameGenre())
        //{
        //    if (count(genres.begin(), genres.end(), genre) != 0) 
        //    {
        //        graph.addVertex(game.getTitle(), genre);
        //        graph.addVertex(genre);
        //        Color color;
        //        color.setBlue(rand() % 255);
        //        color.setGreen(rand() % 255);
        //        color.setRed(rand() % 255);
        //        platforms.emplace((string)genre, color);
        //        graph.addEdge(game.getTitle(), genre);
        //        graph.getEdge(game.getTitle(), genre).setColor(platforms.at(genre));
        //        graph.getVertex(genre)->setShape(STAR);
        //        graph.getVertex(genre)->setSize(30);
        //        graph.getVertex(genre)->setColor(brighten(platforms.at(genre), 50));
        //        graph.getVertex(game.getTitle())->setColor(brighten(platforms.at(genre), 20));
        //    }
        //}
        //graph.addVertex(game.getTitle(), game.getPlatformType());
        //graph.addVertex(game.getPlatformType());
        //Color color;
        //color.setBlue(rand() % 255);
        //color.setGreen(rand() % 255);
        //color.setRed(rand() % 255);
        //platforms.emplace((string)game.getPlatformType(), color);
        //graph.addEdge(game.getTitle(), game.getPlatformType());
        //graph.getEdge(game.getTitle(), game.getPlatformType()).setColor(platforms.at(game.getPlatformType()));
        //graph.getVertex(game.getPlatformType())->setShape(STAR);
        //graph.getVertex(game.getPlatformType())->setSize(30);
        //graph.getVertex(game.getPlatformType())->setColor(brighten(platforms.at(game.getPlatformType()), 50));
        //graph.getVertex(game.getTitle())->setColor(brighten(platforms.at(game.getPlatformType()),20));

    }
    test.searchGame(name);
    GraphAdjList<string, string> graph = test.generateGraph(name);



    // Visualize the graph
    bridges.setDataStructure(&graph);
    bridges.visualize();

    return 0;
}
