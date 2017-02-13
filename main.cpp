#include <iostream>
#include <stdlib.h>
#include<map>
#include<sstream>

#define negInfinity -100000
#define posInfinity +100000


using namespace std;

void get_level();
void setup();
void print_board();
void my_play();
void comp_play();
bool check_game();
void print_winner();
bool test_game();
int alpha_beta(int turn,int alpha,int beta,int depth);
int getMin(int x, int y);
int getMax(int x, int y);
int heuristic();
int get_score(int x,int y);
string get_best_move();


int board[3][3];
int player=1;
int winner;
bool the_end=false;
map<string,int>myMap;
int level;

bool bug_flag=false;
bool first_flag=false;

int main()
{
    get_level();
    setup();
    while(!the_end){
    print_board();
    if(player==1)
        my_play();
    else
        comp_play();
    player=1-player;
    the_end=check_game();
    }
    print_board();
    print_winner();
    return 0;
}

void get_level(){
cout<<"Please choose your level : (1) for [easy] - 2 for [hard]"<<endl;
cin>>level;
}


void setup(){
cout<<"You will play first, you are X "<<endl;
for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
        board[i][j]=-1;
    }
}
}

void print_board(){
for(int i=0;i<3;i++){
  cout<<"\n";
  for(int j=0;j<3;j++){
    if(board[i][j]==-1)
        cout<<" ";
    else if(board[i][j]==1)
        cout<<"X";
    else if(board[i][j]==0)
        cout<<"O";
    if(j!=2)
        cout<<"|";
  }
  if(i!=2){
    cout<<endl;
    cout<<"- - - ";
}
}
cout<<endl;
}

void print_winner(){
    cout<<endl;
if(winner==1)
    cout<<"YOU WON"<<endl;
else if(winner==0)
    cout<<"YOU Lost"<<endl;
else
    cout<<"It's a tie"<<endl;

}


void my_play(){
int i,j;
while(1){
cout<<"Enter Coordinates to play"<<endl;
cin>>i>>j;
if(!first_flag){
   if( (i==2 && j==2) || (i==2 && j==0) )
     bug_flag=true;
     first_flag=true;
}
if(board[i][j]==-1){
    board[i][j]=1;
    break;
}
else{
    cout<<"Invalid Coordinates, Please try again"<<endl;
}
}
}

void comp_play(){

if(bug_flag && first_flag){
    board[1][1]=0;
    bug_flag=false;
}

else{

myMap.clear();
int temp=alpha_beta(player,negInfinity,posInfinity,0);

string stringBestMove=get_best_move(); // choose the best move

int intBestMove=atoi(stringBestMove.c_str());
int j=intBestMove%10;
int i=intBestMove/10;
board[i][j]=0;
}
}


int alpha_beta(int turn,int alpha,int beta,int depth){


//Maximizer : turn = 0 (o)
//Minimizer : turn = 1 (x)

if(beta<=alpha){
    if(turn==0) //Maximizer
        return posInfinity;//+infinity
    else
        return negInfinity;//-infinity
}

if(test_game()) return heuristic(); //leafnode


bool leafFlag=true;
for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
        if(board[i][j]==-1){
            leafFlag=false;
            break;
        }
        else
            continue;
    }
}
if(leafFlag)return 0; //leaf node

int maxValue = negInfinity //-infinity
,minValue = posInfinity; //+infinity

for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
        if(board[i][j]==-1){
        int currentScore=0;

        //Maximizer
        if(turn==0){
          board[i][j]=0;
          currentScore=alpha_beta(1,alpha,beta,depth+1);
          maxValue=getMax(maxValue,currentScore);
          alpha=getMax(alpha,currentScore);
          if(depth==0){ //put score in a current score vector
            cout<<"Point :"<<i<<","<<j<<" Score :"<<currentScore<<endl;
            stringstream ss1;
            string temp;
            ss1<<i;
            temp=ss1.str();
            stringstream ss2;
            ss2<<j;
            temp+=ss2.str();
            myMap[temp]=currentScore;
           }
        }
        //Minimizer
        else if(turn==1){
          board[i][j]=1;
          currentScore=alpha_beta(0,alpha,beta,depth+1);
          minValue=getMin(minValue,currentScore);
          beta=getMin(beta,currentScore);
        }
        board[i][j]=-1; //reset
       if(currentScore == posInfinity || currentScore == negInfinity) break; //prune other siblings here
    }
}
}

  if(turn==0)
    return maxValue;
  else
    return minValue;

}


