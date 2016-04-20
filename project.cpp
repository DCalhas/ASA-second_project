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
  int getMinPath();
  bool verifyConnection(int v);
  void updateWeight(int v, int w);
  list<int> getAdjacents();
  //void runDijkstra(vector<int> *dist, set<Vertex> *Q, int *prev[]);
  void reweightConnections(int * dist, int V);
  bool operator< (const Vertex &right) const {
    return _minPath  < right._minPath;
  }
};



typedef  pair<int, int> Pair;


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

void Vertex::setMinPath(int a) { _minPath = a; }

int Vertex::getMinPath() {
  return _minPath;
}

int Vertex::getWeight(int v) {
  return _weights.find(v)->second;
}


void Vertex::updateWeight(int v, int w) {
  map<int, int>::iterator it = _weights.find(v); 
  if(verifyConnection(v)) 
    if (it != _weights.end())
      it->second = w;
}

list<int> Vertex::getAdjacents() {
  list<int> adjacents;
  map<int, int>::iterator it;
  for(it = _weights.begin(); it != _weights.end(); ++it) 
    adjacents.push_back(it->first);
  return adjacents;
}

void Vertex::reweightConnections(int * dist, int V) {
  int new_w;
  for(int i=0; i < V; i++) //better this loop
    if(verifyConnection(i)) {
      new_w = _weights[i] + dist[_id] - dist[i];
      updateWeight(i, new_w);
    }
}
/*
void Vertex::runDijkstra(vector<int> *dist, set<Vertex> *Q, int *prev[]) {
  int alt, v, w;

  map<int,int>::iterator it;
  set<Vertex>::iterator it2;


  for(it = _weights.begin(); it != _weights.end(); ++it) {
    v = it->first;
    printf("%d v\n", v);
    w = it->second;
    printf("%d w\n", w);
    alt = dist[0][_id] + w;
    printf("%d  source \n", alt);
    if(alt < dist[0][v]) {
      dist[0][v] = alt;
      prev[0][v] = _id;
      set<Vertex>::iterator set_it = Q[0].find(v);
      if(set_it != Q[0].end()) {
        Vertex v1 = *(set_it); //error here core dumped
        Q[0].erase(set_it);
        v1.setMinPath(alt);
        Q[0].insert(v1);
      }
    }
  }
}*/

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
    Vertex u(f-1);
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

  vector<int> minPaths;

  BellmanFord(dist, pred);
  //assuming there are no negative cycles in the graph like said in the instructions


  //reweight the edges
  for(int i = 0; i<_V; i++) {
    printf("%d : %d |", i+1, dist[i]);
    _vertices[i].reweightConnections(dist, _V);
  }


  cout << "\n";
  for(int i = 0; i<_F; ++i) {
    vector<int> paths;
    int v = _filials[i].getVertex();
    paths = Dijkstra(v); 
    for(int j=0; j<_V; j++) {
      if(paths[j] != numeric_limits<int>::max() && j != v)
        paths[j] += dist[j] - dist[v];  
      printf("%d  :   %d\n", j +1, paths[j]);
      if(i == 0) minPaths.push_back(paths[j]);
      else {
        if(minPaths[j] != numeric_limits<int>::max() && paths[j] != numeric_limits<int>::max())
          minPaths[j] += paths[j];
        else {
          minPaths[j] = numeric_limits<int>::max();
        }
      }
    }
  }
  cout << "\n";
  for(int i =0; i<_V; i++) 
    printf("%d  :   %d\n", i+1, minPaths[i]);
  cout << "\n";


  int minCost = numeric_limits<int>::max();
  int local = NIL;
  for(int i =0; i<_V; i++)
    if(minPaths[i] != numeric_limits<int>::max() && minPaths[i] < minCost) {
      minCost = minPaths[i];
      local = i;
    }
  
  //TODO: outputs here!!!

  for(int i = 0; i<_F; ++i) {
    vector<int> shortest;
    int v1 = _filials[i].getVertex();
    shortest = Dijkstra(v1);
    for(int j=0; j<_V; j++) {
      if(shortest[j] != numeric_limits<int>::max() && j != v1)
        shortest[j] += dist[j] - dist[v1];
      printf("%d  :   %d\n", j +1, shortest[j]);
    }
  }


  if(local != NIL) {
    printf("%d %d\n", local + 1, minCost);
    for(int i =0; i <_F; i++) {
      vector<int> finalD;
      int final = _filials[i].getVertex();
      finalD = Dijkstra(final);
      finalD[local] += dist[local] - dist[final];
      printf("%d ", finalD[local]);
    }
    cout << "\n";
  }
  else {
    printf("N \n");
  }
}



vector<int> Graph::Dijkstra(int source) {
  vector<int> paths;
  set< Pair > Q; //pair<Path, Vertex>

  for(int j =0; j < _V; j++) {
    if(j == source) {
      Pair pairSource(0, j);
      Q.insert(pairSource);
      paths.push_back(0);
    }
    else {
      Pair pairPath(numeric_limits<int>::max(), j);
      Q.insert(pairPath);
      paths.push_back(numeric_limits<int>::max());
    }
  }


  while(!Q.empty()) {
    int v_id = (Q.begin())->second;
    Q.erase(Q.begin());
    list<int> adjV = _vertices[v_id].getAdjacents();
    list<int>::iterator list_it;
    for(list_it = adjV.begin(); list_it != adjV.end(); ++list_it) {
      Vertex u = *list_it;
      int u_id = u.getVertex();
      int alt = paths[v_id] + _vertices[v_id].getWeight(u_id);
      if(alt < paths[u_id] && paths[v_id] != numeric_limits<int>::max()) {
        int old_path = paths[u_id];
        paths[u_id] = alt;
        //queue updating
        Pair pairU(old_path, u_id);
        set< Pair >::iterator set_it = Q.find(pairU);
        if(set_it != Q.end()) {
          Pair par = *(set_it); //error here core dumped
          Q.erase(set_it);
          par.first = alt;
          Q.insert(par);
        }
      }
    }
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
      if(_vertices[j].verifyConnection(k)) {
        if(dist[j] + _vertices[j].getWeight(k) < dist[k]) {
          dist[k] = dist[j] + _vertices[j].getWeight(k);
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

