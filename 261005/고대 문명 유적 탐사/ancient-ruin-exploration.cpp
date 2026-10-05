#include <iostream>
#include <queue>

using namespace std;

int map[10][10];
int wall[1000];
int wall_idx =0 ;
int turns, total_wall; // 턴, 벽면에 있는 유물의 개수

int dx[4] = {1,0,-1,0};
int dy[4] = {0,1,0,-1};
int temp_map[10][10] = {};

void print_map(){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

bool visit[10][10] = {}; // 얘는 전역적으로 쓸거니까, 다 쓰고 초기화 반드시 필요

int count_nums(int x, int y){
    int total = 1;

    queue<pair<int,int>> q;
    
    q.push({x,y});
    visit[x][y] = true;
    int target_num = temp_map[x][y];

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0 ; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i]; 
            if(nx < 0 || nx >= 5 || ny < 0 || ny >= 5) continue; // 범위 벗어남
            if(visit[nx][ny] == true) continue;
            if(target_num != temp_map[nx][ny]) continue;
            // 그게 아니면 q에 추가
            total++;
            q.push({nx,ny});
            visit[nx][ny] = true;
        }
    }
    //cout << total << " total \n";
    return total;
}
// 회전해서, 해당 상태로 만들어 놓는 부분


pair<int,int> find_score(int x, int y){
    int cand_max = -1; // 해당 좌표에서의 최댓값을 담는 곳
    int rotate_num = -1;

    
    int temp_temp_map[10][10] = {};
    
    int start_x = x - 1;
    int start_y = y - 1;
    // 맵 복사
    for(int i = 0 ; i < 5; i++){
        for(int j = 0; j < 5; j++){
            temp_map[i][j] = map[i][j];
            temp_temp_map[i][j] = map[i][j];
        }
    }
    // 복사 지점에 회전 진행
    for(int i = 0; i < 3; i++){

        int final_score = 0; // 해당 회전에서의 최종 점수를 확인하기 위함

        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                temp_map[start_x + k][start_y + 2 - j] = temp_temp_map[start_x + j][start_y + k]; // 이런식으로?
            }
        }

        for(int j = 0 ; j < 5; j++){
            for(int k = 0; k < 5; k++){
                temp_temp_map[j][k] = temp_map[j][k];
            }
        }
        // 회전한거 복사
        //cout << "rotate : x, y :" << x << " , " << y << " many : "<< i << "\n";
        // for(int a = 0; a < 5; a++){
        //     for(int b = 0; b < 5 ; b++){
        //         cout << temp_map[a][b] << " ";
        //     }
        //     cout << "\n";
        // }
        //여기서 bfs로 연속된 유적이 있는지 확인
        for(int j = 0 ; j < 5; j++){
            for(int k = 0; k < 5; k++){
                int score = count_nums(j,k);
                if(score >= 3) {
                    final_score += score;
                }
            }
        }
        //cout << final_score << ":  final score \n";
        if(cand_max < final_score){ 
            cand_max = final_score;
            rotate_num = i;
        }
        for(int j = 0 ; j < 5; j++){
            for(int k = 0; k < 5; k++){
                visit[j][k] = false; // 방문지점 초기화 
            }
        }
    }
    //exit(1);
    return {cand_max, rotate_num}; // 해당 내용으로 넣기
}


void erase_idx(int x, int y){
    int total = 1;

    queue<pair<int,int>> q;
    vector<pair<int,int>> erase_idxs;
    q.push({x,y});
    erase_idxs.push_back({x,y});
    visit[x][y] = true;
    int target_num = map[x][y];

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0 ; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i]; 
            if(nx < 0 || nx >= 5 || ny < 0 || ny >= 5) continue; // 범위 벗어남
            if(visit[nx][ny] == true) continue;
            if(target_num != map[nx][ny]) continue;
            // 그게 아니면 q에 추가
            total++;
            q.push({nx,ny});
            erase_idxs.push_back({nx,ny});
            visit[nx][ny] = true;
        }
    }
    if(total >= 3) {
        for(auto erase : erase_idxs){
            map[erase.first][erase.second] = 0; // 이런식으로?
        }
    }
}



