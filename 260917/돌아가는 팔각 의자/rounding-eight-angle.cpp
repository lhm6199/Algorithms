#include <iostream>
#include <cmath> 
using namespace std;

string chair[5];
bool visit[5]; // 탐색을 했는지 안했는지를 정의하는 부분
int rotate_dirs[5]; // 회전 방향을 정의하는 부분 방문을 한 경우에는 rotate를 진행하는 것 -> 이것으로 판별
// 일단 회전 방향을 정의하고 한번에 돌리면 될듯

//1 시계

int chair_num;

void rotate(){
    for(int i = 1; i <= chair_num; i++){
        string temp;
        if(visit[i]){
            if(rotate_dirs[i] == -1){
                for(int j = 1; j < 8; j++){
                    temp.push_back(chair[i][j]);
                }
                temp.push_back(chair[i][0]);
            }
            else{
                temp.push_back(chair[i][7]);
                for(int j = 0; j < 7; j++){
                    temp.push_back(chair[i][j]);
                }
            }
            for(int j = 0; j < 8; j++){
                chair[i][j] = temp[j];
            }
        }

    }
}


int main() {
    string temp; 
    while(1){
        
        cin >> temp;
        //cout << temp.size()  << " size\n";
        if(temp.size() != 8){
            break;
        }
        chair[++chair_num] = temp;
    }

    // for(int i = 0 ; i < chair_num; i++){
    //     cout << chair[i];
    //     cout << "\n";
    // }
    //1인덱스 기준
    int rotate_num = stoi(temp);
    //cout << rotate_num << "\n";
    //return 1;
    for(int i = 0; i < rotate_num; i++){
        int rotate_chair, rotate_dir;
        cin >> rotate_chair >> rotate_dir;
        visit[rotate_chair] = true;
        rotate_dirs[rotate_chair] = rotate_dir; // 방향 정의
        
        //왼쪽
        for(int j = rotate_chair - 1; j > 0; j--){
            if(chair[j+1][6] == chair[j][2] ) break;
            else{
                visit[j] = true;
                rotate_dirs[j] =  -1 * rotate_dirs[j+1]; // 반대 방향으로 회전
            }
        }

        //오른쪽 4인경우는 없는 거죠
        for(int j = rotate_chair + 1; j <= chair_num; j++){
            if(chair[j-1][2] == chair[j][6] ) break;
            else{
                visit[j] = true;
                rotate_dirs[j] = -1 * rotate_dirs[j-1]; // 반대 방향으로 회전
            }
        }
        rotate();
        for(int i = 0; i < 5; i++){
            visit[i] = false;
            rotate_dirs[i] = 0;
        }
    }

    // for(int i = 1 ; i <= chair_num; i++){
    //     cout << chair[i];
    //     cout << "\n";
    // }

    //     for(int i = 1 ; i <= chair_num; i++){
    //     cout << rotate_dirs[i];
        
    // }
    // cout << "\n";

    //     for(int i = 1 ; i <= chair_num; i++){
    //     cout << visit[i];
        
    // }
    // cout << "\n";

    
    
    // cout << "after rotate!\n";
    //     for(int i = 1 ; i <= chair_num; i++){
    //     cout << chair[i];
    //     cout << "\n";
    // }
    int result = 0;
    for(int i = 1 ; i <= chair_num; i++){
        if(chair[i][0] == '1'){
            result += pow(2,i-1);
        }
        
    }
    cout << result <<"\n";
    return 0;
}