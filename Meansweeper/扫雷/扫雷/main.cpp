//
//  main.cpp
//  扫雷
//
//  Created by Yige Sun on 2023/1/2.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
using namespace std;

int lastSize = 0;
int lastRand = 0;
string hasOutline;
bool outline;
int square = 0;
char numArray[10000];
char hiddenArray[10000];

void getRandomStars();
void calculateStar();

void getAll0(int pos);

bool checkAllSign();
bool checkAllAppear();

int countLength(int num);
void printLine(int count, int lastSize);

void fullfillWithNum();

int checkCMD();
void rotate();
void melt();

void printMatrixWithOutLine();
void printMatrix();
void printHiddenMatrixWithOutLine();
void printHiddenMatrix();

char input1, input2, input3;

int main() {
    ifstream fin;
    fin.open("/Users/yigesun/Desktop/C++/hw8/上机8/上机8/config.txt");
    if(fin){
        while(!fin.eof()){
            string SIZEorRAND;
            char equal;
            int NorR;
            fin >> SIZEorRAND;
            fin >> equal;
            if(SIZEorRAND[0] != '#'){
                if((SIZEorRAND == "SIZE") || (SIZEorRAND == "RAND")){
                    fin >> NorR;
                    if(SIZEorRAND == "SIZE"){
                        lastSize = NorR;
                    }
                    if(SIZEorRAND == "RAND"){
                        lastRand = NorR;
                    }
                }
                
                if(SIZEorRAND == "BOX"){
                    fin >> hasOutline;
                    if(hasOutline == "TRUE"){
                        outline = true;
                    }else{
                        outline = false;
                    }
                }
            }else{
                fin >> NorR;
            }
        }
    }else{
        lastSize = 0;
        
    }
    
    if(lastSize <= 0){
        cout << "程序配置不对" << endl;
        lastSize = 9;
        lastRand = 30;
        outline = true;
    }

    cout << "lastSize:" << lastSize << endl;
    cout << "lastRand:" << lastRand << endl;
    cout << "box:" << hasOutline << endl;
    square = lastSize * lastSize;
    
    //用数字填充
    fullfillWithNum();
    
    //clean the whole array to be 0.
    for(int i = 0; i < (square); i++){
        numArray[i] = 0;
    }
    int openCMD = checkCMD();
    if(openCMD == 1){
        //use * to fulfill some of the places
        getRandomStars();
        //calculate the number of * in the places around.
        calculateStar();
        
        //output the melted mitrix
        if(outline){
            printMatrixWithOutLine();
        }else{
            printMatrix();
        }
        cout << endl;
        
        //output the hidden matrix
        if(outline){
            printHiddenMatrixWithOutLine();
        }else{
            printHiddenMatrix();
        }
        
        while(true){
            //用户输入行下标&列下标
            cin >> input1;
            if(input1 == '#'){
                cin >> input2 >> input3;
                int inputCol = input2 - 48;
                int inputRAW = input3 - 48;
                int pos = (inputCol-1)*lastSize + inputRAW - 1;
                hiddenArray[pos] = '#';
                if(outline){
                    printHiddenMatrixWithOutLine();
                }else{
                    printHiddenMatrix();
                }
                if(checkAllSign()){
                    cout << "YOU WIN";
                    return 2;
                }
            }else{
                cin >> input2;
                int inputCol = input1 - 48;
                int inputRAW = input2 - 48;
                //cout << inputCol << " " << inputRAW << endl;
                int pos = (inputCol-1)*lastSize + inputRAW - 1;
                //cout << pos;
                if(numArray[pos] == '*'){
                    cout << "GAME OVER";
                    return 1;
                }
                if((numArray[pos] != '*') && (numArray[pos] != 0)){
                    hiddenArray[pos] = 'A';
                    if(outline){
                        printHiddenMatrixWithOutLine();
                    }else{
                        printHiddenMatrix();
                    }
                }
                if(numArray[pos] == 0){
                    //int zeroList[10000];
                    getAll0(pos);
                    if(outline){
                        printHiddenMatrixWithOutLine();
                    }else{
                        printHiddenMatrix();
                    }
                }
                if(checkAllAppear()){
                    cout << "YOU WIN";
                    return 3;
                }
            }
        }
    }
    return 0;
}


