#include <iostream>
#include <limits.h>
#include <queue>
#include <cstring>
#include <algorithm>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Class {
    public:
        string name;
        int max_capacity;
        int index = 1;

        Class() {

        }

        bool operator==(const Class* c2) const {
            return this->name == c2->name;
        }
};

class Student {
    public:
        string name;
        int index;

        Student() {
            
        }

        bool operator==(const Student* p2) const {
            return this->name == p2->name;
        }
};

struct compare {
    bool operator() (const Student* p1, const Student* p2) const {
        return p1->name < p2->name;
    }
};

bool breadthFirstSearch(vector<vector<int>> &resAdjMatrix, int &source, int &sink, vector<int> &parent) {
    int size = resAdjMatrix.size();
    bool visited[size] = { false };
        
    queue<int> q;
    
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
        
    while(q.empty() == false) {
        int u = q.front();
        q.pop();
        
        for(int i = 0; i < size; i++) {
            int v = i;
            int capacity = resAdjMatrix[u][v];
            
            if(visited[v] == false && capacity > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    if(visited[sink] == true) 
        { return true; }
        
    return false;
}
// Source: https://www.srcmake.com/home/cpp-ford-fulkerson-max-flow
int FordFulkerson(vector<vector<int>> &adjMatrix, int source, int sink, int desired) {
    int maxflow = 0;
    vector< vector<int> > resAdjMatrix;
    int n = adjMatrix.size();
    for(int i = 0; i < n; i++) {
        vector<int> row;
        resAdjMatrix.push_back(row);
        for(int j = 0; j < adjMatrix[i].size(); j++) {
            resAdjMatrix[i].push_back(adjMatrix[i][j]);
        }
    }
    
    vector<int> parent;
    for(int i = 0; i < n; i++)
        parent.push_back(-1);
    
    while(breadthFirstSearch(resAdjMatrix, source, sink, parent) == true) {
        int pathflow = INT_MAX;
        
        int v = sink;
        while(v != source) {
            int u = parent[v];
            
            int capacity = resAdjMatrix[u][v];
            pathflow = min(pathflow, capacity);
            
            v = u;
        }
        
        v = sink;
        while(v != source) {
            int u = parent[v];
            resAdjMatrix[u][v] -= pathflow;
            resAdjMatrix[v][u] += pathflow;
            v = u;
        }
        maxflow += pathflow;
    }
    //cout << "max flow: " << maxflow << endl;
    if (maxflow == desired) return true;
    return false;
}

int main() {
    map <Student*, vector<Class*>, compare> UVA;
    vector<Class*>classes;
    vector<Student*>students;
    int r; // number of student registration requests
    int c; // number of courses
    int n; // number of classes each student is to be enrolled in
    cin >> r;
    cin >> c;
    cin >> n;

    while(r != 0 && c != 0 && n != 0) {
        int V = c + 2;
        int index = 1;
        int s = 0;
        for(int i = 0; i < r; i++) {
            string name, classname;
            cin >> name;
            cin >> classname;
            Class *c = new Class();
            c->name = classname;
            Student *p = new Student();
            p->name = name;

            vector<Class*>::iterator classfind;
            classfind = find_if(classes.begin(), classes.end(), [c](Class const * c1) {
                return c1->name == c->name;
            });
            vector<Student*>::iterator studentfind;
            studentfind = find_if(students.begin(), students.end(), [p](Student const * p1) {
                return p1->name == p->name;
            });

            if(classfind != classes.end()) { // course have already shown up
                c = classes.at(classfind - classes.begin());
            }
            else { // first instance of course
                classes.push_back(c);
                c->index = index;
                index++;
            }

            if(studentfind != students.end()) { // person has already shown up
                p = students.at(studentfind - students.begin());
            }
            else {  // first instance of person
                students.push_back(p);
                V++;
                p->index = index;
                index++;
                s++;
            }

            map<Student*,vector<Class*>>::iterator it;
            it = UVA.find(p);
            if(it == UVA.end()) { // if the name is not already in the map
                vector<Class*>v(1);
                v.at(0) = c;
                UVA.insert({p, v});
            }
            else {
                UVA.at(p).push_back(c);
            }

        }
        int S = V;
        vector<vector<int>> G(S, vector<int>(S, 0));
        for(int i = 0; i < c; i++) {
            string classname, capacity;
            cin >> classname;
            cin >> capacity;
            for(int j = 0; j < classes.size(); j++) {
                if(classes[j]->name == classname) {
                    classes[j]->max_capacity = stoi(capacity);
                }
            }
        }
        for (auto const &pair: UVA) {
            int c1 = pair.first->index;
            G[0][c1] = n;
            for(int j = 0; j < pair.second.size(); j++) {
                int c2 = pair.second.at(j)->index;
                G[c1][c2] = 1;
                G[c2][S-1] = pair.second.at(j)->max_capacity;
            }
        }
        /*for(int i = 0; i < S; i++) {
            for(int j = 0; j < S; j++) cout << G[i][j] << " ";
            cout << endl;
        }*/
        bool ans = FordFulkerson(G, 0, S-1, s*n);
        if(ans) cout << "Yes" << endl;
        else cout << "No" << endl;

        cin >> r;
        cin >> c;
        cin >> n;

        classes.clear();
        students.clear();
        UVA.clear();
    }
    return 0;
}