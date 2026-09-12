#include <iostream>

using namespace std;


int dice_nums[6];
int board[30][30];
int row,col;
int x,y; // 좌표를 나타냄 (행, 열)
int command_num;
int dy[4] = {1,-1,0,0}; // 동, 서
int dx[4] = {0,0,-1,1}; // 북, 남


void change_dice(int cmd){
    int temp_dice[6] = {};
    if(cmd == 1){
        temp_dice[0] = dice_nums[1];
        temp_dice[1] = dice_nums[5];
        temp_dice[2] = dice_nums[2];
        temp_dice[3] = dice_nums[0];
        temp_dice[4] = dice_nums[4];
        temp_dice[5] = dice_nums[3];
    }
    else if(cmd == 2){
        temp_dice[0] = dice_nums[3];
        temp_dice[1] = dice_nums[0];
        temp_dice[2] = dice_nums[2];
        temp_dice[3] = dice_nums[5];
        temp_dice[4] = dice_nums[4];
        temp_dice[5] = dice_nums[1];
    }
    else if(cmd == 4){ // 북
        temp_dice[0] = dice_nums[4];
        temp_dice[1] = dice_nums[1];
        temp_dice[2] = dice_nums[0];
        temp_dice[3] = dice_nums[3];
        temp_dice[4] = dice_nums[5];
        temp_dice[5] = dice_nums[2];
    }
    else{ // 남
        temp_dice[0] = dice_nums[2];
        temp_dice[1] = dice_nums[1];
        temp_dice[2] = dice_nums[5];
        temp_dice[3] = dice_nums[3];
        temp_dice[4] = dice_nums[0];
        temp_dice[5] = dice_nums[4];
    }

    for(int i = 0; i < 6; i++){
        dice_nums[i] = temp_dice[i]; // 실은 포인터 자체로 바꿔도 됨
    }
}

void print_map(){
    for(int i = 0; i < row ; i++){
        for(int j = 0 ; j < col ; j++){
            cout << board[i][j] << " ";     
        }
        cout <<  "\n";
    }
}


int main(){

    cin >> row >> col >> x >> y >> command_num;


    for(int i = 0; i < row; i++) {
        for(int j = 0 ; j < col ; j++){
            cin >> board[i][j];
        }
    }
    // 보드 받기

    for(int i = 0 ; i < command_num; i++){
        int cmd;
        cin >> cmd;
        int temp_x = x;
        int temp_y = y;
        temp_x += dx[cmd-1];
        temp_y += dy[cmd-1]; // 이런식으로 좌표 옮기기

        // 범위를 벗어난 경우도 생각하기

        if(temp_x < 0 || temp_x>= row || temp_y < 0 || temp_y >= col){
            continue; // 범위를 벗어나는 경우
        }

        x = temp_x;
        y = temp_y; // 이런식으로 좌표 옮기기

        change_dice(cmd); //자리 바꾸기

        if(board[x][y] == 0){
            board[x][y] = dice_nums[0];
        }
        else{
            dice_nums[0] = board[x][y];
            board[x][y] = 0;
        }
        // print_map();
        // cout << "change_to x , y : " << x << " " << y << endl;
        cout << dice_nums[5] << "\n";
        // for(int i = 0; i < 6; i++){
        //     cout << i << "th dice : "  << dice_nums[i] << endl;
        // }

    }

}