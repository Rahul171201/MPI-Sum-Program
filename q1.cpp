#include<bits/stdc++.h>
#include<mpi.h>

using  namespace std;

bool check_power(int n){
	int count = 0;
	int total = log2(n) + 1;
	while(n>0){
		if(n&1)
			count++;
		n = n >> 1;
	}
	if(count!=1)
		return false;
	else
		return true;
}

int main(int argc, char *argv[]){
	
	int n = 8, process_id, num_of_procs ,final_sum=0;
	
	int A[n]; // array storing values from 1 to n
	for(int i=0;i<n;i++){
		A[i] = i+1;
	}
	
	if(!check_power(n)){
		cout<<"Invalid value of n : (n should be a power of 2)";
		return MPI_Abort(MPI_COMM_WORLD, 1);
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_procs);
	
	if(process_id == 0){
		// process id 0 receives the partial sum from all the other processes and sums up the final result	
		for(int i=1;i<num_of_procs;i++){
			int value;
			MPI_Status status;
			int res = MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE,0,MPI_COMM_WORLD, &status);
			if(res == MPI_SUCCESS){
				final_sum+=value;
			}
			else{
				cout<<"Error receiving at root : aborting...";
				return MPI_Abort(MPI_COMM_WORLD,1);
			}
		}
		
	}
	
	else{
		int part = (n / (num_of_procs-1));
		int sum=0;
		for(int i=((process_id-1)*part); i<(process_id)*part;i++){
			sum = sum + A[i];
		}
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
		cout<<"Process no :"<<process_id<<" has completed it's task and sent the message with value : "<<sum<<" to process 0\n";
	}
	
	if(process_id == 0){
		cout<<"The final sum is : "<<final_sum<<"\n";
	}
	MPI_Finalize();
}
