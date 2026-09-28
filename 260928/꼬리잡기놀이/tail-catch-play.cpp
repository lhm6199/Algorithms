#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct team_info{
    int leader_x;
    int leader_y;

    int back_x;
    int back_y; // 뒤에 정보도 추가

    int team_members;
    int team_score;
};

vector<team_info> teams;

int N;
int M;
int rounds;
int map[30][30];
int origin_path[30][30]; // 나중에 경로 복사용;

int dx[4] = {0,-1,0,1};
int dy[4] = {1,0,-1,0};

void print_map(){
    cout << "==================\n";
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N ; j++){
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

void print_path(){
    cout << "==================\n";
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N ; j++){
            cout << origin_path[i][j] << " ";
        }
        cout << "\n";
    }
}

int bfs_memebers(int x,int y, int idx){
    int member_num = 0;
    queue<pair<int,int>> q;
    bool visit[30][30] = {};
    q.push({x,y});
    visit[x][y] = true;
    member_num++;
    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0; i < 4; i++){
            int now_x = now.first + dx[i];
            int now_y = now.second + dy[i];

            if(now_x < 0 || now_x >= N || now_y < 0 || now_y >= N ) continue;
            if(map[now_x][now_y] == 0) continue;
            if(visit[now_x][now_y]) continue; // 이미 방문한 경우에 pass
            if(map[now_x][now_y] == 3){
                teams[idx].back_x = now_x;
                teams[idx].back_y = now_y; // 맨 뒷정보 추가
            }
            if(map[now_x][now_y] != 4){
                member_num++;
                q.push({now_x,now_y});
                visit[now_x][now_y] = true;
            }
        }
    }
    return member_num;
}




void move_people(){
    int temp[30][30] = {};
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N; j++){
            temp[i][j] =  origin_path[i][j]; //temp에 옮길것임
        }
    }

    for(int i = 0; i < teams.size(); i++){
        bool moved = false;

        int leader_x = teams[i].leader_x;
        int leader_y = teams[i].leader_y;
        // map기준으로 주변에 4 찾기
        for(int j = 0; j < 4; j++){
            int cand_x = leader_x + dx[j];
            int cand_y = leader_y + dy[j];
            if(cand_x < 0 || cand_x >= N ||
                cand_y < 0 || cand_y >= N)
                continue;
            
            if(map[cand_x][cand_y] == 4){
                teams[i].leader_x = cand_x;
                teams[i].leader_y = cand_y; // 이때 리더의 위치를 옮기기;
                temp[cand_x][cand_y] = 1; // 해당 지점에 리더 표시;
                            moved = true;
                break;
            }
        }
        if(!moved){

            int bx = teams[i].back_x;
            int by = teams[i].back_y;

            // 머리와 꼬리가 붙어있는 경우
            if(abs(leader_x - bx) + abs(leader_y - by) == 1){
                teams[i].leader_x = bx;
                teams[i].leader_y = by;
                temp[bx][by] = 1;
            }
        }
    } // 여기까지 리더를 옮기고 뒤에는 맴버 옮기기

    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            if(map[i][j] == 1 || map[i][j] == 2){
                temp[i][j] = 2;
            }
        }
    }
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            if(map[i][j] == 3){
                int cand_idx = -1;
                for(int k = 0; k < teams.size(); k++){
                    if(teams[k].back_x == i && teams[k].back_y == j){
                        cand_idx = k; // 뒷 내용 옮기기 위함
                    }
                }

                for(int k = 0; k < 4; k++){
    
                    int cand_back_x = i + dx[k];
                    int cand_back_y = j + dy[k];
                    if(cand_back_x < 0 || cand_back_x >= N || cand_back_y < 0 || cand_back_y >= N) continue;
                    if(map[cand_back_x][cand_back_y] == 2){
                        temp[cand_back_x][cand_back_y] = 3; // 마지막에 인접한 녀석
                        teams[cand_idx].back_x = cand_back_x;
                        teams[cand_idx].back_y = cand_back_y;
                    }
                }
            }
        }
    }

    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
           map[i][j] = temp[i][j] ;
        }
    }
}


bool gate = 0; // 0 이면 x 1이면 y 변환;
bool increase_x = 1; //1이면 증가, 2이면 감소;
bool increase_y = 1; //1이면 증가, 2이면 감소;
pair<int,int> move_axix = {0,0};

bool corner_hold = false;

void change_gate(){

    // 꼭짓점에 도착한 직후라면 이번 턴에는 이동하지 않음
    if(corner_hold){
        corner_hold = false;
        return;
    }

    if(gate == 0){
        // x 변화
        if(increase_x){
            move_axix.first++;
        }
        else{
            move_axix.first--;
        }

        if(move_axix.first == N-1){
            gate = 1;
            increase_x = false;

            corner_hold = true;
        }
        else if(move_axix.first == 0){
            gate = 1;
            increase_x = true;

            corner_hold = true;
        }
    }
    else{
        // y 변화
        if(increase_y){
            move_axix.second++;
        }
        else{
            move_axix.second--;
        }

        if(move_axix.second == N-1){
            gate = 0;
            increase_y = false;

            corner_hold = true;
        }
        else if(move_axix.second == 0){
            gate = 0;
            increase_y = true;

            corner_hold = true;
        }
    }
}

