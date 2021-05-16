#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <numeric>
#include <iterator>
#include <sstream>
#include <cmath>
#include <iomanip>

using namespace std;

vector<vector<double>> X;
vector<vector<double>> Y;
vector<vector<double>> Z;

bool conditionX(const vector<double>& v1, const vector<double>& v2) { 
    return v1[0] < v2[0]; 
} 

bool conditionY(const vector<double>& v1, const vector<double>& v2) { 
    return v1[1] < v2[1]; 
} 

bool conditionZ(const vector<double>& v1, const vector<double>& v2) { 
    return v1[2] < v2[2];
} 


double threeDistance(double i1, double i2, double i3, double j1, double j2, double j3) {
    return sqrt(pow((i1 - j1), 2) + pow((i2 - j2), 2) + pow((i3 - j3), 2));
}

double trading(vector<vector<double>> xD, double delta) {
    Y = xD;
    sort(Y.begin(), Y.end(), conditionY);
    for(int i = 0; i < Y.size(); i++) {
        for(int j = i+1; j < Y.size(); j++) {
            double dist = abs(Y[j][1] - Y[i][1]);
            if(dist >= delta) break;
            else {
                dist = threeDistance(Y[j][0], Y[j][1], Y[j][2], Y[i][0], Y[i][1], Y[i][2]);
                delta = min(delta, dist);
            }
        }
    }
    Z = xD;
    sort(Z.begin(), Z.end(), conditionZ);
    for(int i = 0; i < Z.size(); i++) {
        for(int j = i+1; j < Z.size(); j++) {
            double dist = abs(Z[j][1] - Z[i][1]);
            if(dist >= delta) break;
            else {
                dist = threeDistance(Z[j][0], Z[j][1], Z[j][2], Z[i][0], Z[i][1], Z[i][2]);
                delta = min(delta, dist);
            }
        }
    }
    return delta;
}

double routes(vector<vector<double>> c, int l, int r) {
    if(r - l <= 1) return threeDistance(c[0][0], c[0][1], c[0][2], c[1][0], c[1][1], c[1][2]);
    double distance = 0;
    double ret = INT_MAX;
    if(r - l <= 1000) {
        for(int i = l; i < r; i++) {
            for(int j = i+1; j < r; j++) {
                distance = threeDistance(c[j][0], c[j][1], c[j][2], c[i][0], c[i][1], c[i][2]);
                ret = min(ret, distance);
            }
        }
        return ret;
    }
    /*
    Sort points by x coordinate and recurisvely split sorted points in half to find minimum distance on left and right halves and compare those
    now we have a measure for delta based on the left and right halves. Now we have to check for points that extend along the cut.
    check points along both the y and z strips using BRUTE FORCE!
    Subset that list delta to the left and the right and now we sort by y and z for the two strips
    Only one strip referring to recursive call
    */
    int mid = (l + r) / 2;
    return min(routes(X, l, mid), routes(X, mid, r));
}

int main(int argc,  char *argv[]) {
    vector<string> input;
    string line;
    while (getline(cin, line)) {
        input.push_back(line);
    }
    for(int i = 0; i < input.size(); i++) {
        if(input[i].compare("0") == 0) break;
        int n = 0;
        if(input[i].find(" ") == string::npos) {
            n = stoi(input[i]);
            i++;
        }
        vector<vector<double>> coordinates(n, vector<double>(3));
        int j;
        int index = 0;
        for(j = i; j < n+i; j++) {
            stringstream ss(input[j]);
            double d1; ss >> d1;
            double d2; ss >> d2;
            double d3; ss >> d3;
            coordinates[index][0] = d1;
            coordinates[index][1] = d2;
            coordinates[index][2] = d3;
            index++;
        }
        X = coordinates;
        sort(X.begin(), X.end(), conditionX);
        vector<vector<double>> xD;
        xD.push_back(X[X.size()/2]);
        double delta = routes(X, 0 , X.size());
        for(int i = X.size()/2+1; i < X.size(); i++) {
            if(abs(X[i][0] - X[X.size()/2][0]) < delta) xD.push_back(X[i]);
            else break;
        }
        for(int i = X.size()/2-1; i > -1; i--) {
            if(abs(X[i][0] - X[X.size()/2][0]) < delta) xD.push_back(X[i]);
            else break;
        }
        double ret = trading(xD, delta);
        if(ret > 10000) cout << "infinity" << endl;
        else cout << setprecision(4) << fixed << ret << endl;
        coordinates.clear();
        i = j - 1;
    }
    return 0;
}