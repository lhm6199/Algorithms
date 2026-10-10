#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct turtle_info {
    int x, y; // 좌표 
    bool alive; // 생존 여부
    int arrive_turn; // 도착 여부
};

struct volcano_info{
    int x, y;
    int pressure; // 해당내용의 압력
    int max_pressure; // 임계치
    bool explore; // 터졌는가?를 나타내는 부분
};

int map[30][30]; // 실제 장애물, 화석 거북이를 표시하기 위한 위치
int heat_map[30][30]; // 실제 열기 정보를 담기 위한 위치;

vector <turtle_info> turtles;
vector <volcano_info> volcanos; // 각각 거북이, 화산을 담는 배열

int N, num_turtle, num_volcano;

void print_turtle() {
    for (int i = 0; i < turtles.size(); i++) {
        cout << turtles[i].x << " x "
            << turtles[i].y << " y "
            << turtles[i].alive << " alive! "
            << turtles[i].arrive_turn << " arrive_turn!\n";
    }
}

void print_map() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

void print_heat_map() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << heat_map[i][j] << " ";
        }
        cout << "\n";
    }
}

int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 }; // 우 하 좌 상의 우선순위

bool outofrange(int x, int y) {
    if (x < 0 || x >= N || y < 0 || y >= N) return true;
    return false;
}
bool is_turtle(int x, int y, turtle_info turtle) {
    for (int i = 0; i < turtles.size(); i++) {
        if (turtle.x == turtles[i].x && turtle.y == turtles[i].y) continue; // 현재 거북이는 상관 없음
        if (turtles[i].arrive_turn > 0) continue; // 이미 도착한 거북이
        if (turtles[i].x == x && turtles[i].y == y) return true;
    }
    return false;
}


pair<int, int> cand_axis(turtle_info turtle) {
    
    pair<int, int> result = { -1,-1 };

    int x = N-1;
    int y = N-1; //도착지로 부터 출격
    int visit[30][30] = {};
    queue<pair<int, int>> q;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visit[i][j] = -1; // -1로 초기화
        }
    }


    q.push({ x,y });
    visit[x][y] = 0;
    
    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            if (outofrange(nx, ny) == true) continue; // 범위를 벗어난 경우
            if (map[nx][ny] == 1 || map[nx][ny] == 2) continue; // 산호초(1) / 화석(2)인 경우
            // 다른 거북이가 막고 있는 경우
            if (is_turtle(nx, ny, turtle) == true) continue;
            
            if (visit[nx][ny] >= 0) continue; // 이미 방문한 경우
            q.push({ nx,ny });
            visit[nx][ny] = visit[now.first][now.second] + 1; // 현재 기준으로 합치기
        }
    }

    // 이렇게하면 도착지로부터 거북이와의 거리가 나옴



    if (visit[turtle.x][turtle.y] == -1) { return { turtle.x,turtle.y }; } // 이때는 거북이가 못움직임
    // 그게 아닌 경우
    int min_distance = visit[turtle.x][turtle.y];

    for (int i = 0; i < 4; i++) {
        int nx = turtle.x + dx[i];
        int ny = turtle.y + dy[i];
        if (outofrange(nx, ny) == true) continue;

        if (min_distance - 1 == visit[nx][ny]) {
            result = { nx,ny };
            break;
        }
    }
    return result;
}

void move_turtles(int turn) {
    for (int i = 0; i < turtles.size(); i++) {
        if (turtles[i].alive == false) continue; // 화석이 된 경우
        if (turtles[i].arrive_turn > 0) continue; // 이미 도착한 경우
        // 좌표 기준 bfs 진행

        pair<int, int> move_axis = cand_axis(turtles[i]);

        turtles[i].x = move_axis.first;
        turtles[i].y = move_axis.second;
        



        // 도착 처리
        if (turtles[i].x == N - 1 && turtles[i].y == N - 1) turtles[i].arrive_turn = turn;
    }
}

void add_volcanos() {
    for (int i = 0; i < volcanos.size(); i++) {
        volcanos[i].pressure += 10;
    }
}

