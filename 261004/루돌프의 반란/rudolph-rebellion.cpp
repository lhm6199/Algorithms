#include <iostream>
#include <vector>
#include <queue>

using namespace std;


// 1base index이용
struct santa_info{
    int x,y; //좌표를 담는 배열
    int move_dir; // 움직인 방향
    int lazy_turn; // 기절 유무 충돌이 발생하면 해당 내용을 충돌한 턴으로 바꾸기
    // 그래서 해당 내용이 현재 턴보다 1 작으면 움직이지 않기로
    int score;
    bool live;// 살아 있는가?
};

struct dear_info{
    int x,y; //좌표를 담는 배열
    int move_dir; // 움직인 방향
};

int N, turn, santa_num;
int dear_strong, santa_strong;
//여기서는 따로 게임판을 만들어야할까?
// 일단 pass

santa_info santa_list[40];
vector<santa_info> santas;
dear_info dear;

int dx8[8] = {-1,0,1,0, -1,-1,1,1};
int dy8[8] = {0,1,0,-1, 1,-1,-1,1};
int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1}; // 상 우 하 좌의 순서

void print_santa(){
    for(auto santa : santas){
        cout << " x : " << santa.x 
            << " y : " << santa.y
            << " move direction : " << santa.move_dir
            << " lazy turn : " << santa.lazy_turn
            << " score : " << santa.score
            << " live : " << santa.live << " \n";
    }
}


void print_dear(){
    cout << " x : " <<dear.x 
        << " y : " <<dear.y
        << " dir : " <<dear.move_dir << "\n";
}

int distance(int x1, int y1, int x2, int y2){
    int dist_x = x1 - x2;
    int dist_y = y1 - y2;
    return dist_x * dist_x + dist_y * dist_y;// 실제 거리를 리턴
}



void chain_santa_bfs(int start_santa, int dir){
    bool visit[40] = {};
    vector<int> cand_idxs;
    queue<int> q;
    visit[start_santa] = true; // 해당 지점 방문 표시
    // 처음에 해당 시작 지점 기준으로 다른 산타가 있는지 확인
    for(int i = 0; i < santas.size(); i++){
        if(santas[i].live == false) continue;
        if(visit[i] == true) continue;
        if(santas[i].x == santas[start_santa].x && santas[i].y == santas[start_santa].y){
            q.push(i); // 해당 인덱스를 넣어
            visit[i] = true;
            cand_idxs.push_back(i);
        }
    }
    //cout << cand_idxs[0] <<"th will move!\n";
    while(!q.empty()){
        int now_idx = q.front();
        q.pop();

        int move_x = santas[now_idx].x + dx8[dir];
        int move_y = santas[now_idx].y + dy8[dir];
        if(move_x <= 0 || move_x > N || move_y <= 0 || move_y > N ) continue;
        for(int i = 0; i < santas.size(); i++){
            if(santas[i].live == false) continue;
            if(visit[i] == true) continue;
            if(santas[i].x == move_x && santas[i].y == move_y){
                visit[i] = true;
                q.push(i);
                cand_idxs.push_back(i); // 밀리는 애들 넣기
            }
        }
    }
    // 밀쳐짐 구현 완료
    // for(int i = 0; i < cand_idxs.size(); i++){
    //     cout << " pull idx  : " << cand_idxs[i] << " \n";
    // }
    // 이제 각각의 내용을 실제로 움직이기
    for(int i = 0; i < cand_idxs.size(); i++){
        if(santas[cand_idxs[i]].live == false) continue; // 죽은 산타는 없애
        int cand_x = santas[cand_idxs[i]].x + dx8[dir];
        int cand_y = santas[cand_idxs[i]].y + dy8[dir];
        // 이렇게 움직일 위치 확인
        if(cand_x <= 0 || cand_x > N || cand_y <= 0 || cand_y > N ){
            // 해당 산타는 죽음
            santas[cand_idxs[i]].live = false;
            continue;
        }
        // 이게 아니면 다른 산타는 움직임
        santas[cand_idxs[i]].x = cand_x;
        santas[cand_idxs[i]].y = cand_y;
    }
    //exit(1);
}