bool test_game(){

bool flag=true;
int value;

for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
        if(board[i][j]==-1){
            flag=false;
            break;
        }
        else{
            continue;
        }
    }
}

if(flag){
    return true;
}

if(board[0][0]!=-1){
    value=board[0][0];
    if(board[0][1]==value && board[0][2]==value){
        return true;
    }
    else if(board[1][0]==value && board[2][0]==value){
        return true;
    }
    else if(board[1][1]==value && board[2][2]==value){
        return true;
    }
}
if(board[1][1]!=-1){
    value=board[1][1];
    if(board[0][1]==value && board[2][1]==value){
        return true;
    }
    else if(board[1][0]==value && board[1][2]==value){
        return true;
    }
    else if(board[0][2]==value && board[2][0]==value){
        return true;
    }

}
if(board[2][2]!=-1){
    value=board[2][2];
    if(board[0][2]==value && board[1][2]==value){
        return true;
    }
    else if(board[2][0]==value && board[2][1]==value){
        return true;
    }
}
return false;


}


bool check_game(){
int value;
bool flag=false;

for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
        if(board[i][j]==-1){
            flag=true;
            break;
        }
        else{
            continue;
        }
    }
}

if(!flag){
    winner=2; //2=tie
    return true;
}

if(board[0][0]!=-1){
    value=board[0][0];
    if(board[0][1]==value && board[0][2]==value){
        winner=value;
        return true;
    }
    else if(board[1][0]==value && board[2][0]==value){
        winner=value;
        return true;
    }
    else if(board[1][1]==value && board[2][2]==value){
        winner=value;
        return true;
    }
}
if(board[1][1]!=-1){
    value=board[1][1];
    if(board[0][1]==value && board[2][1]==value){
        winner=value;
        return true;
    }
    else if(board[1][0]==value && board[1][2]==value){
        winner=value;
        return true;
    }
    else if(board[0][2]==value && board[2][0]==value){
        winner=value;
        return true;
    }

}
if(board[2][2]!=-1){
    value=board[2][2];
    if(board[0][2]==value && board[1][2]==value){
        winner=value;
        return true;
    }
    else if(board[2][0]==value && board[2][1]==value){
        winner=value;
        return true;
    }
}
return false;

}


int heuristic() {
        int score = 0;

        //Check all rows
        for (int i = 0; i < 3; ++i) {
            int X = 0;
            int O = 0;
            for (int j = 0; j < 3; ++j) {
                if (board[i][j] == 0) {
                    X++;
                } else {
                    O++;
                }
            }
            score+=get_score(X, O);
        }

        //Check all columns
        for (int j = 0; j < 3; ++j) {
            int blank = 0;
            int X = 0;
            int O = 0;
            for (int i = 0; i < 3; ++i) {
                 if (board[i][j] == 0) {
                    X++;
                } else {
                    O++;
                }
            }
            score+=get_score(X, O);
        }

        int X = 0;
        int O = 0;

        //Check diagonal (first)
        for (int i = 0, j = 0; i < 3; ++i, ++j) {
            if (board[i][j] == 0) {
                X++;
            } else if (board[i][j] == 1) {
                O++;
            }
        }

        score+=get_score(X, O);

        X = 0;
        O = 0;

        //Check Diagonal (Second)
        for (int i = 2, j = 0; i > -1; --i, ++j) {
            if (board[i][j] == 0) {
                X++;
            } else if (board[i][j] == 1) {
                O++;
            }
        }

        score+=get_score(X, O);

        return score;
    }


int get_score(int X, int O){
        int change;
        if (X == 3) {
            change = 100;
        } else if (X == 2 && O == 0) {
            change = 10;
        } else if (X == 1 && O == 0) {
            change = 1;
        } else if (O == 3) {
            change = -100;
        } else if (O == 2 && X == 0) {
            change = -10;
        } else if (O == 1 && X == 0) {
            change = -1;
        } else {
            change = 0;
        }
        return change;
    }



string get_best_move(){

int best=myMap.begin()->second;

for(map <string,int>::iterator it = myMap.begin();it != myMap.end(); ++it)
{
    if(level==1){
        if(best>(it->second))
           best=it->second;
    }
    else if(level==2){
        if(best<(it->second))
           best=it->second;
    }
}

string myBest;

for(map <string,int>::iterator it = myMap.begin();it != myMap.end(); ++it)
{
    if(it->second==best){
        myBest=it->first;
        break;
    }
    else
        continue;
}

return myBest;
}

int getMax(int x, int y){
 return x>y?x:y;
}
int getMin(int x, int y){
 return x>y?y:x;
}



