# Variáveis para inclusão de diretórios e bibliotecas
INCLUDE = -I/opt/ibm/ILOG/CPLEX_Studio_Community221/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio_Community221/concert/include
FLAGS = -DIL_STD -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -w
LPATH = -L/opt/ibm/ILOG/CPLEX_Studio_Community221/concert/lib/x86-64_linux/static_pic -L/opt/ibm/ILOG/CPLEX_Studio_Community221/cplex/lib/x86-64_linux/static_pic
LIBRARIES = -lconcert -lilocplex -lcplex -lpthread -ldl

# Alvo padrão para construir o executável
all: mainpd.exe

# Regra para criar o executável
mainpd.exe: mainpd.o
	g++ -O3 mainpd.o -o mainpd.exe $(INCLUDE) $(FLAGS) $(LPATH) $(LIBRARIES)

# Regra para criar o arquivo objeto
mainpd.o: mainpd.cpp
	g++ -O3 -c mainpd.cpp $(INCLUDE) $(FLAGS)

# Regra para limpar arquivos gerados
clean:
	rm -f *.exe *.o
