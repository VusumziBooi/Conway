#include<iostream>
#include<vector>
#include<utility>
#include<fstream>
#include<mpi.h>

using namespace  std;
int rSIZE , cSIZE;

struct Conway{ 
    vector<vector<int>>board;
    vector<vector<int>>copy;

    Conway(vector<vector<int> >board){
        this->board = board;
        this->copy = board;
    }

    bool isAlive(int row , int col){
        if(board[row][col] == 1){
            return true;
        }
        return false;
    }

    bool isValid(int row , int col){
        if(row < 0 || row > rSIZE-1 || col < 0 || col > cSIZE - 1){
            return false;
        }
        return true;
    }

    vector<pair<int , int>> findNeighbours(int row , int col){
        vector<pair<int , int>>neighbours;
        pair<int , int> Coord;

        int n1r,n2r,n3r,n4r,n5r,n6r,n7r,n8r;
        int n1c,n2c,n3c,n4c,n5c,n6c,n7c,n8c;
        n1r = row , n1c = col-1;
        n2r = row , n2c = col+1;
        n3r = row -1 , n3c = col;
        n4r = row + 1 , n4c = col; 
        n5r = row -1 , n5c = col -1;
        n6r = row + 1 , n6c = col + 1;
        n7r = row + 1 , n7c = col -1;
        n8r = row -1 , n8c = col + 1;
        if(isValid(n1r , n1c)){
            Coord.first = n1r;
            Coord.second = n1c;
            neighbours.push_back(Coord);
        }
        if(isValid(n2r , n2c)){
            Coord.first = n2r;
            Coord.second = n2c;
            neighbours.push_back(Coord);
        }
        if(isValid(n3r , n3c)){
            Coord.first = n3r;
            Coord.second = n3c;
            neighbours.push_back(Coord);
        }
        if(isValid(n4r , n4c)){
            Coord.first = n4r;
            Coord.second = n4c;
            neighbours.push_back(Coord);
        }
        if(isValid(n5r , n5c)){
            Coord.first = n5r;
            Coord.second = n5c;
            neighbours.push_back(Coord);
        }
        if(isValid(n6r , n6c)){
            Coord.first = n6r;
            Coord.second = n6c;
            neighbours.push_back(Coord);
        }
        if(isValid(n7r , n7c)){
            Coord.first = n7r;
            Coord.second = n7c;
            neighbours.push_back(Coord);
        }
        if(isValid(n8r , n8c)){
            Coord.first = n8r;
            Coord.second = n8c;
            neighbours.push_back(Coord);
        }

        return neighbours;
    }

    vector<vector<int>> nextLife(vector<vector<int>> &mylist , int rank , int iterations , int increment){
        this->board = mylist;
        int counter = 0;
        int i = rank;
        while(counter < iterations){
            for(int j = 0 ; j < cSIZE ; j++){
                int curr = board[i][j];
                if(isAlive(i , j)){
                    //find neighbours
                    vector<pair<int , int>>neighbours = findNeighbours(i , j);
                    //count all live neighbours
                    int counter = 0;
                    for(pair<int , int> temp : neighbours){
                        if(isAlive(temp.first , temp.second)){
                            counter++;
                        }
                    }

                    if(counter < 2){
                        copy[i][j] = 0;
                    }
                    else if(counter == 2 || counter == 3){
                        copy[i][j] = 1;
                    }
                    else{
                        copy[i][j] = 0;
                    }


                }
                else{//dead cell
                    vector<pair<int , int>>neighbours = findNeighbours(i , j);
                    //count all live neighbours
                    int counter = 0;
                    for(pair<int , int> temp : neighbours){
                        if(isAlive(temp.first , temp.second)){
                            counter++;
                        }
                    }
                    if(counter == 3){
                        copy[i][j] = 1;
                    }
                    else{
                        copy[i][j] = 0;
                    }
                }
            }
            i+=increment;
            counter ++;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        board = copy;
        return board;
    }
};

void printBoard(vector<vector<int>> mylist){
    string space = "";
    for(int i = 0 ; i < mylist.size();i++){
        for(int j = 0 ; j < mylist[0].size();j++){
            cout << space << mylist[i][j];
            space = " ";
        }
        space = "";
        cout << endl;
    }
}

int main(int &argc , char *argv[]){
        int r , c , input , gen , num_procs , myrank;
        vector<vector<int>> mylist;        

        MPI_Init(&argc , &argv);

        MPI_Comm_size(MPI_COMM_WORLD , &num_procs);
        MPI_Comm_rank(MPI_COMM_WORLD , &myrank);

        if(myrank == 0){
            cout << "Please insert the dimensions of your game \n";
            cin >> r >>c;
            cout << "Please enter your initial state \n";         
            for(int i = 0 ; i < r ; i++){
                vector<int>temp;
                for(int j = 0 ; j < c ; j++){
                    cin>>input;
                    temp.push_back(input);
                }
                mylist.push_back(temp);
            }
            rSIZE = mylist.size();
            cSIZE = mylist[0].size();
            cout << "Please enter the number of generations you want to explore \n";
            cin >> gen;
            rSIZE = r;
            cSIZE = c;
        }

        

        Conway game(mylist);
        if(myrank == 0){
            cout << endl;
        }

        int iterations = (int)rSIZE/num_procs*1.0;
        int increment = num_procs;

        /*vector<vector<int>>myVector = game.nextLife(mylist , myrank , iterations , increment);
        if(myrank == 0){
            cout << "Generation " << 1 << " results are : \n";
            printBoard(myVector);
            cout << "-----------------------------------------------------------------------"<<endl;
            mylist = myVector;
        }*/

    

        


    return 0;
}


