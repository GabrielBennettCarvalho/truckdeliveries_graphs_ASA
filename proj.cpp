#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// Usamos 'unsigned long long' para garantir que temos 64 bits de capacidade.
// Isto previne overflow antes de aplicarmos a lógica do módulo.
using ull = unsigned long long;

ull M; // Número total de camiões (pode ser grande)
int N, m1, m2, K;
vector<vector<int>> adj; // Lista de adjacências do grafo
vector<int> in_degree;   // Grau de entrada de cada nó (para o Topological Sort)

int main() {
    // ---------------------------------------------------------
    // 1. Otimização de I/O (Input/Output)
    // Desliga a sincronização com stdio do C para ser mais rápido
    // ---------------------------------------------------------
    std::ios::sync_with_stdio(0); 
    std::cin.tie(0);

    // Leitura dos dados iniciais
    if (!(cin >> N >> M >> m1 >> m2 >> K)) return 0;

    // Redimensionar estruturas (N+1 porque os cruzamentos são base-1)
    adj.resize(N + 1); 
    in_degree.assign(N + 1, 0); 

    // Leitura das estradas (arestas)
    for (int i = 0; i < K; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v); 
        in_degree[v]++; // Conta quantas estradas chegam a 'v'
    }

    // ---------------------------------------------------------
    // 2. Topological Sort (Algoritmo de Kahn)
    // Necessário porque o grafo é um DAG (Directed Acyclic Graph).
    // Permite calcular caminhos processando nós numa ordem linear.
    // ---------------------------------------------------------
    queue<int> q;
    vector<int> topological_order;
    topological_order.reserve(N); // Reserva memória para evitar re-alocações

    // Coloca na fila todos os nós que não têm dependências (grau de entrada 0)
    for (int i = 1; i <= N; i++) {
        if (in_degree[i] == 0)
        q.push(i);
    }

    // Processa a fila
    while (!q.empty()) {
        int u = q.front(); 
        q.pop();           

        topological_order.push_back(u); // Guarda a ordem correta

        // Remove as arestas que saem de 'u'
        for (int v : adj[u]) {
            in_degree[v]--; 
            if (in_degree[v] == 0) { // Se 'v' ficou livre, adiciona à fila
                q.push(v); 
            }
        }
    }

    // Criação de um mapa reverso: 
    // Dado um nó 'X', em que posição do vetor topológico ele está?
    // Isto permite saltar iterações desnecessárias no DP.
    int topo_size = topological_order.size();
    vector<int> position_at_topo(N + 1, -1);
    for (int k = 0; k < topo_size; k++) {
        position_at_topo[topological_order[k]] = k;
    }

    // ---------------------------------------------------------
    // 3. Otimização de Memória (O "Offset")
    // Em vez de criar um vetor gigante de tamanho M (que daria erro de memória se M for 10^9),
    // criamos apenas o tamanho necessário para o intervalo [m1, m2].
    // ---------------------------------------------------------
    int range_size = m2 - m1 + 1;
    
    // deliveries[0] vai guardar dados do camião m1
    // deliveries[1] vai guardar dados do camião m1+1, etc.
    vector<vector<pair<int, int>>> deliveries(range_size);
    
    // Vetor para contar caminhos (reutilizado para cada nó de origem)
    vector<ull> paths(N + 1, 0);

    // ---------------------------------------------------------
    // 4. Loop Principal (Programação Dinâmica)
    // Para cada nó 'i', calculamos os caminhos para todos os nós seguintes
    // ---------------------------------------------------------
    for (int i = 1; i <= N; i++) {

        paths[i] = 1; // Existe 1 caminho de 'i' para ele próprio (base)

        // Se o nó não faz parte da ordem topológica (ex: isolado ou erro), ignora
        if (position_at_topo[i] == -1) {
            paths[i] = 0;
            continue;
        }

        // Otimização: Começa a processar apenas a partir da posição de 'i' na ordem topológica
        int start_index = position_at_topo[i]; 

        // Itera sobre todos os nós 'u' alcançáveis a partir de 'i'
        for (int k = start_index; k < topo_size; k++) { 
            int u = topological_order[k];      

            ull flow = paths[u]; // Quantidade de caminhos acumulados até 'u'

            // Se não há caminhos até aqui, não propaga nada (O '0' real)
            if (flow == 0) continue;

            // Se 'u' não é o próprio início, temos um par válido (i -> u)
            if (u != i) {
                // FÓRMULA: 1 + caminhos % M
                int id_truck = 1 + (flow % M);

                // Guarda o par APENAS se o camião estiver no intervalo pedido [m1, m2]
                if (m1 <= id_truck && id_truck <= m2) {
                    // Usa o offset (-m1) para encontrar a gaveta correta no vetor pequeno
                    deliveries[id_truck - m1].push_back({i, u});
                }
            }

            // Propaga o fluxo para os vizinhos de 'u'
            for (int v : adj[u]) {
                
                // ---------------------------------------------------------
                // 5. Aritmética Segura (Correção do Bug do Zero)
                // Queremos manter os valores no intervalo [1, M] (exceto o 0 real).
                // Se usássemos (soma % M), quando a soma fosse M, virava 0.
                // E 0 seria interpretado como "sem caminho", cortando a rota erradamente.
                // ---------------------------------------------------------
                
                // Verifica overflow antes de somar: (paths[v] + flow > M) ?
                if (flow > M - paths[v]) {
                    // Se a soma ultrapassa M, subtraímos M para manter o "módulo"
                    // mas mantendo o valor M se for múltiplo exato.
                    paths[v] = flow - (M - paths[v]);
                } else {
                    // Soma segura normal
                    paths[v] += flow;
                }
            }

            // Reset do nó atual para não sujar o array para a próxima iteração do loop 'i'
            paths[u] = 0;
         }
    }

    // ---------------------------------------------------------
    // 6. Output Final
    // Imprime apenas os camiões no intervalo m1 a m2
    // ---------------------------------------------------------
    for (int k = m1; k <= m2; k++) {

        cout << "C" << k;

        // Calcula o índice correto usando o mesmo offset (-m1)
        int idx = k - m1;

        // Ordena os pares lexicograficamente (pedido no enunciado)
        sort(deliveries[idx].begin(), deliveries[idx].end());

        // Imprime pares separados por espaço
        for (const auto& par : deliveries[idx]) {
            cout << " " << par.first << "," << par.second;
        }
        
        cout << "\n";
    }

    return 0;
}