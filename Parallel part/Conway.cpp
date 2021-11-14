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

    vector<vector<int>> nextLife(vector<vector<int>> &mylist , int start , int stop){
        this->board = mylist;
        for(int i = start ; i < stop ; i++){
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
        }
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

int main(int argc , char *argv[]){
        int r , c , input , gen , num_procs , myrank , iterations , start ,stop , last;
        vector<vector<int>> mylist ;     

        MPI_Init(&argc , &argv);

        MPI_Comm_size(MPI_COMM_WORLD , &num_procs);
        MPI_Comm_rank(MPI_COMM_WORLD , &myrank);

        if(myrank == 0){
            mylist.clear();
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
            cout << "Please enter the number of generations you want to explore \n";
            cin >> gen;
            rSIZE = r;
            cSIZE = c;

        }
         
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&rSIZE , 1, MPI_INT , 0 ,MPI_COMM_WORLD);
        MPI_Bcast(&cSIZE , 1, MPI_INT , 0 ,MPI_COMM_WORLD);
        MPI_Bcast(&gen , 1, MPI_INT , 0 ,MPI_COMM_WORLD);
        mylist.resize(rSIZE , vector<int>(cSIZE));
        for(int i = 0 ; i < rSIZE ; i++){
            MPI_Bcast(&mylist[i][0] , cSIZE, MPI_INT , 0 ,MPI_COMM_WORLD);
        }
        
        
        

        iterations = (int)rSIZE/num_procs*1.0;
        start = myrank * iterations;
        if(rSIZE % num_procs != 0){//last process will do extra job
            if(myrank == num_procs-1){
                iterations = rSIZE - iterations*(num_procs-1);
                last = iterations;
            }
        }
        stop = start + iterations;
    
        Conway game(mylist);
        for(int z = 1 ; z <= gen ; z++){

            vector<vector<int>>myVector = game.nextLife(mylist , start , stop);
            mylist = myVector;
            int row = 0;
            for(int i = 0 ; i < num_procs ;i++){//for each process
                int counter = 0;
                while(counter < iterations){
                    MPI_Bcast(&mylist[row][0] , cSIZE, MPI_INT , i ,MPI_COMM_WORLD);
                    counter++;
                    row++;
                }
            }
            

            
            MPI_Barrier(MPI_COMM_WORLD);

            if(myrank == 0){
                cout << endl;
                cout << "Generation "<< z<< " results are:  "<<endl;
                printBoard(mylist);
                cout << "-----------------------------------------------------------------------"<<endl;
            }

        }
        
        

    

        MPI_Finalize();

        



    return 0;
}


