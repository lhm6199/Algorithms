#include <iostream>
#include <vector>

using namespace std;

struct runner_info{
    int x,y;
    int dir; // 0 이면 시작  1 이면 바뀜
    int option; // 상하냐 좌우냐를 결장하는 부분 좌우 == 1, 상하 ==2
};

struct chase_info{
    int x,y;
    int dir; 
    int mov_count;
};

chase_info chaser;

bool tree[101][101];

// d == 1 좌우 (오른쪽으로 시작)
// d == 2 상하 (아래로 시작)

int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};

int change_move[2] = {1,-1}; // 도망자용
int chaser_mount[110];
int mount_idx = 1;
int total_result;
vector<runner_info> runners;
int N, runner_num, tree_num, turn_num;


bool is_move(runner_info now_runner){
    int dist_x = now_runner.x - chaser.x;
    int dist_y = now_runner.y - chaser.y;

    if(dist_x < 0) dist_x = -1 * dist_x;
    if(dist_y < 0) dist_y = -1 * dist_y;

    return dist_x + dist_y <= 3;
}


void move_runner(){
    for(int i = 0; i < runners.size(); i++){
        if(is_move(runners[i])){
            if(runners[i].option == 1){
                // 좌우로 시작
                int change_y = runners[i].y + change_move[runners[i].dir];
                if(change_y < 0 || change_y >= N){
                    runners[i].dir = 1 - runners[i].dir;
                    change_y = runners[i].y + change_move[runners[i].dir]; // 일단 후보니까 움직여놓기
                }   // 벽인경우
                if(chaser.x == runners[i].x && change_y ==chaser.y) continue; // 움직이려는 곳에 술레가 있는 경우 움직 x
                else{
                    runners[i].y  = change_y ; // 만약에 술래가 없으면 넘어가기
                } 
            }
            else{
                int change_x = runners[i].x + change_move[runners[i].dir];
                if(change_x < 0 || change_x >= N){
                    runners[i].dir = 1 - runners[i].dir;
                    change_x = runners[i].x + change_move[runners[i].dir]; // 일단 후보니까 움직여놓기
                }   // 벽인경우
                if(chaser.y == runners[i].y && change_x == chaser.x) continue; // 움직이려는 곳에 술레가 있는 경우 움직 x
                else{
                    runners[i].x  = change_x ; // 만약에 술래가 없으면 넘어가기
                }
            }
        }
    }
}

bool clock_wise;

void move_chaser_1(){

    //cout << chaser.dir << " dir\n";
    int count;
    if(mount_idx == N-1){
        count = 3;
    }
    else{
        count = 2;
    }
    if(chaser_mount[mount_idx] < count){
        //cout <<"1213\n";
        if(chaser.mov_count < mount_idx){
            chaser.x = chaser.x + dx[chaser.dir];
            chaser.y = chaser.y + dy[chaser.dir];
            chaser.mov_count++; // 해당 방향으로 몇번 움직였나?
            //cout <<"12132\n";
        }
        if(chaser.mov_count == mount_idx){
            chaser.mov_count = 0;
            chaser.dir = (chaser.dir + 1) % 4; // 바로 방향전환
            chaser_mount[mount_idx]++;
        }
        
    }
     

    if(chaser.x == 0 && chaser.y == 0){
        //cout << "turn!\n" << mount_idx << " idx\n";
        chaser.dir = (chaser.dir + 1) % 4;
        clock_wise = false;
        return;
    }

    if(chaser_mount[mount_idx] == count){
        mount_idx++;
    }
}
void move_chaser_2(){


    if(chaser_mount[mount_idx] > 0){
        //cout << chaser_mount[mount_idx] << " " << mount_idx << "\n";
        //exit(1);
        if(chaser.mov_count < mount_idx){
            chaser.x = chaser.x + dx[chaser.dir];
            chaser.y = chaser.y + dy[chaser.dir];
            chaser.mov_count++; // 해당 방향으로 몇번 움직였나?
        }
        if(chaser.mov_count == mount_idx){
            chaser.mov_count = 0;
            chaser.dir = (chaser.dir + 3) % 4; // 바로 방향전환
            chaser_mount[mount_idx]--; 
        }
    }

    if(chaser.x == (N-1)/2 && chaser.y == (N-1)/2){
        chaser.dir = (chaser.dir + 3) % 4;
        clock_wise = true;
        return;
    }

    if(chaser_mount[mount_idx] == 0){
        mount_idx--;
    }
} // 달팽이 로직 너무 복잡하게 짬..

// 이건 내가 잘못했다



int kill_runner(){
    int result = 0;
    vector<pair<int,int>> kill_axis;
    int now_kill_x = chaser.x;
    int now_kill_y = chaser.y;
    for(int i = 0; i < 3;i++){
        if(now_kill_x < 0 || now_kill_x >= N || now_kill_y < 0 || now_kill_y >= N) break;
        kill_axis.push_back({now_kill_x,now_kill_y});
        now_kill_x += dx[chaser.dir];
        now_kill_y += dy[chaser.dir]; // 바라보는 방향
    }

    for(auto kill_axi : kill_axis){
        if(tree[kill_axi.first][kill_axi.second] == true) continue;
        else{
            for(int i = 0; i < runners.size();){
                if(runners[i].x == kill_axi.first && runners[i].y == kill_axi.second){
                    result++;
                    runners.erase(runners.begin() + i);
                }
                else{
                    i++;
                }
            }
        }
    }
    return result;
}

void print_runner(){
    for (auto runner : runners){
        cout << runner.x << " x " << runner.y << " y \n";
    }
}




int main(){

    cin >> N >> runner_num >> tree_num >> turn_num;
    chaser.x = (N-1)/2;
    chaser.y = (N-1)/2;

    for(int i = 0; i < runner_num; i++){
        runner_info temp;
        cin >> temp.x >> temp.y >> temp.option;
        temp.dir = 0;
        temp.x--;
        temp.y--;
        runners.push_back(temp);
    }
    
    for(int i = 0; i < tree_num; i ++){
        int x,y;
        cin >> x >> y;
        tree[x-1][y-1] = true; 
    }
    clock_wise = true;
    for(int i = 0 ; i < turn_num ; i++){
        //print_runner();
        move_runner();
        //cout << chaser.x << " x " << chaser.y << " y \n";


        //exit(1);
       // cout << chaser.x << " x " << chaser.y << " y \n";
        if(clock_wise == true){
            move_chaser_1();
           // cout << chaser.x << " 1x " << chaser.y << " y \n";
        }
        else{
            //mount_idx << "\n";
            move_chaser_2();
           // cout << chaser.x << " x " << chaser.y << " y \n";
        }
        int kill_count = kill_runner();
        total_result = total_result + kill_count*(i+1);
        
        //exit(1);
        
    }
    cout << total_result << "\n";
    //return 1;
}