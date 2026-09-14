#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int N;

int nums[40];

bool visit[40];
int work[100][100];

vector<int> temp;
vector<int> temp2;

int morning;
int evening;
int mins;

vector<int> temp3;
bool visit3[40];

void divide2(int k){
    if(temp3.size() == 2){
        // for(auto t : temp3){
        //     cout << "inner1: " << t << " ";
        // }
        // cout << "\n";
        morning += work[temp3[0]][temp3[1]];
        morning += work[temp3[1]][temp3[0]];
        return;
    }
    for(int i = k; i < N/2; i++){
        temp3.push_back(temp[i]);
        visit3[temp[i]] = true;
        divide2(i+1);
        visit3[temp[i]] = false;
        temp3.pop_back();
    }
    //temp3.clear();
}

void divide3(int k){
    if(temp3.size() == 2){
        evening += work[temp3[0]][temp3[1]];
        evening += work[temp3[1]][temp3[0]];
        return;
    }

    for(int i = k; i < N/2; i++){
        temp3.push_back(temp2[i]);
        visit3[temp2[i]] = true;
        divide3(i+1);
        visit3[temp2[i]] = false;
        temp3.pop_back();
    }
    //temp3.clear();
}



void divide(int k){
    if(temp.size() == N/2){
        temp2.clear();
        for(int i = 1 ; i <= N ; i++){
            if(visit[i] == 0){
                temp2.push_back(i);
            }
        }

        // for(auto t : temp){
        //     cout << "first: " << t << " ";
        // }
        // cout <<"\n";
        // for(auto t : temp2){
        //     cout << "second: " << t << " ";
        // }
        // cout <<"\n";
        //exit(1);
        divide2(0);
        divide3(0);


        // cout << "m , e " << morning << " " << evening << "\n";
        if(mins > abs(morning - evening)){
            mins = abs(morning - evening);
        }
        // cout << mins << " mins" <<endl;
        morning = 0;
        evening = 0;


        return;
    }

    for(int i = k; i <= N; i++){
        
        temp.push_back(i);
        visit[i] = true;
        divide(i+1);
        visit[i] = false;
        temp.pop_back();
    }
    
}








int main() {
    // Please write your code here.
    mins = 100000000;
    cin >> N;
    // for(int i = 0; i <= N ; i++){
    //     nums[i] = i;
    // }

    for(int i = 1 ; i <= N ; i++){
        for(int j = 1; j <= N; j++){
            cin >> work[i][j] ;// 업무 강도
        }
    }
    
    divide(1);
    cout << mins <<"\n";
    return 0;
}