void dear_attack(int move_turn){
    for(int i = 0; i < santas.size(); i++){
        if(santas[i].live == false) continue;
        if(dear.x == santas[i].x && dear.y == santas[i].y){
            santas[i].score += dear_strong; // 사슴의 힘만큼 추가
            santas[i].lazy_turn = move_turn; //충돌 당한 턴 추가
            int dear_dir = dear.move_dir;
            int cand_x = santas[i].x + dx8[dear_dir] * dear_strong;
            int cand_y = santas[i].y + dy8[dear_dir] * dear_strong;
            
            if(cand_x <= 0 || cand_x > N || cand_y <= 0 || cand_y > N ){
                //범위 밖으로 밀리는 경우
                santas[i].live = false;
                return;
            }
            //범위 밖이 아닌경우 -> 일단 이동 시킴
            //이후 해당 내용 기준으로 bfs꼴로 해당 방향으로 산타가 있는지 확인
            santas[i].x = cand_x;
            santas[i].y = cand_y;
            // cout << "after_attack\n" ;
            // print_santa();
            chain_santa_bfs(i, dear_dir);
            // cout << "after_chain\n" ;
            // print_santa();
            break; // 어차피 한명만 밀림
        }
    }
    

}

void move_dear(int turn){//루돌프를 움직이기
    vector<int> cand_santa_idx;
    int min = 10000;
    // 산타랑 거리 측정
    for(int i = 0; i < santas.size(); i++){
        //탈락한 산타는 빼고 생각
        if(santas[i].live == false) continue;
        int temp_dist = distance(santas[i].x, santas[i].y, dear.x , dear.y); // 이렇게 거리를 구한 이후
        if(temp_dist < min){
            cand_santa_idx.clear();
            cand_santa_idx.push_back(i); //해당 인덱스 집어 넣기
            min = temp_dist;
        }
        else if(temp_dist == min){
            cand_santa_idx.push_back(i); //해당 인덱스 집어 넣기
        }
    }
    // for(auto idx : cand_santa_idx){
    //     cout << idx << "th santa\n";
    // }
    
    int max_x = -1;
    int max_y = -1;
    int final_santa_idx = -1;

    // 가장 가까운 산타 정하기
    // 행, 열이 클수록 해당 산타에 가까이 감
    for(int i = 0; i < cand_santa_idx.size(); i++){
        if(max_x < santas[cand_santa_idx[i]].x){
            max_x = santas[cand_santa_idx[i]].x;
            max_y = santas[cand_santa_idx[i]].y;
            final_santa_idx = cand_santa_idx[i];
        }
        else if(max_x == santas[cand_santa_idx[i]].x){
            if(max_y < santas[cand_santa_idx[i]].y){
                max_x = santas[cand_santa_idx[i]].x;
                max_y = santas[cand_santa_idx[i]].y;
                final_santa_idx = cand_santa_idx[i];
            }
        }
    }
    // cout << cand_santa_idx.size() << " size " << endl;
    // cout << max_x << " x " << max_y  << " y santa idx : " << final_santa_idx << endl;

    //해당 산타랑 가까운 방향으로 이동
    int min_x = -1;
    int min_y = -1;
    int move_dir = -1;
    int min_dist = 10000;
    for(int i = 0; i < 8; i++){
        int cand_x = dear.x + dx8[i];
        int cand_y = dear.y + dy8[i]; // 해당 방향대로 움직이게
        int dist = distance(cand_x, cand_y , max_x, max_y);
        if(min_dist > dist){
            min_x = cand_x;
            min_y = cand_y;
            move_dir = i; // i번째 방향으로 이동했음을 나타냄
            min_dist = dist;
        }
    }

    dear.x = min_x;
    dear.y = min_y;
    dear.move_dir = move_dir;

    // 이렇게 설정하고 이제 충돌 판정
    //해당 위치에 있는가?를 봄
    dear_attack(turn);
    
    
    
}



