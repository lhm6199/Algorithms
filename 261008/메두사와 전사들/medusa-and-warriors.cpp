#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int N , M;
// 전사 수, 용사 수
int turn = 0;
struct warrior_info{
    int x, y;
    int status; // 1 사망 , 2 생존;
    int stone_turn; // 돌이 된 turn을 나타냄
};
struct snake_info{
    int x,y;
    int direction; // 바라보는 방향
} ;


vector<warrior_info> warriors;
snake_info snake;
int map[100][100]; // 거리를 나타내는 것

int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1}; // 상하 좌우의 순서


int dx2[4] = {0,0,-1,1};
int dy2[4] = {-1,1,0,0}; // 상하 좌우의 순서
pair<int,int> snake_home;
pair<int,int> snake_park; //

void print_warriors(){
    for(auto warrior : warriors){
        cout << "x : " << warrior.x
            << " y : " << warrior.y 
            << " status : " << warrior.status
            << "stone_turn : "  << warrior.stone_turn
            << "\n";
    }
}
void print_snake(){
        cout << "x : " << snake.x
            << " y : " << snake.y 
            << " direction : " << snake.direction
            << "\n";
}

int path[100][100] = {};

bool bfs(){
    queue<pair<int,int>> q;
    
    pair<int,int> start = snake_park;
    q.push(start);
    path[start.first][start.second] = 1;

    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0 ; i < 4; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if(map[nx][ny] == 1) continue; // 도로가 아닌 경우 0 일때 도로
            if(path[nx][ny] > 0) continue; // 이미 방문한 지점은 안됨
            
            // 이제 진짜 탐색 시작
            path[nx][ny] = path[now.first][now.second] + 1;
            q.push({nx,ny});
        }
    }
    return path[snake_home.first][snake_home.second]; // 이런식으로 처리
}

int distance(int x1, int y1, int x2, int y2){
    int dist_x = x1 - x2;
    int dist_y = y1 - y2;
    if(dist_x < 0) dist_x = dist_x * -1;
    if(dist_y < 0) dist_y = dist_y * -1;
    return dist_x + dist_y;
}

void move_snake(){
    int move_way = -1; // 해당 내용은 어느 방향으로 이동하는가를 나타내는 것
    int now_dist = path[snake.x][snake.y];
    for(int i = 0; i < 4 ; i++){
        int nx = snake.x + dx[i];
        int ny = snake.y + dy[i];
        if(nx < 0 || nx >= N || ny < 0 || ny >= N) continue; // 범위를 벗어나는 경우
        if(map[nx][ny] == 1) continue; // 도로가 아닌 경우
        int new_dist = path[nx][ny];
        if(new_dist ==  now_dist - 1){
            now_dist = new_dist;
            move_way = i; //해당 방향으로 변경
            break;

        }
    }
    // 실제로 이동
    snake.x = snake.x + dx[move_way];
    snake.y = snake.y + dy[move_way]; // 이런식으로 움직이기
}

int is_warrior(){
    int die_num = 0;
    for(int i = 0; i < warriors.size(); i++){
        if(warriors[i].status == 1) continue;
        if(warriors[i].x == snake.x && warriors[i].y == snake.y){
            die_num++;
            // 해당 전사의 status 바꾸기
            warriors[i].status = 1;
        }
    }
    return die_num;
}
int one_way_dist(int first , int second){
    int dist = first - second;
    if(dist < 0) dist = dist * -1;
    return dist;
}



