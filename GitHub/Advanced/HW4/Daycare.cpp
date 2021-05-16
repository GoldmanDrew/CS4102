#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <string>
#include <climits>
using namespace std;

// I collaborated with Jaden Kyler-Wank on the overall approach to solving this algorithm

vector<vector<int>> rms;

bool cmp(const vector<int> &l1, const vector<int> &l2)
{
    int d2 = l2[1] - l2[0];
    int d1 = l1[1] - l1[0];

    bool b1 = l1[0] < l2[0];
    bool b2 = l1[1] > l2[1];
    bool b3 = d1 > d2;

    if (d2 == d1)
    {
        if (d1 > 0)
            return b1;
        else if (d1 <= 0)
            return b2;
    }
    else if (d2 < 0 && d1 < 0)
        return b2;
    return b3;
}

int daycare(int ind, int ans)
{
    int size = rms.size();
    int delta = 0;
    if (0 == ind)
    {
        ans += rms[ind][0];
        rms[ind][0] = 0;
        return daycare(1 + ind, ans);
    }
    else if (size == ind)
        return ans;
    int move = rms[ind][0];
    for (int i = 0; i < ind; i++)
    {
        while (!(move <= 0) && !(rms[i][0] >= rms[i][1]))
        {
            move--;
            delta++;
            rms[i][0]++;
        }
    }
    rms[ind][0] -= delta;
    ans += rms[ind][0];
    rms[ind][0] = 0;

    return daycare(1 + ind, ans);
}

int main()
{
    string l;
    while (true)
    {
        string input;
        getline(cin, input);
        if (strlen(l.data()) == 0 && strlen(input.data()) == 0)
            break;
        int n = stoi(input);
        rms.resize(n, vector<int>(2, 0));
        for (int i = 0; i < n; i++)
        {
            int current;
            cin >> current;
            int prime;
            cin >> prime;

            rms[i][1] = prime;
            rms[i][0] = current;
        }
        sort(rms.begin(), rms.end(), cmp);

        cout << daycare(n * 0, n * 0) << endl;
        rms.clear();
        getline(cin, l);
    }
}