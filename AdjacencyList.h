#pragma once
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
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
    GameNode(string title, float rating, vector<string> genres) : title(title), rating(rating), genres(genres) {}
    //platform vector later gets pushed in when adding games
};

bool hasSameGenre(const vector<string>& vec1, const vector<string>& vec2)
{
    unordered_set<string> set1(vec1.begin(), vec1.end());
    for (string genre : vec2)
    {
        if (set1.count(genre) > 0)
            return true;
    }
    return false;
}
Color getRatingColor(int rating) {
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


class AdjacencyList
{
private:
    int counter;
    map<string, vector<pair<string,float>>> platform_map;//map for platform value:vector of game names offered on the platform
    map<string, vector<string>> genre_map;//map for genre value: count of games in each genre
    //map<int, GameNode*> game_i;//map for game with key as int representation of game
    map<string, GameNode*> game_s;//map for game with key as string and value as int representation
    map<string, vector<pair<string, pair<int, float>>>> I_graph;//internal graph, float weight=relativeness+destination rating
public:
    void insertLink(Game game);
    vector<string> TopThreePlatform(int option);
    void searchGame(string gameName);
    bool searchGameQuiet(string gameName);
    GraphAdjList<string, string> generateGraphOne(string gameName);
    GraphAdjList<string, string> generateGraph(string gameName);
    AdjacencyList() { counter = 0; }
    ~AdjacencyList();
};
AdjacencyList::~AdjacencyList()
{
    for (auto n : game_s)
    {
        delete n.second;
    }
}
void AdjacencyList::insertLink(Game game)
{
    //add to platform if not exist, add game name to each platform
    if (platform_map.find(game.getPlatformType()) == platform_map.end())
    {
        vector<pair<string,float>> temp = { make_pair(game.getTitle(),game.getRating()) };
        platform_map.insert(make_pair(game.getPlatformType(), temp));//add new platform
    }
    else
    {
        platform_map[game.getPlatformType()].push_back(make_pair(game.getTitle(),game.getRating()));//push game to this platform
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

}
vector<string> AdjacencyList::TopThreePlatform(int option)
{
    multimap<int, string> rank;
    vector<string> rtn;
    if (option != 1 && option != 2 && option != 3)
    {
        cout << "warning message" << endl;
        return {};
    }
    if (option == 1)
    {
        //most games
        for (pair<string, vector<pair<string,float>>> pfpair : platform_map)
        {
            rank.insert(make_pair(pfpair.second.size(), pfpair.first));
        }

    }
    else if (option == 2)
    {
        //highest avg games rating
        for (auto p : platform_map)
        {
            float sum = 0;
            int i = 0;
            for (auto g : p.second)
            {
                i++;
                sum += g.second;
            }
            rank.insert(make_pair(sum/i, p.first));
        }
    }
    else if (option == 3)
    {
        //top platforms with related games
        string game;
        cout << "Enter your favorite game name" << endl;
        cin >> game;
        vector<string> genres = game_s[game]->genres;
        map<string, int> m;
        for (auto i : platform_map) {
            for (auto gm : i.second) {
                if (hasSameGenre(genres, game_s[gm.first]->genres)) {
                    if (m.find(i.first) == m.end())
                        m[i.first] = 1;
                    else
                        m[i.first]++;
                }
            }
        }
        for (auto i : m)
        {
            rank.insert(make_pair(i.second, i.first));
        }
    }
    //fix me: add option 2 and 3

    auto last_it = rank.rbegin(); // Iterator to the last element
    int count = 0;
    while (last_it != rank.rend() && count < 3) {
        rtn.push_back(last_it->second);
        ++last_it;
        ++count;
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
    //cout << i_graph[gameName] << endl;
    for (pair<string, pair<int, float> > p : I_graph[gameName])
    {
        cout << p.first << "  " << p.second.first + p.second.second << endl;
    }
}

bool AdjacencyList::searchGameQuiet(string gameName)
{
    if (game_s.find(gameName) == game_s.end())
    {
        cout << "Game doesn't exist in this dataset" << endl;
        return false;
    }
    else { return true; }
}
GraphAdjList<string, string> AdjacencyList::generateGraphOne(string gameName)
{
    if (game_s.find(gameName) != game_s.end())
    {
        //int c = 0;
        for (string platform : game_s[gameName]->platforms)
        {
            for (pair<string, float> gm : platform_map[platform])
            {


                if (gm.first == gameName)
                    continue;
                if (game_s[gm.first]->genres == game_s[gameName]->genres)
                {
                    //cout << c << endl;
                    //cout << "***************truetruesame**********************" << gm << "  " << gameName << endl;
                    I_graph[gameName].push_back(make_pair(gm.first, make_pair(5, gm.second)));//5pts for matching platform and all genres + rating
                }
                else if (hasSameGenre(game_s[gm.first]->genres, game_s[gameName]->genres))
                {
                    //cout << c << endl;
                    //cout << "atleast one match**************" << gm << "  " << gameName << endl;
                    I_graph[gameName].push_back(make_pair(gm.first, make_pair(1, gm.second)));//5pts for matching platform and all genres + rating;
                }
                //c++;
            }
        }
    }

    GraphAdjList<string, string> graph;
    map<string, Color> platform_colors;
    graph.addVertex(gameName);
    graph.getVertex(gameName)->setShape(STAR);
    graph.getVertex(gameName)->setSize(30);
    //int i = 0;
    for (pair<string, pair<int, float >> p : I_graph[gameName])
    {
        //cout << i << endl;
        //i++;
        //cout << p.first << endl;// << p.second.first << " " << p.second.second << endl;
        graph.addVertex(p.first);
        graph.getVertex(p.first)->setSize(p.second.first * 5 + p.second.second * 3);
        graph.getVertex(p.first)->setColor(getRatingColor(p.second.second));
        graph.addEdge(gameName, p.first);
        Color color;
        color.setBlue(rand() % 255);
        color.setGreen(rand() % 255);
        color.setRed(rand() % 255);
        platform_colors.emplace((string)gameName, color);
        graph.getEdge(gameName, p.first).setColor(platform_colors.at(gameName));
    }

    return graph;
}
GraphAdjList<string, string> AdjacencyList::generateGraph(string gameName)
{
    if (game_s.find(gameName) != game_s.end())
    {
        for (string platform : game_s[gameName]->platforms)
        {
            for (pair<string, float> gm : platform_map[platform])
            {
                if (gm.first == gameName)
                    continue;
                if (game_s[gm.first]->genres == game_s[gameName]->genres)
                {
                    //cout << "***************truetruesame**********************" << gm << "  " << gameName << endl;
                    I_graph[gameName].push_back(make_pair(gm.first, make_pair(5, gm.second)));//5pts for matching platform and all genres + rating
                }
                else if (hasSameGenre(game_s[gm.first]->genres, game_s[gameName]->genres))
                {
                    //cout << "atleast one match**************" << gm << "  " << gameName << endl;
                    I_graph[gameName].push_back(make_pair(gm.first, make_pair(1, gm.second)));//5pts for matching platform and all genres + rating;
                }
            }
        }
    }

    GraphAdjList<string, string> graph;
    map<string, Color> platform_colors;
    graph.addVertex(gameName);
    graph.getVertex(gameName)->setShape(STAR);
    graph.getVertex(gameName)->setSize(30);
    //int i = 0;
    for (pair<string, pair<int, float >> p : I_graph[gameName])
    {
        //cout << i << endl;
        //i++;
        //cout << p.first << endl;// << p.second.first << " " << p.second.second << endl;
        graph.addVertex(p.first);
        graph.getVertex(p.first)->setSize(p.second.first * 5 + p.second.second * 3);
        graph.getVertex(p.first)->setColor(getRatingColor(p.second.second));
        graph.addEdge(gameName, p.first);
        Color color;
        color.setBlue(rand() % 255);
        color.setGreen(rand() % 255);
        color.setRed(rand() % 255);
        platform_colors.emplace((string)gameName, color);//fix me:should be platform color not game title
        graph.getEdge(gameName, p.first).setColor(platform_colors.at(gameName));
    }
    return graph;
}
