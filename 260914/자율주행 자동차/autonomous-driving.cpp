#include <iostream>
using namespace std;

bool visit[51][51];
int map[51][51];

int direction; // 북 동 남 서;
int change_count; // 몇번 회전을 시도했는가? 일단 없어도 되긴 할듯

int row, col;
int total;

int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};


void print_map(){
    cout << "======map:\n";
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < col ; j++){
            cout <<  map[i][j] << " ";
        }
        cout << "\n";
    }
}

void print_visit(){
    cout << "======visit:\n";
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < col ; j++){
            cout <<  visit[i][j] << " ";
        }
        cout << "\n";
    }
}


int main() {
    cin >> row >> col;  
    int start_x, start_y;
    
    cin >> start_x >> start_y >> direction;
    visit[start_x][start_y] = true;
    total++;

    for(int i = 0; i < row ; i++){
        for(int j = 0; j < col ; j++){
            cin >> map[i][j];
        }
    }
    //print_map();
    //print_visit();
    while(1){
        //cout << "count : " << change_count << endl;
        //print_map();
        if(change_count == 4){
            //cout << "count : " << change_count << endl;
            // 후진을 해야하는데, 만약에 해당 후진 지점이 도보면 out
            int temp_x = start_x + dx[(direction+2) % 4];
            int temp_y = start_y + dy[(direction+2) % 4]; // 후진

            if(map[temp_x][temp_y]) break;
            start_x = temp_x;
            start_y = temp_y;
            change_count = 0;
        }
        change_count++;

        int now_direction = (direction - change_count + 4) % 4;

        int now_x = start_x + dx[now_direction];
        int now_y = start_y + dy[now_direction];
    
        if(now_x < 0 || now_x >= row || now_y  < 0 || now_y >= col) continue; // 범위를 벗어난 경우
        if(visit[now_x][now_y] == true) continue;  // 이미 방문한 경우
        if(map[now_x][now_y]) continue; // 해당 지점이 인도인 경우

        //여기까지 왔다는것은 실제로 넘어갈 수 있다는 것
        change_count = 0; // 초기화
        direction = now_direction;
        start_x = now_x;
        start_y = now_y;
        // 해당 내용으로 다 동기화
        visit[start_x][start_y] = true; // 해당 지점에 방문 표시
        //cout << "dirction : " << direction;
        //print_visit();
        total++; // 실제 이동 


    }

    //print_visit();
    cout << total;
    return 0;
}