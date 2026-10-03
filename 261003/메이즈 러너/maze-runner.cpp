#include <iostream>
#include <vector>
using namespace std;

int map[20][20]; // 여기에 벽의 정보가 들어감 0이면 빈칸, 1 이상이면 벽

pair<int,int> exit_axis; // 출구의 좌표를 나타냄

struct people_info{
    int x,y;
    int move_num;
    bool is_exit;
};

int dx[4] = {-1,1,0,0}; // 상하가 우선임
int dy[4] = {0,0,-1,1};

// 0 index 기준으로 진행
int N ,people_num, turns;

void print_map(){
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
}

vector<people_info> peoples;

void print_peoples(){
    for(auto people: peoples){
        cout << " x : " << people.x 
            << " y : " << people.y
            << " move num : " << people.move_num
            << " exit? " << people.is_exit << " \n";

    }
}

bool is_end(){
    for(auto people: peoples){
        if(people.is_exit == false){
            return false;
        }
    }
    return true;
}

void move_people(){
    for(int i = 0; i < peoples.size(); i++){
        if(peoples[i].is_exit == true) continue;
        int min = abs(peoples[i].x - exit_axis.first)
                + abs(peoples[i].y - exit_axis.second);
        vector<int> result_move_x ;
        vector<int> result_move_y ;
        for(int j = 0; j < 4; j++){
            int cand_x = peoples[i].x + dx[j];
            int cand_y = peoples[i].y + dy[j];

            if(cand_x < 0 || cand_x >= N || cand_y < 0 || cand_y >= N) continue;// 이때는 넘어가기
            if(map[cand_x][cand_y] > 0 ) continue; // 벽인경우
            
            // 이때는 움직일 수 있음
            int dist_x = cand_x - exit_axis.first;
            int dist_y = cand_y - exit_axis.second;
            if(dist_x < 0) dist_x = -1 * dist_x;
            if(dist_y < 0) dist_y = -1 * dist_y; // 음수 변형
            int dist = dist_x + dist_y;
            if(min > dist){
                result_move_x.clear();
                result_move_y.clear();
                result_move_x.push_back(cand_x);
                result_move_y.push_back(cand_y);
                min = dist; 
            }
            else if(min == dist){
                result_move_x.push_back(cand_x);
                result_move_y.push_back(cand_y);
            }
        }
        if(result_move_x.empty() == 1 && result_move_y.empty() == 1) continue; // 이떄는 움직일 수 없는 것
        
        for(int k = 0; k < result_move_x.size(); k++){
            if(map[result_move_x[k]][result_move_y[k]] == 0 ){
                peoples[i].x = result_move_x[k];
                peoples[i].y = result_move_y[k]; // 좌표 갱신
                peoples[i].move_num++;
                break;
            }
        }

    }
}

void exit_peoples(){
    for(int i = 0; i < peoples.size(); i++){
        if(exit_axis.first == peoples[i].x && exit_axis.second == peoples[i].y){
            peoples[i].is_exit = true;
        }
    }
}

pair<pair<int,int>,int> find_naemo(){
    // length 가 정사각형의 길이를 나타냄
    pair<pair<int,int>,int> result;
    for(int length = 1; length < N; length++){

        for(int i = 0;  i + length < N; i++){
            for(int j = 0; j + length < N ; j++){
                bool exit_in = false;
                bool people_in = false;
                //cout << "start : " << i << " " << j << "\n";
                for(int real_x = i ; real_x <= i + length; real_x++){
                    for(int real_y = j ; real_y <= j + length; real_y++){
                        //cout << "mid : " << real_x << " " << real_y << "\n";
                        if(exit_axis.first == real_x && exit_axis.second == real_y){
                            exit_in = true;
                            //cout <<"okay exit " << real_x  << " " <<real_y << endl;
                        }
                        
                        for(auto people : peoples){
                            if(people.is_exit == true) continue; //이미 탈출한 경우에는 out
                            if(people.x == real_x && people.y == real_y) {
                                //cout <<"okay people " << real_x  << " " <<real_y << endl;
                                people_in = true; // 사람도 탈출시키기
                            }
                        }
                        if(exit_in == true && people_in == true){
                            return {{i,j},length};
                        }
                    }
                }
            }
        }
    }
    return {{-1,-1},-1};
}