int bfs_dist(int x, int y){

    queue<pair<int,int>> q;
    int visit[30][30] = {};

    q.push({x, y});
    visit[x][y] = 1;

    int target_team = -1;

    // 공에 맞은 사람이 머리인 경우
    if(map[x][y] == 1){

        for(int i = 0; i < teams.size(); i++){
            if(teams[i].leader_x == x &&
               teams[i].leader_y == y){

                target_team = i;
                break;
            }
        }

        // 머리 <-> 꼬리 변경
        if(target_team != -1){

            int old_leader_x = teams[target_team].leader_x;
            int old_leader_y = teams[target_team].leader_y;

            int old_back_x = teams[target_team].back_x;
            int old_back_y = teams[target_team].back_y;

            map[old_leader_x][old_leader_y] = 3;
            map[old_back_x][old_back_y] = 1;

            teams[target_team].leader_x = old_back_x;
            teams[target_team].leader_y = old_back_y;

            teams[target_team].back_x = old_leader_x;
            teams[target_team].back_y = old_leader_y;
        }

        return 1;
    }


    while(!q.empty()){

        int now_x = q.front().first;
        int now_y = q.front().second;
        q.pop();

        for(int dir = 0; dir < 4; dir++){

            int nx = now_x + dx[dir];
            int ny = now_y + dy[dir];

            if(nx < 0 || nx >= N ||
               ny < 0 || ny >= N)
                continue;

            if(visit[nx][ny])
                continue;

            // 사람이 아닌 위치는 이동하지 않음
            if(map[nx][ny] == 0 ||
               map[nx][ny] == 4)
                continue;


            /*
                중요!!

                사람이 선을 전부 채우고 있는 경우

                1 - 2
                |   |
                3 - 2

                처럼 머리(1)와 꼬리(3)가 붙을 수 있음.

                이때
                3 -> 1
                로 바로 이동해버리면
                몇 번째 사람인지 잘못 계산됨.
            */

            if(map[now_x][now_y] == 3 &&
               map[nx][ny] == 1)
                continue;

            if(map[now_x][now_y] == 1 &&
               map[nx][ny] == 3)
                continue;


            visit[nx][ny] = visit[now_x][now_y] + 1;

            // 머리를 찾았음
            if(map[nx][ny] == 1){

                for(int i = 0; i < teams.size(); i++){

                    if(teams[i].leader_x == nx &&
                       teams[i].leader_y == ny){

                        target_team = i;
                        break;
                    }
                }

                if(target_team != -1){

                    int old_leader_x =
                        teams[target_team].leader_x;

                    int old_leader_y =
                        teams[target_team].leader_y;

                    int old_back_x =
                        teams[target_team].back_x;

                    int old_back_y =
                        teams[target_team].back_y;


                    // 지도에서 머리 / 꼬리 변경
                    map[old_leader_x][old_leader_y] = 3;
                    map[old_back_x][old_back_y] = 1;


                    // team 정보 변경
                    teams[target_team].leader_x =
                        old_back_x;

                    teams[target_team].leader_y =
                        old_back_y;

                    teams[target_team].back_x =
                        old_leader_x;

                    teams[target_team].back_y =
                        old_leader_y;
                }

                return visit[nx][ny];
            }


            q.push({nx, ny});
        }
    }

    return 0;
}


int main(){
    int score = 0;
    cin >> N >> M >> rounds;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N ; j++){
            cin >> map[i][j];
            if(map[i][j] != 0) origin_path[i][j] = 4; // 경로만 따로 저장
            if(map[i][j] == 1) {
                // 리더인 경우
                team_info temp;
                temp.leader_x = i;
                temp.leader_y = j;
                temp.team_members = 0; // 일단 0으로 설정
                temp.team_score = 0;
                teams.push_back(temp); 
            }
        }
    }

    for(int i = 0; i < teams.size(); i++){
        teams[i].team_members = bfs_memebers(teams[i].leader_x,teams[i].leader_y , i);
    }


    for(int i = 0; i < rounds; i++){
        
        move_people();
        // print_map();
    //exit(1);
        // print_map();
        // move_people();
        //print_map();

        int ball_dir = (i/(N)) % 4;
        pair<int,int> temp = move_axix;
        //cout << move_axix.first << " first " << move_axix.second <<"second\n";
        //cout << ball_dir << " dir\n";
        for(int j = 0; j < N; j++){
            if(map[temp.first][temp.second] != 0 && map[temp.first][temp.second] != 4){

                //해당 놈이 1이랑 얼마나 떨어져있는가를 계산
                // bfs로
                //cout << "map : " << map[temp.first][temp.second] << "\n";
                
                int dist = bfs_dist(temp.first,temp.second);
                score = score + dist * dist; 
                

                break; // 처음으로 공이 맞았으니 이때는 break;
            }
            temp.first += dx[ball_dir];
            temp.second += dy[ball_dir]; // 그것이 아니라면 공 이동
        }
        
        
        change_gate(); // 옮기기
        
    }
    cout << score << "\n";
}