void move_santa(int turn){
    for(int i = 0; i< santas.size() ; i++){
        // 죽은 산타
        if(santas[i].live == false) continue; 
        // 기절한 산타
        if(santas[i].lazy_turn == turn || santas[i].lazy_turn == turn - 1) continue;
        //cout << i <<"th santa move!\n"; 
        // 이제 산타 움직임
        int min_dist = distance(santas[i].x, santas[i].y , dear.x, dear.y); // 최소 거리(지금 루돌프와의 거리로 생각)
        int min_dir = -1; // 그때의 방향
        
        for(int j = 0; j < 4; j++){
            int cand_x = santas[i].x + dx[j]; // j가 방향
            int cand_y = santas[i].y + dy[j];
            
            if(cand_x <= 0 || cand_x > N || cand_y <= 0 || cand_y > N) continue;
            
            // 해당 지점에 다른 산타가 있는 경우
            bool can_move = true;
            for(int k = 0 ; k < santas.size(); k++){
                if( k == i ) continue;
                if(santas[k].live == false) continue;
                if(santas[k].x == cand_x && santas[k].y == cand_y){
                    can_move = false;
                    break;
                }
            }
            if(can_move == false) continue;
            //cout << cand_x << " x " << cand_y << " y cand\n";
            int cand_dist = distance(cand_x, cand_y , dear.x, dear.y);
            if(cand_dist < min_dist){
                min_dist = cand_dist;
                min_dir = j;
            }
        }

        // 여기서 해당 산타는 실제 이동
        if(min_dir != -1){
            santas[i].x = santas[i].x + dx[min_dir];
            santas[i].y = santas[i].y + dy[min_dir];
        }
        // axis : " << santas[i].x << " , " << santas[i].y << "\n";

        // 여기서 충돌 체크
        if(santas[i].x == dear.x && santas[i].y == dear.y ){
            santas[i].lazy_turn = turn; // 충돌한 시점을 기록
            santas[i].score += santa_strong;
            int mov_dir = (min_dir + 2) % 4;
            int cand_x = santas[i].x + santa_strong * dx[mov_dir];
            int cand_y = santas[i].y + santa_strong * dy[mov_dir];
            if(cand_x <= 0 || cand_x > N || cand_y <= 0 || cand_y > N){
                santas[i].live = false;
                continue;
            }
            // 이게 아니면 해당 방향으로 갱신
            santas[i].x = cand_x;
            santas[i].y = cand_y;
            chain_santa_bfs(i, mov_dir);
        }
    }
}


int main(){
    
    cin >> N >> turn >> santa_num >> dear_strong >> santa_strong;
    
    cin >> dear.x >> dear.y; //초기 위치 담기

    for(int i = 0; i < santa_num; i++){
        int idx, start_x, start_y;
        cin >> idx >> start_x >> start_y;
        santa_list[idx].x = start_x;
        santa_list[idx].y = start_y;
        santa_list[idx].move_dir = 0;
        santa_list[idx].live = true;
        santa_list[idx].score = 0;
        santa_list[idx].lazy_turn = -2; //처음에는 관련 없음
    }
    for(int i = 0; i < 40; i++){
        if(santa_list[i].live){
            santas.push_back(santa_list[i]); // 인덱스 순서로 해당 내용에 집어 넣기
        }
    }
    //print_santa();

    for(int i = 0; i < turn; i++){
        //cout <<"======= "<< i <<" turn======\n";
        int all_die = true;
        for(int i = 0; i < santas.size(); i++){
            if(santas[i].live == true){
                all_die = false;
            }
        }
        if(all_die == true) break;

        // cout << "before dear\n"; 
        // print_dear();

        move_dear(i);

        // cout << "after dear\n"; 
        //print_dear();

        move_santa(i);

        // cout << "after move santa\n"; 
        // print_santa();

        for(int i = 0; i < santas.size(); i++){
            if(santas[i].live == true){
                santas[i].score++;
            }
        }
        // cout << "after add score santa\n"; 
        // print_santa();
        //exit(1);
    }
    for(int i = 0; i < santas.size(); i++){
        cout << santas[i].score << " ";
    }
    cout << "\n";

}