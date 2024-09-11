#include <bits/stdc++.h>
#include <ilcplex/ilocplex.h>

using namespace std;
ILOSTLBEGIN

ILOSTLBEGIN //macro para "using namespace" para ILOCPLEX

/* Nomes: Elias de Paula Pereira - 182050049

Luiz Filipe Magalhães Freitas - 212050049 */

//def de variaveis globais

int N; // Numero de nos
int A;  // Numero de arcos
//vector<vector<int>> custos; // Custos unitarios de transporte
//vector<vector<int>> fluxosMinimos; //fluxos minimos
vector<vector<int>> fluxosMaximos; //fluxos maximos
vector<int> ofertas; //ofertas
//vector<int> demandas; //demandas

//Funcao parar fazer a leitura do arquivo 
void readData(const string& in) {
    ifstream file(in);
    if (!file) {
        cerr << "Nao foi possivel abrir o arquivo." << endl;
        exit(1);
    }
    file >> N >> A;

    //custos.resize(N, vector<int>(N, 0));
    //fluxosMinimos.resize(N, vector<int>(N, 0));
    fluxosMaximos.resize(N, vector<int>(N, 0));
    ofertas.resize(N, 0);
    //demandas.resize(N, 0);

    for (int i = 0; i < A; ++i) {
        int origem, destino, capacidade;
            file >> origem >> destino >> capacidade;
                //custos[origem - 1][destino - 1] = capacidade;
                //fluxosMinimos[origem - 1][destino - 1] = 0; //valor zerado
                fluxosMaximos[origem - 1][destino - 1] = capacidade;
    }

    for (int i = 0; i < N; i++)
    {
        int aux;
        file >> aux;
        file >> ofertas[aux - 1];
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

	for( int i = 0; i < N; i++ ){
		x.add(IloNumVarArray(env));
		for( int j = 0; j < N; j++ ){
			x[i].add(IloIntVar(env, 0, 10000));
		}
	}
    
    
    //funcao objetivo
    IloExpr sum(env);
    sum.clear();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum += fluxosMaximos[i][j] * x[i][j];
        }
    }
    model.add(IloMinimize(env, sum));

    //restricao de oferta
    for (int i = 0; i < N; i++) {
        if (ofertas[i] < 0) {break;}
        
        IloExpr ofertaSum(env);
        ofertaSum.clear();
        for (int j = 3; j < N; ++j) {
            if (i != j) {
                ofertaSum += x[i][j];
            }
        }
        //base do problema do mochila
        model.add(ofertaSum <= ofertas[i]);
    }

    //restricao de demanda
    for (int i = 0; i < N; i++) {
        if (ofertas[i] > 0) {continue;}
        
        IloExpr demandaSum(env);
        demandaSum.clear();
        for (int j = 0; j < 3; ++j) {
            if (i != j) {
                demandaSum += x[j][i];
            }
        }
        //base do problema do mochila
        model.add(demandaSum == ofertas[i]*(-1));
    }


    //restricao de Conservacao de Fluxo
    /*
    for (int i = 0; i < N; ++i) {
        IloExpr entradaSum(env);
        IloExpr saidaSum(env);
        for (int j = 0; j < N; ++j) {
            if (i != j) {
                entradaSum += x[j][i];
                saidaSum += x[i][j];
            }
        }
        //base do problema do mochila
        model.add(saidaSum - entradaSum == 0);
    }

    //restricao de capacidade
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (fluxosMaximos[i][j] > 0) {
                model.add(x[i][j] <= fluxosMaximos[i][j]);
            }
            if (fluxosMinimos[i][j] > 0) {
                model.add(x[i][j] >= fluxosMinimos[i][j]);
            }
        }
    }*/

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
            if (i >= 3) {break;}
            
            for (int j = 0; j < N; j++) {
                if (j <3 ) {continue;}
                
                cout << "x[" << i << "][" << j << "] = " << cplex.getValue(x[i][j])<< endl;
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
    for (const auto& linha : fluxosMaximos) {
        for (int valor : linha) {
            
                cout << valor << " ";
            
        }
        cout << endl;
        
    }

    cout << "Fluxos Minimos e Maximos:" << endl;
    /*
    for (const auto& linha : fluxosMinimos) {
        for (int valor : linha) {
            cout << valor << " ";
        }
        cout << endl;
    }
    for (const auto& linha : fluxosMaximos) {
        for (int valor : linha) {
            cout << valor << " ";
        }
        cout << endl;
    }*/

    cout << "Ofertas:" << endl;
    for (int valor : ofertas) {
        if (valor < 0){
            break;
        }
        cout << valor << " ";
    }
    cout << endl;

    cout << "Demandas:" << endl;
    for (int valor : ofertas) {
        if (valor > 0){
            continue;
        }
        cout << valor << " ";
    }
    cout << endl;

    //chamada da funcao CPLEX
    cplex();

    return 0;
}