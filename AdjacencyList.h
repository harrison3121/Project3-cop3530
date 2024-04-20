#pragma once
#pragma once
#include <string>
#include <map>
#include <vector>
#include <queue>
#include "GraphAdjList.h"
#include "DataSource.h"
#include "Bridges.h"
#include "Array1D.h"

using namespace std;
using namespace bridges;

struct GameNode {
    string title;
    vector<string> platforms;
    float rating;
    vector<string> genres;
    GameNode(string title, float r, vector<string> g) : title(title), rating(rating), genres(genres) {}
    //platform vector later gets pushed in when adding games
};
class AdjacencyList
{
private:
    int counter;
    map<string, vector<string>> platform_map;//map for platform value:vector of game names offered on the platform
    map<string, vector<string>> genre_map;//map for genre value: count of games in each genre
    //map<int, GameNode*> game_i;//map for game with key as int representation of game
    map<string, GameNode*> game_s;//map for game with key as string and value as int representation
    map<string, vector<pair<string, int>>> I_graph;//internal graph
public:
    void insertLink(Game game);
    vector<string> TopThreePlatform(int option);
    void searchGame(string gameName);
    GraphAdjList<string, string> generateGraph(string gameName);
    AdjacencyList() { counter = 0; }
};
void AdjacencyList::insertLink(Game game)
{
    //add to platform if not exist, add game name to each platform
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
    if (game_s.find(game.getTitle()) == game_s.end())
    {
        counter++;
        game_s.insert(make_pair(game.getTitle(), new GameNode(game.getTitle(), game.getRating(), game.getGameGenre())));//add game node
        game_s[game.getTitle()]->platforms.push_back(game.getPlatformType());//put in the platform for this game
    }
    else
    {
        game_s[game.getTitle()]->platforms.push_back(game.getPlatformType());//add addition platform to this game

    }
    for (string gm : platform_map[game.getPlatformType()])
    {
        if (I_graph.find(game.getTitle()) == I_graph.end())
        {
            I_graph[game.getTitle()].push_back(make_pair(gm, 5));
        }

    }
}
vector<string> AdjacencyList::TopThreePlatform(int option)
{
    map<int, string> rank;
    vector<string> rtn;
    if (option != 1 || option != 2 || option != 3)
    {
        cout << "warning message" << endl;
        return {};
    }
    if (option == 1)
    {
        //most games
        for (pair<string, vector<string>> pfpair : platform_map)
        {
            rank.insert(make_pair(pfpair.second.size(), pfpair.first));//fix me: platform with same amount of games
        }

    }
    int i = 0;
    for (pair<int, string> rtpair : rank)
    {
        if (i > 2)
            break;
        rtn.push_back(rtpair.second);
    }
    return rtn;
}
void AdjacencyList::searchGame(string gameName)
{
    if (game_s.find(gameName) == game_s.end())
    {
        cout << "Game doesn't exist in this dataset" << endl;
        return;
    }
    else
    {
        cout << gameName << " exist in the dataset, and offered on " << game_s[gameName]->platforms.size() << " platforms, which are " << endl;
        for (string pf : game_s[gameName]->platforms)
        {
            cout << pf << endl;
        }
    }
}
GraphAdjList<string, string> AdjacencyList::generateGraph(string gameName)
{
    GraphAdjList<string, string> graph;
    map<string, Color> platform_colors;
    for (pair<string, vector<pair<string, int>>> graphNode : I_graph)
    {
        graph.addVertex(graphNode.first);
        for (pair<string, int > to : graphNode.second)
        {
            graph.addVertex(to.first);
            graph.addEdge(graphNode.first, to.first);
            Color color;
            color.setBlue(rand() % 255);
            color.setGreen(rand() % 255);
            color.setRed(rand() % 255);
            platform_colors.emplace((string)graphNode.first, color);//fix me:should be platform color not game title
            graph.getEdge(graphNode.first, to.first).setColor(platform_colors.at(graphNode.first));
        }
    }
    return graph;
}