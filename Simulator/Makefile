O_filefolder = ./build/O_files
Core_src = ./Core/Src
Core_inc = ./Core/Inc

simulator: sim_base.o main.o
	gcc $(O_filefolder)/*.o -o simulator

sim_base.o:
	gcc -I$(Core_inc) -c $(Core_src)/sim_screen.c -o $(O_filefolder)/sim_base.o

main.o:
	gcc -I$(Core_inc) -c $(Core_src)/sim_main.c -o $(O_filefolder)/main.o
