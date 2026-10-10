#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int N, robot_num, test_num;
int map[50][50]; // -1인경우 물건, 양수인 경우에는 먼지가 존재
// 1 based index
struct robot_info {
    int x, y;
    // 로봇의 좌표를 담기 위함
};

vector<robot_info> robots;

int dx[4] = { 0,1,0,-1 };
int dy[4] = { 1,0,-1,0 };
// 오른쪽, 아래쪽 왼쪽 위쪽 순서대로


void print_map() {
    for (int i = 1; i <= N; i++) { // 1 based index
        for (int j = 1; j <= N; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

void print_robot() {
    for (int i = 0; i < robots.size(); i++) {
        cout << i << "th " << robots[i].x << " x "
            << robots[i].y << " y\n";
    }
}


bool is_robot(int x, int y) {
    for (auto robot : robots) {
        if (x == robot.x && y == robot.y) return true;
    }
    return false; // 여기에 오면 겹치는 로봇이 없다는 뜻
}

pair<int, int> cand_axis(robot_info robot) {
    pair<int, int> final_axis = { -1,-1 };
    // 여기서 이동할 수있는 좌표 찾기
    // 이동거리 짧, 행 짧, 열 짧 순의 우선순위 존재
    vector<pair<int,pair<int,int>>> cand_list; // 이동거리, 행, 열 이 들어가는 벡터
    queue<pair<int, int>> q;

    vector<pair<int, int>> paths;

    int visit[50][50];
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            visit[i][j] = -1; // -1이 방문 안했다는 표시
        }
    }

    if (map[robot.x][robot.y] > 0) return { robot.x , robot.y };
    // 해당 위치에 아직도 먼지가 있는경우?

    q.push({ robot.x, robot.y });
    visit[robot.x][robot.y] = 0; // 처음 위치는 방문 표시(거리 0)
    paths.push_back({ robot.x,robot.y });

    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];

            if (nx <= 0 || nx > N || ny <= 0 || ny > N) continue;
            if (map[nx][ny] == -1) continue; // 벽이 있는경우
            if (visit[nx][ny] >= 0) continue; // 이미 방문한 경우
            if (is_robot(nx, ny) == true) continue; // 다른 청소기가 있는 경우
            
            // 위 조건에서 어긋나는게 없는 경우는 이제 방문해도 됨
            q.push({ nx,ny });
            visit[nx][ny] = visit[now.first][now.second] + 1;
            paths.push_back({ nx,ny });
            // 여기서 해당 지점에 먼지가 있는 경우 벡터에 넣기
            if(map[nx][ny] > 0 ) { // 먼지가 있는경우
                cand_list.push_back({ visit[nx][ny] , {nx,ny} }); // 이런 꼴로 넣기
            }
        }
    }
    sort(cand_list.begin(), cand_list.end());

    //for (auto cand : cand_list) {
    //    cout << cand.first << " length " << cand.second.first << " x " << cand.second.second << " y \n";
    //}
    //exit(1);

    if (!cand_list.empty()) {
        final_axis = cand_list[0].second;
    }
    else {
        //cout << robot.x << " x " << robot.y << " y\n";
        //cout << "no dust!\n";
        //sort(paths.begin(), paths.end());
        //for (auto path : paths) {
        //    cout << path.first << " cx " << path.second << " cy \n";
        //}
        final_axis = {robot.x, robot.y};
    }
    return final_axis;
}

void move_robot() {
    for (int i = 0; i < robots.size(); i++) {
        pair<int, int> temp = cand_axis(robots[i]);
        // 이를 바탕으로 실제 로봇 움직이기
        //cout << temp.first << " x " << temp.second << " y \n";
        robots[i].x = temp.first;
        robots[i].y = temp.second; 
    }
}

// sum = 자기 자신, 오 아 왼 위 합치고, 자신이 바라보는 방향만 빼버리기