int change_map(){
    int final_x = -1;
    int final_y = -1;
    int max_score = -1; // 최종 점수를 담는 분
    int min_rotate = 100;

    for(int j = 1; j < 4; j++){
        for(int i = 1; i < 4; i++){
            // 이런식으로 중앙만 돌리기
            //i : 행 , j : 열
            pair<int,int> temp = find_score(i,j);
            //first 점수, second 회전 수
            if(temp.first == 0){
                continue;
            }
            if(max_score < temp.first){
                final_x = i;
                final_y = j;
                max_score = temp.first;
                min_rotate = temp.second;
            }
            else if(max_score == temp.first){
                if(temp.second < min_rotate){
                    min_rotate = temp.second ;
                    final_x = i;
                    final_y = j; // 회전수가 작은거
                }
            }
        }
    }
    // 이렇게 하면 회전하는 좌표와 회전 위치가 나와야함
    if(max_score == -1){
        return 0; // 아무것도 없을때
    }
    //cout << max_score << " score!\n"; // 죄종 스코어 정리
    //cout << final_x << " x " << final_y << " y " << min_rotate << " rotate\n";
    // 해당 좌표, 해당 회전 기준으로 실제 map 변형
    int tmp[10][10] = {};
    int start_x = final_x - 1;
    int start_y = final_y - 1;
    for(int rotates = 0 ; rotates <= min_rotate; rotates++){
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3 ; j++){
                tmp[start_x + j][start_y + 2 - i] = map[start_x + i][start_y + j];
            }
        }
        for(int i = 0; i < 3 ; i++){
            for(int j = 0; j < 3 ; j++){
                map[start_x + i][start_y + j] = tmp[start_x + i][start_y + j];
            }
        }
    }

    // cout << "after! map \n";
    // print_map();

    // 여기서 이제 해당 수들 빼기
    for(int i = 0; i < 5; i++){
        for(int j = 0 ; j < 5; j++){
            erase_idx(i,j); // 지울 좌표들 찾기
        }
    }

    for(int j = 0 ; j < 5; j++){
        for(int k = 0; k < 5; k++){
            visit[j][k] = false; // 방문지점 초기화 
        }
    } // visit 배열 이용했으니 다시 초기화

    // cout << "after! erase \n";
    // print_map();

    // for(auto erase : erase_idxs){
    //     cout << erase.first << " x " << erase.second << " y \n";
    // }
    // 이후에 해당 지점 바탕으로 진짜 지우기
    return max_score;
}


void add_map(){
    for(int j = 0; j < 5; j++){
        for(int i = 4; i >= 0 ; i--){
            if(map[i][j] == 0){
                map[i][j] = wall[wall_idx++];
            }
        }
    }
    // cout << "after! add_map \n";
    // print_map();
    //exit(1);
}


int count_num2(int x, int y){
    int total = 1;

    queue<pair<int,int>> q;
    
    q.push({x,y});
    visit[x][y] = true;
    int target_num = map[x][y];

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0 ; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i]; 
            if(nx < 0 || nx >= 5 || ny < 0 || ny >= 5) continue; // 범위 벗어남
            if(visit[nx][ny] == true) continue;
            if(target_num != map[nx][ny]) continue;
            // 그게 아니면 q에 추가
            total++;
            q.push({nx,ny});
            visit[nx][ny] = true;
        }
    }
    //cout << total << " total \n";
    return total;
}

int final_score;

int additional(){ // 추가적인 연쇄가 있는가?
    int addition = 0;
    while(1){
        int total = 0;
        for(int i = 0 ; i < 5; i++){
            for(int j = 0; j < 5; j++){
                int cand = count_num2(i,j);
                
                if(cand >=3 ){
                    total += cand;
                }
            }
        }
        
        for(int j = 0 ; j < 5; j++){
            for(int k = 0; k < 5; k++){
                visit[j][k] = false; // 방문지점 초기화 
            }
        }

        if(total == 0) break; // 추가적인 점수가 있는가?
        addition += total;
        
        //여기서 이제 연쇄 반응
        for(int i = 0; i < 5; i++){
            for(int j = 0 ; j < 5; j++){
                erase_idx(i,j); // 좌표 지우기
            }
        }

        
        for(int j = 0 ; j < 5; j++){
            for(int k = 0; k < 5; k++){
                visit[j][k] = false; // 방문지점 초기화 
            }
        }
        add_map(); // 이제 추가


        
        //exit(1);
    }
    //cout <<"addtion : " << addition << "\n";
    return addition;
}

int main(){
    cin >> turns >> total_wall;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cin >> map[i][j];
        }
    }
    for(int i = 0; i < total_wall; i++){
        cin >> wall[i];
    }
    for(int i = 0; i < turns; i++){
        // 여기서 탐사할 유물이 있는지 먼저 보기
        int final_score = 0;
        int total = change_map();
        if(total == 0) break;
        final_score += total;
        // cout << i << "th ";
        // cout << final_score << "\n";
        add_map();


        final_score += additional();
        cout << final_score << " ";
    }
    cout << "\n";
}