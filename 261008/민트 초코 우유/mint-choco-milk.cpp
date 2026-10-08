#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
using namespace std;

struct student_info{
    int love; // 신앙심
    vector<char> love_thing; // 좋아하는게 뭔지 정의 // 이렇게 가져가는게 낫겠다.
};

student_info students[60][60];
// 이렇게 하고 bfs 진행을 하자.

int N , turn;

vector<pair<int, tuple<int,int,int>>> represents; // {좋아하는 개수, - 신앙심 , 행, 열}
//get<0>(a); 이런식으로 정의

void print_love_thing_num(){
    cout << "==love_thing_num==\n";
    for(int i = 0; i < N ; i++) {
        for(int j = 0 ; j < N ; j++){
            cout << students[i][j].love_thing.size() << " ";
        }
        cout << "\n";
    }
}


void print_love(){
    cout << "==love==\n";
    for(int i = 0; i < N ; i++) {
        for(int j = 0 ; j < N ; j++){
            cout << students[i][j].love << " ";
        }
        cout << "\n";
    }
}

void morning(){
    // 신앙심 1 추가
    for(int i = 0; i < N ; i++) {
        for(int j = 0 ; j < N ; j++){
            students[i][j].love++;
        }
    }
}

int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1}; // 위 아래 왼쪽 오른쪽 순서


bool same_love_thing(student_info st1, student_info st2){
    if(st1.love_thing.size() != st2.love_thing.size()) return false;
    // 서로 사이즈가 같은 경우
    sort(st1.love_thing.begin(), st1.love_thing.end());
    sort(st2.love_thing.begin(), st2.love_thing.end());
    for(int i = 0; i < st1.love_thing.size(); i++){
        if(st1.love_thing[i] != st2.love_thing[i]){
            return false;
        }
    }
    return true;
}

void make_group(int x, int y , bool visit[60][60]){ // 사실상 bfs // 여기는 사실상 대표자를 뽑아야함
    int start_x = x;
    int start_y = y;
    //int max_love = students[x][y].love;// 최상위 충성심
    //pair<int,int> represent_group = {x,y}; // 그룹의 대표자 // 초기 값으로 세팅
    queue<pair<int,int>> q;
    vector<tuple<int , int , int>> group_members; // 그룹에 속한 애들 넣기 나중에 신앙심 뺏기 위함
    // 이렇게 진행
    q.push({x,y});
    group_members.push_back({-students[x][y].love,x,y}); // 이런식으로 넣으면 될듯
    visit[x][y] = true;
    while(!q.empty()){
        pair<int,int> now = q.front();
        q.pop();
        for(int i = 0; i < 4 ; i++){
            int nx = now.first + dx[i];
            int ny = now.second + dy[i];
            //범위 아웃
            if(nx < 0 || nx >= N || ny < 0 || ny >= N) continue; // 범위 벗어난 경우
            // 이미 방문한 경우
            if(visit[nx][ny] == true) continue;
            // 구성이 서로 다른 경우 구성을 보는 함수 새로 만들기
            if(same_love_thing(students[now.first][now.second], students[nx][ny]) == false) continue;

            // 이제는 서로 같은 그룹의 내용임
            q.push({nx,ny});
            group_members.push_back({-students[nx][ny].love,nx,ny}); //해당 내용 맴버에 넣기
            visit[nx][ny] = true;
        }
    }
    // cout << "group_member_num!\n";
    // cout << group_members.size() << "\n";
    // cout << "=============\n";
    // for(int i = 0; i < group_members.size(); i++){
    //     cout << get<0>(group_members[i]) << " love " << get<1>(group_members[i]) << " x "   << get<2>(group_members[i])  << " y\n";
    // }
    sort(group_members.begin(), group_members.end()); // 오름차순으로 정렬
    // for(int i = 0; i < group_members.size(); i++){
    //     cout << get<0>(group_members[i]) << " love " << get<1>(group_members[i]) << " x "   << get<2>(group_members[i])  << " y\n";
    // }
    ///exit(1);
    // 신앙심 업데이트
    // cout <<"bf_update\n";
    // for(int i = 0; i < group_members.size(); i++){
    //     cout << get<0>(group_members[i]) << " love " << get<1>(group_members[i]) << " x "   << get<2>(group_members[i])  << " y\n";
    // }
    
    for(int i = 1; i < group_members.size(); i++){
        students[get<1>(group_members[i])][get<2>(group_members[i])].love--; 
        get<0>(group_members[i])++; //  음수로 바꿨었으니까 해당 내용처럼 넣기
    }
    // 실제 맵에도 적용해줘야함


    students[get<1>(group_members[0])][get<2>(group_members[0])].love += (group_members.size()-1); 
    get<0>(group_members[0]) = get<0>(group_members[0]) - (group_members.size()-1);
    // cout <<"after_update\n";
    // for(int i = 0; i < group_members.size(); i++){
    //     cout << get<0>(group_members[i]) << " love " << get<1>(group_members[i]) << " x "   << get<2>(group_members[i])  << " y\n";
    // }
    //exit(1);

    //대표자 넣기
    //represents.push_back()
    // 이제 대표자 선정 -> 해당 그룹의 마지막 인덱스
    // {좋아하는 개수, - 신앙심 , 행, 열}
    int love_num = students[get<1>(group_members[0])][get<2>(group_members[0])].love_thing.size();
    //cout << love_num << " levevv\n";
    represents.push_back({love_num, group_members[0]});
    // 대표자 넣기
}

    void print_T(){
        for(int i = 0; i < N; i++){
            for(int j = 0 ; j < N ; j++){
                bool is_in = false;
                for(auto thing : students[i][j].love_thing){
                    if(thing == 'T') {
                        is_in = true;
                        cout << "T";
                    }
                }
                if(is_in == false)
                    cout << "0";
            }
            cout << "\n";
        }
    }
    void print_C(){
        for(int i = 0; i < N; i++){
            for(int j = 0 ; j < N ; j++){
                bool is_in = false;
                for(auto thing : students[i][j].love_thing){
                    if(thing == 'C') {
                        is_in = true;
                        cout << "C";
                    }
                }
                if(is_in == false)
                    cout << "0";
            }
            cout << "\n";
        }
    }
    void print_M(){
        for(int i = 0; i < N; i++){
            for(int j = 0 ; j < N ; j++){
                bool is_in = false;
                for(auto thing : students[i][j].love_thing){
                    if(thing == 'M') {
                        is_in = true;
                        cout << "M";
                    }
                }
                if(is_in == false)
                    cout << "0";
            }
            cout << "\n";
        }
    }

