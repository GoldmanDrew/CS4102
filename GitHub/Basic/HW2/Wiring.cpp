#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cmath>
#include <iomanip>
using namespace std;

/**
 * Subset class to create subsets for Kruskal's algorithm
 * Every subset has a parent (pi) and a rank
 */
class Subset {
public:
    int pi, rank;
};

int find(Subset subsets[], int i) {
    if (i!= subsets[i].pi) subsets[i].pi = find(subsets, subsets[i].pi);
    return subsets[i].pi;
}

void Union(Subset subsets[], int a, int b) {
    int aR = find(subsets, a);
    int bR = find(subsets, b);
    if (subsets[bR].rank > subsets[aR].rank) subsets[aR].pi = bR;
    else if (subsets[bR].rank < subsets[aR].rank) subsets[bR].pi = aR;
    else {
        subsets[bR].pi = aR;
        subsets[aR].rank++;
    }
}

/**
 * Node class to contain the nodes of the graph
 * Every node has a type, id, parent, and a number
 */
class Node {
public:
    int num;
    string type, id, pi;
};

/**
 * Edge class to contain the edges of the graph
 * Every edge has a parent node, a child node, and a weight
 */
class Edge {
public:
    int weight;
    Node pi, child;
};

/**
 * Class to construct a Graph G(V,E) for the Wiring Problem
 * Every graph has |V| vertices and |E| edges
 */
class Graph {
public:
    int V, E;
    vector<Edge> edges;
    Graph(int v, int e) {
        V = v;
        E = e;
        edges.resize(E);
    }
};

Graph *createGraph(int V, int E) {
    return new Graph(V,E);
}


// Boolean function to determine whether breaker connected to a switch is conditionally valid
bool switchConditional(vector<Edge> result, Node p, Node c, string key) {
    Node s;
    if (key == "pi") s = p;
    else s = c;
    for (int i = 0; i < result.size(); i++) {
        // If there is a node in result that matches the switchnode
        if (s.num == result[i].pi.num) {
            // if the switchnode is connected to either a box or a outlet, breaker---switch is not valid
            if (result[i].child.type == "box" || result[i].child.type == "outlet" || result[i].child.type == "breaker") return false;
        }
        else if (s.num == result[i].child.num) {
            if (result[i].pi.type == "box" || result[i].pi.type == "outlet" || result[i].pi.type == "breaker") return false;
        }
    }
    return true;
}

void KruskalMST(Graph *graph)
{
    int V = graph->V;
    vector<Edge> result(V);
    int e = 0;
    int i = 0;
    sort(graph->edges.begin(), graph->edges.end(), [](const Edge &e1, const Edge &e2) {
        return e1.weight < e2.weight;
    });    
    Subset *subsets = new Subset[(V * sizeof(Subset))];
    for (int v = 0; v < V; v++) {
        subsets[v].pi = v;
        subsets[v].rank = 0;
    }
    while (e < V - 1 && i < graph->E) {
        Edge next_edge = graph->edges[i++];
        int a = find(subsets, next_edge.pi.num);
        int b = find(subsets, next_edge.child.num);
        string key = "";
        if (a != b) {
            bool conditional = true;
            if ((next_edge.pi.type == "breaker" && next_edge.child.type == "switch") || (next_edge.pi.type == "switch" && next_edge.child.type == "breaker")) {
                if (next_edge.pi.type == "switch") key = "pi";
                else key = "child";
                if (!switchConditional(result, next_edge.pi, next_edge.child, key)) conditional = false;
            }
            else if ((next_edge.pi.type == "box" && next_edge.child.type == "switch") || (next_edge.pi.type == "switch" && next_edge.child.type == "box")) {
                if (next_edge.pi.type == "switch") key = "pi";
                else key = "child";
                if (!switchConditional(result, next_edge.pi, next_edge.child, key)) conditional = false;
            }
            else if ((next_edge.pi.type == "outlet" && next_edge.child.type == "switch") || (next_edge.pi.type == "switch" && next_edge.child.type == "outlet")) {
                if (next_edge.pi.type == "switch") key = "pi";
                else key = "child";
                if (!switchConditional(result, next_edge.pi, next_edge.child, key)) conditional = false;
            }
            if (conditional) {
                result[e++] = next_edge;
                Union(subsets, a, b);
            }
        }
    }
    int minimumCost = 0;
    for (i = 0; i < e; ++i) minimumCost += result[i].weight;
    cout << minimumCost;
}

int findSpace(const string &str, const string &tofind, int nth)
{
    int pos = 0;
    int count = 0;
    while (count != nth){
        pos++;
        pos = str.find(tofind, pos);
        if (pos == string::npos) return -1;
        count++;
    }
    return pos;
}

bool valEdge(Node p, Node c) {
    if(p.type == "switch" && c.type == "light") {
        if(c.pi != p.id) return false;
    }
    if(c.type == "switch" && p.type == "light") {
        if (p.pi != c.id) return false;
    }
    if((p.type == "light" && c.type == "light")) if (p.pi != c.pi) return false;
    if((p.type == "outlet" && c.type == "light") || ((c.type == "outlet" && p.type == "light"))) return false;
    if((p.type == "light" && c.type == "breaker") || ((c.type == "light" && p.type == "breaker"))) return false;
    if((p.type == "box" && c.type == "light") || ((c.type == "box" && p.type == "light"))) return false;
    if((p.type == "switch" && c.type == "switch")) return false;
    return true;
}


int main() {
    string line;
    getline(cin, line);
    stringstream ss(line);
    int V; ss >> V;
    int E; ss >> E;
    Graph *G = createGraph(V, E);
    unordered_map<string, Node> m;
    string prevswitch = "";
    for (int i = 0; i < V; i++) {
        getline(cin, line);
        int space = line.find(" ");
        Node node;
        node.id = line.substr(0, space);
        node.type = line.substr(space + 1, line.length());
        node.num = i;
        if (node.type == "switch") prevswitch = node.id;
        else if (prevswitch != "" && node.type == "light") node.pi = prevswitch;
        else prevswitch = "";
        m.insert({node.id, node});
    }

    for (int i = 0; i < E; i++) {
        getline(cin, line);
        Node p = m.find(line.substr(0, findSpace(line, " ", 1)))->second;
        Node c = m.find(line.substr(findSpace(line, " ", 1) + 1, findSpace(line, " ", 2) - findSpace(line, " ", 1) - 1))->second;
        if (!valEdge(p, c)) continue;
        G->edges[i].pi = p;
        G->edges[i].child = c;
        G->edges[i].weight = stoi(line.substr(findSpace(line, " ", 2), line.length()));
    }
    KruskalMST(G);
}