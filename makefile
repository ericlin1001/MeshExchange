cl=mpic++.mpich
exe=mpirun.mpich
file=main.cpp
out=main
all:Parallel
Serial:$(file)
	$(cl) -o $(out) $(file)
	./$(out)

Parallel:$(file)
	$(cl) -DUSE_MPI -o $(out) $(file)
	$(exe) -n 12 ./$(out)

