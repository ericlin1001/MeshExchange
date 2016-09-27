#include<iostream>
#include "MPIHelper.h"
#include<vector>
using namespace std;
template<class T>
void printArr(const vector<T>&arr){
	for(int i=0;i<arr.size();i++){
		if(i!=0)cout<<",";
		cout<<arr[i];
	}
}
int main(int argc,char **argv){
	MPIHelper mpi(argc,argv);
	//calculating the sum of nbrs including itself.
	int numRows=4;
	int numCols=3;
	vector<vector<int> >nbrs;
	nbrs.resize(numRows*numCols);
	for(int r=0;r<numRows;r++){
		for(int c=0;c<numCols;c++){
			int i=r*numCols+c;
			int dir[]={-1,0,0,-1,1,0,0,1};
			for(int k=0;k<4;k++){
				int nr=r+dir[2*k];
				int nc=c+dir[2*k+1];
				if(nr>=0&&nr<numRows&&nc>=0&&nc<numCols){
					nbrs[i].push_back(nr*numCols+nc);
				}
			}
		}
	}
	int current=mpi.getID();
	int state=current;
	int xs[4];
	int realState=state;
	int res=0;
	vector<int>&nbr=nbrs[current];
	//before:output state.
	if(mpi.isMaster()){
	cout<<"***********Before State************"<<endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Client(%d)> %d\n",mpi.getID(),state);
	//Process
	//cout<<"***********Processing************"<<endl;
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Request requests[4];
	MPI_Status statuses[4];
	int nnbr=nbr.size();
	//start process
	/*version1
	for(int i=0;i<nbr.size();i++){
		mpi.arecv(xs[i],nbr[i]);
		requests[i]=mpi.getRequest();
	}
	for(int i=0;i<nbr.size();i++){
		mpi.send(state,nbr[i]);
	}
	MPI_Waitall(nbr.size(),requests,statuses);
	*/
	for(int i=0;i<nbr.size();i++){
		requests[i]=mpi.arecv(xs[i],nbr[i]);
	}
	for(int i=0;i<nbr.size();i++){
		mpi.asend(state,nbr[i]);
	}
	mpi.waitAll(nbr.size(),requests);
	//end process.
	for(int i=0;i<nbr.size();i++){
		state+=xs[i];
		realState+=nbr[i];
	}
	
	//after:output state.
	MPI_Barrier(MPI_COMM_WORLD);
	if(mpi.isMaster()){
	cout<<"***********After State************"<<endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	//state=
	printf("Client(%d)> %d\t%d\t%d\n",mpi.getID(),state,realState,state==realState);
	//cout<<"Client("<<mpi.getID()<<")> "<<state<<endl;
/*	
	for(int i=0;i<numRows*numCols;i++){
		cout<<i<<":";printArr(nbrs[i]);cout<<endl;
	}
	cout<<"ehllo"<<endl;
	*/
	return 0;
}