void warrior_be_stone(int x, int y, int way, bool sight_map[100][100]){
    // for(int i = 0; i < warriors.size(); i++){
    //     if(warriors[i].x == x && warriors[i].y == y) {
    //         warriors[i].stone_turn = turn; // 해당 턴에 돌이 되어버려
    //     }
    // } // 이건 나중에 처리하자 
    // 이제 해당 범위 기준으로 sight map 변경 시작
    if(way == 0){
        int col_height[100] = {};
        // 뱀 기준으로 왼쪽 오른쪽인가?
        if(snake.y < y){ // 오른쪽
            for(int j = N-1; j >= y ; j--){
                int dist = one_way_dist(j , y); // y와의 거리를 나타냄
                if(dist <= 1) col_height[j] = x ;
                else{
                    col_height[j] = x - (dist - 1);
                }
            }
        }
        else if(snake.y == y){ // 같은 위치
            col_height[y] = x;
        }
        else{ // 왼쪽
            for(int j = 0; j <= y ; j++){
                int dist = one_way_dist(j , y); // y와의 거리를 나타냄
                if(dist <= 1) col_height[j] = x;
                else{
                    col_height[j] =  x - (dist - 1);
                }
            }
        }
        for(int j = 0; j < N ; j++){
            int row_idx = 0;
            for(int k = 0; k < col_height[j]; k++){
                sight_map[row_idx++][j] = 0; // 이런식으로 채우기
            }
        }
    }
    else if(way == 1){ // 아래 방향
        int col_height[100] = {};
        // 뱀 기준으로 왼쪽 오른쪽인가?
        if(snake.y < y){ // 오른쪽
            for(int j = N-1; j >= y ; j--){
                int dist = one_way_dist(j , y); // y와의 거리를 나타냄
                if(dist <= 1) col_height[j] = ((N - 1) - x) ;
                else{
                    col_height[j] = ((N - 1) - x) - (dist - 1);
                }
            }
        }
        else if(snake.y == y){ // 같은 위치
            col_height[y] = ((N - 1) - x);
        }
        else{ // 왼쪽
            for(int j = 0; j <= y ; j++){
                int dist = one_way_dist(j , y); // y와의 거리를 나타냄
                if(dist <= 1) col_height[j] = ((N - 1) - x) ;
                else{
                    col_height[j] = ((N - 1) - x) - (dist - 1);
                }
            }
        }
        for(int j = 0; j < N ; j++){
            int row_idx = N-1;
            for(int k = 0; k < col_height[j]; k++){
                sight_map[row_idx--][j] = 0; // 이런식으로 채우기
            }
        }
    }
    else if(way == 2){ // 좌측 방향
        int row_height[100] = {};
        // 뱀 기준으로 위쪽인가 아래쪽인가?

        //cout << "\n";
        if(snake.x < x){ // 아래쪽
            for(int i = N-1; i >= x ; i--){
                int dist = one_way_dist(i , x); // y와의 거리를 나타냄
                if(dist <= 1) row_height[i] = y ;
                else{
                    row_height[i] = y - (dist - 1);
                }
            }
        }
        else if(snake.x == x){ // 같은 위치
            row_height[x] = y;
        }
        else{ // 왼쪽
            for(int i = 0; i <= x ; i++){
                int dist = one_way_dist(i , x); // y와의 거리를 나타냄
                if(dist <= 1) row_height[i] = y;
                else{
                    row_height[i] =  y - (dist - 1);
                }
            }
        }
        // cout<<"=====\n";
        // for(int i = 0; i < N ; i++){
        //     cout << row_height[i] << " ";
        // }
        // cout <<"\n";
        // cout<<"=====\n";
        for(int i = 0; i < N ; i++){
            int row_idx = 0;
            for(int k = 0; k < row_height[i]; k++){
                sight_map[i][row_idx++] = 0; // 이런식으로 채우기
            }
        }
    }
    else if(way == 3){ // 오른쪽
        int row_height[100] = {};
        // 뱀 기준으로 위쪽인가 아래쪽인가?
        if(snake.x < x){ // 아래쪽
            for(int i = N-1; i >= x ; i--){
                int dist = one_way_dist(i , x); // y와의 거리를 나타냄
                if(dist <= 1) row_height[i] = N - 1 - y ;
                else{
                    row_height[i] = N - 1 - y  - (dist - 1);
                }
            }
        }
        else if(snake.x == x){ // 같은 위치
            row_height[x] = N - 1 - y;
        }
        else{ // 왼쪽
            for(int i = 0; i <= x ; i++){
                int dist = one_way_dist(i , x); // y와의 거리를 나타냄
                if(dist <= 1) row_height[i] = N - 1 - y;
                else{
                    row_height[i] =  N - 1 - y - (dist - 1);
                }
            }
        }
        for(int i = 0; i < N ; i++){
            int row_idx = N-1;
            for(int k = 0; k < row_height[i]; k++){
                sight_map[i][row_idx--] = 0; // 이런식으로 채우기
            }
        }
    }
}

bool result_sight[100][100]; // 얘는 최소가 갱신되면 바뀜

void apply_sight(bool sight_map[100][100]){
    for(int i = 0; i < N ; i++){
        for(int j = 0 ; j < N ; j++){
            result_sight[i][j] = sight_map[i][j];
        }
    }
}

