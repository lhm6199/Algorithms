#include <iostream>
#include <vector>
#include <queue>
using namespace std;


int N, M, turn;

struct tops_info{
    int strength; // 공격력을 담음
    int recent_turn; // 최근에 공격?
    bool alive;
};

tops_info map[20][20]; // 공격력을 담는 배열 -> 구조체로 관리


void print_strength(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cout << map[i][j].strength << " ";
        }
        cout << "\n";
    }
}

void print_turn(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cout << map[i][j].recent_turn << " ";
        }
        cout << "\n";
    }
}

void print_alive(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cout << map[i][j].alive << " ";
        }
        cout << "\n";
    }
}


pair<int,int> attack_cand(){
    vector<pair<int,int>> cand_list1;

    int min = 10000;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(map[i][j].strength == 0) continue;


            if(min > map[i][j].strength){
                cand_list1.clear(); // 처음에 초기화 진행
                min = map[i][j].strength;
                cand_list1.push_back({i,j}); // 해당 내용 집어 넣음
            }
            else if(min == map[i][j].strength){
                // 지금의 작은 값과 매칭이 되는경우
                cand_list1.push_back({i,j});
            }
            // 다른 경우는 out
        }
    }

    // 최근에 공격?
    vector<pair<int,int>> cand_list_recent;
    int max_turn = -1;
    for(int i = 0; i < cand_list1.size(); i++){
        int temp = map[cand_list1[i].first][cand_list1[i].second].recent_turn;
        if(temp > max_turn){
            cand_list_recent.clear(); // 한번 비워주기
            max_turn = temp;
            cand_list_recent.push_back({cand_list1[i].first,cand_list1[i].second});
        }
        else if(temp == max_turn){
            cand_list_recent.push_back({cand_list1[i].first,cand_list1[i].second});
        }
    }

    // 여기가 행 + 열 합 보기
    vector<pair<int,int>> cand_list2;
    int max = -1;
    for(int i = 0; i < cand_list_recent.size(); i++){
        int temp = cand_list_recent[i].first + cand_list_recent[i].second;
        if(temp > max){
            cand_list2.clear();
            max = temp;
            cand_list2.push_back({cand_list_recent[i].first,cand_list_recent[i].second});
        }
        else if(temp == max){
            cand_list2.push_back({cand_list_recent[i].first,cand_list_recent[i].second});
        }
    }

    // 여기는 열이 가장 큰 포탑 보기

    pair<int,int> final_axis = {-1,-1};
    int max_col = -1;
    for(int i = 0; i < cand_list2.size(); i++){
        if(cand_list2[i].second > max_col){
            max_col = cand_list2[i].second;
            final_axis = cand_list2[i];
        }
    }
    return final_axis;
}


pair<int,int> hit_cand(){ // 맞는 애 고르기
   vector<pair<int,int>> cand_list1;

    int max = -1;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(map[i][j].strength == 0) continue;


            if(max < map[i][j].strength){
                cand_list1.clear(); // 처음에 초기화 진행
                max = map[i][j].strength;
                cand_list1.push_back({i,j}); // 해당 내용 집어 넣음
            }
            else if(max == map[i][j].strength){
                // 지금의 작은 값과 매칭이 되는경우
                cand_list1.push_back({i,j});
            }
            // 다른 경우는 out
        }
    }

    //cout << cand_list1[0].first << " " << cand_list1[0].second << " \n";

    //cout << max << " max!\n";
    // 가장 오래전에 공격?
    vector<pair<int,int>> cand_list_recent;
    int min_turn = 1000000;
    for(int i = 0; i < cand_list1.size(); i++){
        int temp = map[cand_list1[i].first][cand_list1[i].second].recent_turn;
        if(temp < min_turn){
            cand_list_recent.clear(); // 한번 비워주기
            min_turn = temp;
            cand_list_recent.push_back({cand_list1[i].first,cand_list1[i].second});
        }
        else if(temp == min_turn){
            cand_list_recent.push_back({cand_list1[i].first,cand_list1[i].second});
        }
    }
    //cout << cand_list_recent[0].first << " " << cand_list_recent[0].second << " \n";
    // 여기가 행 + 열 합 보기
    vector<pair<int,int>> cand_list2;
    int min = 100000;
    for(int i = 0; i < cand_list_recent.size(); i++){
        int temp = cand_list_recent[i].first + cand_list_recent[i].second;
        if(temp < min){
            cand_list2.clear();
            min = temp;
            cand_list2.push_back({cand_list_recent[i].first,cand_list_recent[i].second});
        }
        else if(temp == min){
            cand_list2.push_back({cand_list_recent[i].first,cand_list_recent[i].second});
        }
    }
    //cout << cand_list2[0].first << " 22 " << cand_list2[0].second << " \n";

    // 여기는 열이 가장 큰 포탑 보기

    pair<int,int> final_axis = {-1,-1};
    int min_col = 100000;
    for(int i = 0; i < cand_list2.size(); i++){
        if(cand_list2[i].second < min_col){
            min_col = cand_list2[i].second;
            final_axis = cand_list2[i];
        }
    }
    return final_axis;
}

