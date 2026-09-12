#include <iostream>
#include <vector>
using namespace std;


int row,col;
int map[210][210];
vector<vector<pair<int,int>>> mov_lists = {
    {
        {0,0}, {0,1}, {0,2}, {0,3}
    },
    {
        {0,0}, {0,1}, {1,0}, {1,1}
    },
    {
        {0,0}, {1,0}, {2,0}, {2,1}
    },
    {
        {0,0}, {1,0}, {1,1}, {2,1}
    },
    {
        {0,0}, {1,0}, {2,0}, {1,1}
    }
};


int result = -1;


void rotate(){
    int temp_map[210][210] = {};
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j++){
            temp_map[col - 1 - j][i] = map[i][j]; // (새로운 행 = 기존 행)
        }
    }
    for(int i = 0; i < 210; i ++){
        for(int j = 0; j < 210; j++){
            map[i][j] = temp_map[i][j];
        }
    }
    int temp = row;
    row = col;
    col = temp;
    // cout << "=========\n";
    // for(int i = 0; i < row; i ++){
    //     for(int j = 0; j < col; j++){
    //         cout << map[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
}

void flip(){
    int temp_map[210][210] = {};
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j++){
            temp_map[i][col - 1 - j] = map[i][j]; // (새로운 행 = 기존 행)
        }
    }
    for(int i = 0; i < 210; i ++){
        for(int j = 0; j < 210; j++){
            map[i][j] = temp_map[i][j];
        }
    }
    // for(int i = 0; i < row; i ++){
    //     for(int j = 0; j < col; j++){
    //         cout << map[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
}

int main() {
    // Please write your code here.


    cin >> row >> col;
    for(int i = 0; i < row; i ++){
        for(int j = 0; j < col; j++){
            cin >> map[i][j];
        }
    }
    for(int i = 0; i < 4; i++){

        for(int j = 0; j < row; j++){ // 실제 인덱스 정리
            for(int k = 0; k < col; k++){
                
                for(auto mov_list : mov_lists){
                    int sum = 0;
                    for(int way = 0 ; way < 4; way++){
                        //if(j + mov_list[way].first >= row || k + mov_list[way].second >= col) continue;
                        sum += map[j + mov_list[way].first][k+ mov_list[way].second];

                        if(result < sum){
                            result = sum;
                        }
                    }
                }
            }
        }
        rotate();
    }
    flip();
    for(int i = 0; i < 4; i++){

        for(int j = 0; j < row; j++){ // 실제 인덱스 정리
            for(int k = 0; k < col; k++){
                
                for(auto mov_list : mov_lists){
                    int sum = 0;
                    for(int way = 0 ; way < 4; way++){
                        //if(j + mov_list[way].first >= row || k + mov_list[way].second >= col) continue;
                        sum += map[j + mov_list[way].first][k+ mov_list[way].second];

                        if(result < sum){
                            result = sum;
                        }
                    }
                }
            }
        }
        rotate();
    }
    
    cout << result << endl;
    return 0;
}