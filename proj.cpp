#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
int N, M, m1, m2, K;
vector<vector<int>> adj; // adjacency list
vector<int> in_degree;   // qntas setas apontam para aquele cruzamento

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
    
}