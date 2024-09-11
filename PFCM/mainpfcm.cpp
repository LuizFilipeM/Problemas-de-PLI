#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
ILOSTLBEGIN //macro para "using namespace" for ILOCPLEX

/* Nomes: Elias de Paula Pereira - 182050049

Luiz Filipe Magalhães Freitas - 212050049 */

int N, A;  // Número de nós e arcos
vector<int> ofertas;  // Ofertas
vector<int> demandas;  // Demandas
vector<vector<int>> custos;  // Custos unitários
vector<vector<int>> capacidades;  // Capacidades máximas

void readData(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit(1);
    }

    file >> N >> A;  // Lendo o número de nós e arcos

    // Inicializando vetores
    ofertas.resize(N, 0);
    demandas.resize(N, 0);
    custos.resize(N, vector<int>(N, 0));
    capacidades.resize(N, vector<int>(N, 0));

    // Lendo nós (o = oferta, t = transbordo, d = demanda)
    cout << "Lendo nós:" << endl;
    for (int i = 0; i < N; ++i) {
        char tipo;
        int valor;
        file >> tipo >> valor;
        if (tipo == 'o') {
            ofertas[i] = valor;  // Nó de oferta
            cout << "Nó " << i << ": Oferta = " << valor << endl;
        } else if (tipo == 'd') {
            demandas[i] = valor;  // Nó de demanda
            cout << "Nó " << i << ": Demanda = " << valor << endl;
        } else {
            cout << "Nó " << i << ": Transbordo" << endl;
        }
    }

    // Lendo arcos (origem, destino, custo, capacidade)
    cout << "Lendo arcos:" << endl;
    for (int i = 0; i < A; ++i) {
        int origem, destino, custo, capacidade;
        file >> origem >> destino >> custo >> capacidade;
        custos[origem][destino] = custo;
        capacidades[origem][destino] = capacidade;
        cout << "Arco " << origem << " -> " << destino 
             << ": Custo = " << custo 
             << ", Capacidade = " << capacidade << endl;
    }

    file.close();

    // Mostrar dados armazenados
    cout << "Dados armazenados:" << endl;
    cout << "Ofertas: ";
    for (int i = 0; i < N; ++i) {
        cout << ofertas[i] << " ";
    }
    cout << endl;

    cout << "Demandas: ";
    for (int i = 0; i < N; ++i) {
        cout << demandas[i] << " ";
    }
    cout << endl;

    cout << "Custos:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (custos[i][j] > 0) {
                cout << "Custo [" << i << "][" << j << "] = " << custos[i][j] << endl;
            }
        }
    }

    cout << "Capacidades:" << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (capacidades[i][j] > 0) {
                cout << "Capacidade [" << i << "][" << j << "] = " << capacidades[i][j] << endl;
            }
        }
    }
}

void cplex() {
    IloEnv env;
    IloModel model(env);

    // Definição das variáveis de decisão
    IloArray<IloNumVarArray> x(env);
    for (int i = 0; i < N; ++i) {
        x.add(IloNumVarArray(env));
        for (int j = 0; j < N; ++j) {
            x[i].add(IloIntVar(env, 0, CPXINT_MAX)); // Limite pela capacidade do arco
        }
    }

// Função objetivo
IloExpr sum(env);
for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
        if (custos[i][j] != 0 ) {
            sum += custos[i][j] * x[i][j];
        }
    }
}
model.add(IloMinimize(env, sum));

// Restrições de oferta
for (int i = 0; i < N; ++i) {
    if (ofertas[i] != 0) { // Apenas para nós de oferta
        IloExpr saidaSum(env);
        IloExpr entradaSum(env);
        for (int j = 0; j < N; ++j) {
            if (custos[i][j]!=0 ){
                entradaSum += x[i][j]; // Fluxo que sai do nó i
            }
        }
        for (int k = 0; k < N; ++k) {
            if (custos[k][i]!=0) {
                saidaSum += x[k][i]; // Fluxo que entra no nó i
            }
        }
        model.add(entradaSum - saidaSum <= ofertas[i]);
    }
}
    // Restrições de demanda
for (int i = 0; i < N; ++i) {
    if (demandas[i] != 0) { // Apenas para nós de demanda
        IloExpr entradaSum(env);
        IloExpr saidaSum(env);
        for (int j = 0; j < N; ++j) {
            if (custos[i][j]!=0) {
                entradaSum += x[i][j]; // Fluxo que entra no nó i
            }
        }
        for (int k = 0; k < N; ++k) {
            if (custos[k][i]!=0) {
                saidaSum += x[k][i]; // Fluxo que sai do nó i
            }
        }
        model.add(entradaSum- saidaSum <= -demandas[i]);
    }
}

 // Restrições de transbordo
IloExpr entradaSum(env);
IloExpr saidaSum(env);

for (int i = 0; i < N; ++i) {
    if (ofertas[i] == 0 && demandas[i] == 0) { // Apenas para nós de transbordo
        
        entradaSum.clear();

        for (int j = 0; j < N; ++j) {
            if (custos[i][j]!= 0) {
                entradaSum += x[i][j]; // Fluxo que entra no nó i
            }
        }

        saidaSum.clear();
        
        for (int k = 0; k < N; ++k) {
            if (custos[k][i]!=0) {
                saidaSum += x[k][i]; // Fluxo que sai do nó i
            }
        }

        model.add(entradaSum - saidaSum == 0);
    }
}

    // Adiciona as restrições de capacidade
// Restrições de capacidade
for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
        if (capacidades[i][j] != 0) {
            model.add(x[i][j] <= capacidades[i][j]);
        }
    }
}

    // Adiciona a restrição de não negatividade
// for (int i = 0; i < N; ++i) {
//     for (int j = 0; j < N; ++j) {
//         if (custos[i][j] > 0) {
//             model.add(x[i][j] >= 0);  // Não-negatividade
//         }
//     }
// }

    // Configuração do CPLEX
IloCplex cplex(model);
cplex.setParam(IloCplex::TiLim, 3600);

// Execução do modelo
if (!cplex.solve()) {
    cerr << "Erro ao resolver o modelo: " << cplex.getStatus() << endl;
    env.end();
    return;
}

// Resultados
if (cplex.getStatus() == IloAlgorithm::Optimal) {
    cout << "Solução Ótima Encontrada!" << endl;
    cout << "Valor da Função Objetivo: " << cplex.getObjValue() << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (custos[i][j] > 0) {
                // Verifica se a variável foi encontrada e obtém seu valor
                try {
                    double value = cplex.getValue(x[i][j]);
                    cout << "x[" << i << "][" << j << "] = " << value << endl;
                } catch (const IloException& e) {
                    cerr << "Erro ao obter valor da variável x[" << i << "][" << j << "]: " << e.getMessage() << endl;
                }
            }
        }
    }
} else {
    cout << "Nenhuma solução ótima encontrada." << endl;
}

    // Liberação de memória
    env.end();
}

int main() {
    readData("in.txt");
    cplex();
    return 0;
}

