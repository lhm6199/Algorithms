#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// zero base로 인덱싱 진행

int N;
int player_num;
int rounds;

struct player_info{
    int x,y; // 좌표
    int strength;
    int gun_power;
    int dir; // 방향을 나타냄
    int score;
};


vector<player_info> players; // 플레이어들 정보를 저장하는 배열 
vector<int> gun_map[30][30];

int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1}; // 이런꼴로 저장 가능

int is_player(int idx ,int x, int y){
    for(int i = 0; i < players.size(); i++){
        if(idx == i) continue;
        if(x == players[i].x  && y == players[i].y){
            return i; // 해당 인덱스 도출
        }
    }
    return -1; // 이거는 없다는 뜻
}

void fight(int idx1, int idx2, int x, int y){
    
    int player1_total = players[idx1].gun_power + players[idx1].strength;
    int player2_total = players[idx2].gun_power + players[idx2].strength;
    int win, lose;
    
    if(player1_total > player2_total){
        win = idx1;
        lose = idx2;
    }
    else if (player1_total == player2_total){
        if(players[idx1].strength > players[idx2].strength){
            win = idx1;
            lose = idx2;
        }
        else{ 
            win = idx2;
            lose = idx1;
        }
    }
    else{
        win = idx2;
        lose = idx1;
    }
    // 여기서 이하의 내용 한번에 처리


    players[win].score += abs(player1_total - player2_total);
    // 진 플레이어는 자신의 원래 총을 내려놓음
    if(players[lose].gun_power != 0){
        gun_map[x][y].push_back(players[lose].gun_power);
        sort(gun_map[x][y].begin(), gun_map[x][y].end());

        players[lose].gun_power = 0;
    }
    // 진 플레이어의 이동
    while(1){
        int cand_x = players[lose].x + dx[players[lose].dir];
        int cand_y = players[lose].y + dy[players[lose].dir];
        //cout << lose << " loose idx\n";
        //cout << cand_x << " loose move x " << cand_y << " move y\n";
        if(cand_x < 0 || cand_x >= N || cand_y < 0 || cand_y >= N){
            players[lose].dir = (players[lose].dir + 1) % 4;
            continue;
        }
        if(is_player(lose, cand_x, cand_y) >= 0){
            players[lose].dir = (players[lose].dir + 1) % 4;
            continue;
        }
        // 두가지 상황이 아니라면 이동 및 총 교환

        if(gun_map[cand_x][cand_y].size() > 0){
            int temp_gun = players[lose].gun_power; // 내려놓을 총이 있는가?
            if(players[lose].gun_power < gun_map[cand_x][cand_y].back()){

                players[lose].gun_power = gun_map[cand_x][cand_y].back();
                gun_map[cand_x][cand_y].pop_back();
                if(temp_gun != 0){
                    gun_map[cand_x][cand_y].push_back(temp_gun);
                } 
                sort(gun_map[cand_x][cand_y].begin(), gun_map[cand_x][cand_y].end()); // 정렬
            }
        }
        players[lose].x =cand_x;
        players[lose].y =cand_y;
        break;
    }
    
    //이긴 player
    if(gun_map[x][y].size() > 0){
        int temp_gun = players[win].gun_power; // 내려놓을 총이 있는가?
        if(players[win].gun_power < gun_map[x][y].back()){
            players[win].gun_power = gun_map[x][y].back();
            gun_map[x][y].pop_back();
            if(temp_gun != 0){
                gun_map[x][y].push_back(temp_gun);
            } 
            sort(gun_map[x][y].begin(), gun_map[x][y].end()); // 정렬
        }
    }
}


void move_player(int idx){
    int cand_x , cand_y;
    cand_x = players[idx].x + dx[players[idx].dir];
    cand_y = players[idx].y + dy[players[idx].dir];

    // 격자 아웃

    if(cand_x < 0 || cand_x >= N || cand_y < 0 || cand_y >= N){
        players[idx].dir = (players[idx].dir + 2)%4; // 회전
        cand_x = players[idx].x + dx[players[idx].dir];
        cand_y = players[idx].y + dy[players[idx].dir]; // 다시 방향 전환
    }
    int counter_player = is_player(idx, cand_x,cand_y);

    if(counter_player >= 0) {

        //cout << idx << " and " << counter_player <<" meet\n";
        //cout << cand_x << " move x " << cand_y << " move y\n";
        players[idx].x = cand_x;
        players[idx].y = cand_y;
        fight(idx, counter_player, cand_x, cand_y);// 두 플레이어의 인덱스를 받아서 처리


    }

    else{ // 플레이어가 없다면
        if(gun_map[cand_x][cand_y].size() > 0){
            int temp_gun = players[idx].gun_power; // 내려놓을 총이 있는가?
            if(players[idx].gun_power < gun_map[cand_x][cand_y].back()){

                players[idx].gun_power = gun_map[cand_x][cand_y].back();
                gun_map[cand_x][cand_y].pop_back();
                if(temp_gun != 0){
                    gun_map[cand_x][cand_y].push_back(temp_gun);
                } 
                sort(gun_map[cand_x][cand_y].begin(), gun_map[cand_x][cand_y].end()); // 정렬

            }
        }
        players[idx].x = cand_x;
        players[idx].y = cand_y;
    }
    
}

void print_map(){
    cout << "=========map========\n";
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            cout << gun_map[i][j].size() << " ";
        }
        cout << "\n";
    }
}

void print_player(){
    cout << "=========player========\n";
    for(auto player : players){
        cout << player.x << " x "
            << player.y << " y "
            << player.strength << " str "
            << player.gun_power << " gun "
            << player.dir << " dir "
            << player.score << " score "
            << "\n";
    }
}


int main(){
    cin >> N >> player_num >> rounds;
    
    for(int i = 0 ; i< N; i++){
        for(int j = 0; j < N; j++){
            int temp;
            cin >> temp;
            if(temp == 0) continue;
            gun_map[i][j].push_back(temp); // 이런식으로 넣기
        }
    }

    for(int i = 0 ; i < player_num; i++){
        player_info temp;
        cin >> temp.x >> temp.y >> temp.dir >> temp.strength;
        temp.gun_power = 0;
        temp.score = 0;
        temp.x -- ; // zero based
        temp.y -- ;
        players.push_back(temp);  //이런식으로 넣기
    }
    // print_map();
    // print_player();
    for(int i = 0; i < rounds; i++){
        for(int j = 0; j < players.size(); j++){
            //여기서 순차적으로 각 플레이어들 이동
            move_player(j); // 플레이어의 위치 이동
            // print_map();
            // print_player();
            //if(j == 1) exit(1);
            //exit(1);
        }
        //cout << i << " asdasd\n";
        //if(i == 6)exit(1);
    }

    for(auto player : players){
        cout << player.score << " ";
    }
    cout << "\n";

}
// 초기에는 -1이구나