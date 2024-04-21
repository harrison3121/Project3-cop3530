#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
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
    map<int,string> altindex;
    vector<vector<int>> matrix;//internal graph
public:
    void insertGame(Game game);
    bool similar(GN* from, GN* to);
    //vector<string> TopThreePlatform(int option);
    void searchGame(string gameName);
    void createEdgesOne(string game);
    GraphAdjMatrix<string, string>* generateGraph(string gameName);
    AdjacencyMatrix(int t);

};

AdjacencyMatrix::AdjacencyMatrix(int t) : total(t), counter(0) {
    matrix.resize(t, vector<int>(t, 0));
    //altindex.resize(total, "blank");

}

void AdjacencyMatrix::searchGame(string gameName) {
    if (index.find(gameName) == index.end())
    {
        cout << "Game doesn't exist in this dataset" << endl;
        return;
    }
    else
    {
        cout << gameName << " exist in the dataset, and offered on " << index[gameName]->platforms.size() << " platforms, which are " << endl;
        for (string pf : index[gameName]->platforms)
        {
            cout << pf << endl;
        }
    }

}

void AdjacencyMatrix::insertGame(Game game) {

    for (auto platform : game.getPlatformType()) {
        vector<string> v1;
        platform_map.emplace(make_pair(platform, v1));     //fixxxxxxxxxxxxxxxxxxxxxxxxxx me
        platform_map.at(platform).push_back(game.getTitle());   //fixxxxxxxxxxxxxxxxx me
    }

    for (const auto& genre : game.getGameGenre()) {
        vector<string> v2;
        genre_map.emplace(make_pair(genre, v2));
    }



    vector<string> v;
    GN* g = new GN(game.getTitle(), game.getRating(), v, counter);
    index.emplace(game.getTitle(), g);
    altindex.emplace(counter, game.getTitle());
    counter++;
    
}
    
bool AdjacencyMatrix::similar(GN* from, GN* to) {
    vector<string> platforms;
    vector<string> genres;
    for (auto i : from->platforms) {
        for (auto j : to->platforms) {
            if (i == j) {
                platforms.push_back(i);
            }
        }
    }
    for (auto i : from->genres) {
        for (auto j : to->genres) {
            if (i == j) {
                genres.push_back(i);
            }
        }
    }

    if (platforms.size() != 0 && genres.size() != 0) {
        return true;
    }
    return false;
}

void AdjacencyMatrix::createEdgesOne(string game) {
    int ind = index.find(game)->second->index;
    vector<int> v = matrix[ind];
    for (int j = 0; j < total; j++) {
        if (altindex.find(j) != altindex.end() && index.find(altindex.find(j)->second) != index.end() && index.find(game) != index.end()) {
            string tos = altindex.find(j)->second;
            GN* to = index.find(tos)->second;
            GN* from = index.find(game)->second;
            if (similar(from, to)) {
                matrix[ind][j] = 1;
            }
        }
        

    }

}


GraphAdjMatrix<string, string>* AdjacencyMatrix::generateGraph(string gameName) {
    GraphAdjMatrix<string, string>* graph = new GraphAdjMatrix<string, string>();
    for (auto p : index) {
        int c = 0;
        graph->addVertex(p.first);
        vector<int> v = matrix[p.second->index];
        for (int j = 0; j< total; j++){
            
            c++;
            if (v[j] == 1) {
                //cout << j ;
                graph->addVertex(altindex[j]);
                graph->addEdge(p.first, altindex[j], 5);
            }
        }
    }
    
    return graph;
}









