#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Planet {
    string name;
    double distance;
};

struct PlanetNode {
    Planet data;
    PlanetNode* left;
    PlanetNode* right;
};

using Graph = vector<vector<int>>;

// Thêm một hành tinh vào BST với khóa là khoảng cách đến Mặt Trời.
void insertPlanet(PlanetNode*& root, const string& name, double distance) {
    if (root == nullptr) {
        root = new PlanetNode{{name, distance}, nullptr, nullptr};
        return;
    }

    if (distance < root->data.distance) {
        insertPlanet(root->left, name, distance);
    } else {
        insertPlanet(root->right, name, distance);
    }
}

// Liệt kê các hành tinh cách Mặt Trời nhỏ hơn 100 hoặc lớn hơn 500 triệu km.
void listPlanetsByDistance(const PlanetNode* root) {
    if (root == nullptr) {
        return;
    }

    listPlanetsByDistance(root->left);

    if (root->data.distance < 100 || root->data.distance > 500) {
        cout << root->data.name << " - "
             << root->data.distance << " trieu km\n";
    }

    listPlanetsByDistance(root->right);
}

// Kiểm tra một hành tinh theo tên bằng cách duyệt toàn bộ cây.
bool containsPlanetName(const PlanetNode* root, const string& name) {
    if (root == nullptr) {
        return false;
    }

    if (root->data.name == name) {
        return true;
    }

    return containsPlanetName(root->left, name) ||
           containsPlanetName(root->right, name);
}

// Giải phóng toàn bộ bộ nhớ động đã cấp phát cho BST.
void freeTree(PlanetNode*& root) {
    if (root == nullptr) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    delete root;
    root = nullptr;
}

// Thêm một cạnh vô hướng giữa hai ga vào danh sách kề.
void addUndirectedEdge(Graph& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// In số lượng và danh sách các ga kết nối trực tiếp với ga k.
void printDirectConnections(const Graph& graph, int k) {
    if (k < 0 || k >= static_cast<int>(graph.size())) {
        cout << "Ga khong hop le\n";
        return;
    }

    cout << "Ga " << k << " ket noi truc tiep voi "
         << graph[k].size() << " ga: ";

    for (int neighbor : graph[k]) {
        cout << neighbor << ' ';
    }
    cout << '\n';
}

// Tìm một đường đi ngắn nhất giữa hai ga bằng thuật toán BFS.
vector<int> shortestPath(const Graph& graph, int start, int target) {
    int n = static_cast<int>(graph.size());

    if (start < 0 || start >= n || target < 0 || target >= n) {
        return {};
    }

    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == target) {
            break;
        }

        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    if (!visited[target]) {
        return {};
    }

    vector<int> path;
    for (int current = target; current != -1; current = parent[current]) {
        path.push_back(current);
    }

    reverse(path.begin(), path.end());
    return path;
}

// In đường đi ngắn nhất và các ga trung gian giữa hai ga.
void printConnection(const Graph& graph, int start, int target) {
    vector<int> path = shortestPath(graph, start, target);

    if (path.empty()) {
        cout << "Khong co duong di tu ga "
             << start << " den ga " << target << "\n";
        return;
    }

    cout << "Duong di ngan nhat: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            cout << " -> ";
        }
        cout << path[i];
    }
    cout << '\n';

    if (path.size() == 1) {
        cout << "Hai ga la cung mot ga; khong co ga trung gian.\n";
    } else if (path.size() == 2) {
        cout << "Hai ga ket noi truc tiep; khong co ga trung gian.\n";
    } else {
        cout << "So ga trung gian it nhat: " << path.size() - 2 << '\n';
        cout << "Cac ga trung gian: ";

        for (size_t i = 1; i + 1 < path.size(); ++i) {
            cout << path[i] << ' ';
        }
        cout << '\n';
    }
}

// Duyệt DFS để thu thập tất cả ga thuộc cùng một thành phần liên thông.
void collectComponent(const Graph& graph,
                      int u,
                      vector<bool>& visited,
                      vector<int>& component) {
    visited[u] = true;
    component.push_back(u);

    for (int v : graph[u]) {
        if (!visited[v]) {
            collectComponent(graph, v, visited, component);
        }
    }
}

// Tìm tất cả các thành phần liên thông của đồ thị.
vector<vector<int>> connectedComponents(const Graph& graph) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<vector<int>> components;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            vector<int> component;
            collectComponent(graph, i, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

// In số lượng thành phần liên thông và các ga trong từng thành phần.
void printComponents(const Graph& graph) {
    vector<vector<int>> components = connectedComponents(graph);

    cout << "So cum ket noi: " << components.size() << '\n';

    for (size_t i = 0; i < components.size(); ++i) {
        cout << "Cum " << i + 1 << ": ";
        for (int station : components[i]) {
            cout << station << ' ';
        }
        cout << '\n';
    }
}

// Tạo dữ liệu mẫu và thực hiện toàn bộ yêu cầu của đề bài.
int main() {
    cout << "===== CAU 1: CAY TIM KIEM NHI PHAN =====\n";

    PlanetNode* root = nullptr;

    insertPlanet(root, "Sao Thuy", 57);
    insertPlanet(root, "Sao Kim", 108);
    insertPlanet(root, "Trai Dat", 149);
    insertPlanet(root, "Sao Hoa", 227);
    insertPlanet(root, "Sao Moc", 778);

    cout << "Cac hanh tinh co khoang cach < 100 hoac > 500:\n";
    listPlanetsByDistance(root);

    cout << "Sao Tho "
         << (containsPlanetName(root, "Sao Tho")
                 ? "co trong cay\n"
                 : "khong co trong cay\n");

    cout << "\n===== CAU 2: DO THI GA TAU =====\n";

    Graph graph(7);

    addUndirectedEdge(graph, 0, 1);
    addUndirectedEdge(graph, 0, 4);
    addUndirectedEdge(graph, 1, 2);
    addUndirectedEdge(graph, 1, 5);
    addUndirectedEdge(graph, 2, 3);
    addUndirectedEdge(graph, 2, 6);
    addUndirectedEdge(graph, 3, 4);
    addUndirectedEdge(graph, 4, 5);
    addUndirectedEdge(graph, 5, 6);

    printDirectConnections(graph, 2);
    printConnection(graph, 5, 2);
    printComponents(graph);

    freeTree(root);
    return 0;
}
