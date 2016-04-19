#include <iostream>
#include <cstdio>
#include <list>
#include <limits>
#include <map>
#include <algorithm>
#include <vector>
#include <set>
#include <functional>
#include <iterator>


#define NIL -1
// priority queue for dijkstra findinf a container for the weights of a graph


using namespace std;



class Vertex {
  int _id;
  map<int, int> _weights;
  int _minPath;


public:

  Vertex(int v);
  int getVertex();
  void addConnection(int v, int w);
  int getWeight(int v);
  void setMinPath(int a);
  bool verifyConnection(int v);
  void updateWeight(int v, int w);
  void runDijkstra(vector<int> dist, set<Vertex> Q, int prev[]);
  void reweightConnections(int * dist, int V);
  bool operator< (const Vertex &right) const {
    return _minPath  < right._minPath;
  }
};

Vertex::Vertex(int v) {
  _id = v;
  _minPath = numeric_limits<int>::max();
}

int Vertex::getVertex() {
  return _id;
}

void Vertex::addConnection(int v, int w) {
  _weights[v] = w;
}

bool Vertex::verifyConnection(int v) {
  if(_weights.count(v) > 0) return true;
  return false;
}

int Vertex::getWeight(int v) {
  return _weights.find(v)->second;
}

void Vertex::setMinPath(int a) { _minPath = a; }

void Vertex::updateWeight(int v, int w) {
  map<int, int>::iterator it = _weights.find(v); 
  if(verifyConnection(v)) 
    if (it != _weights.end())
      it->second = w;
}

void Vertex::reweightConnections(int * dist, int V) {
  int new_w;
  for(int i=0; i < V; i++) //better this loop
    if(verifyConnection(i)) {
      new_w = _weights[i] + dist[_id] - dist[i];
      updateWeight(i, new_w);
    }
}

void Vertex::runDijkstra(vector<int> dist, set<Vertex> Q, int prev[]) {
  int alt, v, w;

  map<int,int>::iterator it;
  set<Vertex>::iterator it2;
  for(it = _weights.begin(); it != _weights.end(); ++it) {
    v = it->first;
    w = it->second;
    alt = dist[_id] + w;
    if(alt < dist[v]) {
      dist[v] = alt;
      prev[v] = _id;
      Vertex v1 = *(Q.find(v));
      Q.erase(Q.find(v));
      v1.setMinPath(alt);
      Q.insert(v1);
    }
  }
}



class Graph {
  int _V;
  int _F;
  int _C;
  vector<Vertex> _filials;
  vector<Vertex> _vertices;


public:
  Graph(int V, int F, int C);
  void Johnson();
  vector<int> Dijkstra(int source);
  void BellmanFord(int *dist, int *pred);
};

Graph::Graph(int V, int F, int C) {
  int a,b,w, f;
  this->_V = V;
  this->_F = F;
  this->_C = C;

  //add every vertex to _vertices vector
  for(int j=0; j < _V; j++) {
    Vertex v(j);
    _vertices.push_back(v);
  }


  //add filials ti _filials vector
  for(int k=0; k<_F; k++) {
    scanf("%d", &f);
    Vertex u(f);
    _filials.push_back(u);
  }

  //register each connection given
  for(int i = 0; i < C; i++) {
    scanf("%d %d %d", &a, &b, &w);
    _vertices[a-1].addConnection(b-1, w);
  }
}



void Graph::Johnson() {
  int * dist = new int[_V];
  int * pred = new int[_V];

  vector< vector<int> > matrixPaths;

  BellmanFord(dist, pred);


  for(int i=0; i<_V; i++) 
    printf("%d : %d |", i+1, dist[i]);
  //assuming there are no negative cycles in the graph like said in the instructions


  //reweight the edges
  for(int i = 0; i<_V; i++)
    _vertices[i].reweightConnections(dist, _V);

  int v;

  for(int i= 0; i<_F; i++) {
    v = _filials[i].getVertex();
    printf("%s\n", "filials work");
    vector<int> paths;
    printf("Vertice : %d", v + 1);
    paths = Dijkstra(v);
    printf("%s\n", "Dijkstra works");
    matrixPaths.push_back(paths);
    printf("%s\n", "works");
  }


  printf("%s\n", "EVERYTHING IS WORKING TILL THE END");
  //TODO: outputs here!!!

}



vector<int> Graph::Dijkstra(int source) {
  vector<int> paths;
  int *prev = new int[_V];
  set<Vertex> Q;

  for(int i = 0; i < _V; i++) {
    if(i != source) {
      paths[i] = numeric_limits<int>::max();
    }
  }

  for(int i =0; i < _V; i++) 
    Q.insert(_vertices[i]);

  paths[source] = 0;

  while(!Q.empty()) {
    Vertex v = *(Q.begin());
    v.runDijkstra(paths, Q, prev);
  }
  return paths;
}





void Graph::BellmanFord(int *dist, int *pred) { // dist and pred arrays have _V+1 elements to count the source

  for(int i=0; i<_V; i++) {
    dist[i] = 0;
    pred[i] = NIL;
  }


  for(int j = 0; j<_V; j++) 
    for(int k = 0; k<_V; k++) {
      Vertex v = _vertices[j];
      if(v.verifyConnection(k)) {
        if(dist[j] + v.getWeight(k) < dist[k]) {
          dist[k] = dist[j] + v.getWeight(k);
          pred[k] = j;
        }
      }
    }
}




int main() {
  int N, F, C;
  scanf("%d %d %d", &N, &F, &C);
  Graph graph(N,F,C);
  graph.Johnson();
  return 0;
}




/*
 function Dijkstra(Graph, source):
2      dist[source] ← 0                                    // Initialization
3
4      create vertex set Q
5
6      for each vertex v in Graph:           
7          if v ≠ source
8              dist[v] ← INFINITY                          // Unknown distance from source to v
9              prev[v] ← UNDEFINED                         // Predecessor of v
10
11         Q.add_with_priority(v, dist[v])
12
13
14     while Q is not empty:                              // The main loop
15         u ← Q.extract_min()                            // Remove and return best vertex
16         for each neighbor v of u:                       // only v that is still in Q
17             alt = dist[u] + length(u, v) 
18             if alt < dist[v]
19                 dist[v] ← alt
20                 prev[v] ← u
21                 Q.decrease_priority(v, alt)
22
23     return dist[], prev[]


input: 

5
2
8
1 2
4 5 -5
2 4 3 
1 4 2
3 4 -4
1 5 -1
2 3 5
1 3 4
2 5 -2

  */

