#include"./graph_generator.h"

using namespace std;

int main() {
    int n = 10;
    auto graph = new Network(n, 4, 2);
    graph->print_adjacency_list();
    auto v = graph->shortest_path(0, 6);
    for (auto i : v) {
        cout << i << " " << endl;
    }
}