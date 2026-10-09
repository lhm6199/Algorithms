#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

int map[20][20];

int N, Q;

struct micro_info {
    int size; // 넓이를 나타내는 것
    vector<pair<int, int>> axiss; // 해당좌표를 나타내는 것 이거는 매 턴마다 갱신 필요
    bool alive; // 생존 여부를 나타내는 것

};

micro_info micros[60];

void print_map() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

int dx[4] = { 1,0,-1,0 };
int dy[4] = { 0, 1, 0 ,-1 };

void bfs(int x, int y , bool visit[20][20]) {
    queue<pair<int, int>> q;
    int now_micro = map[x][y];
    
    q.push({ x,y });
    visit[x][y] = true;
    micros[now_micro].axiss.push_back({ x,y }); // 포함되는 좌표 넣기
    micros[now_micro].size = 1;
    while (!q.empty()) {
        pair<int, int> now = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];

            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue; // 범위를 벗어나는 경우
            if (visit[nx][ny] == true) continue; //이미 방문한 경우
            if (map[nx][ny] != now_micro) continue; // 지금 값이랑 다른 경우
            
            q.push({ nx,ny });
            visit[nx][ny] = true;
            micros[now_micro].axiss.push_back({ nx,ny }); // 포함되는 좌표 넣기
            micros[now_micro].size++; //넓이 증가
        }
    }
    //cout << now_micro << " !! " << micros[now_micro].size << "\n";
}




void split_check(int turn) {
    bool visit[20][20] = {};
    int occur_num[100] = {}; // 얼마나 출몰했는가?

    // 그전에 각각의 axis 전부 초기화 필요
    for (int i = 1; i <= turn; i++) {
        micros[i].axiss.clear();
        micros[i].size = 0;
    }

    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) {
            if (visit[i][j] == true) continue;
            if (map[i][j] == 0) continue;
            bfs(i, j, visit);
            occur_num[map[i][j]]++;
            //cout << map[i][j]  <<" occur_num[map[i][j]] " << occur_num[map[i][j]] << "\n";
            
        }
    }
    //이렇게 하고 occur num 확인

    //for (int i = 1; i <= turn; i++) {
    //    cout <<  i << "th " << occur_num[i] << " ";
    //}
    //cout << " \n";

    //exit(1);
    for (int i = 1; i <= turn; i++) {
        if (occur_num[i] >= 2) {
            /*cout << i << "th erase!\n";*/
            // 이때 해당 하는 내용에 접근해서 실제 map에 0으로 전환(죽음 표시)
            micros[i].alive = false;
            for (auto axis : micros[i].axiss) {
                map[axis.first][axis.second] = 0;
            }
            micros[i].axiss.clear(); // 전부 없애버리기
        }
    }
}

pair<int, int> put_new_map(int now_idx, int min_x, int min_y , int max_height, int max_width, int temp_map[60][60]) {
    pair<int, int> result = { -1,-1 };
    
    for (int i = 0; i < N - max_height; i++) {
        for (int j = 0; j < N - max_width; j++) {
            bool is_okay = true;
            // 이렇게 해서 탐색의 범위를 좁히기
            
            //i,j 기준으로 각각의 위치에 다른 녀석이 있는지 확인하면 됨
            //cout << "========= now " << i << " " << j << "\n";
            for (auto axis : micros[now_idx].axiss) {
                int real_x = axis.first - min_x + i;
                int real_y = axis.second - min_y + j;
                //cout << real_x << " x " << real_y << " y ss\n";
                if (temp_map[real_x][real_y] != 0) {
                    
                    is_okay = false;
                    break;
                }
            }
            // 여기를 넘어왔다? 그러면 성공한거니까 해당 좌표 넘기기
            if (is_okay == false) continue;
            result.first = i;
            result.second = j;


            return result;

        }
    }
    return result;
}


