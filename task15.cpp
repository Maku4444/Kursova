/* ------------------------------<Header>------------------------------
 Заголовок: Головоломка з доміно
 Студент:  Джулай Макар Олександрович
 Група:    ТВ-41
 Назва проекту: task15.cpp
 Дата створення: 2025-04-30
 Останнє редагування: 2025-05-15
 Номер завдання: №15
 Опис програми: Мета програми в тому щоб вона розподіляла на заданому по умові полю
                доміно(там де нема чисел треба їх вставити),суть задачі полягає в тому
                щоб програма заповнила все поле,і кожне доміно (від 0-6) повинно попадитись
                лише одноразово.Програма використовує рекурсію та backtracking
 --------------------------------<Header>---------------------------------*/
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#define BORDER -2
#define ROWS 7
#define COLS 10
#define ROUTE 2
using namespace std;
/* ---------------------------------------------------------------------
    Структура Placement яка містить значення доміно(координати обох клітинок(row та col) 
    та значення в них(value))
 ---------------------------------------------------------------------*/
struct Placement {
    int row_0, col_0, row_1, col_1; ///row0-col0-координати першого елемента доміно,row1-col1-координати другого елемента
    int value_0, value_1; ///значення для першого та другого елемента доміно
};
/* ---------------------------------------------------------------------
    Класс Domino_pazzle-реалізує роботу нашої програми
 ---------------------------------------------------------------------*/
class Domino_pazzle {
    bool usedCell[ROWS][COLS] = {};   
    bool usedDomino[7][7]= {};    
    vector<vector<int>> field_or;
    vector<vector<int>> field;
    vector<Placement> placements;
public:
/* ---------------------------------------------------------------------
   Тут оголошено конструктор класу DominoPazzle,і одразу в списку ініціалізації
   задається початкове значення для поля-члена field.
 ---------------------------------------------------------------------*/
    Domino_pazzle()
        : field_or{
            { -2,  0,  0,  1,  0,  -1,  0,  1, 5, -2},
            {  1,  6,  3,  3,  2,   1,  4,  3, 2,  6},
            {  4,  6,  4,  0,  -2,  -2, 4,  5, 5,  4},
            { -1,  2,  3,  -2, -2,  -2, -2, 6, 0,  5},
            {  1,  2,  4,   3,  -2,  -2, 2,  4, -1, 2},
            {  3,  0,  3,   2,  -1,   6, 0,  5, 1,  1},
            { -2, -2,  6,   6,   4,   5, 6,  5, -2, -2}}
            
    {field=field_or;}
/* ---------------------------------------------------------------------
   Окрема функція для виводу початкової умови або виводу поля в input()
 ---------------------------------------------------------------------*/
    void print_field(int use_field = 0) {
    if (use_field != 0) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (field[i][j] == -2) cout << "  ";
                else if (field[i][j] == -1) cout << "? ";
                else cout << field[i][j] << " ";
            }
            cout << "\n";
        }
    } else {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (field_or[i][j] == -2) cout << "  ";
                else if (field_or[i][j] == -1) cout << "? ";
                else cout << field_or[i][j] << " ";
            }
            cout << "\n";
        }
    }
}
/* ---------------------------------------------------------------------
   Функція для скидання всього поля до оригінального виду
 ---------------------------------------------------------------------*/
void reset_for_solve() {
        placements.clear();
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                usedCell[i][j] = false;
            }
        }
        for (int i = 0; i <= 6; ++i) {
            for (int j = 0; j <= 6; ++j) {
                usedDomino[i][j] = false;
            }
        }
        field = field_or;
    }
/* ---------------------------------------------------------------------
   Функція для заповнювання користувачем порожніх клітинок на полі
 ---------------------------------------------------------------------*/
void input() {
        cout << "\nВведіть значення для клітинок з '?', де необхідно замінити на числа (0-6):\n";
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                if (field[row][col] == -1) {
                    int input;
                    cout << "Клітинка (" << row << "-" << col << "): ";
                    while (!(cin >> input) || input < 0 || input > 6) {
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cout << "Невірне значення. Введіть число від 0 до 6: ";
                    }
                    field[row][col] = input;
                    print_field(1); 
                }
            }
        }
    }
