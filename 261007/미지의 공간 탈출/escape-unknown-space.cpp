#include <iostream>
#include <queue>
using namespace std;

struct bad_time_info{
    int x,y;
    int direction;
    int turn; // 해당 info가 확산되는 턴이 어떻게 되는가를 나타냄
    bool alive; // 해당 확산이 살아있는가?
};

int migi[30][30]; // 미지의 공간
int bad_map[30][30]; // 미지의 공간
int wall[5][11][11]; // 시간의 벽
bad_time_info bad_time[20]; // 시간 이상 현상의 정보를 담음

int N , M , F;

pair<int,pair<int,int>> machine_axis; // 타임 머신의 좌표 , 존재하는 면

int dx[4] = {0,0,1,-1};
int dy[4] = {1,-1,0,0}; // 동 서 남 북 형태
pair<int,pair<int,int>> exit_wall; // 벽의 탈출 좌표를 담는 곳 -> 여기에 다다르면 다른 좌표로 이동
pair<int,int> migi_start; // 미지의 공간의 시작 지점
pair<int,int> start_wall; // 벽이 미지의 공간 내부의 시작지점
pair<int,int> exit_axis;


int change_plane[5][4] = {
    {3,2,5,4},
    {2,3,5,4},
    {0,1,5,4},
    {1,0,5,4},
    {0,1,2,3},
}; // 여기서 5 는 미지

pair<int,int> change_axis(
    pair<int, pair<int,int>> now,
    int temp_plane
){
    int now_plane = now.first;
    int x = now.second.first;
    int y = now.second.second;

    // =========================
    // 0 : 동쪽 면
    // =========================
    if(now_plane == 0){

        // 동 -> 북
        // 동쪽 면의 오른쪽 끝 -> 북쪽 면의 왼쪽 끝
        if(temp_plane == 3){
            return {x, 0};
        }

        // 동 -> 남
        // 동쪽 면의 왼쪽 끝 -> 남쪽 면의 오른쪽 끝
        else if(temp_plane == 2){
            return {x, M - 1};
        }

        // 동 -> 위
        // 동쪽 면의 위쪽 끝 -> 윗면의 오른쪽 끝
        else if(temp_plane == 4){
            return {M - 1 - y, M - 1};
        }
    }

    // =========================
    // 1 : 서쪽 면
    // =========================
    else if(now_plane == 1){

        // 서 -> 남
        // 서쪽 면의 오른쪽 끝 -> 남쪽 면의 왼쪽 끝
        if(temp_plane == 2){
            return {x, 0};
        }

        // 서 -> 북
        // 서쪽 면의 왼쪽 끝 -> 북쪽 면의 오른쪽 끝
        else if(temp_plane == 3){
            return {x, M - 1};
        }

        // 서 -> 위
        // 서쪽 면의 위쪽 끝 -> 윗면의 왼쪽 끝
        else if(temp_plane == 4){
            return {y, 0};
        }
    }

    // =========================
    // 2 : 남쪽 면
    // =========================
    else if(now_plane == 2){

        // 남 -> 동
        // 남쪽 면의 오른쪽 끝 -> 동쪽 면의 왼쪽 끝
        if(temp_plane == 0){
            return {x, 0};
        }

        // 남 -> 서
        // 남쪽 면의 왼쪽 끝 -> 서쪽 면의 오른쪽 끝
        else if(temp_plane == 1){
            return {x, M - 1};
        }

        // 남 -> 위
        // 남쪽 면의 위쪽 끝 -> 윗면의 아래쪽 끝
        else if(temp_plane == 4){
            return {M - 1, y};
        }
    }

    // =========================
    // 3 : 북쪽 면
    // =========================
    else if(now_plane == 3){

        // 북 -> 서
        // 북쪽 면의 오른쪽 끝 -> 서쪽 면의 왼쪽 끝
        if(temp_plane == 1){
            return {x, 0};
        }

        // 북 -> 동
        // 북쪽 면의 왼쪽 끝 -> 동쪽 면의 오른쪽 끝
        else if(temp_plane == 0){
            return {x, M - 1};
        }

        // 북 -> 위
        // 북쪽 면의 위쪽 끝 -> 윗면의 위쪽 끝
        else if(temp_plane == 4){
            return {0, M - 1 - y};
        }
    }

    // =========================
    // 4 : 윗면
    // =========================
    else if(now_plane == 4){

        // 위 -> 동
        // 윗면 오른쪽 끝 -> 동쪽 면 위쪽 끝
        if(temp_plane == 0){
            return {0, M - 1 - x};
        }

        // 위 -> 서
        // 윗면 왼쪽 끝 -> 서쪽 면 위쪽 끝
        else if(temp_plane == 1){
            return {0, x};
        }

        // 위 -> 남
        // 윗면 아래쪽 끝 -> 남쪽 면 위쪽 끝
        else if(temp_plane == 2){
            return {0, y};
        }

        // 위 -> 북
        // 윗면 위쪽 끝 -> 북쪽 면 위쪽 끝
        else if(temp_plane == 3){
            return {0, M - 1 - y};
        }
    }

    return {-1, -1};
}

