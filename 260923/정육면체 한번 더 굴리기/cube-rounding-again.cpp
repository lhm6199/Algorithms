#include <iostream>
#include <queue>
using namespace std;

int dice[7];

int map[100][100];
int N;
int dice_rotate;
int result;
int dir; // 주사위의 회전 방향을 나타내는 것 0 오, 1 아래, 2 왼 , 3 위

int now_dice_x;
int now_dice_y;

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};


void change_dice(){
    int temp[7] = {};
    if(dir == 0){
        temp[1] = dice[4];
        temp[2] = dice[2];
        temp[3] = dice[1];
        temp[4] = dice[6];
        temp[5] = dice[5];
        temp[6] = dice[3];

    }
    else if(dir == 1){

        temp[1] = dice[5];
        temp[2] = dice[1];
        temp[3] = dice[3];
        temp[4] = dice[4];
        temp[5] = dice[6];
        temp[6] = dice[2];
    }
    else if(dir == 2){
        temp[1] = dice[3];
        temp[2] = dice[2];
        temp[3] = dice[6];
        temp[4] = dice[1];
        temp[5] = dice[5];
        temp[6] = dice[4];
    }
    else if(dir == 3){
        temp[1] = dice[2];
        temp[2] = dice[6];
        temp[3] = dice[3];
        temp[4] = dice[4];
        temp[5] = dice[1];
        temp[6] = dice[5];
    }
    for(int i = 1 ; i <=6 ;i++){
        dice[i] = temp[i];
    }
}

void cal_sum(int x, int y){
    int count = 0;
    int target_num = map[x][y]; // 목표 숫자
    bool visit[100][100] = {}; // 방문 배열
    queue<pair<int,int>> q;
    q.push({x,y});
    visit[x][y] = true;
    count++;
    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();

        for(int i = 0; i < 4; i++){
            int now_x = now.first + dx[i];
            int now_y = now.second + dy[i];

            if(now_x < 0 || now_x >= N || now_y < 0 || now_y >= N) continue;
            if(visit[now_x][now_y] == true) continue; // 이미 방문한 경우
            if(map[now_x][now_y] != target_num) continue; // target과 같지 않은 경우 넘어감
            // 그렇지 않으면
            visit[now_x][now_y] = true;
            q.push({now_x,now_y}); // 해당 내용 큐에 넣기
            count++;
        }


    }
    result = result + count * target_num;
    //cout << result << "\n";

    // for(int i = 0; i < N; i++){
    //     for(int j = 0; j < N; j++){
    //         cout << visit[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

}

void change_dir(){
    if(map[now_dice_x][now_dice_y] < dice[6]){
        // 90도 회전
        dir = (dir + 1) % 4;
    }
    else if(map[now_dice_x][now_dice_y] > dice[6]){
        dir = (dir - 1 + 4) % 4;
    }
}





int main(){
    cin >> N >> dice_rotate;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N ; j++){
            cin >> map[i][j];
        }
    }

    dir = 0;
    now_dice_x = 0;
    now_dice_y = 0;

    for(int i = 1; i < 7; i++){
        dice[i] = i;
    }

    for(int i = 0; i < dice_rotate; i++){
        now_dice_x = now_dice_x + dx[dir];
        now_dice_y = now_dice_y + dy[dir]; // 주사위 부터 이동
        // 격자를 벗어나는 경우
        if(now_dice_x < 0){
            now_dice_x = 1;
            dir = (dir + 2) % 4; // 반대로 변환
        }
        else if(now_dice_x >= N){
            now_dice_x = N-2;
            dir = (dir + 2) % 4;
        }
        if(now_dice_y < 0){
            now_dice_y = 1;
            dir = (dir + 2) % 4;
        }
        else if(now_dice_y >= N){
            now_dice_y = N-2;
            dir = (dir + 2) % 4;
        }



        //cout << now_dice_x << " x " << now_dice_y << " y \n";
        
        

        change_dice(); // 주사위 밑면 변환
        //cout << dice[6] << " dice " << " dir : " << dir << "\n";
        cal_sum(now_dice_x, now_dice_y); // bfs로 해야하네
        change_dir(); 
        

    }
    cout << result << "\n";
    // map 생성


}