// 시작 지점은 맞는 곳 부터 해서 보면 될듯
//start 가 hitter

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

bool attacked[30][30];

int laser_bfs(int hitter_x, int hitter_y, int attack_x, int attack_y){
    int visit[30][30] = {}; //거리를 담는 곳 0 으로 초기화
    queue<pair<int,int>> q;
    visit[hitter_x][hitter_y] = 1;
    q.push({hitter_x,hitter_y});

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i]; // 이런식으로 설정
            
            if(nx < 0){
                nx = N-1;
            }
            else if(nx >=N){
                nx = 0;
            }
            if(ny < 0){
                ny = M-1;
            }
            else if(ny >=M){
                ny = 0;
            }
            // 위의 내용은 범위 보정
            if(map[nx][ny].alive == false) continue;// 부서진 위치는 못지나감
            if(visit[nx][ny] != 0) continue; // 이미 지나간 지점이라 거기서는 안됨
            visit[nx][ny] = visit[now.first][now.second] + 1;
            q.push({nx,ny}); //해당 좌효 큐에 넣기
        }
    }
    //cout<<"=========\n";
    // for(int i = 0; i <N; i++){
    //     for(int j = 0; j < M; j++){
    //         cout << visit[i][j] << " ";
    //     }
    //     cout << "\n";
    // }
    //printf("okay!!\n");
    if(visit[attack_x][attack_y] == 0) return -1; // 이런 경우에는 해당 경로까지의 점이 없었던것 -> 대포
    //printf("jear\n");
    //exit(1);
   
    vector<pair<int,int>> path_lists; // 여기에 지나간 경로가 존재
    pair<int,int> cand;
    cand.first = attack_x;
    cand.second = attack_y; // 이렇게 초기화
    //printf("okay2!!\n");


    // for(int i = 0; i <N; i++){
    //     for(int j = 0; j < M; j++){
    //         cout << visit[i][j] << " ";
    //     }
    //     cout << "\n";
    // }

    //cout << "asdasd\n";
    //exit(1);
    while(1){
        if(cand.first == hitter_x && cand.second == hitter_y ) break;
        //cout << cand.first << " " << cand.second << "\n";
        for(int i = 0; i < 4; i++){
            int nx = cand.first + dx[i];
            int ny = cand.second + dy[i];
            
            
            // 보정
            if(nx < 0){
                nx = N-1;
            }
            else if(nx >=N){
                nx = 0;
            }
            if(ny < 0){
                ny = M-1;
            }
            else if(ny >=M){
                ny = 0;
            }
            if(map[nx][ny].alive == false) continue; // 죽은것은 out
            if(visit[nx][ny] == visit[cand.first][cand.second] - 1){
                cand.first = nx;
                cand.second = ny; 
                path_lists.push_back({nx,ny});
                break;
            }
        }
    }
    //cout << "path list\n";
    // for(int i = 0; i < path_lists.size();i++){
    //     cout << path_lists[i].first << " "<< path_lists[i].second << endl;
    // }

    path_lists.pop_back(); // 마지막 위치는 종점 인덱스여서 out;
    for(int i = 0; i < path_lists.size();i++){
        map[path_lists[i].first][path_lists[i].second].strength -= (map[attack_x][attack_y].strength / 2);
        attacked[path_lists[i].first][path_lists[i].second] = true;
    }
    map[hitter_x][hitter_y].strength -= map[attack_x][attack_y].strength;
    attacked[hitter_x][hitter_y] = true;
    attacked[attack_x][attack_y] = true;
    return 1;

}
int dx8[8] = {0,-1,-1,-1,0,1,1,1};
int dy8[8] = {1,1,0,-1,-1,-1,0,1};