void print_status(){
    cout <<"T=====\n";
    print_T();
    cout <<"C=====\n";
     print_C();
    cout <<"M=====\n";
      print_M();
}


void print_represent(){
    for(auto represent : represents){
        cout << represent.first << " love_num "
            << get<0>(represent.second) << " love "
            << get<1>(represent.second) << " x "
            << get<2>(represent.second) << " y \n";
    }
}

void lunch(){
    // 그룹 생성부터 -> 이때는 bfs 진행
    bool visit[60][60] = {} ;
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N ; j++){
            if(visit[i][j] == true) continue;
            make_group(i,j,visit);
        }
    }
    //cout <<"represent!s\n";
    //print_represent();
    //sort(represents.begin(), represents.end());
    //cout <<"sort!s\n";
    //print_represent();
    //exit(1);
}

void weak(int x1, int y1, int x2, int y2){
    // 
    set<char> unions;
    for(auto things : students[x1][y1].love_thing){
        unions.insert(things);
    }
    for(auto things : students[x2][y2].love_thing){
        unions.insert(things);
    }
    students[x2][y2].love_thing.clear();
    for(auto uni : unions){
        students[x2][y2].love_thing.push_back(uni);
    }
}

void dinner(){
    // 저녁시간에 신앙심 전파
    sort(represents.begin(), represents.end()); // 정렬부터 진행
    //{좋아하는 개수, tuple{- 신앙심 , 행, 열}}

    // 우선 신앙심을 다시 양수로 변환
    // for(int i = 0; i < represents.size(); i++){
    //     get<0>(represents[i].second) = get<0>(represents[i].second) * -1; // 양수로 전환
    // } => 그냥 map에 있는 내용 기반으로 진행
    //print_represent();

    //exit(1);
    // 이제 전파 시작
    bool visit[100][100] = {}; // 전파자가 해당 내용에 true라면 전파를 건너 뛴다.
    for(int i = 0; i < represents.size() ; i++){
        int repre_x = get<1>(represents[i].second);
        int repre_y = get<2>(represents[i].second);

        if(visit[repre_x][repre_y] == true) continue; // 만약에 이미 전파를 받은 경우 넘어가
        
        int broad = students[repre_x][repre_y].love - 1;  // 퍼트리기
        students[repre_x][repre_y].love = 1;



        int direction = (broad+1) % 4 ; // 4로 나눈 나머지로 진행
        // cout << "refre==========\n";
        // cout << repre_x << " x " << repre_y <<  " y \n";
        // cout << direction << " dir \n";
        //exit(1);

        int nx = repre_x;
        int ny = repre_y;
        while(1){
            nx += dx[direction];
            ny += dy[direction]; // 해당 방향으로 정의
            if(broad <= 0) break; // 0이된다면 이때 전파 종료
            if(nx < 0 || nx >= N || ny < 0 || ny >= N) break; // 범위 벗어난 경우
            if(same_love_thing(students[repre_x][repre_y],students[nx][ny])) continue; // 신봉 음식이 같은 경우 넘어가기
            

            // 만약에 다른 경우
            // 간절함 >  대상_신앙심  강한 신앙심
            if(broad > students[nx][ny].love){
                students[nx][ny].love_thing.clear();
                for(auto loves : students[repre_x][repre_y].love_thing){
                    students[nx][ny].love_thing.push_back(loves);// 하나씩 추가
                }
                // 간절함 out
                broad = broad - (students[nx][ny].love + 1);
                students[nx][ny].love++; // 전파 대상의 신앙심 1 증가
                visit[nx][ny] = true; 
            }
            else{ 
                weak(repre_x,repre_y,nx,ny);
                students[nx][ny].love += broad;
                broad = 0;
                visit[nx][ny] = true; 
            }
        }
        // cout << "status!\n";
        // print_status();
        // cout << "love!\n";
        // print_love();
    }
    
    //exit(1);
}