void getAll0(int pos){
    //上方
    if((pos+1) > lastSize){
        //正上方
        if((numArray[pos-lastSize] == 0)&&(hiddenArray[pos-lastSize] != '-')){
            hiddenArray[pos-lastSize] = '-';
            //cout << "正上方" << endl;
            getAll0(pos-lastSize);
        }else{
            if((numArray[pos-lastSize] != 0)&&(numArray[pos-lastSize] != '*')){
                hiddenArray[pos-lastSize] = 'A';
            }
        }
        
        
        //左上方
        if(pos % lastSize != 0){
            if((numArray[pos-lastSize-1] == 0)&&(hiddenArray[pos-lastSize-1] != '-')){
                //cout << "左上方" << endl;
                hiddenArray[pos-lastSize-1] = '-';
                getAll0(pos-lastSize-1);
            }else{
                if((numArray[pos-lastSize-1] != 0)&&(numArray[pos-lastSize-1] != '*')){
                    hiddenArray[pos-lastSize-1] = 'A';
                }
            }
        }
        
        //右上方
        if((pos + 1) % lastSize != 0){
            if((numArray[pos-lastSize+1] == 0)&&(hiddenArray[pos-lastSize+1] != '-')){
                //cout << "右上方" << endl;
                hiddenArray[pos-lastSize+1] = '-';
                getAll0(pos-lastSize+1);
            }else{
                if((numArray[pos-lastSize+1] != 0)&&(numArray[pos-lastSize+1] != '*')){
                    hiddenArray[pos-lastSize+1] = 'A';
                }
            }
        }
    }
    //左方
    if(pos % lastSize != 0){
        if((numArray[pos-1] == 0)&&(hiddenArray[pos-1] != '-')){
            //cout << "左方" << endl;
            hiddenArray[pos-1] = '-';
            getAll0(pos-1);
        }else{
            if((numArray[pos-1] != 0)&&(numArray[pos-1] != '*')){
                hiddenArray[pos-1] = 'A';
        }
        }
    }
    //右方
    if((pos + 1 + 1) % lastSize != 0){
        if((numArray[pos+1] == 0)&&(hiddenArray[pos+1] != '-')){
            //cout << "右方" << endl;
            hiddenArray[pos+1] = '-';
            getAll0(pos+1);
        }else{
            if((numArray[pos+1] != 0)&&(numArray[pos+1] != '*')){
                hiddenArray[pos+1] = 'A';
            }
        }
    }
    
    //下方
    if(pos < (square - lastSize)){
        //正下方
        if((numArray[pos+lastSize] == 0)&&(hiddenArray[pos+lastSize] != '-')){
            //cout << "下方" << endl;
            hiddenArray[pos+lastSize] = '-';
            getAll0(pos+lastSize);
        }else{
            if((numArray[pos+lastSize] != 0)&&(numArray[pos+lastSize] != '*')){
                hiddenArray[pos+lastSize] = 'A';
            }
        }
        
        
        //左下方
        if(pos % lastSize != 0){
            if((numArray[pos+lastSize-1] == 0)&&(hiddenArray[pos+lastSize-1] != '-')){
                //cout << "左下方" << endl;
                hiddenArray[pos+lastSize-1] = '-';
                getAll0(pos+lastSize-1);
            }else{
                if((numArray[pos+lastSize-1] != 0)&&(numArray[pos+lastSize-1] != '*')){
                    hiddenArray[pos+lastSize-1] = 'A';
                }
            }
        }
        
        //右上方
        if((pos + 1) % lastSize != 0){
            if((numArray[pos+lastSize+1] == 0)&&(hiddenArray[pos+lastSize+1] != '-')){
                //cout << "右下方" << endl;
                hiddenArray[pos+lastSize+1] = '-';
                getAll0(pos+lastSize+1);
            }else{
                if((numArray[pos+lastSize+1] != 0)&&(numArray[pos+lastSize+1] != '*')){
                    hiddenArray[pos+lastSize+1] = 'A';
                }
            }
        }
    }
}

bool checkAllSign(){
    bool test = true;
    for(int i=0; i<square; i++){
        if(numArray[i] == '*'){
            if(hiddenArray[i] != '#'){
                test = false;
                break;
            }
        }else{
            if(hiddenArray[i] == '#'){
                test = false;
                break;
            }
        }
    }
    return test;
}

bool checkAllAppear(){
    bool test= true;
    for(int i=0; i<square; i++){
        if(numArray[i] != '*'){
            if((hiddenArray[i] != 'A') && (hiddenArray[i] != '-')){
                test = false;
                break;
            }
        }
    }
    return test;
}


