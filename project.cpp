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


struct edge {
  int adjacent;
  int weight;
};


class Vertex {
  int _id;
  vector<edge> _adj;
  int _filial;


public:

  Vertex(int v, int filial);
  int getVertex();
  void setFilial(int n);
  int getFilial();
  void addConnection(int v, int w);
  int getWeight(int v);
  bool verifyConnection(int v);
  void updateWeight(int v, int w);
  void printConnections();
  list<int> getAdjacents();
  //void runDijkstra(vector<int> *dist, set<Vertex> *Q, int *prev[]);
  void reweightConnections(int * dist, int V);
};



typedef  pair<int, int> Pair;


Vertex::Vertex(int v, int filial) {
  _id = v;
  _filial = filial;
}

int Vertex::getVertex() {
  return _id;
}

void Vertex::setFilial(int n) {
  _filial = n;
}

int Vertex::getFilial() {
  return _filial;
}

void Vertex::addConnection(int v, int w) {
  edge e;
  e.adjacent = v;
  e.weight = w;
  _adj.push_back(e);
}

bool Vertex::verifyConnection(int v) {
  vector<edge>::iterator it;
  for(it = _adj.begin(); it != _adj.end(); ++it)
    if(it->adjacent == v)
      return true;
  return false;
}

void Vertex::printConnections() {
  vector<edge>::iterator it;
  for(it = _adj.begin(); it != _adj.end(); ++it) {
    printf("%d - %d : %d\n", _id+1, it->adjacent+1, it->weight);
  }
}

int Vertex::getWeight(int v) {
  vector<edge>::iterator it;
  for(it = _adj.begin(); it != _adj.end(); ++it)
    if(it->adjacent == v)
      return it->weight;
  return 0;
}


void Vertex::updateWeight(int v, int w) {
  vector<edge>::iterator it;
  for(it = _adj.begin(); it != _adj.end(); ++it)
    if(it->adjacent == v) {
      it->weight = w; 
      break;
    }
}

list<int> Vertex::getAdjacents() {
  list<int> adjacents;
  vector<edge>::iterator it;
  for(it = _adj.begin(); it != _adj.end(); ++it) {
    int i = it->adjacent;
    adjacents.push_back(i);
  }
  return adjacents;
}

void Vertex::reweightConnections(int * dist, int V) {
  int new_w;
  vector<edge>::iterator it;
  for(it = _adj.begin(); it != _adj.end(); ++it) //better this loop
    if(verifyConnection(it->adjacent)) {
      new_w = it->weight + dist[_id] - dist[it->adjacent];
      updateWeight(it->adjacent, new_w);
    }
}

class Graph {
  int _V;
  int _F;
  int _C;
  vector<Vertex> _vertices;


public:
  Graph(int V, int F, int C);
  void Johnson();
  vector<int> Dijkstra(int source);
  void BellmanFord(int *dist);
  int getFilial(int a);
};

Graph::Graph(int V, int F, int C) {
  int a,b,w;
  this->_V = V;
  this->_F = F;
  this->_C = C;

  for(int k=0; k<_V; k++) {
    Vertex v(k, NIL);
    _vertices.push_back(v);
  }

  for(int i = 0; i<_F; i++) {
    int filial;
    scanf("%d ", &filial);
    _vertices[filial-1].setFilial(i); 
  }

  //register each connection given
  for(int i = 0; i < C; i++) {
    scanf("%d %d %d", &a, &b, &w);
    _vertices[a-1].addConnection(b-1, w);
  }
}



void Graph::Johnson() {
  int * dist = new int[_V];

  vector<int> minPaths;

  BellmanFord(dist);
  //assuming there are no negative cycles in the graph like said in the instructions
  //reweight the edges
  for(int i=0; i < _V; i++) {
    _vertices[i].printConnections();
  }

  for(int i = 0; i<_V; i++) {
    printf("%d   :   %d|\n", i+1, dist[i]);
    _vertices[i].reweightConnections(dist, _V);
  }

  for(int i=0; i < _V; i++) {
    _vertices[i].printConnections();
  }
  //bellman ford works


  for(int i = 0; i<_F; ++i) {
    vector<int> paths;
    int v = getFilial(i);
    printf("%d\n", v+1);
    paths = Dijkstra(v); 
    for(int j=0; j<_V; j++) {
      if(paths[j] != numeric_limits<int>::max() && j != v)
        paths[j] += dist[j] - dist[v];
      printf("%d    :     %d\n", j+1, paths[j]);
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

  int minCost = numeric_limits<int>::max();
  int local = NIL;
  for(int i =0; i<_V; i++)
    if(minPaths[i] != numeric_limits<int>::max() && minPaths[i] < minCost) {
      minCost = minPaths[i];
      local = i;
    }
  
  //TODO: outputs here!!!
  if(local != NIL) {
    printf("%d %d\n", local + 1, minCost);
    for(int i =0; i <_F; i++) {
      vector<int> finalD;
      int final = getFilial(i);
      finalD = Dijkstra(final);
      finalD[local] += dist[local] - dist[final];
      printf("%d ", finalD[local]);
    }
    cout << "\n";
  }
  else {
    printf("N\n");
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
      int u_id = *list_it;
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






void Graph::BellmanFord(int *dist) { // dist and pred arrays have _V+1 elements to count the source

  for(int i=0; i<_V; i++) {
    dist[i] = 0;
  }


  for(int j = 0; j<_V; j++) 
    for(int k = 0; k<_V; k++) {
      if(_vertices[j].verifyConnection(k)) {
        if(dist[j] + _vertices[j].getWeight(k) < dist[k]) {
          dist[k] = dist[j] + _vertices[j].getWeight(k);
        }
      }
    }
}

int Graph::getFilial(int a) {
  int i;
  for(i = 0; i <_V; i++) {
    int f = _vertices[i].getFilial();
    if(a == f) 
      break;
  }
  return i;
}

int main() {
  int N, F, C;
  scanf("%d %d %d", &N, &F, &C);
  Graph graph(N,F,C);
  graph.Johnson();
  return 0;
}