int group_idx(int x, int y){
    if(students[x][y].love_thing.size() == 3){
        return 0; // 민트 초코 우유라는 뜻
    }
    else if(students[x][y].love_thing.size() == 2){
        bool things[3] = {};
        for(auto thing : students[x][y].love_thing){
            if(thing == 'T'){
                things[0] = true;
            }
            else if(thing == 'C'){
                things[1] = true;
            }
            else if(thing == 'M'){
                things[2] = true;
            }
        }
        // 이렇게 정히 하고
        if(things[0] == false){
            // 초코 우유
            return 3;
        }
        else if(things[1] == false){
            // 민트 우유
            return 2;
        }
        else if(things[2] == false){
            // 민트 초코
            return 1;
        }
    }
    else{
        bool things[3] = {};
        for(auto thing : students[x][y].love_thing){
            if(thing == 'T'){
                things[0] = true;
            }
            else if(thing == 'C'){
                things[1] = true;
            }
            else if(thing == 'M'){
                things[2] = true;
            }
        }
        // 이렇게 정히 하고
        if(things[0] == true){
            // 민트
            return 6;
        }
        else if(things[1] == true){
            // 초코
            return 5;
        }
        else if(things[2] == true){
            // 우유
            return 4;
        }
    }
    return -1;
}

void print_result(){
    int results[10] = {};
    for(int i = 0; i < N; i++){
        for(int j = 0 ; j < N ; j++){
            int idx = group_idx(i,j);
            results[idx] += students[i][j].love;
        }
    }
    for(int i = 0 ; i < 7; i++){
        cout << results[i] << " ";
    }
    cout << "\n";
}

int main(){
    cin >> N >> turn;
    string student_love_food;
    for(int i = 0; i < N ; i++){
        cin >> student_love_food;// 음식에 대한 내용을 담음
        for(int j = 0; j < N; j++){
            students[i][j].love_thing.push_back(student_love_food[j]); //이런식으로 값을 넣기
        }
    }
    for(int i = 0; i < N ; i++){
        for(int j = 0; j < N; j++){
            cin >> students[i][j].love; // 각 음식에 대한 신앙심 넣기
        }
    }
    //print_love_thing_num();
     //print_status();
   //exit(1);
    // print_love();

    for(int i = 0 ; i < turn ; i++){

        morning();
        
        // cout << "after morning\n";
        // print_love();
        lunch();
        // cout << represents.size() << " nums!\n";
        // print_represent();
        //print_status();
        //exit(1);
        // cout << "after lunch\n";
        // print_love();
        dinner();

        // 이제 출력
        print_result();
        //print_status();
        //exit(1);
        represents.clear();
    }

}