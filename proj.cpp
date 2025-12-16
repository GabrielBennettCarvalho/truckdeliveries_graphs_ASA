#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
int N, M, m1, m2, K;
vector<vector<int>> adj; // adjacency list
vector<int> in_degree;   // qntas setas apontam para aquele cruzamento

using ll = long long;

int main() {
    std::ios::sync_with_stdio(0); 
    std::cin.tie(0);

    if (!(cin >> N >> M >> m1 >> m2 >> K)) return 0;

    adj.resize(N + 1); // +1 pq os cruzamentos sao de 1 a N, ignoramos o 0
    in_degree.assign(N + 1, 0); // começa tudo a 0

    for (int i = 0; i < K; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v); // u aponta para v
        in_degree[v]++; // mais uma pessoa a apontar para v
    }


    queue<int> q;
    vector<int> topological_order;


    for (int i = 0; i < N; i++) {
        if (in_degree[i] == 0)
        q.push(i);
    }

    while (!q.empty()) {
        int u = q.front(); // ficar com o primeiro elemento da fila
        q.pop();           // retirá-lo

        topological_order.push_back(u); // save it

        for (int v : adj[u]) {
            in_degree[v]--; // "removemos" a aresta u -> v
            if (in_degree[v] == 0) {
                q.push(v); // se v ficou livre, entra na fila
            }
        }
    }
    // Criar um mapa reverso: "Em que posição do vetor está o nó X?"
    vector<int> position_at_top(N + 1);
    for (int k = 0; k < N; k++) {
        // Se na ordem topológica o nó 5 está no índice 0, guardamos posicao[5] = 0
        position_at_top[topological_order[k]] = k;
    }


    //  Matriz para guardar pares por camião: entregas[ID_CAMIAO] = lista de {A, B}
    vector<vector<pair<int, int>>> deliveries(M + 1);
    vector<ll> paths(N + 1, 0);
    // cada i é o cruzamento em que começamos
    for (int i = 0; i <= N; i++) {

        // limpar o vetor para 0
        fill(paths.begin(), paths.end(), 0);

        // Começar o loop interior APENAS onde o 'i' aparece!
        // Ignoramos todos os nós que estão para trás na ordem topológica,
        // porque num DAG é impossível chegar lá.
        int start_index = position_at_top[i]; 

        for (int k = start_index; k < N; k++) { 
            int u = topological_order[k];      
            if (paths[u] == 0) continue; // u n é atingivel a partir de i por isso cagamos

            //se u não é a origem, então u é um Destino 'B' válido
            if (u != i) {
                int id_truck = 1 + (paths[u] % M);

                // se o camiao interessa, guardamos o par
                if (m1 <= id_truck && id_truck <= m2) {
                    deliveries[id_truck].push_back({i, u});
                }

            }
            for (int v : adj[u]) {
                paths[v] = (paths[v] + paths[u]) % M;
            }
         }
    }

    for (int k = m1; k <= m2; k++) {
        if (deliveries[k].empty()) continue; // só por segurança

        // imprimir a cena do camiao, tipo C1, C2
        cout << "C" << k;

        // o sort ja ordena pela ordem lexicografica 
        sort(deliveries[k].begin(), deliveries[k].end());

        for (const auto& par : deliveries[k]) {
            cout << " " << par.first << "," << par.second;
        }
        
        cout << "\n";
    }

}