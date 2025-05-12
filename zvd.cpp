#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Placement {
    int row0, col0, row1, col1;
    int value0, value1;
};

class DominoPazzle {
    static const int ROWS = 7;
    static const int COLS = 10;
    bool usedCell[ROWS][COLS] = {};   
    bool usedDomino[7][7]= {};    
    vector<vector<int>> field;
    vector<Placement> placements;

public:
    DominoPazzle()
        : field{
            { -2,  0,  0,  1,  0,  -1,  0,  1, 5, -2},
            {  1,  6,  3,  3,  2,   1,  4,  3, 2,  6},
            {  4,  6,  4,  0,  -2,  -2, 4,  5, 5,  4},
            { -1,  2,  3,  -2, -2,  -2, -2, 6, 0,  5},
            {  1,  2,  4,   3,  -2,  -2, 2,  4, -1, 2},
            {  3,  0,  3,   2,  -1,   6, 0,  5, 1,  1},
            { -2, -2,  6,   6,   4,   5, 6,  5, -2, -2}
        }
    {
    }

    bool valid(int r, int c) {
    return r >= 0 && r < 7 && c >= 0 && c < 10 && field[r][c] != -2;
}

    bool canBeDomino(int row1, int col1, int row2, int col2, int domino0, int domino1) {
        if (!valid(row1, col1) || !valid(row2, col2)) return false;
        if (!((row1==row2 && abs(col1-col2)==1)||(col1==col2 && abs(row1-row2)==1))) return false;
        int f1 = field[row1][col1], f2 = field[row2][col2];
        return (f1==-1 && f2==-1) ||
               (f1==-1 && (f2==domino0||f2==domino1)) ||
               (f2==-1 && (f1==domino0||f1==domino1)) ||
               (f1==domino0 && f2==domino1) ||
               (f1==domino1 && f2==domino0);
    }

    void printFieldWithDominoConnections() {
        vector<vector<bool>> horiz(ROWS, vector<bool>(COLS,false));
        vector<vector<bool>> vert (ROWS, vector<bool>(COLS,false));

        for (auto &p : placements) {
        if (p.row0 == p.row1) {
            int r = p.row0;
            int c = min(p.col0, p.col1);
            horiz[r][c] = true;
        } else {
            int r = min(p.row0, p.row1);
            int c = p.col0;
            vert[r][c] = true;
        }
    }

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            int v = field[r][c];
            if (v == -2) {
                cout << "   ";
            } else {
                cout << ' ' << v << ' ';
            }
            if (c + 1 < COLS) {
                if (horiz[r][c]) cout << "--";
                else cout << "  ";
            }
        }
        cout << "\n";


        if (r + 1 < ROWS) {
            for (int c = 0; c < COLS; ++c) {
                if (field[r][c] == -2) {
                    cout << "   ";
                } else {
                    if (vert[r][c]) cout << " | ";
                    else            cout << "   ";
                }
                if (c + 1 < COLS) cout << "  ";
            }
            cout << "\n";
        }
    }
    }

    bool solve() {
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                if (!valid(r,c) || usedCell[r][c]) continue;
                static const int dr[2] = {0,1}, dc[2] = {1,0};
                for (int d = 0; d < 2; ++d) {
                    int r2 = r + dr[d], c2 = c + dc[d];
                    if (!valid(r2,c2) || usedCell[r2][c2]) continue;

                    int v1 = field[r][c], v2 = field[r2][c2];
                    vector<pair<int,int>> choices;
                    if (v1!=-1 && v2!=-1) {
                        choices.emplace_back(min(v1,v2), max(v1,v2));
                    }
                    else if (v1==-1 && v2==-1) {
                        for (int i = 0; i <= 6; ++i)
                            for (int j = i; j <= 6; ++j)
                                choices.emplace_back(i,j);
                    }
                    else {
                        int known = (v1 != -1 ? v1 : v2);
                        for (int x = 0; x <= 6; ++x)
                            choices.emplace_back(min(known, x), max(known, x));
                    }

                    for (auto [d0,d1] : choices) {
                        if (usedDomino[d0][d1]) continue;
                        if (!canBeDomino(r,c,r2,c2,d0,d1)) continue;

                        int prev1 = field[r][c], prev2 = field[r2][c2];
                        if (field[r][c] == -1)
                            field[r][c] = (prev2 == -1 || prev2 == d1) ? d0 : d1;
                        if (field[r2][c2] == -1)
                            field[r2][c2] = (field[r][c] == d0 ? d1 : d0);

                        usedCell[r][c]     = true;
                        usedCell[r2][c2]   = true;
                        usedDomino[d0][d1] = true;
                        placements.push_back({r,c,r2,c2,d0,d1});

                        if (solve()) return true;
                        placements.pop_back();
                        usedCell[r][c]     = false;
                        usedCell[r2][c2]   = false;
                        usedDomino[d0][d1] = false;
                        field[r][c]        = prev1;
                        field[r2][c2]      = prev2;
                    }
                }
                return false;
            }
        }
        cout << "\n=== Перше рішення ===\n\n";
        printFieldWithDominoConnections();
        cout << "\nСписок доміно:\n";
        for (auto &p : placements) {
            cout << "("<<p.row0<<" "<<p.col0<<")="<<p.value0
                 <<" <-> "
                 <<"("<<p.row1<<" "<<p.col1<<")="<<p.value1<<"\n";
        }
        return true;
    }
};

int main() {
    DominoPazzle solver;
    if (!solver.solve())
        cout << "Розв'язок не знайдено\n";
    return 0;
}