void getRandomStars(){
    for(int i=1; i <= (lastRand * square /100); i++){
        //get a random number in 0-lastSize^2
        int randPos = rand()%square;
        //check whether the random number position has been changed
        if(numArray[randPos] != 0){
            //if yes, continue to get the random number until the position has
            //been changed
            while(numArray[randPos] != 0){
                randPos = rand()%square;
            }
            numArray[randPos] = '*';
        }else{//if no, change this position to be the new number
            numArray[randPos] = '*';
        }
    }
}

void printLine(){
    for(int i=0; i < lastSize; i++){
        cout << "+";
        cout << "---";
    }
    cout << "+" << endl;
}


void printHiddenMatrixWithOutLine(){
    printLine();
    for(int i = 0; i < square; i++){
        cout << "|";
        if((hiddenArray[i] == 'A') || (hiddenArray[i] == '-') || (hiddenArray[i] == '#') ){
            if(hiddenArray[i] == '-'){
                cout << "  -";
            }
            if(hiddenArray[i] == '#'){
                cout << "  #";
            }
            if(hiddenArray[i] == 'A'){
                printf("%3d", numArray[i]);
            }
        }else{
            cout << "   ";
        }
        //change line if the number is the last num in a line
        if((i + 1) % lastSize == 0){
            cout << "|" << endl;
            printLine();
        }
    }
}


void printHiddenMatrix(){
    // output the matrix
    for(int i = 0; i < square; i++){
        if((hiddenArray[i] == 'A') || (hiddenArray[i] == '-') || (hiddenArray[i] == '#') ){
            if(hiddenArray[i] == '-'){
                cout << "  -";
            }
            if(hiddenArray[i] == '#'){
                cout << "  #";
            }
            if(hiddenArray[i] == 'A'){
                printf("%3d", numArray[i]);
            }
        }else{
            cout << "   ";
        }
        //change line if the number is the last num in a line
        if((i + 1) % lastSize == 0){
            cout << endl;
        }
    }
}

void printMatrixWithOutLine(){
    printLine();
    for(int i = 0; i < square; i++){
        cout << "|";
        if(numArray[i] == '*'){
            cout << "  *";
        }else{
            printf("%3d", numArray[i]);
        }
        //change line if the number is the last num in a line
        if((i + 1) % lastSize == 0){
            cout << "|" << endl;
            printLine();
        }
    }
}


void printMatrix(){
    // output the matrix
    for(int i = 0; i < square; i++){
        if(numArray[i] == '*'){
            cout << "  *";
        }else{
            printf("%3d", numArray[i]);
        }
        //change line if the number is the last num in a line
        if((i + 1) % lastSize == 0){
            cout << endl;
        }
    }
}
 

void fullfillWithNum(){
    srand((unsigned int)time (NULL));
    //change the part of the matrix which has been calculated by the percent
    //to be random natural number
    if(lastRand == 0){
        int randPos = rand()%(square);
        cout << "randPos:" << randPos << endl;
        int randNum = rand()%2;
        int twoOrFour =4;
        if(randNum == 1){
            twoOrFour = 2;
        }
        numArray[randPos] = twoOrFour;
    }else{
        for(int j = 1; j <= (lastRand * square /100); j++){
            //get a random number in 0-lastSize^2
            int randPos = rand()%square;
            //get a random natural num
            int randNum = rand()%20;
            //check whether the random number position has been changed
            if(numArray[randPos] != 0){
                //if yes, continue to get the random number until the position has
                //been changed
                while(numArray[randPos] != 0){
                    randPos = rand()%square;
                }
                numArray[randPos] = randNum;
                
            }else{//if no, change this position to be the new number
                numArray[randPos] = randNum;
            }
        }
    }
}


