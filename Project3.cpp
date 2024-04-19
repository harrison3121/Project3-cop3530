#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stdlib.h>

#include "GraphAdjList.h"
#include "DataSource.h"
#include "Bridges.h"
#include "Array1D.h"

using namespace std;
using namespace bridges;


Color brighten(Color color)
{
    int b = color.getBlue();
    int r = color.getRed();
    int g = color.getGreen();
    (r <= 235) ? r += 20 : r = 255;
    (g <= 235) ? g += 20 : g = 255;
    (b <= 235) ? b += 20 : b = 255;
    Color ncolor(r, g, b);
    return ncolor;
}

struct CustomEdge {
    Color color;
    string from;
    string to;
    double weight;
    string platform;
};


struct AdjacencyList {

    map<string, vector<CustomEdge>> outgoing;
    map<string, vector<CustomEdge>> incoming;
    map<string, Color> platforms;


    GraphAdjList<string, string> convert() {
        GraphAdjList<string, string> graph;
        for (auto i : outgoing) {
            graph.addVertex(i.first);
            if (platforms.find(i.first) != platforms.end()) {graph.getVertex(i.first)->setShape(STAR);}
            for (auto j : i.second) {
                graph.addVertex(j.to);
                graph.addEdge(i.first, j.to);
                graph.getEdge(i.first, j.to).setLabel(to_string(j.weight));
                graph.getEdge(i.first, j.to).setColor(platforms.at(j.platform));
                if (platforms.find(j.to) != platforms.end()) { graph.getVertex(j.to)->setShape(STAR); }
            }
        }
        return graph;
    }
    void addNode(string node) {
        vector<CustomEdge> v;
        vector<CustomEdge> v2;
        outgoing.emplace(node, v);
        incoming.emplace(node, v2);
    }
    void addPlatform(string platform) {
        if (platforms.find(platform) == platforms.end()) {
            addNode(platform);
            Color color;
            color.setBlue(rand() % 255);
            color.setGreen(rand() % 255);
            color.setRed(rand() % 255);
            platforms.emplace(platform, brighten(color));

        }
    }
    void addEdge(string from, string to, double rating = 0.0f, string platform = "PC") {
        CustomEdge edge;
        edge.from = from;
        edge.to = to;
        double weight = 10.0 - rating;
        edge.weight = weight;
        edge.platform = platform;
        outgoing.at(from).push_back(edge);
        incoming.at(to).push_back(edge);
    }
};


int main() {
    // Initialize Bridges
    Bridges bridges(1, "hlucas", "1631129020485");

    // Set title for visualization
    bridges.setTitle("Video Game Graph");

    // Initialize data source
    DataSource ds(&bridges);

    // Get the video game data
    vector<Game> games = ds.getGameData();

    AdjacencyList graph;
    

    //set of all platforms
    map<string, Color> platforms;
    // Add vertices to the graph
    for (int i = 0; i < 1000; i++) {
        Game game = games[i];
        graph.addNode(game.getTitle());
        graph.addPlatform(game.getPlatformType());
        graph.addEdge(game.getTitle(), game.getPlatformType(), game.getRating(), game.getPlatformType());

    }
   




    // Visualize the graph
    GraphAdjList<string, string>graph2 = graph.convert();
    bridges.setDataStructure(&graph2);
    bridges.visualize();

    return 0;
}