void heat_map_update(int volcano_idx) {
    heat_map[volcanos[volcano_idx].x][volcanos[volcano_idx].y] += volcanos[volcano_idx].max_pressure;
    // 현재 위치에 화산 폭발 내용 포함시키기
    for (int i = 0; i < 4; i++) {
        // i -> 퍼지는 방향을 나타냄
        int now_x = volcanos[volcano_idx].x ;
        int now_y = volcanos[volcano_idx].y ;
        int now_score = volcanos[volcano_idx].max_pressure;
        // 그 다음 위치 부터

        while (1) {
            
            now_x += dx[i];
            now_y += dy[i];
            now_score /= 2; // 2 만큼 줄어듦
            
            //cout << i << " dir " << now_x << " x " << now_y << " y " << now_score << " pressure!\n";

            if (outofrange(now_x, now_y)) {
                //cout << i << " dir " << now_x << " x " << now_y << " y break by range\n";
                break; // 해당 지점이 산호초인 경우
            }
            if (now_score <= 0) {
                //cout << i << " dir " << now_x << " x " << now_y << " y break by score\n";
                break; // 해당 지점이 산호초인 경우
            }
            if (map[now_x][now_y] == 1) {
                //cout << i << " dir " << now_x << " x " << now_y << " y break by san\n";
                break; // 해당 지점이 산호초인 경우
            } 

            heat_map[now_x][now_y] += now_score; // 해당 점수로 업데이트
        }
    }
}

void explore() { // 재귀꼴로 해도 될 것 같음
    for (int i = 0; i < volcanos.size(); i++) {
        if (volcanos[i].explore == true) continue; // 이미 터진거는 넘어가
        int volcano_x = volcanos[i].x;
        int volcano_y = volcanos[i].y;

        if (volcanos[i].pressure + heat_map[volcano_x][volcano_y] >= volcanos[i].max_pressure) {
            // 이때 터짐
            volcanos[i].explore = true;
            heat_map_update(i); // 이렇게 업데이트 하고 한번 더 재귀
            
            //cout << i << "th volcano explore!\n";
            //print_heat_map();
            
            explore();
        }
    }
    return;
}

void die_turtle() {
    for (int i = 0; i < turtles.size(); i++) {
        if (turtles[i].alive == false) continue; // 이미 죽은 경우
        if (turtles[i].arrive_turn > 0) continue; // 이미 도착한 경우 넘어가

        if (heat_map[turtles[i].x][turtles[i].y] >= 20) {
            turtles[i].alive = false;
            map[turtles[i].x][turtles[i].y] = 2; // 시체 박아두기
        }
    }

}

void resets() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            heat_map[i][j] = 0;
        }
    }
    for (int i = 0; i < volcanos.size(); i++) {
        if (volcanos[i].explore == true) {
            volcanos[i].pressure = 0;
        }
        volcanos[i].explore = false;
    }

}

int main() {
    cin >> N >> num_turtle >> num_volcano;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> map[i][j]; // 빈공간 0 , 산호초 1 넣기
        }
    }
    for (int i = 0; i < num_turtle; i++) {
        turtle_info temp;
        cin >> temp.x >> temp.y;
        temp.alive = true; // 살아있고
        temp.arrive_turn = -1; // 도착하지 않았고 + 도착한 턴을 기록함
        turtles.push_back(temp);
    }
    for (int i = 0; i < num_volcano; i++) {
        volcano_info temp;
        cin >> temp.x >> temp.y >> temp.max_pressure;
        temp.pressure = 0; // 압력 0
        temp.explore = false; // 아직 안터짐
        volcanos.push_back(temp);
    }

    for (int turn = 1; turn <= 100; turn++) {
    // 최대 100 턴 진행
        // 1. 거북이의 이동
        //if (turn == 8) break;
        //cout << turn << " turn!\n";
        //
        //cout << "bf_move\n";
        //print_turtle();


        move_turtles(turn);

        //cout << "after_move\n";
        //print_turtle();
        
        
        // 2. 화산 압력 10 추가
        add_volcanos();
        explore();
        die_turtle();

        //cout << "after_explore\n";
        //print_turtle();
        
        //초기화 진행


        resets();
        //exit(1);
    }

    for (int i = 0; i < turtles.size(); i++) {
        cout << turtles[i].arrive_turn << "\n";
    }

}