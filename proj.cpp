#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
using ull = unsigned long long;

ull M;
int N, m1, m2, K;
vector<vector<int>> adj; 
vector<int> in_degree;   



int main() {
    std::ios::sync_with_stdio(0); 
    std::cin.tie(0);

    if (!(cin >> N >> M >> m1 >> m2 >> K)) return 0;

    adj.resize(N + 1); 
    in_degree.assign(N + 1, 0); 

    for (int i = 0; i < K; i++) {
        int u, v;
        cin >> u >> v;

        adj[u].push_back(v); 
        in_degree[v]++; 
    }


    queue<int> q;
    vector<int> topological_order;
    topological_order.reserve(N);

    for (int i = 1; i <= N; i++) {
        if (in_degree[i] == 0)
        q.push(i);
    }

    while (!q.empty()) {
        int u = q.front(); 
        q.pop();           

        topological_order.push_back(u); 

        for (int v : adj[u]) {
            in_degree[v]--; 
            if (in_degree[v] == 0) {
                q.push(v); 
            }
        }
    }

    int topo_size = topological_order.size();

    vector<int> position_at_topo(N + 1, -1);
    for (int k = 0; k < topo_size; k++) {
        position_at_topo[topological_order[k]] = k;
    }


    int range_size = m2 - m1 + 1;
    
    vector<vector<pair<int, int>>> deliveries(range_size);
    vector<ull> paths(N + 1, 0);


  
    for (int i = 1; i <= N; i++) {

        paths[i] = 1;

        if (position_at_topo[i] == -1) {
            paths[i] = 0;
            continue;
        }

        int start_index = position_at_topo[i]; 

        for (int k = start_index; k < topo_size; k++) { 
            int u = topological_order[k];      

            ull flow = paths[u];

            if (flow == 0) continue;

            if (u != i) {
                int id_truck = 1 + (flow % M);

             
                if (m1 <= id_truck && id_truck <= m2) {
                    deliveries[id_truck].push_back({i, u});
                }

            }

            for (int v : adj[u]) {
                if (flow > M - paths[v]) {
                    paths[v] = flow - (M - paths[v]);
                } else {
                    paths[v] += flow;
                }
            }

            paths[u] = 0;
         }
    }

    for (int k = m1; k <= m2; k++) {


        cout << "C" << k;

        int idx = k - m1;

        sort(deliveries[idx].begin(), deliveries[idx].end());

        for (const auto& par : deliveries[idx]) {
            cout << " " << par.first << "," << par.second;
        }
        
        cout << "\n";
    }

}