void tank(int hitter_x, int hitter_y, int attack_x, int attack_y){
    for(int i = 0; i <8; i++){
        int nx = hitter_x + dx8[i];
        int ny = hitter_y+ dy8[i];
        if(nx < 0){
           nx = N-1;
        }
        else if(nx >=N){
            nx = 0;
        }
        if(ny < 0){
            ny = M-1;
        }
        else if(ny >=M){
            ny = 0;
        }
        //cout << nx << " nx " << ny <<" ny \n";
        if(map[nx][ny].alive == false) continue;
        if(nx == attack_x && ny == attack_y) continue;
        map[nx][ny].strength -= (map[attack_x][attack_y].strength / 2);
        attacked[nx][ny] = true;
    }
    map[hitter_x][hitter_y].strength -= map[attack_x][attack_y].strength;
    attacked[hitter_x][hitter_y] = true;
    attacked[attack_x][attack_y] = true;
}

void die(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(map[i][j].strength <= 0){
                map[i][j].strength = 0;
                map[i][j].alive = false;
            }
        }
    }
}

void fix(){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            if(map[i][j].alive == false) continue;
            if(attacked[i][j] == true) continue;
            map[i][j].strength += 1;
        }
    }
}


int main(){
    cin >> N >> M >> turn;

    for(int i = 0; i < N; i++){
        for(int j = 0; j < M; j++){
            cin >> map[i][j].strength;
            map[i][j].recent_turn = 0; // 이런식으로 초기화
            if(map[i][j].strength == 0){
                map[i][j].alive = false; // 처음에 공격력 0인거는 죽음 표시
            }
            else{
                map[i][j].alive = true; 
            }
        }
    }
    for(int i = 1; i <= turn; i++){
        // turn제도로 운영
        for(int j = 0; j < N; j++){
            for(int k = 0; k < M; k++){
                attacked[j][k] = false; // 공격 가담, 피해자를 담는 배열
            }
        }

        //공격하는 대상 선정
        pair<int,int> attacker = attack_cand();
        // 공격 받는 대상 선정
        pair<int,int> hitter = hit_cand();
        //cout << attacker.first << " attack " << attacker.second << " \n";
        map[attacker.first][attacker.second].recent_turn = i; // 1 부터 시작
        map[attacker.first][attacker.second].strength += (N + M);
        

       // cout << hitter.first << " hit " << hitter.second << "\n";
        // cout << "before\n";
        // print_strength();
        //레이저 공격, 포탄 공격
        int gate = laser_bfs(hitter.first, hitter.second ,attacker.first , attacker.second);
        // cout << "after\n";
        // print_strength();

        if(gate == -1){
            tank(hitter.first, hitter.second ,attacker.first , attacker.second);
            //printf("asdasd\n");
        }
        //print_alive();
        // 이제 공격력 0 이하인 것들 전부 죽음 표기
        die();
        // 포탑 정비;
        // cout << "before\n";
        // print_strength();
        fix();
        // cout << "after\n";
        // print_strength();
        
        int live_num = 0;
        for(int j = 0; j < N; j++){
            for(int k = 0; k < M; k++){
                if(map[j][k].alive == 1) {
                    live_num++;
                }
            }
        }
        if(live_num == 1) break;
    }
    int max_strong = -1;
    for(int j = 0; j < N; j++){
        for(int k = 0; k < M; k++){
            if(max_strong < map[j][k].strength) {
                max_strong = map[j][k].strength;
            }
        }
    }
    cout << max_strong <<"\n";
}

// 4 4 1
// 0 1 4 1
// 8 0 1 13
// 8 1 11 26
// 0 0 0 0