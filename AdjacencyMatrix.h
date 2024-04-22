#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "GraphAdjMatrix.h" 
#include "GraphAdjList.h" 
#include "Bridges.h"
#include "Array1D.h"
#include "DataSource.h"
using namespace std;
using namespace bridges;




Color RatingColor(int rating) {
    int r, g, b = 30;

    if (rating < 3) {
        r = 255;
        g = 0;
    }
    else if (rating > 8) {
        r = 0;
        g = 255;
    }
    else {
        r = static_cast<int>(255 * (8 - rating) / 5.0);
        g = 255 - r;
    }
    Color ncolor(r, g, b);
    return ncolor;
}

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
    int similar(GN* from, GN* to, string d);
    //vector<string> TopThreePlatform(int option);
    bool hasSameGenre(const vector<string>& vec1, const vector<string>& vec2);
    void searchGame(string gameName);
    GraphAdjList<string, string>* generateGraphOne(string gameName);
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


    

    if (platform_map.find(game.getPlatformType()) == platform_map.end())
    {
        vector<string> temp = { game.getTitle() };
        platform_map.insert(make_pair(game.getPlatformType(), temp));//add new platform
    }
    else
    {
        platform_map[game.getPlatformType()].push_back(game.getTitle());//push game to this platform
    }
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
    
    //g->platforms.emplace(game.getPlatformType());
    
    //index.emplace(game.getTitle(), g);

    vector<string> v;
    GN* g = new GN(game.getTitle(), game.getRating(), v, counter);
    altindex.emplace(counter, game.getTitle());
        
    if (index.find(game.getTitle()) == index.end())
    {
        index.insert(make_pair(game.getTitle(), new GN(game.getTitle(), game.getRating(), game.getGameGenre(), counter)));;//add game node
        index[game.getTitle()]->platforms.push_back(game.getPlatformType());//put in the platform for this game
    }
    else
    {
        index[game.getTitle()]->platforms.push_back(game.getPlatformType());//add addition platform to this game

    }
    

    
    counter++;
}
    

bool AdjacencyMatrix::hasSameGenre(const vector<string>& vec1, const vector<string>& vec2)
{
    unordered_set<string> set1(vec1.begin(), vec1.end());
    for (string genre : vec2)
    {
        if (set1.count(genre) > 0)
            return true;
    }
    return false;
}






GraphAdjList<string, string>* AdjacencyMatrix::generateGraphOne(string gameName) {


    if (index.find(gameName) != index.end()) {
        GN* from = index[gameName];
        int c = 0;
        for (string platform : from->platforms) {
            for (string gm : platform_map[platform]) {
                int i = index[gameName]->index;
                int j = index[gm]->index;
                if (gm == from->title) {
                    continue;
                }



                if (index[gm]->genres == index[from->title]->genres) {
                    //cout << c << endl;
                    //cout << "***************truetruesame**********************" << gm << "  " << gm << endl;
                    matrix[i][j] = 5;
                }
                else if (hasSameGenre(index[gm]->genres, index[from->title]->genres)) {
                    //cout << c << endl;
                    //cout << "atleast one match**************" << gm << "  " << gm << endl;
                    matrix[i][j] = 1;
                }
                c++;
            }
        }
    }



    GraphAdjList<string, string>* graph = new GraphAdjList<string, string>();
    map<string, Color> platform_colors;
    int ind = index.find(gameName)->second->index;
    int c = 0;
    //int i = 0;
    //cout<<
    graph->addVertex(gameName);
    graph->getVertex(gameName)->setShape(STAR);
    graph->getVertex(gameName)->setSize(30);
    vector<string> v;
    for (auto t : matrix[ind]) {
        if (t != 0) {
            //cout << i<<endl ;
            //i++;
            
            GN* to = index[altindex[c]];
            v.push_back(to->title);
            graph->addVertex(to->title);
            graph->getVertex(to->title)->setSize(t * 5 + to->rating * 3);
            //cout << to->title<<endl;//<<t<<" " << to->rating << endl;
            graph->getVertex(to->title)->setColor(RatingColor(to->rating));
            graph->addEdge(gameName, to->title);
            Color color;
            color.setBlue(rand() % 255);
            color.setGreen(rand() % 255);
            color.setRed(rand() % 255);
            platform_colors.emplace((string)gameName, color);
            graph->getEdge(gameName, to->title).setColor(platform_colors.at(gameName));

        }

        c++;
    }
    int v1 = v.size();
    return graph;
}










