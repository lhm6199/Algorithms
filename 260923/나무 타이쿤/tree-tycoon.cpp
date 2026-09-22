#include <iostream>

using namespace std;


int trees[20][20];
int year;
int N;
bool nutri[20][20]; // 영양제


void print_trees(){
    cout << "==============\n";
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << trees[i][j] << " ";
        }
         cout << "\n";
    }
}

void print_nutri(){
    cout << "======nut========\n";
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << nutri[i][j] << " ";
        }
         cout << "\n";
    }
}
int dx_8[8] = {0,-1,-1,-1,0,1,1,1};
int dy_8[8] = {1,1,0,-1,-1,-1,0,1};

int dia_x[4] = {1,1,-1,-1};
int dia_y[4] = {1,-1,1,-1}; // 대각선 방향

bool out_range(int dx, int dy){
    return dx < 0 || dx >= N || dy < 0 || dy >= N;
}


void move_nutri(int dir, int how_many){
    //cout << "okay\n";
    //bool visit[20][20] = {};
    bool temp[20][20] = {};
    for(int i = 0; i < N; i++){
        for(int j = 0 ; j < N; j++){
            
            if(nutri[i][j] == true) {
                nutri[i][j] = false; // false로 전환
                int now_i = i + how_many * dx_8[dir];
                int now_j = j + how_many * dy_8[dir]; // 방향 만큼 이동
                //cout << now_i << " i " << now_j << " j \n";
                while(now_i < 0){
                    now_i += N;
                }
                while(now_j < 0){
                    now_j += N;
                }
                now_i = now_i % N;
                now_j = now_j % N; // 이렇게 범위 좁히기
                //nutri[now_i][now_j] = true;
                temp[now_i][now_j] = true;
                //cout << "okay4\n";
                //visit[now_i][now_j] = true; // 해당 지점은 이미 바뀐 구간으로 표시
                //cout << "okay5\n";
            }
        }
    }
    for(int i = 0; i < N ; i++){
        for(int j = 0; j <N; j++){
            nutri[i][j] = temp[i][j];
        }
    }
    //cout << "okay2\n";
    //print_nutri();
}

void grow_trees(){
    for(int i = 0; i < N; i++){
        for(int j = 0 ; j < N; j++){
            if(nutri[i][j]) {
                trees[i][j]++;
                //nutri[i][j] = tru; // 이미 영양제 준곳은 out
            }
        }
    }
    //print_trees();
}

void dia_grow(){
// print_nutri();
    for(int i = 0; i < N; i++){
        for(int j = 0 ; j < N; j++){
            if(nutri[i][j]) {
                for(int k = 0; k < 4; k++){
                    int now_x = i + dia_x[k];
                    int now_y = j + dia_y[k];
                    if(out_range(now_x,now_y)) continue;// 바깥인경우
                    
                    if(trees[now_x][now_y] > 0){
                        trees[i][j]++; // 대각선 방향으로 tree 올리기
                    }
                    

                }
            }
        }
    }
    //print_trees();

}

void new_nutri(){

    bool temp[20][20] = {};
    for(int i = 0; i < N ; i++){
        for(int j = 0; j <N; j++){
            if(nutri[i][j]) continue; // 이미 영양제를 준곳
            if(trees[i][j] >= 2){
                trees[i][j] = trees[i][j] - 2; // 2만큼 잘라냄
                temp[i][j] = true;
            }
        }
    }
    for(int i = 0; i < N ; i++){
        for(int j = 0; j <N; j++){
            nutri[i][j] = temp[i][j];
        }
    }
    //print_nutri();
    //print_nutri();
}


int main(){
    cin >> N >> year;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cin >> trees[i][j];
        }
    }
    //print_trees();

    nutri[N-2][0] = true;
    nutri[N-2][1] = true;
    nutri[N-1][0] = true;
    nutri[N-1][1] = true;
    //print_nutri();
    for(int i = 0; i < year ; i++){
        //cout << "start new year!" << " \n";
        //print_trees();
        int dir, how_many;
        cin >> dir >> how_many;
        //print_nutri();
        //cout << "0\n";
        move_nutri(dir-1, how_many);
        //print_nutri();
        //print_trees();
        //cout << "1\n";
        grow_trees();
        //cout << "2\n";
        dia_grow();
        //cout << "3\n";
        new_nutri();

    }
    int result = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            result += trees[i][j];
        }
    }
    cout << result << "\n";
}