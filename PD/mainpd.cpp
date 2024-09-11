#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>

using namespace std;
ILOSTLBEGIN //macro para "using namespace" para ILOCPLEX

/* Nomes: Elias de Paula Pereira - 182050049

Luiz Filipe Magalhães Freitas - 212050049 */

//estrutura para armazenar os custos
struct Aresta {
    int custo;
};

//conjuntos do problema
int numFuncionarios; //quantidade de funcionários
vector<vector<Aresta>> matrizCustos; //matriz de custos
int numTarefas; //quantidade de tarefas

void readData(const string& fileName) {
    ifstream inputFile(fileName);
    
    if (!inputFile) {
        cerr << "Erro ao abrir o arquivo" << endl;
        exit(1);
    }

    //leitura de dados
    int i, funcionario, tarefa, custo;
    inputFile >> numFuncionarios >> numTarefas;

    matrizCustos.resize(numFuncionarios);
    for (i = 0; i < numFuncionarios; i++) {
        matrizCustos[i].resize(numTarefas);
    }

    while (inputFile >> funcionario >> tarefa >> custo) {
        matrizCustos[funcionario][tarefa].custo = custo;
    }

    inputFile.close();
}

void cplex() {
    try {
        IloEnv ambiente; //criacao do ambiente do CPLEX
        IloModel modelo(ambiente); 

        //definicao das variaveis de decisao
        IloArray<IloNumVarArray> x(ambiente, numFuncionarios);
        for (int i = 0; i < numFuncionarios; i++) {
            x[i] = IloNumVarArray(ambiente, numTarefas, 0, IloInfinity, ILOINT);
        }

        //funcao objetivo 
        IloExpr objetivo(ambiente);
        for (int i = 0; i < numFuncionarios; i++) {
            for (int j = 0; j < numTarefas; j++) {
                objetivo += matrizCustos[i][j].custo * x[i][j];
            }
        }
        modelo.add(IloMinimize(ambiente, objetivo));
        objetivo.end();

        //restricoes
        for (int i = 0; i < numFuncionarios; i++) {
            IloExpr soma(ambiente);
            for (int j = 0; j < numTarefas; j++) {
                soma += x[i][j];
            }
            modelo.add(soma == 1);
            soma.end();
        }

      

		//solucao do problema
        IloCplex resolvedor(modelo);

        if (resolvedor.solve()) { //tentativa de resolver o modelo
            cout << "Solucao encontrada com custo: " << resolvedor.getObjValue() << endl;
            for (int i = 0; i < numFuncionarios; i++) {
                for (int j = 0; j < numTarefas; j++) {
                    if (IloRound(resolvedor.getValue(x[i][j])) != 0) {
                        cout << "Funcionario " << i << " executa a tarefa " << j << endl;
                    }
                }
            }
        } else {
            cout << "Nenhuma solucao otima foi encontrada." << endl;
        }

        //fim do ambiente CPLEX
        resolvedor.end(); 
        ambiente.end();    
    } catch (IloException& e) {
        cerr << "Erro CPLEX: " << e.getMessage() << endl;
    } catch (...) {
        cerr << "Erro desconhecido." << endl;
    }
}

int main(int argc, char* argv[]) {
    //apenas testes de abertura do arquivo
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << "in" << endl;
        return 1;
    }
    //chamada basica da funcao
    readData(argv[1]);

    //verificacao dos dados lidos
    cout << "Numero de funcionarios: " << numFuncionarios << endl;
    cout << "Numero de tarefas: " << numTarefas << endl;
    for (int i = 0; i < numFuncionarios; i++) {
        for (int j = 0; j < numTarefas; j++) {
            cout << "Funcionario: " << i << " - Tarefa: " << j << " - Custo: " << matrizCustos[i][j].custo << endl;
        }
    }

    cplex();

    return 0;
}
