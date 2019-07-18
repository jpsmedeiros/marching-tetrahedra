# marching-tetrahedra

### Instalando e rodando no Ubuntu  
	
	sudo apt-get install build-essentials freeglut3-dev

Compile usando nosso script  
	./compile.sh arquivo.cpp output.o  

### MPI

Para instalar o MPI na sua máquina
	
	mkdir openmpi
	cd openmpi
	wget https://download.open-mpi.org/release/open-mpi/v3.1/openmpi-3.1.0.tar.gz
	tar -xzvf openmpi-3.1.0.tar.gz
	cd openmpi-3.1.0
	./configure --prefix=$HOME/openmpi
	make all
	make install

Após isso adicione essa linha ao seu .bashrc e reinicie seu terminal:

	export PATH=$PATH:$HOME/openmpi/bin

Para rodar o programa:

	mpicc main.cpp -o main.o
	mpirun -np X -mca plm_rsh_no_tree_spawn 1 main.o

OBS: -np X , onde X representa o número de processos a serem criados
OBS2: a opção -mca plm_rsh_no_tree_spawn 1 é útil apenas se você precisar que o MPI não distribua a operação por SSH em modo de árvore  
do nó A pro B depois do B pro C. Com essa opção, as SSH's exxecutadas para fazer operações de inicialização são a partir do nó A para os outros nós apenas.