#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int map[50][50];

int N, knight_num , turn;

struct knight_info{
    int x,y;
    int h,w;
    int strength;
    int damages;
    bool live; // 죽었는지 살았는지 보기 위함
};

vector<knight_info> knights;

void print_knight(){
    for(auto knight : knights){
        cout << knight.x << " x "
            << knight.y << " y "
            << knight.strength << " strength "
            << knight.live << " is live?\n";
    }
}

vector<int> move_cand; 

int is_knight(int x, int y){ // 해당 좌표가 다른 기사의 영역인가를 확인하는 함수
    for(int i = 0; i < knights.size(); i++){
        if(knights[i].live == false) continue; // 죽은 기사의 경우에는 넘어가기
        bool inner_x = false;
        bool inner_y = false;
        if(knights[i].x <= x  && x < knights[i].x + knights[i].h) inner_x = true;
        if(knights[i].y <= y  && y < knights[i].y + knights[i].w) inner_y = true;

        if(inner_x == true && inner_y == true) return i;
    }
    return -1; // 해당 범위 안에는 기사가 없는 것
}


bool move_knight_cand(int id, int dir){ 
    //out << "enter\n"; 
    //0 ,1,2,3 -> 위
    // 여기에 움직이는 대상들 저장
    queue<int> q;
    bool visit[100] = {};
    move_cand.push_back(id);
    q.push(id);
    // 각 방향대로 설정해두기
    while(!q.empty()){
        //printf("ingi\n");
        knight_info knight = knights[q.front()];
        q.pop();
        if(dir == 0){
            //위 방향으로 이동
            int base_x = knight.x - 1;
            if(base_x <= 0 || base_x > N) return false; // 범위를 벗어나는 경우
            for(int i = knight.y; i <knight.y + knight.w ; i++){
                if(map[base_x][i] == 2)  return false; // 벽이 있는 경우 => 아무도 이동 못함 
                int success_knight = is_knight(base_x, i); // 연속된 곳에 knight가 있는경우
                if(success_knight != -1){
                    if(visit[success_knight] == false){
                        visit[success_knight] = true;
                        q.push(success_knight);
                        move_cand.push_back(success_knight);
                    }
                }
            }
        }
        else if(dir == 1){
            int base_y = knight.y + knight.w;
            if(base_y <= 0 || base_y > N) return false; // 범위를 벗어나는 경우
            for(int i = knight.x; i < knight.x + knight.h ; i++){
                if(map[i][base_y] == 2)  return false; // 벽이 있는 경우 => 아무도 이동 못함 
                int success_knight = is_knight(i, base_y); // 연속된 곳에 knight가 있는경우
                if(success_knight != -1){
                    if(visit[success_knight] == false){
                        visit[success_knight] = true;
                        q.push(success_knight);
                        move_cand.push_back(success_knight);
                    }
                }
            }
        }
        else if(dir == 2){
            int base_x = knight.x + knight.h;
            if(base_x <= 0 || base_x > N) return false;
            for(int i = knight.y; i <knight.y + knight.w ; i++){
                if(map[base_x][i] == 2)  return false; // 벽이 있는 경우 => 아무도 이동 못함 
                int success_knight = is_knight(base_x, i); // 연속된 곳에 knight가 있는경우
                if(success_knight != -1){
                    if(visit[success_knight] == false){
                        visit[success_knight] = true;
                        q.push(success_knight);
                        move_cand.push_back(success_knight);
                    }
                }
            }
        }
        else if(dir == 3){
            int base_y = knight.y - 1 ;
            if(base_y <= 0 || base_y > N) return false;
            for(int i = knight.x; i < knight.x + knight.h ; i++){
                if(map[i][base_y] == 2)  return false; // 벽이 있는 경우 => 아무도 이동 못함 
                int success_knight = is_knight(i, base_y); // 연속된 곳에 knight가 있는경우
                if(success_knight != -1){
                    if(visit[success_knight] == false){
                        visit[success_knight] = true;
                        q.push(success_knight);
                        move_cand.push_back(success_knight);
                    }
                }
            }
        }
    }
    return true;
    // cout << "close\n"; 
    // if(move_cand.empty()) cout<< "no move\n";
    // for(auto move : move_cand){
    //     cout << move << " idxs\n";
    // }
}
int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};

void move_knight(int dir){
    for(int i = 0; i < move_cand.size() ; i++){
        int idx = move_cand[i];
        knights[idx].x = knights[idx].x + dx[dir];
        knights[idx].y = knights[idx].y + dy[dir]; // 이렇게만 움직이면 됨
    }
}
int total_damage;

void damage(int start_id){
    for(int i = 0; i < move_cand.size() ; i++){
        int idx = move_cand[i];
        if(start_id == idx) continue;
        // 나머지 밀쳐진 놈들은 전부 out
        int damages = 0;
        //exit(1);
        for(int j = knights[idx].x ; j < knights[idx].x + knights[idx].h ; j++){
            for(int k = knights[idx].y ; k < knights[idx].y + knights[idx].w ; k++){
                if(map[j][k] == 1) {
                    damages++;
                }
            }
        }
        // cout << idx << " id get" << damages << "\n";
        knights[idx].strength -= damages;
        knights[idx].damages += damages; // 데미지 추가하기
    }
}
void killed(){
    for(int i = 0 ; i < knights.size(); i++){
        if(knights[i].strength <= 0) knights[i].live = false;
    }
}

int main(){
    cin >> N >> knight_num >> turn;

    for(int i = 1; i <= N; i++){
        for(int j = 1 ; j <= N ; j++){
            cin >> map[i][j];
        }
    }

    for(int i = 0 ; i < knight_num ; i++){
        knight_info temp;
        cin >> temp.x >> temp.y >> temp.h >> temp.w >> temp.strength;
        temp.live = true;
        temp.damages = 0;
        knights.push_back(temp);
    }

    for(int i = 0 ; i < turn; i++){
        int id , dir; // id -> zero base로 전환
        cin >> id >> dir;
        id--;
        if(knights[id].live == false) continue; // 죽은애를 고르는 경우
        if(move_knight_cand(id, dir) == false){
            //cout << i << " turn falsed\n";
            move_cand.clear();
            continue;
        }
        
        // 실제 이동을 해야함
        // cout << "before\n" << i << "turn\n";
        // print_knight();
        move_knight(dir);
        
        // cout << "after move\n" << i << "turn\n";
        // print_knight();
        
        // cout <<move_cand.size() << " size " <<endl;
        
        damage(id);
        
        // cout << "after damage\n" << i << "turn\n";
        // print_knight();
        // //exit(1);
        killed();
        
        // cout << "after kill\n" << i << "turn\n";
        // print_knight();
        move_cand.clear();
        // for(auto move : move_cand){
        //     cout << move << " idxs\n";
        // }
        //exit(1);
    }

    for(auto knight : knights){
        if(knight.live == true){
            total_damage += knight.damages;
        }
    }
    cout << total_damage<< "\n";
    
}