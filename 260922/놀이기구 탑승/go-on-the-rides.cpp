#include <iostream>
#include <vector>

using namespace std;

int N;
int love_friend[500][5]; // 좋아하는 친구 리스트를 담기 위함
int total_loves[500]; // 최종적으로 좋아하는 친구들이 담기는 배열
struct field
{
    int love_nums;
    int empty_nums;
}; // 이건 temp 배열로 둘것
int result;
int maps[30][30]; // 실제 사람이 놓이는 부분
vector<int> students;

int dx[4] = {-1,0,1,0};
int dy[4] = {0,1,0,-1};

int main(){
    cin >> N;
    for(int i = 0; i < N* N;i++){
        int temp_student;
        cin >> temp_student;
        students.push_back(temp_student); // 학생들 집어넣기
        for(int j = 0; j < 4; j++){
            int temp_love;
            cin >> temp_love;
            love_friend[temp_student][j] = temp_love; // 이런식으로 친구들이 담김
        }
    }

    for(auto student : students){
        int max_friend = -1;
        int max_empty = -1;
        int cand_i, cand_j;
        field temp[30][30] = {};
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N ; j++){
                if(maps[i][j] != 0) continue;
                
                for(int k = 0; k< 4; k++){
                    if(i + dx[k] < 0 || i + dx[k] >= N || j + dy[k] < 0 || j + dy[k] >= N) continue;
                    if(maps[i + dx[k]][j + dy[k]] == 0) temp[i][j].empty_nums++; // 주위가 비어있다는 뜻
                    else{
                        int temp_friend = maps[i + dx[k]][j + dy[k]]; // 누군가가 있다는 뜻
                        for(int l = 0; l < 4; l++){
                            if(love_friend[student][l] == temp_friend){
                                //cout << student << " st loves " <<  temp_friend << " \n";
                                temp[i][j].love_nums++; // 여기에 들어온거면 좋아하는 친구가 주변에 있다는 뜻
                            }
                        }
                    }
                }

                // 여기서 이제 최댓값 찾기
                if(max_friend < temp[i][j].love_nums){
                    max_friend = temp[i][j].love_nums;
                    max_empty = temp[i][j].empty_nums;
                    cand_i = i;
                    cand_j = j;
                }
                else if (max_friend == temp[i][j].love_nums){
                    if(max_empty < temp[i][j].empty_nums){
                        max_empty = temp[i][j].empty_nums;
                        cand_i = i;
                        cand_j = j;
                    }
                } // 갱신

                

            }
        }
        maps[cand_i][cand_j] = student;
        // cout << student << " student " << cand_i << " x " << cand_j << " y \n";
        // cout << "with "<< temp[cand_i][cand_j].love_nums << "loves\n";
        // // cout << 
        //total_loves[student] = temp[cand_i][cand_j].love_nums; // 좋아하는 사람으로 채워짐;


        // if(temp[cand_i][cand_j].love_nums == 1){
        //     result += 1;
        // }
        // else if(temp[cand_i][cand_j].love_nums == 2){
        //     result += 10;
        // }
        // else if(temp[cand_i][cand_j].love_nums == 3){
        //     result += 100;
        // }
        // else if(temp[cand_i][cand_j].love_nums == 4){
        //     result += 1000;
        // }
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            int friend_num = 0;
            for(int k = 0; k < 4;k++){
                int now_x = i + dx[k];
                int now_y = j + dy[k];
                if(now_x < 0 || now_x >= N || now_y < 0 || now_y >=N) continue;
                int temp_friend = maps[now_x][now_y];

                for(int l = 0; l < 4; l++){
                    if( love_friend[maps[i][j]][l] == temp_friend ) friend_num ++;
                }
            }
            if(friend_num == 1){
                result += 1;
            }
            else if(friend_num == 2){
                result += 10;
            }
            else if(friend_num == 3){
                result += 100;
            }
            else if(friend_num == 4){
                result += 1000;
            }
        }
    }
    

    cout << result << "\n";
    // for(auto student : students){
    //     cout << student << " friend\n";
    //     for(int j = 0; j < 4; j++){
    //         cout << love_friend[student][j];
    //     }
    //     cout << "\n";
    // }

}