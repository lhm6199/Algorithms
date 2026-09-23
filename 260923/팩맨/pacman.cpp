#include <iostream>
#include <vector>
#include <stack>

using namespace std;


struct pack_man_info{
    int x,y;
};

struct monster_info
{
    int x,y;
    int dir; // 이동 방향 
};

struct die_moster_info
{
    int x,y;
    int alive_turn;
};

struct egg_info{
    int x,y;
    int dir;
};

vector <monster_info> monsters;
vector <die_moster_info> die_monsters;
vector <egg_info> eggs;

pack_man_info pack_man;
vector<pair<int,int>> cand_route;
vector<pair<int,int>> result_route(3);

int dx8[8] = {-1,-1,0,1,1,1,0,-1};
int dy8[8] = {0,-1,-1,-1,0,1,1,1};

int dx4[4] = {-1,0,1,0};
int dy4[4] = {0,-1,0,1}; // 상 좌 하 우의 우선순위

int num_monster;
int turns;

void replica(){
    for(auto monster : monsters){
        egg_info temp;
        temp.x = monster.x;
        temp.y = monster.y;
        temp.dir = monster.dir;
        eggs.push_back(temp);
    }
}

void move_monster(){
    for(int i = 0; i < monsters.size(); i++){
        int now_x = monsters[i].x;
        int now_y = monsters[i].y;
        int move_count = 0;
        while(move_count != 8){
            move_count++; 
            int cand_x = now_x + dx8[monsters[i].dir];
            int cand_y = now_y + dy8[monsters[i].dir];
            // cout << move_count << " cou\n";
            // cout << cand_x << " cx " << cand_y << " cy\n";
            if(cand_x <= 0 || cand_x > 4 || cand_y <= 0 || cand_y > 4){
                monsters[i].dir ++;
                monsters[i].dir %= 8;

                continue;
            } 
            if(cand_x == pack_man.x && cand_y == pack_man.y){
                monsters[i].dir ++;
                monsters[i].dir %= 8;
                continue;
            }  // 해당 위치에 팩맨이 있는경우
            bool die_monster_exist = false;
            for(auto die_monster : die_monsters){
                if(cand_x == die_monster.x && cand_y == die_monster.y){
                    die_monster_exist = true;
                    continue;
                }
            }
            if(die_monster_exist){
                monsters[i].dir ++;
                monsters[i].dir %= 8;
                continue;
            }  // 죽은 몬스터가 있는 경우는 pass

            // 여기까지 왔으면 살아남은 것
            monsters[i].x = cand_x;
            monsters[i].y = cand_y;
            break; // 이동을 했으니 break;
        }
    }

}
int max_s = -1;
bool visit[5][5];
void mov_pack_mans(int x, int y, int k, int count){

    if(k == 3){
        if(max_s < count){
            max_s = count;

            pack_man.x = x;
            pack_man.y = y;

            for(int i = 0; i < cand_route.size(); i++){
                result_route[i] = cand_route[i];
            }
        }
        return;
    }

    for(int i = 0; i < 4; i++){

        int now_x = x + dx4[i];
        int now_y = y + dy4[i];

        if(now_x <= 0 || now_x > 4 ||
           now_y <= 0 || now_y > 4)
            continue;

        cand_route.push_back({now_x, now_y});

        bool first_visit = !visit[now_x][now_y];

        int temp_cnt = 0;

        if(first_visit){
            for(auto monster : monsters){
                if(now_x == monster.x &&
                   now_y == monster.y){
                    temp_cnt++;
                }
            }

            count += temp_cnt;
            visit[now_x][now_y] = true;
        }

        mov_pack_mans(now_x, now_y, k + 1, count);

        if(first_visit){
            count -= temp_cnt;  
            visit[now_x][now_y] = false;
        }

        cand_route.pop_back();
    }
}
void print_route(){
    for(auto route : result_route){
        cout << route.first << " first " << route.second << " second\n";
    }
}

void erase_monsters(){

    bool route_map[5][5] = {};

    for(auto route : result_route){
        route_map[route.first][route.second] = true;
    }

    bool killed[5][5] = {};

    vector<monster_info> next_monsters;
    next_monsters.reserve(monsters.size());

    for(auto monster : monsters){

        if(route_map[monster.x][monster.y]){
            killed[monster.x][monster.y] = true;
        }
        else{
            next_monsters.push_back(monster);
        }
    }

    monsters.swap(next_monsters);


    for(int x = 1; x <= 4; x++){
        for(int y = 1; y <= 4; y++){

            if(killed[x][y]){

                die_moster_info temp;

                temp.x = x;
                temp.y = y;
                temp.alive_turn = 3;

                die_monsters.push_back(temp);
            }
        }
    }
}


void make_monsters(){

    for(auto egg : eggs){

        monster_info temp;

        temp.x = egg.x;
        temp.y = egg.y;
        temp.dir = egg.dir;

        monsters.push_back(temp);
    }

    eggs.clear();
}

void erase_dies(){

    vector<die_moster_info> next;

    for(auto die : die_monsters){

        die.alive_turn--;

        if(die.alive_turn > 0){
            next.push_back(die);
        }
    }

    die_monsters.swap(next);
}


void print_monsters(){
    for(auto monster : monsters){
        cout << monster.x << " x " << monster.y << " y " <<monster.dir << "\n";
    }
}
void print_eggs(){
    for(auto egg : eggs){
        cout << egg.x << " x " << egg.y << " y " <<egg.dir << "\n";
    }
}
void print_dies(){
    for(auto die : die_monsters){
        cout << die.x << " x " << die.y << " y " <<die.alive_turn << "\n";
    }
}


int main(){
    cin >> num_monster >> turns;

    cin >> pack_man.x >> pack_man.y; // 초기 팩맨의 위치 제공
    
    for(int i = 0; i < num_monster; i++){
        monster_info temp;
        cin >> temp.x >> temp.y >> temp.dir;
        temp.dir -= 1; // 0 based index
        monsters.push_back(temp); // 몬스터 집어넣기
    }

    for(int i = 0; i < turns; i++){
        erase_dies();
        replica();
        //print_eggs();
        // cout << "monster bef\n";
        // print_monsters();
        move_monster();
        // cout << "monster af\n";
        // print_monsters();
        // print_monsters();
        mov_pack_mans(pack_man.x,pack_man.y, 0, 0);
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5 ; k++ ){
                visit[j][k] = false;
            }
        }
        max_s = -1;
        //cout << pack_man.x << " " <<pack_man.y << "\n";
        erase_monsters();
        // print_monsters();
        // cout << "dies\n";
        // print_dies();
        make_monsters();
        //cout << "after monster\n";
        // print_monsters();
       
        //exit(1);
    }
     cout << monsters.size() << "\n";
}