void rotate(pair<pair<int,int>,int> naemo_info){
    pair<int,int> axis = naemo_info.first;
    int length = naemo_info.second; // legth = x1 - x2 
    int tmep_map[20][20];
    // 아래 방법이 훨씬 깔끔하네
    for(int i = 0; i <= length ; i++){
        for(int j = 0; j <= length ; j++){
            int nx = axis.first  + j;
            int ny = axis.second + length - i;
            tmep_map[nx][ny] = map[i + axis.first][axis.second  + j]; // 이렇게 돌리는게 더욱 편하네
            // 이제 해당 좌표에 사람이 있는지 확인
        }
    }

    for(int i = 0; i < peoples.size(); i++){
        if( axis.first  <= peoples[i].x && peoples[i].x  <= axis.first + length ){
            if(axis.second  <= peoples[i].y && peoples[i].y  <= axis.second + length){
                // 범위에 속하는 경우
                int np_x = axis.first + peoples[i].y - axis.second;
                int np_y = axis.second + length - (peoples[i].x - axis.first);
                peoples[i].x = np_x;
                peoples[i].y = np_y;
            }
        }
    }

    int exit_x = axis.first + exit_axis.second - axis.second;
    int exit_y = axis.second + length - (exit_axis.first - axis.first);
    exit_axis.first = exit_x;
    exit_axis.second = exit_y;

    for(int i = axis.first ; i <= axis.first + length; i++){
        for(int j = axis.second ; j <= axis.second + length; j++){
            map[i][j] = tmep_map[i][j]; // 실제 map에 적용
            if(map[i][j] > 0) map[i][j]--; // 돌려진 곳은 음수화
        }
    }
    

}

int main(){
    cin >> N >> people_num >> turns;
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            cin >> map[i][j];
        }
    }
    for(int i = 0; i < people_num; i++){
        people_info temp;
        cin >> temp.x >> temp.y;
        temp.x--;
        temp.y--;
        temp.is_exit = false;
        temp.move_num = 0;
        peoples.push_back(temp); //이런식으로 사람들 넣기
    }
    cin >> exit_axis.first >> exit_axis.second;
    exit_axis.first--;
    exit_axis.second--; // zero based;

    for(int i = 0; i < turns; i++){
        
        
        // 참가자들 움직이기 (탈출 못한 사람들만)
        // cout << "before\n";
        // print_peoples();

        move_people();

        // cout << "after\n";
        // print_peoples();
        // move_people();


        // cout << "after2\n";
        // print_peoples();
        // 참가자들 출구에 도착했는지 확인하기
        // 이때 도착한 참가자들은 상태 갱신
        exit_peoples();
        if(is_end()) break; // 전체 끝났는지 확인하기

        //cout << exit_axis.first << " x " << exit_axis.second << " y \n";
        pair<pair<int,int>, int> naemo_info = find_naemo();
        //cout << naemo_info.first.first << " " << naemo_info.first.second << " " << naemo_info.second << endl;
        
        // cout << "before \n";
        // print_map();
        // cout <<"peoples\n";
        // print_peoples();

        //cout << exit_axis.first << " ex x " << exit_axis.second << " y \n";


        rotate(naemo_info);

        // cout << "after \n";
        // cout <<"peoples\n";
        // print_map();
        // print_peoples();

        //cout << exit_axis.first << " ex x " << exit_axis.second << " y \n";

        //exit(1);
        // 미로 돌리기
        
        
    }
    int result = 0;
    for(auto people : peoples){
        result += people.move_num;
    }
    cout <<  result << "\n";
    cout << exit_axis.first+1 << " " << exit_axis.second+1 << "\n";
}


