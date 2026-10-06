#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct golem_info{
    int x,y; // 중앙 좌표
    int direction; // 출구의 좌표를 나타내는 것
};

int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1}; // 북 동 남 서 꼴

golem_info golems[10000];

int forest[100][100];
bool is_exit[100][100]; // 출구의 정보를 담아놓음
int R,C, fairy_num; // R에는 3을 더함
int total = 0 ;
// 0 ~ 2 -> 바깥으로 간주 => 처음 시작 행 지점
bool south_side(int idx){

    golem_info golem = golems[idx]; // 이런식으로
    if(golem.x + 2 > R) return false; // 내려가는게 최대 인 경우
    if(forest[golem.x + 2][golem.y] > 0 || forest[golem.x + 1][golem.y+1] > 0 || forest[golem.x + 1][golem.y-1] > 0 ) {
        //cout << forest[golem.x + 2][golem.y] << " "  << forest[golem.x + 1][golem.y+1] << " " << forest[golem.x + 1][golem.y-1] << "\n";
        //cout <<"false!!\n";
        return false;
    }
    

    // 이게 아닌 경우에는 return true
    // 남쪽으로 이제 해당 좌표로 이동
    golems[idx].x++;
    //cout << "south : " << golems[idx].x << " , " << golems[idx].y << "\n";

    return true;
}

bool west_side(int idx){

    golem_info golem = golems[idx]; // 이런식으로

    if(golem.y - 2 <= 0) return false;
    if(forest[golem.x][golem.y-2] > 0 || forest[golem.x - 1][golem.y - 1] > 0 || forest[golem.x + 1][golem.y - 1] > 0 ) return false;
    // 왼쪽에 뭔가가 있는 경우
    // 이게 아닌 경우에는 return true
    // 남쪽으로 이제 해당 좌표로 이동
    golems[idx].y--;
    //cout << "west : " << golems[idx].x << " , " << golems[idx].y << "\n";

    // 일단 이동해두고, south side가 true인 경우에 확실하게 이동, 그렇지 않으면 다시 golem 좌표로 복구
    if(south_side(idx) == false){
        golems[idx].x = golem.x;
        golems[idx].y = golem.y; // 원상 복구
        return false; // 이때 이동 못함
    }
    else{
        golems[idx].direction = (golems[idx].direction + 3) % 4; // 이런식으로 방향 정의
    }
    return true;
}

bool east_side(int idx){
    golem_info golem = golems[idx];

    if(golem.y + 2 > C) return false; // colum을 벗어나는 경우
    if(forest[golem.x][golem.y+2] > 0 || forest[golem.x - 1][golem.y + 1] > 0 || forest[golem.x + 1][golem.y + 1] > 0 ) return false;
    // 오른쪽에 뭔가가 있는 경우
    golems[idx].y++;
    //cout << "east : " << golems[idx].x << " , " << golems[idx].y << "\n";

    // 일단 이동해두고, south side가 true인 경우에 확실하게 이동, 그렇지 않으면 다시 golem 좌표로 복구
    if(south_side(idx) == false){
        golems[idx].x = golem.x;
        golems[idx].y = golem.y; // 원상 복구
        return false; // 이때 이동 못함
    }
    else{
        golems[idx].direction = (golems[idx].direction + 1) % 4; // 이런식으로 방향 정의
    }
    return true;
}

void gravity(int idx){
    //cout << "before : " << golems[idx].x << " , " << golems[idx].y << "\n";
    while(1){
        if(south_side(idx) == true) continue;
        else if(west_side(idx) == true) continue;
        else if(east_side(idx) == true) continue;
        
        else{
            break; // 더이상 이동할 수 없을때
        }
    }
}

bool golem_out(int idx){
    // 해당 인덱스의 골렘이 바깥에 있는가?
    vector<pair<int,int>> golem_axis;
    golem_axis.push_back({golems[idx].x,golems[idx].y});
    for(int i = 0; i < 4; i++){
        int now_x = golems[idx].x + dx[i];
        int now_y = golems[idx].y + dy[i];
        golem_axis.push_back({now_x,now_y}); // 상 하 좌 우 넣기
    }

    for(auto axis : golem_axis){
        if(axis.first <= 3) return true; // 좌표 하나라도 밖에 있는 경우에는 out
    }
    return false; 
}

void clearing(){
    for(int i = 1; i <= R; i++){
        for(int j = 1; j <= C; j++){
            forest[i][j] = 0; // 숲 초기화
            is_exit[i][j] = false;
        }
    }
}