int escape_machine(){
    int visit[5][11][11] = {};
    queue<pair<int,pair<int,int>>> q;
    
    q.push(machine_axis); // 해당 머신의 내용 넣기
    visit[machine_axis.first][machine_axis.second.first][machine_axis.second.second] = 1;

    while(!q.empty()){
        
        pair<int,pair<int,int>> now = q.front();
        // cout << "now\n";
        
        // cout << now.first <<" plane " << now.second.first << " x " << now.second.second << " y \n";
        int now_plain = now.first; // 현재의 면을 나타내는 것
        q.pop();

        for(int i = 0; i < 4 ;i++){
            //cout <<"change\n" ;
            int nx = now.second.first + dx[i];
            int ny = now.second.second + dy[i];
            //cout << nx << " x " << ny << " y \n";
            int temp_plane = now_plain;
            if(nx < 0 || nx >= M || ny < 0 || ny >= M){
                //cout <<"enter!\n";
                //cout << nx << " x " << ny << " y \n";
                temp_plane = change_plane[now_plain][i]; // 이런식으로 변경
                
                if(temp_plane == 5) {
                    // 여기서는 wall[][][]에 접근하면 안 됨
                    // 시간의 벽 탈출 처리
                    continue;
                }

                
                pair<int,int> change = change_axis(now,temp_plane);
                nx = change.first;
                ny = change.second; // 이런식으로 정의
                //cout << "plane_changed\n";
                //cout << temp_plane << " plane " << nx << " x " << ny << " y \n";
            }
            if(nx < 0 || nx >= M || ny < 0 || ny >= M){
                continue;
            }
            if(wall[temp_plane][nx][ny] == 1){
                //cout <<"wall!\n";
                //cout << temp_plane << " plane " << nx << " x " << ny << " y \n";
                continue; // 벽인 경우
            } 
            if(visit[temp_plane][nx][ny] > 0) {
                //cout <<"visited!\n";
                //cout << temp_plane << " plane " << nx << " x " << ny << " y \n";
                continue; 
            } 
            q.push({temp_plane, {nx,ny}});
            visit[temp_plane][nx][ny] = visit[now_plain][now.second.first][now.second.second] + 1;
            if(temp_plane == exit_wall.first && nx == exit_wall.second.first && ny == exit_wall.second.second){
                // 여기서 실제 머신 좌표 옮겨주기
                machine_axis.first = 5;// 평면위에 있음
                machine_axis.second.first = migi_start.first;
                machine_axis.second.second = migi_start.second;
                return visit[temp_plane][nx][ny]; // 도착지까지 얼마나 걸리는지 저장
            }
        }
    }
    return -1;
}



void wall_exit(int direction, int x, int y){

    int relative_x = x - start_wall.first;
    int relative_y = y - start_wall.second;

    exit_wall.first = direction;

    if(direction == 0){ // 동
        exit_wall.second.first = M - 1;
        exit_wall.second.second = M - 1 - relative_x;
    }

    else if(direction == 1){ // 서
        exit_wall.second.first = M - 1;
        exit_wall.second.second = relative_x;
    }

    else if(direction == 2){ // 남
        exit_wall.second.first = M - 1;
        exit_wall.second.second = relative_y;
    }

    else if(direction == 3){ // 북
        exit_wall.second.first = M - 1;
        exit_wall.second.second = M - 1 - relative_y;
    }
}

void print_migi(){
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            cout << migi[i][j] << " ";
        }
        cout << "\n";
    }
}
void print_bad_map(){
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            cout << bad_map[i][j] << " ";
        }
        cout << "\n";
    }
}

void bad_time_arrive(){
    for(int i = 0; i < F; i++){
        for(int j = 2; j < 10000; j++){
            if(j % bad_time[i].turn == 0){ // 해당 시점을 기준으로
                int nx = bad_time[i].x + dx[bad_time[i].direction];
                int ny = bad_time[i].y + dy[bad_time[i].direction];

                if(nx < 0 || nx >= N || ny < 0 || ny >= N){
                    bad_time[i].alive = false;
                    break;
                } // 범위를 벗어나는 경우
                if(migi[nx][ny] == 1 || migi[nx][ny] == 3 || migi[nx][ny] == 4 ){
                    bad_time[i].alive = false;
                    break;
                } // 범위를 벗어나는 경우
                // 그게 아니면 
                // 해당 위치에 도착하는 bad time 의 최소 시간을 저장하기
                if(bad_map[nx][ny] > j){
                    bad_map[nx][ny] = j; // 이런식으로 정의
                }
                bad_time[i].x = nx;
                bad_time[i].y = ny;
            }
        }
    }
}

