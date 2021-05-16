#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>
#include <stdio.h>
#include <numeric>

using namespace std;

int p;
int counter;

bool fit(vector<int> t, int d, vector<int> differences) {
    int count = 0;
    int sum = 0;
    for(int i = 0; i < differences.size(); i++) {
        sum += differences[i];
        if(sum >= d) {
            count++;
            sum = 0;
        }
    }    
    if(count >= p-1) return true;
    else return false;
}

int binaryDivide(vector<int> t, int l, int r, vector<int> differences) {
    if(r - l <= 1) {
        if(fit(t, r, differences)) return r;
        return l;
    }
    if(counter > t.size()) return 1;
    int mid = (l + r) / 2;
    counter++;
    if(fit(t, mid, differences)) return binaryDivide(t, mid, r, differences);
    else return binaryDivide(t, l, mid-1, differences);
}

int main() {
    int n;
    cin >> n;
    cin >> p;
    int pos;
    vector<int> tables;
    tables.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> pos;
        tables[i] = pos;
    }
    int maxDistance = tables[tables.size() - 1] - tables[0];
    if(p == 2) {
        cout << maxDistance;
        exit(0);
    }
    vector<int> deriv;
    deriv.resize(maxDistance);
    /*for(int i = 1; i <= maxDistance; i++) {
        deriv[i-1] = i;
    }*/
    vector<int> differences;
    differences.resize(tables.size()-1);
    for(int i = 1; i < tables.size(); i++) {
        differences[i-1] = tables[i] - tables[i-1];
    }
    cout << binaryDivide(tables, 1, maxDistance, differences);
    return 0;
}