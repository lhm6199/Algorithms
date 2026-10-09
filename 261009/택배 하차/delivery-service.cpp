#include<iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct box_info {
    int x, y;
    int h, w;
    bool alive; // 해당 박스가 맵 상에 존재하는지 보기 위함;
};

box_info boxs[200]; // 인덱스 기준으로 박스 관리
int map[100][100]; //box의 index가 담김ㄴ
int N, M;

void print_map() {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}


void gravity(int idx) {
    // box의 인덱스 기반으로 시작

    int temp[100][100] = {};
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] == idx) {
                temp[i][j] = 0;
            }
            else {
                temp[i][j] = map[i][j];
            }
        }
    }

    int min_x = 10000;
    

    int start_x = boxs[idx].x + boxs[idx].h;
    int start_y = boxs[idx].y;
    int end_y = boxs[idx].y + boxs[idx].w; //  탐색 범위 : start_y <= ㅁ < end_y ; 
    //cout << idx << " idx " << start_y << " start " << end_y << " end\n";
    for (int j = start_y; j < end_y; j++) {
        int temp = N;
        for (int i = start_x; i <= N; i++) {
            if (map[i][j] != 0) {
                // 이때는 다른 박스가 있다는 뜻
                temp = i - 1; // 새로운 바닥
                //cout << idx << "'s " << temp << " cand_x\n";
                break;
            }
        }
        //cout << idx << "'s " << temp << " cand_x\n";
        if (min_x > temp) {
            min_x = temp; // 새로운 값으로 갱신
        }
    }
    //cout << idx << "'s min_x :" << min_x << "\n";
    // 전체를 다 거친 이후에 새로 갱신
    boxs[idx].x = min_x - boxs[idx].h + 1;

    // 이후에 블록 정리
    for (int i = 0; i < boxs[idx].h; i++) {
        for (int j = 0; j < boxs[idx].w; j++) {
            temp[i + boxs[idx].x][j + boxs[idx].y] = idx; // 상대 좌표 기준으로 정의
        }
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            map[i][j] = temp[i][j]; // 실제로 옮기기
        }
    }
}

void put_box(int k, int h, int w ,int c) {
    boxs[k].h = h;
    boxs[k].w = w;
    boxs[k].x = 1; // 1 base로 가자
    boxs[k].y = c; 
    boxs[k].alive = true;

    // 이후에 일단 map에 박스 박아두기
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            map[i + boxs[k].x][j + boxs[k].y] = k; // 상대 좌표 기준으로 정의
        }
    }
    //print_map();

    // 이제 중력 기반으로 내리기
    gravity(k);
    //cout << "after_gravity\n";
    //print_map();
    //exit(1);
}

int left_out_idx() { // 왼쪽으로 빠지는 인덱스 찾기
    vector<int> candidate;
    for (int idx = 0; idx < 101; idx++) {
    // 각 택배 번호 별로 왼쪽으로 빠질 수 있는게 있는지 확인
        if (boxs[idx].alive == false) continue; // 활성화 x인 경우 out
        // 그렇지 않은경우
        // 자신의 왼쪽에 뭔가가 있는지 보기

        int start_x = boxs[idx].x;
        int end_x = boxs[idx].x + boxs[idx].h; // 범위 : start_X <= s < end_x
        int start_y = boxs[idx].y -1; // 자기보다 왼쪽부터 시작
        int end_y = 1; // 1 에서 끝남 범위 : start_y <= s <= end_y

        bool is_okay = true;

        for (int i = start_x; i < end_x; i++) {
            for (int j = start_y; j >= end_y; j--) {
                if (map[i][j] != 0) // 0 이 아닌 경우
                {
                    is_okay = false;
                }
            }
        }
        if (is_okay == true) {
            candidate.push_back(idx);
        }
    }
    sort(candidate.begin(), candidate.end());

    //for (auto cand : candidate) {
    //    cout << cand << " ";
    //}
    //cout << "\n";

    return candidate[0]; // 오름차순으로 정렬된것중 제일 앞에 있는 것
}

int right_out_idx() { // 왼쪽으로 빠지는 인덱스 찾기
    vector<int> candidate;
    for (int idx = 0; idx < 101; idx++) {
        // 각 택배 번호 별로 왼쪽으로 빠질 수 있는게 있는지 확인
        if (boxs[idx].alive == false) continue; // 활성화 x인 경우 out
        // 그렇지 않은경우
        // 자신의 왼쪽에 뭔가가 있는지 보기

        int start_x = boxs[idx].x;
        int end_x = boxs[idx].x + boxs[idx].h; // 범위 : start_X <= s < end_x
        int start_y = boxs[idx].y + boxs[idx].w; // 자기보다 왼쪽부터 시작
        int end_y = N; // 1 에서 끝남 범위 : start_y <= s <= end_y

        bool is_okay = true;

        for (int i = start_x; i < end_x; i++) {
            for (int j = start_y; j <= end_y; j++) {
                if (map[i][j] != 0) // 0 이 아닌 경우
                {
                    is_okay = false;
                }
            }
        }
        if (is_okay == true) {
            candidate.push_back(idx);
        }
    }
    sort(candidate.begin(), candidate.end());

    //cout << candidate.size() << " size!\n";
    //exit(1);

    //for (auto cand : candidate) {
    //    cout << cand << " ";
    //}
    //cout << "\n";
    //exit(1);
    return candidate[0]; // 오름차순으로 정렬된것중 제일 앞에 있는 것
}


void erase_box(int idx) {
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] == idx) {
                map[i][j] = 0;
            }
        }
    }
}

void after_erase_gravity() {
    bool visit[100] = {};
    for (int i = N; i > 0; i--) {
        for (int j = N; j > 0; j--) {
            // 오른쪽 아래에서 부터 진행
            int now_idx = map[i][j];
            if (now_idx == 0) continue;
            if (visit[now_idx] == true) continue; // 이미 방문한 곳
            visit[now_idx] = true; // 해당 지점 방문 표시

            gravity(now_idx);

        
        }
    }

}




int main() {
    cin >> N >> M;
    for (int turn = 0; turn < M; turn++) {
        int k, h, w, c; // 택배 번호, 세로 크기, 가로 크기, 좌측 좌표
        cin >> k >> h >> w >> c;
        put_box(k, h, w, c);
    }
    //print_map();
    vector <int> result;
    // 이제 좌 우측 나누면서 블록 빼기
    for (int turn = 0; turn < M; turn++) {
        if (turn % 2 == 0) {
            int out_idx = left_out_idx();
            boxs[out_idx].alive = false;
            erase_box(out_idx);
            //cout << out_idx << "left out!\n";
            // 이제 gravity 적용
            after_erase_gravity();
            
            //print_map();
            result.push_back(out_idx);
            //exit(1);
            // 왼쪽
        }
        else { // 오른쪽
            int out_idx = right_out_idx();
            
            boxs[out_idx].alive = false;
            erase_box(out_idx);
            //cout << out_idx << " right out!\n";
            after_erase_gravity();
            //print_map();
            result.push_back(out_idx);

            //exit(1);
        }
    }
    //cout << "map!\n";
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << "\n";
    }

}