void put_golem(int idx){
    // 그냥 bfs로 하는게 나을듯 -> 해당 좌표가 exit인 경우에는 다른 칸으로 이동 가능
    golem_info golem = golems[idx];
    forest[golem.x][golem.y] = idx;
    for(int i = 0; i < 4; i++){
        forest[golem.x + dx[i]][golem.y + dy[i]] = idx; // 상하 좌우도 표시
    }
    //cout << idx <<"th golem exit : " << golem.x + dx[golem.direction] << " , " << golem.y + dy[golem.direction];
    //cout << "\n";
}

void bfs(int x, int y , int idx){
    queue<pair<int,int>> q;
    bool visit[100][100] = {};
    
    q.push({x,y});
    visit[x][y] = true;
    int target_num = forest[x][y];
    forest[x][y] = idx;

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];

            if(nx <= 3 || nx > R || ny <= 0 || ny > C) continue; // 벗어나는 경우 out
            if(visit[nx][ny] == true) continue; // 이미 방문한 경우 pass
            if(forest[nx][ny] != target_num) continue; // 원하는 값이랑 다른 경우에 continue;
            // 해당 경우들이 아니면 탐색의 대상
            q.push({nx,ny});
            visit[nx][ny] = true;
            forest[nx][ny] = idx;
        }
    }
}

void connect(int idx){
    // 출구 좌표 기준으로 상하좌우에 다른 칸이 있다면 해당 내용과 연결
    int exit_x = golems[idx].x + dx[golems[idx].direction];
    int exit_y = golems[idx].y + dy[golems[idx].direction];
    is_exit[exit_x][exit_y] = true; // 출구만 저장
    // for(int i = 0; i < 4; i++){
    //     int nx = exit_x + dx[i];
    //     int ny = exit_y + dy[i];

    //     if(nx <= 3 || nx > R || ny <= 0 || ny > C) continue;
    //     if(nx == golems[idx].x && ny == golems[idx].y) continue;

    //     // 이제 다른 골렘이 있는지 확인
    //     if(forest[nx][ny] > 0){
    //         //cout << "golem okay! : " << nx << " , " << ny <<"\n";
    //         bfs(nx,ny, idx); // 해당 인덱스로 칠해버려
    //     }
    // }
}


void fairy_move(int idx){
    int max_x = -1; // 최대 행을 나타내는 것

    queue<pair<pair<int,int>,int>> q; // x,y, idx
    bool visit[100][100] = {};
    
    q.push({{golems[idx].x,golems[idx].y},idx});
    visit[golems[idx].x][golems[idx].y] = true;

    while(!q.empty()){
        pair<int,int> now = q.front().first;
        int target = q.front().second;
        q.pop();
        for(int i = 0; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];

            if(nx <= 3 || nx > R || ny <= 0 || ny > C) continue; // 벗어나는 경우 out
            if(visit[nx][ny] == true) continue; // 이미 방문한 경우 pass
            if(forest[nx][ny] == 0) continue;
            if(forest[nx][ny] != target){
                if(is_exit[now.first][now.second] == true){
                    // 해당 지점은 이동 가능
                    q.push({{nx,ny}, forest[nx][ny]});
                    visit[nx][ny] = true;
                    if(max_x < nx){
                        max_x = nx;
                    }
                }
                continue;
            }

            // 그게 아니면 갱신
            q.push({{nx,ny}, forest[nx][ny]});
            visit[nx][ny] = true;
            if(max_x < nx){
                max_x = nx;
            }
        }
    }
    //cout << "max_x : " << max_x-3 << "\n";
    total = total + max_x - 3;
}


void print_map(){
    for(int i = 1; i <= R; i++){
        if(i == 4) cout <<"=====wall=====\n";
        for(int j = 1 ; j <= C; j++){
            cout << forest[i][j] << " ";
        }
        cout << "\n";
    }
}

int main(){
    cin >> R >> C >> fairy_num;
    R += 3; // 3 더하기
    for(int i = 1; i <= fairy_num ; i++){
        //if(i == 5) exit(1);
        cin >> golems[i].y >> golems[i].direction;
        golems[i].x = 2; // 이렇게 정의
        gravity(i); // 해당 인덱스에 있는 내용 내리기
        //cout << "final idx : " << golems[i].x << " , " << golems[i].y << "\n";
        if(golem_out(i)){
            clearing(); //완전 초기화
            continue;
        }
        // out 이 아닌 경우 실제 골렘 배치
        put_golem(i);

        //cout <<"after put\n";
        //print_map();
        
        connect(i);
        //cout <<"after connect\n";
        //print_map();
        // 행에서 3을 꼭 빼줘야함
        // 이제 정령의 탐색
        //cout << "fairy_move_result\n";
        fairy_move(i);

    }
    cout << total << "\n";
}