void print_bad_status(){
    for(int i = 0; i < F; i++){
        cout << bad_time[i].x << " x "
            << bad_time[i].y << " y "
            << bad_time[i].alive << " alive "
            << bad_time[i].turn << " turn \n";
    }
}

int total_clock = 0;


int machine_out(){
    pair<int,int> machine = machine_axis.second;
    int visit[100][100] = {};
    queue<pair<int,int>> q;
    q.push(machine);
    visit[machine.first][machine.second] = total_clock; // 현재 시간대로 설정

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0; i < 4 ; i++){
            //벽이거나, 자신보다 이상 시간이 앞서거나, 3인 경우에 out , 이미 방문한 경우도
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            int now_time = visit[now.first][now.second] + 1;
            if(nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if(migi[nx][ny] == 1 || migi[nx][ny] == 3 ) continue;
            if(visit[nx][ny] > 0) continue;
            if(bad_map[nx][ny] <= now_time ) continue;

            // 이런게 아닌 경우만 탐색 가능
            q.push({nx,ny});
            visit[nx][ny] = now_time;
        }
    }
    return visit[exit_axis.first][exit_axis.second];
}


int main(){
    cin >> N >> M >> F;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N ; j++){
            cin >> migi[i][j];
            if(migi[i][j] == 4){
                exit_axis.first = i;
                exit_axis.second = j;
            }
            bad_map[i][j] = 100000; // 해당 지점에 얼마나 빨리 해당 값이 들어가는가?
        }
    }

    for(int i = N-1; i >= 0; i--){
        for(int j = N -1 ; j >= 0 ; j--){
            if(migi[i][j] == 3){
                start_wall.first = i;
                start_wall.second = j;
                // 이런식으로 벽의 좌표 갱신
            }
        }
    }

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N ; j++){
            if(migi[i][j] == 3){
                // 해당 좌표 기준으로 상 하 좌 우 0 인 지점이 있는지 찾기
                for(int direction = 0; direction < 4; direction++){
                    int now_x = i + dx[direction];
                    int now_y = j + dy[direction];
                    if(now_x < 0 || now_x >= N || now_y < 0 || now_y >= N) continue;
                    if(migi[now_x][now_y] == 0){
                        // 이러면 해당 방향에 맞는 면에 탈출구가 있는 것
                        wall_exit(direction, i,j);
                        migi_start.first = now_x;
                        migi_start.second = now_y;
                        // 이렇게 정의

                    }
                }
            }
        }
    }
    // cout << "migi_exit\n";
    // cout <<  migi_start.first << " " << migi_start.second << "\n";
    // cout << "wall_exit\n";
    // cout << exit_wall.first << " plane " <<  exit_wall.second.first <<  " " << exit_wall.second.second << "\n";
    
    for(int i = 0; i < 5; i++){
        for(int j = 0 ; j < M; j++){
            for(int k = 0; k < M; k++){
                cin >> wall[i][j][k];
                if(i == 4){
                    // 윗면인 경우에, 윗면 기준 좌표 정리
                    if(wall[i][j][k] == 2){
                        machine_axis.second.first = j;
                        machine_axis.second.second = k;
                        machine_axis.first = i; // 윗면에 존재함을 나타냄
                    }

                }
            }
        }
    }

    

    for(int i = 0; i < F; i++){
        cin >> bad_time[i].x >>bad_time[i].y >> bad_time[i].direction >> bad_time[i].turn;
        bad_time[i].alive = true; // 일단 이렇게 정의
        bad_map[bad_time[i].x][bad_time[i].y] = 1; // 해당값은 실제 이상현상
    }
    //print_migi();
    
    total_clock = escape_machine();
    //cout << " before\n";
    //print_bad_map();
    bad_time_arrive();
    //cout << " after\n";
    //print_bad_map();
    //exit(1);
    if(total_clock != -1){
        
        //cout << total_clock << "\n";
        if(bad_map[machine_axis.second.first][machine_axis.second.second] <= total_clock){
            total_clock = -1 ; //못나감
        }
        else{
            int temp = machine_out();
            if(temp == 0) total_clock = -1;
            else{
                total_clock = temp;
            }
        }


    // 이제 실제 exit

    //1. 타임 머신의 탈출 -> 탈출 할때의 최소 시간을 찾기
    

    // 이후 최소 시간 만큼의 내용을 바탕으로 각 시간 이상 현상을 map에 표시하기

    // 이떄, 해당 탈출구를 시간이상 현상이 막는 경우에, 탈출 못함


    // 그렇지 않은 경우에는 bfs 돌려서 다른 최단 경로 찾기 // 여기서는 bfs 내부에 확산 내용을 넣는게 나아보임
    }
    cout << total_clock << "\n";
}