void move_another(int turn) {

    int temp_map[60][60] = {};

    vector<pair<int, int>> sort_micro; // -넓이, 넣은 턴 으로 오름차순으로 정렬
    for (int i = 1; i <= turn; i++) {
        // 턴 기준으로 넣어버려
        if (micros[i].alive == false) continue; // 만약 죽어있다면 넘어가
        // 살아있는 겨웅에만
        sort_micro.push_back({-micros[i].size, i}); 
    }

    sort(sort_micro.begin(), sort_micro.end()); // 이렇게 정렬 시작



    // 결국 사용하는거는 sort_micro의 인덱스 이용(second)

    for (int i = 0; i < sort_micro.size(); i++) {
        int now_idx = sort_micro[i].second; //해당 인덱스가 정렬된 상태의 인덱스임
        int max_x = -1;
        int max_y = -1;
        int min_x = 100;
        int min_y = 100;

        for (auto axis : micros[now_idx].axiss) {
            if (max_x < axis.first) {
                max_x = axis.first;
            }
            if (max_y < axis.second) {
                max_y = axis.second;
            }
            if (min_x > axis.first) {
                min_x = axis.first;
            }
            if (min_y > axis.second) {
                min_y = axis.second;
            }
        }

        // 이제 각 최대 세로 넓이, 최대 가로 넓이확인

        int max_height = max_x - min_x;
        int max_width = max_y - min_y; 
        //cout << max_height << " h " << max_width << " w \n ";
        pair<int, int> start_axis = put_new_map(now_idx, min_x , min_y, max_height, max_width, temp_map);
        //cout << start_axis.first << " x " << start_axis.second << " y\n ";
        //exit(1);
        if (start_axis.first == -1 && start_axis.second == -1) continue;
        // 그게 아닌경우에는 해당좌표 기준으로 실제배치
        for (auto axis : micros[now_idx].axiss) {
            temp_map[start_axis.first - min_x + axis.first][start_axis.second - min_y + axis.second] = now_idx;
        }
    }


    // 이제실제 map에 배치

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            map[i][j] = temp_map[i][j];
        }
    }

}

int total_result() {
    set<pair<int, int>> attachs; // 붙어있는 애들을 넣는 자료구조
    int result = 0;
    // 위 아래 부터
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == 0 || map[i + 1][j] == 0) continue;
            if (map[i][j] != map[i + 1][j]) {
                attachs.insert({ map[i][j], map[i + 1][j] });
                attachs.insert({map[i+1][j], map[i][j]}); // 두번 넣어서 나중에 반으로 나누기
            }
        }
    }
    // 양옆
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N-1; j++) {
            if (map[i][j] == 0 || map[i][j+1] == 0) continue;
            if (map[i][j] != map[i][j+1]) {
                attachs.insert({ map[i][j], map[i][j + 1] });
                attachs.insert({ map[i][j + 1], map[i][j] }); // 두번 넣어서 나중에 반으로 나누기
            }
        }
    }

    // 출력

    //for (auto attach : attachs) {
    //    cout << attach.first << "th micro & " << attach.second << "th micro attach\n";
    //}
    //exit(1);

    for (auto attach : attachs) {
        int temp = micros[attach.first].axiss.size() * micros[attach.second].axiss.size();
        result += temp;
    }
    //cout << result << "\n";
    return result/2;
}


int main() {
    cin >> N >> Q;

    for (int turn = 1; turn <= Q; turn++) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        for (int i = r1; i < r2; i++) {
            for (int j = c1; j < c2; j++) {
                map[i][j] = turn; // 각 턴으로 해당 미생물 넣기 // 그냥 90도 돌아갔다고 생각
                
            }
        }
        micros[turn].alive = true;
        //cout << "before\n";
        //print_map();
        // 이제 split 확인 그리고 해당 내용 지우기


        split_check(turn);
        //cout << "after split\n";
        //print_map();


        move_another(turn);
        //cout << "after move\n";
        //print_map();


        // 이제 실제 넓이 구하기
        //cout << " attachs\n";
        int result = total_result();
        cout << result << "\n";
    }

}