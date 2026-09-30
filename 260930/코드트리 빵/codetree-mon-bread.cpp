#include <iostream>
#include <vector>
#include <queue>

using namespace std;

bool wall[20][20]; // 편의점, 베이스캠프 방문시에 벽의 역할을하는 부분
int map[20][20]; // 실제 베이스 캠프의 정보를 나타내는 부분

struct people_info{
    int x,y;
    int status; // -1이면 출발 안함, 0이면 모두 도착, 1이면 아직 이동중 
};
vector<people_info> peoples; // 사람들의 정보를 나타내는 부분
vector<pair<int,int>> conven; // 편의점 정보를 나타내는 부분

int N;
int people_num;




void print_map(){
    for(int i = 0 ; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << map[i][j] << " "; // 베이스의 위치 채우기 (0 이면 빈공간, 1이면 베이스캠프)
            // 사람들 끼리는 겹칠 수 있음에 유의
        }
        cout << "\n";
    }
}

void print_wall(){
    for(int i = 0 ; i < N; i++){
        for(int j = 0; j < N; j++){
            cout << wall[i][j] << " "; // 베이스의 위치 채우기 (0 이면 빈공간, 1이면 베이스캠프)
            // 사람들 끼리는 겹칠 수 있음에 유의
        }
        cout << "\n";
    }
}

void print_people(){
    for(auto people : peoples){
        cout << people.x << " x "
            << people.y << " y "
            << people.status << " arrived\n";
    }
}


bool arrived(){
    for(auto people : peoples){
        if(people.status == -1 || people.status == 1){
            
            return 0; // 하나라도 false면 out
        }
    }
    return 1;
}

int dx[4] = {-1,0,0,1};
int dy[4] = {0,-1,1,0}; // 우선순위 보정

// 일단 각 베이스 캠프 전부다 거리 측정을 하고, 거기서 최단 거리인것 정리

pair<int,int> near_base(int x, int y){
    int visit[30][30];

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            visit[i][j] = -1;

    queue<pair<int,int>> q;
    q.push({x,y});
    visit[x][y] = 0;

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        
        for(int i = 0 ; i < 4 ; i ++){
            int cand_x = now.first + dx[i];
            int cand_y = now.second + dy[i];

            // 범위를 벗어나는 경우
            if(cand_x < 0 || cand_x >= N || cand_y < 0 || cand_y >= N ) continue;
            // 이미 방문한 경우
            if(visit[cand_x][cand_y] > 0) continue;
            // 벽인 경우
            if(wall[cand_x][cand_y] == true) continue;

            visit[cand_x][cand_y] = visit[now.first][now.second] + 1; // 이런식으로 거리 누적하면서 쓰기
            q.push({cand_x,cand_y}); 
        }
        

    }
    // 여기서 visit 정보를 이용해서 실제 어느 base에 둘것인지 정의
    int min = 1000;
    pair<int,int> cand = {0,0};

    // cout << "length\n";
    // for(int i = 0; i < N; i++){
    //     for(int j = 0; j < N; j++){
    //         cout << visit[i][j] << " ";
    //     }
    //     cout << " \n";
    // }// 이렇게 cand 정의를 해주고


    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(map[i][j] == 1 && wall[i][j] == false && visit[i][j] != -1){
                if(min > visit[i][j]){
                    min = visit[i][j];
                    cand.first = i;
                    cand.second = j;
                }
            }
        
        }
    }// 이렇게 cand 정의를 해주고



    wall[cand.first][cand.second] = true; // 해당 지점은 이제 못지나감
    return cand; // 이런식으로 넘겨주기 
}


