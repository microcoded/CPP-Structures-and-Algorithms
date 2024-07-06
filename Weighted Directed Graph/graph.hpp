#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <iostream>
#include <fstream>
#include <utility>
#include <functional>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>
#include <limits>
#include <stdexcept>

// Sources
// 1.
//      URL: https://stackoverflow.com/a/13409381/13640042
//      Location of use: `isTreeDFS()`
//      Referred to this to conceptually understand how to determine if graph is a tree.
//

template<typename T>
class Graph {
private:
    std::vector<std::unordered_map<int, T> > adjList{};
    int numVertices{};

public:
    // empty graph with N vertices
    explicit Graph(int N);

    // construct graph from edge list in filename
    explicit Graph(const std::string &filename);

    // add an edge directed from vertex i to vertex j with given weight
    void addEdge(int i, int j, T weight);

    // removes edge from vertex i to vertex j
    void removeEdge(int i, int j);

    // is there an edge from vertex i to vertex j?
    bool isEdge(int i, int j) const;

    // return weight of edge from i to j
    // will throw an exception if there is no edge from i to j
    T getEdgeWeight(int i, int j) const;

    // returns number of vertices in the graph
    int size() const;

    // alias a const iterator to our adjacency list type to iterator
    using iterator =
            typename std::vector<std::unordered_map<int, T> >::const_iterator;

    // cbegin returns const iterator pointing to first element of adjList
    iterator begin() const {
        return adjList.cbegin();
    }

    iterator end() const {
        return adjList.cend();
    }

    // return iterator to a particular vertex
    iterator neighbours(int a) const {
        return adjList.begin() + a;
    }
};

template<typename T>
Graph<T>::Graph(int N) : adjList(N), numVertices{N} {
    std::vector<bool> visited(static_cast<int>(adjList.size()), false);
}

template<typename T>
Graph<T>::Graph(const std::string &inputFile) {
    std::ifstream infile{inputFile};
    if (!infile) {
        std::cerr << inputFile << " could not be opened\n";
        return;
    }
    // first line has number of vertices
    infile >> numVertices;
    adjList.resize(numVertices);
    int i{};
    int j{};
    double weight{};
    // assume each remaining line is of form
    // origin dest weight
    while (infile >> i >> j >> weight) {
        addEdge(i, j, static_cast<T>(weight));
    }
}

template<typename T>
int Graph<T>::size() const {
    return numVertices;
}

template<typename T>
void Graph<T>::addEdge(int i, int j, T weight) {
    if (i < 0 or i >= numVertices or j < 0 or j >= numVertices) {
        throw std::out_of_range("invalid vertex number");
    }
    adjList[i].insert({j, weight});
}

template<typename T>
void Graph<T>::removeEdge(int i, int j) {
    // check if i and j are valid
    if (i >= 0 && i < numVertices && j >= 0 && j < numVertices) {
        adjList[i].erase(j);
    }
}

template<typename T>
bool Graph<T>::isEdge(int i, int j) const {
    if (i >= 0 && i < numVertices && j >= 0 && j < numVertices) {
        return adjList.at(i).contains(j);
    }
    return false;
}

template<typename T>
T Graph<T>::getEdgeWeight(int i, int j) const {
    return adjList.at(i).at(j);
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const Graph<T> &G) {
    for (int i = 0; i < G.size(); ++i) {
        out << i << ':';
        for (const auto &[neighbour, weight]: *(G.neighbours(i))) {
            out << " (" << i << ", " << neighbour << ")[" << weight << ']';
        }
        out << '\n';
    }
    return out;
}

