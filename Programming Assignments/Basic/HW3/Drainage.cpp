#include <string>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<vector<int>> memoization;

int drainage(vector<vector<int>> matrix, int r, int c) {
    int currentNode = matrix[r][c];
    int columns = matrix[r].size();
    int rows = matrix.size();

    int leftNode = 0;
    int rightNode = 0;
    int upNode = 0;
    int downNode = 0;
    
    if (c < columns && r < rows) {
        if(memoization[r][c] != -2) return memoization[r][c]; // DYNAMIC PROGRAMMING !

        if(c + 1 < columns) {
            if(matrix[r][c+1] < currentNode) rightNode += 1 + drainage(matrix, r, c+1);
        }
        if(r + 1 < rows) {
            if(matrix[r+1][c] < currentNode) downNode += 1 + drainage(matrix, r+1, c);
        }
        if(r - 1 >= 0) {
            if(matrix[r-1][c] < currentNode) upNode += 1 + drainage(matrix, r-1, c);
        }
        if(c - 1 >= 0) {
            if(matrix[r][c-1] < currentNode) leftNode += 1 + drainage(matrix, r, c-1);
        }
        return max(max(upNode, downNode), max(rightNode, leftNode));
    }
    else return 0;
}

/* Runs in Theta(n^2)*/
int dynamicIteration(vector<vector<int>> matrix) {
    int optimalPath = 0;
    for (int r = 0; r < matrix.size(); r++) {
        for (int c = 0; c < matrix[r].size(); c++) {
            int drainagePath = drainage(matrix, r, c);
            memoization[r][c] = drainagePath;
            optimalPath = max(drainagePath, optimalPath);
        }
    }
    return optimalPath + 1;
}

/* This method was adapated from StackOverflow to process input */
int findSpace(const string &str, const string &tofind, int nth) {
    int pos = 0;
    int count = 0;
    while (count != nth) {
        pos++;
        pos = str.find(tofind, pos);
        if (pos == string::npos) return -1;
        count++;
    }
    return pos;
}


int main() {
    string line;
    getline(cin, line);
    int iterations = stoi(line);
    vector<int> solutions;
    vector<string> titles;
    string p = " ";
    for (int i = 0; i < iterations; i++) {
        getline(cin, line);
        string title = line.substr(0, findSpace(line, p, 1));
        int rows = stoi(line.substr(findSpace(line, p, 1) + 1, findSpace(line, p, 2)));
        int columns = stoi(line.substr(findSpace(line, p, 2) + 1, line.length()));
        int index = 1;
        memoization.clear();
        memoization.resize(rows);
        vector<vector<int>> matrix(rows);
        for (int r = 0; r < rows; r++) {
            getline(cin, line);
            for (int c = 0; c < columns; c++) {
                memoization[r].push_back(-2);
                if (c == 0) matrix[r].push_back(stoi(line.substr(0, findSpace(line, p, 1))));
                else if (c == (columns - 1)) matrix[r].push_back(stoi(line.substr(findSpace(line, p, index) + 1, line.length())));
                else {
                    matrix[r].push_back(stoi(line.substr(findSpace(line, p, index) + 1, findSpace(line, p, index + 1))));
                    index++;
                }
            }
            index = 1;
        }
        int optimal = dynamicIteration(matrix);
        solutions.push_back(optimal);
        titles.push_back(title);
    }
    for (int i = 0; i < iterations; i++) cout << titles[i] << ": " << solutions[i] << endl;

    return 0;
}

