#include "graph.h"

#include <algorithm>
#include <stack>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

DisjointSet::DisjointSet(int numVertices){
    subsets.resize(numVertices);
    for(Vertex v = 0; v<numVertices; v++){
        makeSet(v);
    }
}

void DisjointSet::makeSet(Vertex x){
    subsets[x].parent = x;
    
}

Vertex DisjointSet::findSet(Vertex x){
    if(subsets[x].parent == x){
        return x; //it's itself
    }
    else{
        return findSet(subsets[x].parent); //find the parent
    }
}

void DisjointSet::unionSets(Vertex x, Vertex y){
    //only if they are different subsets
    Vertex repX = findSet(x);
    Vertex repY = findSet(y);
    if(repX != repY){
        subsets[repX].parent = repY;
    }
}


Graph Graph::sort_edges() const{
    Graph g = *this; //copy over Graph 
    /*auto compare_edge = [](const Edge& a, const Edge& b){
        return a.weight<b.weight; //can be used to passinto sort
    };*/

    sort(g.begin(), g.end(),[](const Edge& a, const Edge& b){
        return a.weight<b.weight; //can be used to passinto sort
    });
    return g;
     
}

VertexList Graph::edges_from(Vertex vertex) const{
    VertexList vl;
    for(const Edge& e: *this){
        if(vertex == e.u){
            vl.push_back(e.v);
        }
    }
    return vl;
}

EdgeList Kruskals(const Graph& G){
    EdgeList el;
    //make set of trees
    DisjointSet ds(G.numVertices);
    
    //sort list of edges
    Graph sorted = G.sort_edges();
    //iterate through each edge in list
    //for each one check if it connects already connected nodes
    //if so: ignore
    //else: add to spanning tree
    for(const Edge& e:sorted){
        if(ds.findSet(e.u)!=ds.findSet(e.v)){
            el.push_back(e);
            ds.unionSets(e.u, e.v);
        }
    }
    return el;

}

int sum_weights(EdgeList const& L){
    int sum =0;
    for(auto& e: L){
        sum+=e.weight;
    }
    return sum;
}

void file_to_graph(string filename, Graph& G){
    ifstream in(filename);
    in>>G;
}


VertexList dfs(const Graph& graph, Vertex startVertex){
    stack<Vertex> s;
    vector<bool> visited(graph.numVertices,false);
    VertexList vl;
    visited[startVertex] = true;
    s.push(startVertex);

    while(!s.empty()){
        Vertex curr = s.top();
        s.pop();
        vl.push_back(curr);
        VertexList edges = graph.edges_from(curr);
        for(Vertex v: edges){
            if(!visited[v]){
                s.push(v);
                visited[v] = true;
            }
        }
    }
    return vl;
    
}

VertexList bfs(const Graph& graph, Vertex startVertex){
    queue<Vertex> q;
    vector<bool> visited(graph.numVertices,false);
    VertexList vl;
    //label vertex as explored 
    visited[startVertex] = true;
    q.push(startVertex);
    //vl.push_back(startVertex);
    while(!q.empty()){
        Vertex curr = q.front();
        q.pop();
        vl.push_back(curr);
        VertexList edges= graph.edges_from(curr);
        for(Vertex v: edges){
            if(!visited[v]){
                visited[v] = true;
                //__ = v; //set parent to curr; 
                q.push(v);
                
            }
        }

    }
    return vl;
}



string get_arg(int argc, char *argv[], string def){
    if(argc>1) return string(argv[1]);

    return def;
}
