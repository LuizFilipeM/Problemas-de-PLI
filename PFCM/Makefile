# Variáveis para inclusão de diretórios e bibliotecas
INCLUDE = -I/opt/ibm/ILOG/CPLEX_Studio_Community221/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio_Community221/concert/include
FLAGS = -DIL_STD -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -w
LPATH = -L/opt/ibm/ILOG/CPLEX_Studio_Community221/concert/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio_Community221/cplex/lib/x86-64_linux/static_pic
LIBRARIES = -lconcert -lilocplex -lcplex -lpthread -ldl

# Alvo padrão para construir o executável
all: mainpfcm.exe

# Regra para criar o executável
mainpfcm.exe: mainpfcm.o
	g++ -O3 mainpfcm.o -o mainpfcm.exe $(INCLUDE) $(FLAGS) $(LPATH) $(LIBRARIES)

# Regra para criar o arquivo objeto
mainpfcm.o: mainpfcm.cpp
	g++ -O3 -c mainpfcm.cpp $(INCLUDE) $(FLAGS) $(LPATH) $(LIBRARIES)

# Regra para limpar arquivos gerados
clean:
	rm -f *.exe *.o