int near_conven_dist(int x, int y, int idx){
    int visit[30][30] = {}; // 초기화 무조건 해주기..
    queue<pair<int,int>> q;
    q.push({x,y});
    visit[x][y] = 1; 

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        
        for(int i = 0 ; i < 4 ; i ++){
            int cand_x = now.first + dx[i];
            int cand_y = now.second + dy[i];

            // 범위를 벗어나는 경우
            if(cand_x < 0 || cand_x >= N || cand_y < 0 || cand_y >= N ) continue;
            // 이미 방문한 경우
            if(visit[cand_x][cand_y] > 0) continue;
            // 벽인 경우
            if(wall[cand_x][cand_y] == true) continue;
            visit[cand_x][cand_y] = visit[now.first][now.second] + 1; // 이런식으로 거리 누적하면서 쓰기
            q.push({cand_x,cand_y}); 
            if(cand_x == conven[idx].first && cand_y == conven[idx].second){
                // 이미 해당 지점을 방문한 경우
                return visit[cand_x][cand_y];
            }
        }
    }
    return -1;
}


int main(){
    cin >> N >> people_num;
    for(int i = 0 ; i < N; i++){
        for(int j = 0; j < N; j++){
            cin >> map[i][j]; // 베이스의 위치 채우기 (0 이면 빈공간, 1이면 베이스캠프)
            // 사람들 끼리는 겹칠 수 있음에 유의
        }
    }

    for(int i = 0; i < people_num; i++){
    // 사람들 끼리는 겹칠 수 있다
        pair<int,int> temp;
        cin >> temp.first >> temp.second;
        temp.first--;
        temp.second--;
        conven.push_back(temp); // 편의점 내부로 push 진행
        
        people_info new_people;
        new_people.x = -1;
        new_people.y = -1;
        new_people.status = -1;
        peoples.push_back(new_people); // 새로운 사람 넣기
        
    }


    int timer = 0; // 0 base minute 이용
    while(1){

        if(arrived()){
            //cout <<"12312\n";
            break; // 전부 도착한 경우에 out
        }
        // 우선 사람들 이동부터
        for(int i = 0; i < peoples.size(); i++){
            //cout << i << "th people\n";
            if(peoples[i].status != 1) continue; // 이동중이 아닌 경우에는 continue;
            int cand_dir[4] = {};
            for(int j = 0; j < 4; j++){
                int cand_x = peoples[i].x + dx[j];
                int cand_y = peoples[i].y + dy[j];
                
                if(cand_x < 0 || cand_x >= N || cand_y < 0 || cand_y >= N ){
                    //cout << cand_x << " x1 " << cand_y << " y\n";
                    cand_dir[j] = -1; 
                    continue;
                }

                if(wall[cand_x][cand_y] == true) {
                    //cout << cand_x << " x2 " << cand_y << " y\n";
                    cand_dir[j] = -1; 
                    continue;
                }
                if(cand_x == conven[i].first && cand_y == conven[i].second){
                    cand_dir[j] = 1;
                    continue;
                }
                cand_dir[j] = near_conven_dist(cand_x,cand_y, i);
            }
            int min = 1000;
            int cand_idx = -1;
            for(int j = 0; j < 4 ; j++){
                if(cand_dir[j] == -1) continue;
                if(min > cand_dir[j]){
                    min = cand_dir[j];
                    cand_idx = j;
                }
            }
            // cout << i <<"th people move " << cand_idx << "\n";
            // for(int j = 0; j < 4 ; j++){
            //     cout << cand_dir[j] << " ";
            // }
            // cout << "\n";
            // 여기서 해당 방향으로 업데이트
            peoples[i].x = peoples[i].x + dx[cand_idx];
            peoples[i].y = peoples[i].y + dy[cand_idx];
        }

        // 이후에 편의점 도착시, 해당 편의점 못지나감

        for(int i = 0 ; i < conven.size(); i++){
            if(peoples[i].status == -1 || peoples[i].status == 0) continue;
            if(peoples[i].x == conven[i].first && peoples[i].y ==  conven[i].second){
                peoples[i].status = 0;
                wall[peoples[i].x][peoples[i].y] = true;
            }
        }


        //
        if(timer < people_num){ //이때 사람부터 배치
            //cout << "conven " << conven[timer].first << " " <<conven[timer].second << "\n";
            pair<int,int> temp = near_base(conven[timer].first, conven[timer].second);
            peoples[timer].x = temp.first;
            peoples[timer].y = temp.second;
            peoples[timer].status = 1;
        }
        //cout << "timer : " << timer << " \n";
        //print_people();
        //print_wall();



        timer++;
    }
    cout << timer << "\n";

}