template<typename T>
bool isSubgraph(const Graph<T> &H, const Graph<T> &G) {
    // Subgraph cannot be larger
    if (H.size() > G.size()) return false;
    // All edges in subgraph have to be in graph
    // First we go to all the vertexes
    for (int vertexI = 0; vertexI < H.size(); ++vertexI) {
        // Every edge from the vertex
        for (const auto &[neighbour, weight]: *H.neighbours(vertexI)) {
            // Must be an edge and have the same weight to be valid subgraph
            if (not G.isEdge(vertexI, neighbour) or G.getEdgeWeight(vertexI, neighbour) not_eq weight) return false;
        }
    }
    // If we got here, we have a subgraph!
    return true;
}

template<typename T>
bool isTreePlusIsolated(const Graph<T> &G, int root) {
    // ————————————— IS TREE —————————————
    std::vector<bool> visited(G.size(), false);
    visited[static_cast<unsigned long>(root)] = true; // TODO: weird warning, casting as unsigned long fixes it even though it is always int?!?!?
    if (not isTreeDFS(G, root, visited)) return false; // Contains a cycle

    // ————————————— IS ISOLATED —————————————
    for (int i = 0; i < G.size(); ++i) {
        if (not visited[i] and G.neighbours(i)->size() > 0) return false; // Non-isolated vertex not reachable from root
    }

    return true; // Is tree + isolated 🎉
}

template<typename T>
bool isTreeDFS(const Graph<T> &G, int root, std::vector<bool> &visited) {
    // For this conceptually used https://stackoverflow.com/a/13409381/13640042
    for (const auto &[neighbour, weight]: *G.neighbours(root)) {
        if (visited[neighbour]) return false;  // If the neighbour vertex has been visited, there is a cycle >:(
        visited[neighbour] = true;             // If it hasn't been visited, mark it as visited so that we can keep the recursion going.
        if (not isTreeDFS(G, neighbour, visited)) return false;
    }
    // Woo, each vertex visited once! We ARE a tree. Yay.
    return true;
}

template<typename T>
void pathLengthsDFS(const Graph<T> &tree, int vertex, T distance, std::vector<T> &bestDistanceTo) {
    bestDistanceTo[vertex] = distance;
    // Modification of the DFS function above. Instead of returning a bool we are modifying a given vector.
    // Ideally, I could combine these functions into one and return some type of pair, so that we're not rewriting code for no reason.
    for (const auto &[neighbour, weight]: *tree.neighbours(vertex)) {
        if (bestDistanceTo[neighbour] == false) {
            // If the neighbour vertex has not been visited, we continue DFS-ing!
            pathLengthsDFS(tree, neighbour, distance + weight, bestDistanceTo);
        }
    }
}

template<typename T>
std::vector<T> pathLengthsFromRoot(const Graph<T> &tree, int root) {
    // Create a vector of the size of the tree. Set everything to false as we have not visited the vertex and calculated a distance for it yet.
    std::vector<T> bestDistanceTo(tree.size(), false);
    // Our recursive DFS from above to populate this vector with distances.44
    pathLengthsDFS(tree, root, 0, bestDistanceTo);
    return bestDistanceTo;
}

template<typename T>
bool allEdgesRelaxed(const std::vector<T> &bestDistanceTo, const Graph<T> &G, int source) {
    if (not bestDistanceTo[source] == 0) return false; // Shortest path to the source from the source must be 0
    for (int vertex = 0; vertex < G.size(); ++vertex) {                                    // Loop through every vertex
        for (const auto &[neighbour, weight]: *G.neighbours(vertex)) {                   // Every neighbour at the vertex
            if (bestDistanceTo[neighbour] > bestDistanceTo[vertex] + weight) return false;  // Edge relaxed condition. Return false if not relax and stop.
        }
    }
    return true;    // All edges are relaxed!
}

//template <typename T>
//T infinity() {
//    if (std::numeric_limits<T>::has_infinity) {
//        return std::numeric_limits<T>::infinity();
//    } else if constexpr (std::is_same_v<T, MyInteger>) {        // Check if T == MyInteger
//        return MyInteger {std::numeric_limits<int>::max()};
//    } else {
//        return std::numeric_limits<T>::max();
//    }
//}

#endif      // GRAPH_HPP_