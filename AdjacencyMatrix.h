#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "GraphAdjMatrix.h" 
#include "Bridges.h"
#include "Array1D.h"
#include "DataSource.h"
using namespace std;
using namespace bridges;

struct GN {
    string title;
    vector<string> platforms;
    float rating;
    vector<string> genres;
    int index;

    GN(string title, float r, vector<string> g, int i) : title(title), rating(r), genres(g), index(i) {}
    //platform vector later gets pushed in when adding games
};


class AdjacencyMatrix {
private:
    int counter;
    int total;
    map<string, vector<string>> platform_map;//map for platform value:vector of game names offered on the platform
    map<string, vector<string>> genre_map;//map for genre value: count of games in each genre
    map<string, GN*> index;
    vector<string> altindex;
    vector<vector<int>> matrix;//internal graph
public:
    void insertLink(Game game);
    vector<string> TopThreePlatform(int option);
    void searchGame(string gameName);
    GraphAdjMatrix<string, string>* generateGraph(string gameName);
    AdjacencyMatrix(int t);

};

AdjacencyMatrix::AdjacencyMatrix(int t) : total(t), counter(0) {
    matrix.resize(t, vector<int>(t, 0));
    altindex.resize(total, "blank");
}

void AdjacencyMatrix::insertLink(Game game) {

    if (platform_map.find(game.getPlatformType()) == platform_map.end())
    {
        vector<string> temp = { game.getTitle() };
        platform_map.insert(make_pair(game.getPlatformType(), temp));//add new platform
    }
    else
    {
        platform_map[game.getPlatformType()].push_back(game.getTitle());//push game to this platform
    }
    //add to genre if not exist, add game name to each genre type
    for (string genre : game.getGameGenre())
    {
        if (genre_map.find(genre) == genre_map.end())
        {
            vector<string> temp = { game.getTitle() };
            genre_map.insert(make_pair(genre, temp));//add new genre
        }
        else
        {
            genre_map[genre].push_back(game.getTitle());//increment game to this genre
        }
    }
    if (index.find(game.getTitle()) == index.end())
    {
        counter++;
        altindex[counter] = game.getTitle();
        index.insert(make_pair(game.getTitle(), new GN(game.getTitle(), game.getRating(), game.getGameGenre(), counter)));//add game node
        index[game.getTitle()]->platforms.push_back(game.getPlatformType());//put in the platform for this game
    }
    else
    {
        index[game.getTitle()]->platforms.push_back(game.getPlatformType());//add addition platform to this game

    }
    for (string gm : platform_map[game.getPlatformType()])
    {
        int i = index.at(game.getTitle())->index;
        int j = index.at(gm)->index;
        if (index.find(game.getTitle()) == index.end())
        {
            matrix[i][j] == 1;
        }

    }

}
    


GraphAdjMatrix<string, string>* AdjacencyMatrix::generateGraph(string gameName) {
    GraphAdjMatrix<string, string>* graph = new GraphAdjMatrix<string, string>();
    for (auto gm : index){
        int count = 0;
        graph->addVertex(gm.first);
        for (int j : matrix[gm.second->index])
        {
            count++;
            if (j == 1) {
                graph->addVertex(altindex[j]);
                graph->addEdge(gm.first, altindex[j], 5);
            }
        }
    }
    return graph;
}









