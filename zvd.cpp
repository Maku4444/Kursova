#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> field = {
    { -2,  0,  0,  1,  0,  -1,  0, 1, 5, -2},
    { 1,  6,  3,  3,  2,  1,  4,  3,  2,  6},
    { 4,  6,  4,  0,  -2,  -2,  4,  5, 5, 4},
    { -1,  2,  3,  -2,  -2, -2, -2, 6, 0, 5},
    { 1,  2,  4,  3,  -2,  -2,  2, 4, -1, 2},
    { 3,  0,  3,  2,  -1,  6,  0,  5,  1,  1},
    { -2,  -2,  6,  6,  4,  5, 6, 5, -2, -2}
};

vector<pair<int, int>> needed_dominoes() {
    vector<pair<int, int>> dominoes;
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            dominoes.push_back({i, j});
        }
    }
    return dominoes;
}
///-----------------
bool valid(int r, int c) {
    return r >= 0 && r < field.size() && c >= 0 && c < field[r].size() && field[r][c] != -2;
}

bool can_be_domino(int r1, int c1, int r2, int c2, int val1, int val2) {

    if (!valid(r1, c1) || !valid(r2, c2)) {
        return false;
    }

    if (!((r1 == r2 && abs(c1 - c2) == 1) || (c1 == c2 && abs(r1 - r2) == 1))) {
        return false;
    }

    int field1 = field[r1][c1];
    int field2 = field[r2][c2];

    return (field1 == -1 && field2 == -1) ||
           (field1 == -1 && field2 == val1) ||
           (field1 == -1 && field2 == val2) ||
           (field1 == val1 && field2 == -1) ||
           (field1 == val2 && field2 == -1) ||
           (field1 == val1 && field2 == val2) ||
           (field1 == val2 && field2 == val1);
}
///----------------
 
int main() {
    ///--------------
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            if(field[i][j] == -2) {
                cout << "  ";
            }
            else if(field[i][j] == -1) {
                cout << "? ";
            }
            else {
                cout << field[i][j] << " ";
            }
        }
        cout << "\n";
    }
    ///-----------
    cout << "\nУсі можливі доміно(За умовою):\n";
    vector<pair<int, int>> my_dominoes = needed_dominoes();
    for (const auto& domino : my_dominoes) {
        cout << domino.first << "-" << domino.second << " ";
    }
    cout << endl;
    ///------------


    return 0;
}