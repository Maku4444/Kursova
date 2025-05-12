/* ------------------------------<Header>------------------------------
 Заголовок: Головоломка з доміно
 Студент:  Джулай Макар Олександрович
 Група:    ТВ-41
 Опис програми: Мета програми в тому щоб вона розподіляла на заданому по умові полю
                доміно(там де нема чисел треба їх вставити),суть задачі полягає в тому
                щоб програма заповнила все поле,і кожне доміно (від 0-6) повинно попадитись
                лише одноразово.Програма використовує рекурсію та backtracking
 --------------------------------<Header>---------------------------------*/
#include <iostream>
#include <vector>
#include <cmath>
#define border -2
using namespace std;
/* ---------------------------------------------------------------------
    Структура Placement яка містить значення доміно(координати обох клітинок(row та col) 
    та значення в них(value))
 ---------------------------------------------------------------------*/
struct Placement {
    int row0, col0, row1, col1;
    int value0, value1;
};
/* ---------------------------------------------------------------------
    Класс DominoPazzle-реалізує роботу нашої програми
 ---------------------------------------------------------------------*/
class DominoPazzle {
    static const int ROWS = 7;
    static const int COLS = 10;
    bool usedCell[ROWS][COLS] = {};   
    bool usedDomino[7][7]= {};    
    vector<vector<int>> field;
       
    vector<Placement> placements;

public:
/* ---------------------------------------------------------------------
   Тут оголошено конструктор класу DominoPazzle,і одразу в списку ініціалізації
   задається початкове значення для поля-члена field.
 ---------------------------------------------------------------------*/
    DominoPazzle()
        : field{
            { -2,  0,  0,  1,  0,  -1,  0,  1, 5, -2},
            {  1,  6,  3,  3,  2,   1,  4,  3, 2,  6},
            {  4,  6,  4,  0,  -2,  -2, 4,  5, 5,  4},
            { -1,  2,  3,  -2, -2,  -2, -2, 6, 0,  5},
            {  1,  2,  4,   3,  -2,  -2, 2,  4, -1, 2},
            {  3,  0,  3,   2,  -1,   6, 0,  5, 1,  1},
            { -2, -2,  6,   6,   4,   5, 6,  5, -2, -2}}
            
    {}
    vector<vector<int>> field2=field; 
/* ---------------------------------------------------------------------
   Окрема функція для виводу початкової умови
 ---------------------------------------------------------------------*/
    void print_field() {    
    for ( int i = 0; i < field2.size(); ++i) {
        for (int j = 0; j < field2[i].size(); ++j) {
            if(field2[i][j] == -2) {
                cout << "  ";
            }
            else if(field2[i][j] == -1) {
                cout << "? ";
            }
            else {
                cout << field2[i][j] << " ";
            }
        }
        cout << "\n";
    }
}
/* ---------------------------------------------------------------------
   Перевіряє чи елемент входить в межі поля
 ---------------------------------------------------------------------*/
    bool valid(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS && field[r][c] != border;
}
/* ---------------------------------------------------------------------
   Ця функція перевіряє, чи можна в даний момент покласти одну кістку доміно (зі значеннями domino0 та domino1)
   Зокрема вона робить три перевірки:
   -Ділянки поля існують і не заблоковані
   -Клітинки сусідні
   -Значення в клітинках відповідають кінцям доміно
 ---------------------------------------------------------------------*/
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
    /* ---------------------------------------------------------------------
   Виводить наше поле в коректній формі(з'єднує пари чисел в доміно щоб користувачу було наглядно видно роботу програми)
 ---------------------------------------------------------------------*/
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
            if (v == border) {
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
                if (field[r][c] == border) {
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
/* ---------------------------------------------------------------------
   Функція solve() послідовно перебирає всі незайняті клітинки поля й 
   для кожної намагається поставити одну з можливих кісток доміно вправо або вниз.
 ---------------------------------------------------------------------*/
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
                        int known;
                        if (v1 != -1) {known = v1;} 
                        else {known = v2;}
                        for (int x = 0; x <= 6; ++x) {
                            choices.emplace_back(min(known, x), max(known, x));
                        }
                           }
                    for (auto [d0,d1] : choices) {
                        if (usedDomino[d0][d1]) continue;
                        if (!canBeDomino(r,c,r2,c2,d0,d1)) continue;

                        int prev1 = field[r][c], prev2 = field[r2][c2];
                        if (field[r][c] == -1) {
                          if (prev2 == -1 || prev2 == d1) {
                            field[r][c] = d0;
                        } else {
                            field[r][c] = d1;
                        }
                       }
                        if (field[r2][c2] == -1) {
                          if (field[r][c] == d0) {
                            field[r2][c2] = d1;
                        } else {
                            field[r2][c2] = d0;
                        }
                    }

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
    DominoPazzle pazzle;
    int choice;
    do {
        cout << "\nМеню:\n";
        cout << "1 - Запустити розв'язок\n";
        cout << "2 - Вивести початкове поле\n";
        cout << "3 - Вийти\n";
        cout << "Ваш вибір: ";
        if(!(cin>>choice)){
            cin.clear();
        }
        switch(choice) {
            case 1: {
                if(!pazzle.solve())
                    cout << "Розв'язок не знайдено\n";
                break;
            }
            case 2:
                pazzle.print_field();
                break;
            case 3:
                cout << "Вихід з програми.\n";
                break;
            default:
                cout << "Неправильний вибір. Спробуйте ще раз.\n";
        }
    } while(choice!=3);
    return 0;
}
