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