/* ---------------------------------------------------------------------
   Перевіряє чи елемент входить в межі поля
 ---------------------------------------------------------------------*/
    bool valid(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS && field[r][c] != BORDER;
}
/* ---------------------------------------------------------------------
   Ця функція перевіряє, чи можна в даний момент покласти одну кістку доміно (зі значеннями domino0 та domino1)
   Зокрема вона робить три перевірки:
   -Ділянки поля існують і не заблоковані
   -Клітинки сусідні
   -Значення в клітинках відповідають кінцям доміно
 ---------------------------------------------------------------------*/
    bool can_be_domino(int row_1, int col_1, int row_2, int col_2, int domino_0, int domino_1) {
        if (!valid(row_1, col_1) || !valid(row_2, col_2)) return false;
        if (!((row_1==row_2 && abs(col_1-col_2)==1)||(col_1==col_2 && abs(row_1-row_2)==1))) return false;
        int f1 = field[row_1][col_1], f2 = field[row_2][col_2];
        return (f1==-1 && f2==-1) ||
               (f1==-1 && (f2==domino_0||f2==domino_1)) ||
               (f2==-1 && (f1==domino_0||f1==domino_1)) ||
               (f1==domino_0 && f2==domino_1) ||
               (f1==domino_1 && f2==domino_0);
    }
    /* ---------------------------------------------------------------------
   Виводить наше поле в коректній формі(з'єднує пари чисел в доміно щоб користувачу було наглядно видно роботу програми)
 ---------------------------------------------------------------------*/
    void print_field_with_domino_connections() {
        vector<vector<bool>> horiz(ROWS, vector<bool>(COLS,false));
        vector<vector<bool>> vert (ROWS, vector<bool>(COLS,false));

        for (auto &p : placements) {
        if (p.row_0 == p.row_1) {
            int r = p.row_0;
            int c = min(p.col_0, p.col_1);
            horiz[r][c] = true;
        } else {
            int r = min(p.row_0, p.row_1);
            int c = p.col_0;
            vert[r][c] = true;
        }
    }

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            int v = field[r][c];
            if (v == BORDER) {
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
                if (field[r][c] == BORDER) {
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
                static const int dr[ROUTE] = {0,1}, dc[ROUTE] = {1,0};
                for (int d = 0; d < ROUTE; ++d) {
                    int r2 = r + dr[d], c2 = c + dc[d];
                    if (!valid(r2,c2) || usedCell[r2][c2]) continue;

                    int v1 = field[r][c], v2 = field[r2][c2];
                    vector<pair<int,int>> choices;
                    if (v1!=-1 && v2!=-1) {
                        choices.emplace_back(min(v1,v2), max(v1,v2));
                    }
                    else if (v1==-1 && v2==-1) {
                        for (int i = 0; i <= 6; ++i)
                            for (int j = i; j <= 6; ++j){
                                choices.emplace_back(i,j);
                            }
                    }
                    else {
                        int known;
                        if (v1 != -1){
                            known = v1;
                        } 
                        else {
                            known = v2;
                        }
                        for (int x = 0; x <= 6; ++x) {
                            choices.emplace_back(min(known, x), max(known, x));
                        }
                    }
                    for (auto [d0,d1] : choices) {
                        if (usedDomino[d0][d1]) continue;
                        if (!can_be_domino(r,c,r2,c2,d0,d1)) continue;

                        int prev1 = field[r][c], prev2 = field[r2][c2];
                        if (field[r][c] == -1) {
                          if (prev2 == -1 || prev2 == d1) {
                            field[r][c] = d0;
                        } 
                        else {
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
                        placements.push_back({r, c, r2, c2, field[r][c], field[r2][c2]});

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
        cout <<"\n"<<setw(40)<<"=== Рішення ===\n\n";
        print_field_with_domino_connections();
        cout << "\nСписок доміно:\n";
        for (auto &p : placements) {
            cout << "("<<p.row_0<<"-"<<p.col_0<<")="<<p.value_0
                 <<" <-> "
                 <<"("<<p.row_1<<"-"<<p.col_1<<")="<<p.value_1<<"\n";
        }
        return true;
    }
};

int main() {
    cout <<" _____________________________________________________________________________________\n"
        "|Заголовок: Головоломка з доміно                                                      |\n"<<
        "|Студент: Джулай Макар Олександрович                                                  |\n"<<
        "|Група: ТВ-41                                                                         |\n"<<
        "|Назва проекту: task15.cpp                                                            |\n"<<
        "|Номер завдання: №15                                                                  |\n"<<
        "|Дата створення: 2025-04-30                                                           |\n"<<
        "|Останнє редагування: 2025-05-15                                                      |\n"<<
        "|Опис програми: Мета програми в тому щоб вона розподіляла на заданому по умові полю   |\n"<<
        "|доміно(там де нема чисел треба їх вставити),суть задачі полягає в тому               |\n"<<
        "|щоб програма заповнила все поле,і кожне доміно (від 0-6) повинно попадитись          |\n"<<
        "|лише одноразово.Програма використовує рекурсію та backtracking                       |\n"<<
        "|_____________________________________________________________________________________|\n";
    Domino_pazzle pazzle;
    int choice;
    do {
        cout << "\nМеню:\n";
        cout << "1 - Запустити розв'язок\n";
        cout << "2 - Ввести значення для '?' і вирішити з ними \n";
        cout << "3 - Вивести початкове поле\n";
        cout << "4 - Вийти\n";
        cout << "Ваш вибір: ";
        if(!(cin>>choice)){
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); /// Ігнорування всіх не чисельних символів
            cout << "Невірний ввід. Введіть число (від 1 до 4): ";
        }
        switch(choice) {
            case 1: {
                pazzle.reset_for_solve();
                if(!pazzle.solve())
                    cout << "Розв'язок не знайдено\n";
                break;}
            case 2:
                pazzle.reset_for_solve();
                pazzle.input();
                if(!pazzle.solve())
                    cout << "Розв'язок не знайдено\n";
                break;
            case 3:
                pazzle.print_field();
                break;
            case 4:
                cout << "Вихід з програми.\n";
                break;
            default:
                cout << "Неправильний вибір. Спробуйте ще раз.\n";
        }
    } while(choice!=4);
    return 0;
}
