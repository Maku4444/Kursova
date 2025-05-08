#include <iostream>
#include <vector>
using namespace std;
bool usedCell[7][10] = {false};             
bool usedDominoes[7][7] = {false};  
int solutions = 0;
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
/// ------------------------------------
void printField() {
    for (size_t i = 0; i < field.size(); ++i) {
        for (size_t j = 0; j < field[i].size(); ++j) {
            if (field[i][j] == -2)
                cout << "   ";
            else if (field[i][j] == -1)
                cout << " ? ";
            else
                cout << " " << field[i][j] << " ";
        }
        cout << "\n";
    }
}
/// -------------------------------------
void calculate() {
    bool isPlaced = false;  
    for (int row = 0; row < 7 && !isPlaced; ++row) {  
        for (int col = 0; col < 10 && !isPlaced; ++col) {  
            if (!valid(row, col) || usedCell[row][col]) continue;

            vector<pair<int, int>> directions = {{0, 1}, {1, 0}}; 

            for (auto [deltaRow, deltaCol] : directions) {  
                int row2 = row + deltaRow;  
                int col2 = col + deltaCol;  

                if (!valid(row2, col2) || usedCell[row2][col2]) continue;

                int value1 = field[row][col];  
                int value2 = field[row2][col2];  

                vector<pair<int, int>> availableDominoes;  

                if (value1 != -1 && value2 != -1)
                    availableDominoes.push_back({min(value1, value2), max(value1, value2)});
                else if (value1 == -1 && value2 == -1)
                    availableDominoes = needed_dominoes();
                else {
                    int knownValue = (value1 != -1) ? value1 : value2;  
                    for (int i = 0; i <= 6; ++i)
                        availableDominoes.push_back({min(knownValue, i), max(knownValue, i)});
                }

                for (auto [domino1, domino2] : availableDominoes) {  
                    if (usedDominoes[domino1][domino2]) continue;
                    if (!can_be_domino(row, col, row2, col2, domino1, domino2)) continue;
                    
                    int prevValue1 = value1, prevValue2 = value2;  

                    if (value1 == -1) field[row][col] = (value2 == -1 || value2 == domino2) ? domino1 : domino2;
                    if (value2 == -1) field[row2][col2] = (field[row][col] == domino1) ? domino2 : domino1;

                    usedCell[row][col] = true;
                    usedCell[row2][col2] = true;
                    usedDominoes[domino1][domino2] = true;

                    calculate();

                    field[row][col] = prevValue1;
                    field[row2][col2] = prevValue2;
                    usedCell[row][col] = false;
                    usedCell[row2][col2] = false;
                    usedDominoes[domino1][domino2] = false;
                }

                isPlaced = true;  
            }
        }
    }
    if (!isPlaced) {
        ++solutions;
        cout << "\n Рішення #" << solutions << ":\n";
        printField();
    }
}
///----------------------------------
int main() {
    cout << "Поле доміно:\n";
    printField();
    cout << "\nУсі можливі доміно(28 пар):\n";
    vector<pair<int, int>> my_dominoes = needed_dominoes();
    for (const auto& domino : my_dominoes) {
        cout << domino.first << "-" << domino.second << " ";
    }
    cout << "\n\n Запуск пошуку рішень\n";
    calculate();  
    if (solutions == 0)
        cout << "\n Рішень не знайдено.\n";
    else
        cout << "\n Всього знайдено рішень: " << solutions << "\n";
    return 0;
}