#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

ILOSTLBEGIN //macro para "using namespace" para ILOCPLEX

/* Nomes: Elias de Paula Pereira - 182050049

Luiz Filipe Magalhães Freitas - 212050049 */

//def de variaveis globais
int A; // Numero de arcos
int N; // Numero de nos

vector<vector<int>> custos; // Custos unitarios de transporte

//Funcao parar fazer a leitura do arquivo 
void readData(const string& in) {
    ifstream file(in);
    if (!file) {
        cerr << "Nao foi possivel abrir o arquivo." << endl;
        exit(1);
    }
    file >> N >> A;

    custos.resize(N, vector<int>(N, 0));
    

    for (int i = 0; i < A; ++i) {
        int origem, destino, custo;
        file >> origem >> destino >> custo;
        custos[origem][destino] = custo;
    }
    /*exemplo simples
    ofertas.assign(N, 0);
    demandas.assign(N, 0);*/

    file.close();
}


//funcao de modelagem com CPLEX
void cplex() {
    IloEnv env;
    IloModel model(env);

    //def das variaveis de decisao
    IloArray<IloNumVarArray> x(env);
	for(int i = 0; i < N; i++ ){
		x.add(IloNumVarArray(env));
		for(int j = 0; j < N; j++ ){
            x[i].add(IloIntVar(env, 0, 100000));
		}
	}

    //funcao objetivo
    IloExpr sum(env);
    IloExpr sum2(env);
    sum.clear();
    for (int i = 0; i < N; i++) {
        if (custos[0][i] == 0) continue;
        sum += x[0][i];
    }
    model.add(IloMaximize(env, sum));


    //tudo que sai da origem é igual ao que chega no destino
    sum.clear();
    sum2.clear();
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            model.add(0 <= x[i][j] <= custos[i][j]);
        }
        
    }
    



    for(int i = 0; i < N; i++){
        if(i == 0 || i == 6) continue;
        sum.clear();
        sum2.clear();

		for(int j = 0; j < N; j++){
			if(i==j) continue;
        	sum += x[i][j];	
		}

		for (int k = 0; k < N; k++){
			if(custos[k][i] == 0) continue;
			sum2 += x[k][i];
		}

		
        model.add(sum == sum2);
	}
    
    


    //conf do CPLEX
    IloCplex cplex(model);
    cplex.setParam(IloCplex::TiLim, 3600);

    //execucao do modelo
    cplex.solve();

    //resultados
    if (cplex.getStatus() == IloAlgorithm::Optimal) {
        cout << "Solução Ótima Encontrada!" << endl;
        cout << "Valor da Função Objetivo: " << cplex.getObjValue() << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if(custos[i][j] == 0) continue;
                cout << "x[" << i << "][" << j << "] = " << cplex.getValue(x[i][j]) << endl;
            }
        }
    } else {
        cout << "Nenhuma solução ótima encontrada." << endl;
    }

    //liberacao de memoria
    env.end();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo de dados>" << endl;
        return 1;
    }

    string filename = argv[1];
    readData(filename);

    // Exibir dados para verificacao
    cout << "Numero de Nos: " << N << endl;
    cout << "Custos Unitarios de Transporte:" << endl;
   
    for (const auto& linha : custos) {
        for (int valor : linha) {
            cout << valor << " ";
        }
        cout << endl;
    }


    //chamada da funcao CPLEX
    cplex();

    return 0;
}