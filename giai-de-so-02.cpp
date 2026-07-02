#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct ServerNode {
    string code;
    string role;
    vector<ServerNode*> children;
};

using AdjacencyMatrix = vector<vector<int>>;

// Tìm máy chủ có mã cho trước trong cây tổng quát bằng DFS.
ServerNode* findServer(ServerNode* root, const string& code) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->code == code) {
        return root;
    }

    for (ServerNode* child : root->children) {
        ServerNode* result = findServer(child, code);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

// Thêm máy chủ mới làm cấp dưới trực tiếp của máy chủ quản lý đã biết.
bool addServer(ServerNode* root,
               const string& managerCode,
               const string& newCode,
               const string& role) {
    if (findServer(root, newCode) != nullptr) {
        return false;
    }

    ServerNode* manager = findServer(root, managerCode);
    if (manager == nullptr) {
        return false;
    }

    manager->children.push_back(new ServerNode{newCode, role, {}});
    return true;
}

// Liệt kê tất cả máy chủ có vai trò không phải là máy chủ điều phối.
void listNonCoordinatorServers(const ServerNode* root) {
    if (root == nullptr) {
        return;
    }

    if (root->role != "May chu dieu phoi") {
        cout << root->code << " - " << root->role << '\n';
    }

    for (const ServerNode* child : root->children) {
        listNonCoordinatorServers(child);
    }
}

// In thông tin máy chủ nếu mã cần tìm tồn tại trong hệ thống.
void printServerInformation(ServerNode* root, const string& code) {
    ServerNode* server = findServer(root, code);

    if (server == nullptr) {
        cout << "Khong tim thay may chu co ma " << code << ".\n";
        return;
    }

    cout << "Ma may chu: " << server->code << '\n';
    cout << "Vai tro: " << server->role << '\n';
}

// Giải phóng toàn bộ bộ nhớ động đã cấp phát cho cây máy chủ.
void freeServerTree(ServerNode*& root) {
    if (root == nullptr) {
        return;
    }

    for (ServerNode*& child : root->children) {
        freeServerTree(child);
    }

    delete root;
    root = nullptr;
}

// In số lượng và danh sách các ga không kết nối trực tiếp với ga k.
void printNonDirectStations(const AdjacencyMatrix& graph, int k) {
    int n = static_cast<int>(graph.size());

    if (k < 0 || k >= n) {
        cout << "Ga khong hop le.\n";
        return;
    }

    vector<int> stations;
    for (int i = 0; i < n; ++i) {
        if (i != k && graph[k][i] == 0) {
            stations.push_back(i);
        }
    }

    cout << "Ga " << k << " khong ket noi truc tiep voi "
         << stations.size() << " ga: ";

    for (int station : stations) {
        cout << station << ' ';
    }
    cout << '\n';
}

// Tìm một đường đi ngắn nhất giữa hai ga bằng BFS trên ma trận kề.
vector<int> shortestPath(const AdjacencyMatrix& graph,
                         int start,
                         int target) {
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

        for (int v = 0; v < n; ++v) {
            if (graph[u][v] != 0 && !visited[v]) {
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

// In kiểu kết nối, đường đi và các ga trung gian giữa hai ga.
void printConnection(const AdjacencyMatrix& graph,
                     int start,
                     int target) {
    vector<int> path = shortestPath(graph, start, target);

    if (path.empty()) {
        cout << "Khong co duong di tu ga "
             << start << " den ga " << target << ".\n";
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

// Duyệt DFS để thu thập các ga thuộc cùng một thành phần liên thông.
void collectComponent(const AdjacencyMatrix& graph,
                      int u,
                      vector<bool>& visited,
                      vector<int>& component) {
    visited[u] = true;
    component.push_back(u);

    int n = static_cast<int>(graph.size());
    for (int v = 0; v < n; ++v) {
        if (graph[u][v] != 0 && !visited[v]) {
            collectComponent(graph, v, visited, component);
        }
    }
}

// Tìm tất cả các thành phần liên thông của đồ thị.
vector<vector<int>> connectedComponents(const AdjacencyMatrix& graph) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<vector<int>> components;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            vector<int> component;
            collectComponent(graph, i, visited, component);
            sort(component.begin(), component.end());
            components.push_back(component);
        }
    }

    return components;
}

// In số lượng thành phần liên thông và các ga trong từng thành phần.
void printComponents(const AdjacencyMatrix& graph) {
    vector<vector<int>> components = connectedComponents(graph);

    cout << "So tuyen hoat dong doc lap: " << components.size() << '\n';

    for (size_t i = 0; i < components.size(); ++i) {
        cout << "Tuyen " << i + 1 << ": ";
        for (int station : components[i]) {
            cout << station << ' ';
        }
        cout << '\n';
    }
}

// Tạo dữ liệu mẫu và thực hiện toàn bộ yêu cầu của đề số 02.
int main() {
    cout << "===== CAU 1: CAY TONG QUAT MAY CHU =====\n";

    ServerNode* root = new ServerNode{"SV01", "May chu trung tam", {}};

    addServer(root, "SV01", "SV02", "May chu dieu phoi");
    addServer(root, "SV01", "SV03", "May chu dieu phoi");
    addServer(root, "SV02", "SV04", "May chu xu ly");
    addServer(root, "SV03", "SV05", "May chu xu ly");

    cout << "Cac may chu khong phai may chu dieu phoi:\n";
    listNonCoordinatorServers(root);

    string code;
    cout << "Nhap ma may chu can tim: ";
    cin >> code;
    printServerInformation(root, code);

    cout << "\n===== CAU 2: DO THI TAU DIEN NGAM =====\n";

    AdjacencyMatrix graph = {
        {0, 1, 1, 0, 0, 1, 0},
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 1},
        {0, 1, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 0, 1, 0, 0}
    };

    printNonDirectStations(graph, 1);
    printConnection(graph, 1, 6);
    printComponents(graph);

    freeServerTree(root);
    return 0;
}