int snake_sight(){
    int warrior_map[100][100] = {}; // 전사의 위치를 나타내기 위함
    int max_stone = -1;
    int final_dir = -1;
    for(int i = 0; i < warriors.size(); i++){
        if(warriors[i].status == 1) continue;
        warrior_map[warriors[i].x][warriors[i].y]++;
    }
    
    // 이제 각 방향 대로 메두사의 시선 처리
    for(int way = 0; way < 4 ; way++){ // 바라보는 방향
        int now_stone = 0;
        bool sight_map[100][100] = {}; // 얘는 결정되면 나중에 갖고 오기
        if(way == 0){// 위 방향을 보는 것
            int col_height[100] = {};
            for(int j = 0; j < N ; j++){
                int dist = one_way_dist(j , snake.y);
                //cout << j << " , " << snake.y << " dist: " << dist << "\n";
                if(dist <= 1) col_height[j] = snake.x ; // 양옆, 본인 인경우
                else{
                    col_height[j] = snake.x - (dist-1); // 이런식으로 정의
                }
            }
            // for(int i = 0 ; i < N; i++){
            //     cout << col_height[i] << " ";
            // }
            // cout << "\n";
            // sight를 위에서 부터 채우기
            for(int j = 0; j < N ; j++){
                int row_idx = 0;
                for(int k = 0; k < col_height[j]; k++){
                    sight_map[row_idx++][j] = 1; // 이런식으로 채우기
                }
            }
            //print_snake();

            for(int i = N-1 ; i >= 0 ; i--){ // 아래서 부터 올라옴
                for(int j = 0 ; j < N ; j++){
                    if(sight_map[i][j] == 1){
                        if(warrior_map[i][j] > 0){
                            warrior_be_stone(i,j,way, sight_map);
                            now_stone +=  warrior_map[i][j]; //
                            // 이제 여기서 해당 방향으로 
                        }
                    }
                }
            }
        }


        else if(way == 1){ // 아래 방향
            int col_height[100] = {};
            for(int j = 0; j < N ; j++){
                int dist = one_way_dist(j , snake.y);
                if(dist <= 1) col_height[j] = ((N - 1) - snake.x) ; // 양옆, 본인 인경우
                else{
                    col_height[j] = ((N - 1) - snake.x) - (dist - 1); // 이런식으로 정의
                }
            }
            // sight를 밑에서 부터 채우기
            for(int j = 0; j < N ; j++){
                int row_idx = N-1;
                for(int k = 0; k < col_height[j]; k++){
                    sight_map[row_idx--][j] = 1; // 이런식으로 채우기
                }
            }
            //print_snake();

            for(int i = 0 ; i < N ; i++){
                for(int j = 0 ; j < N ; j++){
                    if(sight_map[i][j] == 1){
                        if(warrior_map[i][j] > 0){
                            warrior_be_stone(i,j,way, sight_map);
                            now_stone += warrior_map[i][j] ; //
                            // 이제 여기서 해당 방향으로 
                        }
                    }

                }
            }
        }

        else if(way == 2){// 왼쪽방향
            int row_height[100] = {};
            for(int i = 0; i < N ; i++){
                int dist = one_way_dist(i , snake.x);
                if(dist <= 1) row_height[i] = snake.y ; // 양옆, 본인 인경우
                else{
                    row_height[i] = snake.y - (dist - 1); // 이런식으로 정의
                }
            }
            // sight를 왼쪽에서 부터 채우기
            for(int i = 0; i < N ; i++){
                int col_idx = 0;
                for(int k = 0; k < row_height[i]; k++){
                    sight_map[i][col_idx++] = 1; // 이런식으로 채우기
                }
            }

            for(int j = snake.y - 1; j >= 0; j--){
                for(int i = 0; i < N; i++){
                    if(sight_map[i][j] == 1 &&
                    warrior_map[i][j] > 0){

                        warrior_be_stone(i, j, way, sight_map);
                        now_stone += warrior_map[i][j];
                    }
                }
            }
        }
        else if(way == 3){
            int row_height[100] = {};
            for(int i = 0; i < N ; i++){
                int dist = one_way_dist(i , snake.x);
                if(dist <= 1) row_height[i] = (N - 1) - snake.y ; // 양옆, 본인 인경우
                else{
                    row_height[i] = (N - 1) - snake.y - (dist - 1); // 이런식으로 정의
                }
            }
            // sight를 오른쪽 부터 채우기
            for(int i = 0; i < N ; i++){
                int col_idx = N-1;
                for(int k = 0; k < row_height[i]; k++){
                    sight_map[i][col_idx--] = 1; // 이런식으로 채우기
                }
            }

           for(int j = snake.y + 1; j < N; j++){
                for(int i = 0; i < N; i++){
                    if(sight_map[i][j] == 1 &&
                    warrior_map[i][j] > 0){

                        warrior_be_stone(i, j, way, sight_map);
                        now_stone += warrior_map[i][j];
                    }
                }
            }

        } // 오른쪽 방향

        if(max_stone < now_stone){
            max_stone = now_stone;
            final_dir = way; // 해당 방향으로 바라본다.
            apply_sight(sight_map); // 해당 내용으로 적용
        }
    }
    // 해당 final 에 있는 애들은 돌로 변화한다는 내용 추가로 구현 필요

    for(int i = 0; i < warriors.size(); i++){
        if(result_sight[warriors[i].x][warriors[i].y] == 1){
            warriors[i].stone_turn = turn; // 이렇게 돌로 만들기
        }
    }

    if(max_stone == -1 ){
        return 0;
    }
    return max_stone;
}