int cand_dir(robot_info robot) {
    int total = 0;
    int result_direction = -1;
    total += min(20,map[robot.x][robot.y]); // 우선 현재 위치 더하기
    int max_cleans = -1;
    for (int i = 0; i < 4; i++) {
        int nx = robot.x + dx[i];
        int ny = robot.y + dy[i];
        if (nx <= 0 || nx > N || ny <= 0 || ny > N) continue; // 범위를벗어나는 경우
        if (map[nx][ny] == -1) continue; // 물건이 있는 경우

        total += min(20, map[nx][ny]); // 지금 볼 수 있는 범위에서 전체를 합한 내용
    }
    for (int i = 0; i < 4; i++) {
        int cand_cleans = 0;
        int cand_direction = (i + 2) % 4; // 지금 바라보는방향(i)의 반대 방향
        
        
        int nx = robot.x + dx[cand_direction];
        int ny = robot.y + dy[cand_direction];
        if (nx <= 0 || nx > N || ny <= 0 || ny > N) { // 뺴야하는 방향이 범위를 벗어난 경우
            cand_cleans = total;
        }
        else if (map[nx][ny] == -1) { // 빼야하는 방향이 벽인 경우
            cand_cleans = total;
        } 
        else {
            cand_cleans = total - min(20, map[nx][ny]); // 그렇지 않으면 해당 방향대로 빼기
        }
        
        if (cand_cleans > max_cleans) {
            max_cleans = cand_cleans; // 갱신
            result_direction = i; // 최종 위치
        }
    }
    //if (result_direction == -1) {
    //    printf("no direction!\n");
    //    exit(1);
    //}
    return result_direction;
}

void cleaning() {
    // 로봇의 순서대로 해당 내용을 진행함에 유의!
    for (int i = 0; i < robots.size(); i++) {
        int direction = cand_dir(robots[i]);
        // 해당방향의 반대 방향을 제외하고 전부 내용 없애기
        
        //cout << direction << " dir\n";
        
        map[robots[i].x][robots[i].y] -= min(20, map[robots[i].x][robots[i].y]);
        // 현재 위치 에서 빼기
        int no_add_dir = (direction + 2) % 4;
        // 
        for (int j = 0; j < 4; j++) {
            if (j == no_add_dir) continue;
            int nx = robots[i].x + dx[j];
            int ny = robots[i].y + dy[j];
            if (nx <= 0 || nx > N || ny <= 0 || ny > N) continue;
            if (map[nx][ny] == -1) continue; // 물건이 있는 경우 
            
            map[nx][ny] -= min(20, map[nx][ny]);
        }
    }
}

void add_dust() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] > 0) {
                map[i][j] += 5;
            }
        }
    }
}



void spread_dust() {
    int temp[50][50] = {} ;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            int total = 0;
            if (map[i][j] == 0) // 빈칸인 경우
            {// 상 하 좌우 의 합 구하기;

                for (int k = 0; k < 4; k++) {
                    int nx = i + dx[k];
                    int ny = j + dy[k];
                    if (nx <= 0 || nx > N || ny <= 0 || ny > N) continue;
                    if (map[nx][ny] == -1) continue; // 벽인경우
                    total += map[nx][ny];
                }
            }
            temp[i][j] = (total / 10);
        }
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (temp[i][j] == 0) continue;
            map[i][j] = temp[i][j];
        }
    }

}

int print_total_dust() {
    int total = 0;
    for (int i = 1; i <= N; i++) { // 1 based index
        for (int j = 1; j <= N; j++) {
            //cin >> map[i][j];
            if (map[i][j] > 0) total += map[i][j];
        }
    }
    return total;
}

int main() {
    cin >> N >> robot_num >> test_num;
    for (int i = 1; i <= N; i++) { // 1 based index
        for (int j = 1; j <= N; j++) {
            cin >> map[i][j];
        }
    }
    for (int i = 0; i < robot_num; i++) {
        int x, y;
        cin >> x >> y;
        robots.push_back({ x,y });
    }// 이렇게 입력은 다 받음


    for (int turn = 0; turn < test_num; turn++) {
    //이런식으로 실제 동작 시작

        //cout << "before\n";
        //print_robot();
        
        move_robot();
        //exit(1);
        //cout << "after_move\n";
        //print_robot();


        // 실제 청소
        cleaning();
        
        //cout << "after_clean\n";
        //print_map();


        add_dust();

        //cout << "after_add_dust\n";
        //print_map();
        

        spread_dust();

        //cout << "after_spread_dust\n";
        //print_map();

        int final_dust = print_total_dust();
        if (final_dust == 0) {
            cout << final_dust << "\n";
            break;
        }
        cout << final_dust << "\n";
        //exit(1);
    }
}