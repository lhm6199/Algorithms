#include <iostream>
#include <vector>


using namespace std;

int num_Rest;
vector<int> customers;
int check_custs[2]; // 0 팀장, 1 팀원 검사 가능 인원 수

long int total_people;

int main(){

    cin >> num_Rest;
    for(int i = 0 ; i < num_Rest; i++){
        int temp;
        cin >> temp;
        customers.push_back(temp);
    }
    cin >> check_custs[0] >> check_custs[1];

    for(auto customer : customers){
        int temp = customer;
        temp -= check_custs[0]; // 우선 팀장 인원 부터 빼기
        total_people++;
        if(temp <= 0){
            continue;
        } 
        total_people += temp / check_custs[1];
        if(temp % check_custs[1] != 0){
            total_people++;
        }
    }
    cout << total_people;

}