void calculateStar(){
    for(int i=0; i<square; i++){
        int sum = 0;
        if(numArray[i] != '*'){
            if((lastSize < i) && ((i % lastSize) != 0) &&
               (((i + 1) % lastSize) != 0) && (i < (square - lastSize))){
                //左上
                if(numArray[i - lastSize - 1] == '*'){
                    sum +=1;
                }
                //上
                if(numArray[i - lastSize] == '*'){
                    sum += 1;
                }
                //右上
                if(numArray[i -lastSize + 1]=='*'){
                    sum += 1;
                }
                //左
                if(numArray[i - 1] == '*'){
                    sum += 1;
                }
                //右
                if(numArray[i+1] == '*'){
                    sum +=1;
                }
                if(numArray[i + lastSize - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize + 1] == '*'){
                    sum += 1;
                }
            }
            if(i == 0){
                if(numArray[i+1] == '*'){
                    sum +=1;
                }
                if(numArray[i + lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize + 1] == '*'){
                    sum += 1;
                }
            }
            if(i == (lastSize -1)){
                if(numArray[i - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize] == '*'){
                    sum += 1;
                }
            }
            if(i == square - lastSize){
                if(numArray[i - lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i -lastSize + 1]=='*'){
                    sum += 1;
                }
                if(numArray[i+1] == '*'){
                    sum +=1;
                }
            }
            if(i == square - 1){
                if(numArray[i - lastSize - 1] == '*'){
                    sum +=1;
                }
                if(numArray[i - lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i - 1] == '*'){
                    sum += 1;
                }
            }
            if( (0 < i) && (i <(lastSize-1))){
                if(numArray[i - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i+1] == '*'){
                    sum +=1;
                }
                if(numArray[i + lastSize - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize + 1] == '*'){
                    sum += 1;
                }
            }
            if( (i!=0) && (i!= (square - lastSize)) && (i %lastSize == 0)){
                if(numArray[i - lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i -lastSize + 1]=='*'){
                    sum += 1;
                }
                if(numArray[i+1] == '*'){
                    sum +=1;
                }
                if(numArray[i + lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize + 1] == '*'){
                    sum += 1;
                }
            }
            if((i != (lastSize-1)) && (i != (square -1)) && (((i + 1) % lastSize) == 0)){
                if(numArray[i - lastSize - 1] == '*'){
                    sum +=1;
                }
                if(numArray[i - lastSize] == '*'){
                    sum += 1;
                }
                if(numArray[i - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize - 1] == '*'){
                    sum += 1;
                }
                if(numArray[i + lastSize] == '*'){
                    sum += 1;
                }
            }
            if(((square - lastSize) < i) && (i < (square - 1))){
                //左上
                if(numArray[i - lastSize - 1] == '*'){
                    sum +=1;
                }
                //上
                if(numArray[i - lastSize] == '*'){
                    sum += 1;
                }
                //右上
                if(numArray[i -lastSize + 1]=='*'){
                    sum += 1;
                }
                //左
                if(numArray[i - 1] == '*'){
                    sum += 1;
                }
                //右
                if(numArray[i+1] == '*'){
                    sum +=1;
                }
            }
            numArray[i] = sum;
        }
    }
}


int checkCMD(){
    ifstream fin2;
    fin2.open("cmd.txt");
    if(fin2){
        string inst;
        while(true){
            //input the instruct from the cmd.txt
            fin2 >> inst;
            //if meet Q, end this program
            if(inst == "Q"){
                break;
            }else{
                //if the instruct is rotate
                if(inst == "rotate"){
                    rotate();
                    printMatrix();
                    continue;
                }
                //if the instruct is melt
                if(inst == "melt"){
                    melt();
                    printMatrix();
                    continue;
                }
            }
        }
        return 0;
    }else{
        return 1;
    }
}

void rotate(){
    int numArrayNew[square];
    //copy the elements in numArray to be in a new array
    for(int i=0; i < square; i++){
        numArrayNew[i] = numArray[i];
    }
    for(int i=0; i < lastSize; i++){
        for(int j=i*lastSize; j<(i * lastSize+lastSize); j++){
            numArray[j] = numArrayNew[(square - (lastSize - i)) -
                                      lastSize * (j-i*lastSize)];
        }
    }
}


void melt(){
    //move all of the 0 in each row to the top of the row
    for(int i = 0; i<lastSize; i++){
        int zeroNum = 0;
        for(int j=0; j<lastSize; j++){
            //calculate how many 0s in one row
            if(numArray[i + lastSize*j] == 0){
                zeroNum ++;
            }else{//if is not 0, move it down down with the distance of the number of 0s
                numArray[i + lastSize*j + lastSize*zeroNum] = numArray[i + lastSize*j];
            }
        }
        //we have known about how many 0s we have, so we change the top numbers to be 0
        for(int j = 0; j < zeroNum; j++){
            numArray[i + j*lastSize] = 0;
        }
    }
    
    //modify the number pairs
    for(int i=0; i<(lastSize - 1); i++){
        for(int j=0; j<lastSize; j++){
            //if the number and the number below it is the same, and both of them are not 0,
            //double the below one
            if ((numArray[j + lastSize*i] != 0) &&
                (numArray[j + lastSize*i] == (numArray[j + lastSize*(i+1)]))){
                numArray[j + lastSize*i] = 0;
                numArray[j + lastSize*(i+1)] = 2 * numArray[j + lastSize*(i+1)];
            }
        }
    }
}