pair<int,int> move_warrior(){
    int total_move = 0;
    int total_attack = 0;
    for(int i = 0; i < warriors.size(); i++){
        int move_way = -1;
        int now_dist = distance(warriors[i].x ,warriors[i].y , snake.x , snake.y );
        if(warriors[i].status == 1) continue;
        if(warriors[i].stone_turn == turn) continue;
        
        for(int way = 0 ; way < 4 ; way++){
            int nx = warriors[i].x + dx[way];
            int ny = warriors[i].y + dy[way];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if(result_sight[nx][ny]) continue;
            // 그렇지 않은 경우에 움직이기
            int new_dist = distance(nx ,ny , snake.x , snake.y );
            if(now_dist > new_dist){
                now_dist = new_dist;
                move_way = way;
            }
        }
        if(move_way != -1){
            total_move++;
            warriors[i].x = warriors[i].x + dx[move_way];
            warriors[i].y = warriors[i].y + dy[move_way];
        }
        if(warriors[i].x == snake.x && warriors[i].y == snake.y){
            total_attack++;
            warriors[i].status = 1;
        }
        // 메두사 만나는 경우 체크
    }
    // 해당 내용으로 이동

    // 한번 더 이동
    for(int i = 0; i < warriors.size(); i++){
        int move_way = -1;
        int now_dist = distance(warriors[i].x ,warriors[i].y , snake.x , snake.y );
        if(warriors[i].stone_turn == turn) continue;
        if(warriors[i].status == 1) continue;
        
        for(int way = 0 ; way < 4 ; way++){
            int nx = warriors[i].x + dx2[way];
            int ny = warriors[i].y + dy2[way];
            if(nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if(result_sight[nx][ny]) continue;
            // 그렇지 않은 경우에 움직이기
            int new_dist = distance(nx ,ny , snake.x , snake.y );
            if(now_dist > new_dist){
                now_dist = new_dist;
                move_way = way;
            }
        }
        if(move_way != -1){
            total_move++;
            warriors[i].x = warriors[i].x + dx2[move_way];
            warriors[i].y = warriors[i].y + dy2[move_way];
        }
        if(warriors[i].x == snake.x && warriors[i].y == snake.y){
            total_attack++;
            warriors[i].status = 1;
        }
    }
    return {total_move, total_attack};
}

int main(){
    cin >> N >> M;
    cin >> snake_home.first >> snake_home.second >> snake_park.first >> snake_park.second;
    for(int i =0; i < M; i++){
        // 좌표가 차례대로 들어감
        warrior_info temp;
        cin >> temp.x >> temp.y;
        temp.status = 2;// 처음에는 생존 상태
        temp.stone_turn = -1; // -1은 돌이 안되었다는 뜻
        warriors.push_back(temp); //temp 내용 집어넣기
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            cin >> map[i][j];// map에 대한 내용 넣기
        }
    }
    if(bfs() == 0){
        // false 인 경우에는 
        cout << "-1\n";
    }
    else{
        snake.x = snake_home.first;
        snake.y = snake_home.second;
        snake.direction = -1; // 아직은 방향 없음
        while(1){
            int die_num = 0; // 얘는 필요 없긴 하네
            int stone_num = 0 ;
            pair<int,int> infos;
            // snake 이동
            move_snake();
            // 해당 위치에 전사 있는지 확인
            die_num = is_warrior();
            if(snake.x == snake_park.first && snake.y == snake_park.second){
                cout << "0\n";
                break;
            }
            stone_num = snake_sight();
            infos = move_warrior();
            //cout << turn << " turn!\n";
            cout << infos.first << " " << stone_num << " " << infos.second << "\n";
            turn ++;
            //if(turn == 4) exit(1);
        }
    }
    //exit(1);
    // 이런식으로 집어 넣기

}
