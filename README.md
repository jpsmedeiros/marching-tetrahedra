# marching-tetrahedra

### Instalando no Ubuntu
	
	$ sudo apt-get install build-essential freeglut3-dev libxmu-dev libxi-dev

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

	export PATH=$PATH:$HOME/openmpi/bin1



## Projeto Escrita

Nessa pasta encontra-se o programa que vai rodar no oscar.  
Ele é responsável por gerar o arquivo com os pontos de superfície e normais para a   
renderização final (pelo projeto-leitura) e também contém a lógica do MPI para ser executado 
  em paralelo no cluster.

Compile e rode com mpi usando nosso script  
(o primeiro parâmetro é o nº de processos para ser passado ao MPI e o segundo é a resolução para renderizarmos a imagem)

	$ ./compile.sh 8 400
	
Para compilar no cluster

    $ /var/programas/openmpi-1.8.4/bin/mpic++ Decimate.cpp Isosurface.cpp Gyroid.cpp math3D.cpp main.cpp -o main.o	

## Projeto Leitura

Nessa pasta encontra-se o programa que efetivamente lê o resultado do arquivo com os pontos  
necessários gerado pelo projeto-escrita e exibe na tela a imagem resultando utilizando openGL.


Compile com cmake:
    
    $ cd projeto-leitura/build
    $ cmake --build .
    $ cmake ..
