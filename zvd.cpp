#include <iostream>

using namespace std;

int field[7][10] = {
    { -2,  0,  0,  1,  0,  -1,  0, 1, 5, -2},
    { 1,  6,  3,  3,  2,  1,  4,  3,  2,  6},
    { 4,  6,  4,  0,  -2,  -2,  4,  5, 5, 4},
    { -1,  2,  3,  -2,  -2, -2, -2, 6, 0, 5},
    { 1,  2,  4,  3,  -2,  -2,  2, 4, -1, 2},
    { 3,  0,  3,  2,  -1,  6,  0,  5,  1, 1},
    { -2,  -2,  6,  6,  4,  5, 6, 5, -2, -2}
};

int main() {
    for (int i = 0; i <= 6; i++) {
        for (int j=0 ; j <= 9; j++) {
            if(field[i][j]==-2){
               cout<<"  ";
            }
            else if(field[i][j]==-1){
                cout<<"?"<< " ";
            }
            else{
                cout << field[i][j] << " ";}
        }
        cout<<"\n";
    }
    return 0;
}