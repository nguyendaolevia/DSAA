#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Device {
    string name;
    double price;
};

struct DeviceNode {
    Device data;
    DeviceNode* left;
    DeviceNode* right;
};

using Graph = vector<vector<int>>;

// Thêm một thiết bị vào BST với khóa là giá bán.
void insertDevice(DeviceNode*& root, const string& name, double price) {
    if (root == nullptr) {
        root = new DeviceNode{{name, price}, nullptr, nullptr};
        return;
    }

    if (price < root->data.price) {
        insertDevice(root->left, name, price);
    } else {
        insertDevice(root->right, name, price);
    }
}

// Liệt kê các thiết bị có giá bán từ 5 đến 20 triệu đồng.
void listDevicesInPriceRange(const DeviceNode* root) {
    if (root == nullptr) {
        return;
    }

    listDevicesInPriceRange(root->left);

    if (root->data.price >= 5 && root->data.price <= 20) {
        cout << root->data.name << " - "
             << root->data.price << " trieu dong\n";
    }

    listDevicesInPriceRange(root->right);
}

// Tìm thiết bị theo tên bằng cách duyệt toàn bộ cây.
DeviceNode* findDeviceByName(DeviceNode* root, const string& name) {
    if (root == nullptr) {
        return nullptr;
    }

    if (root->data.name == name) {
        return root;
    }

    DeviceNode* result = findDeviceByName(root->left, name);
    if (result != nullptr) {
        return result;
    }

    return findDeviceByName(root->right, name);
}

// In thông tin thiết bị nếu tên cần tìm tồn tại trong cây.
void printDeviceInformation(DeviceNode* root, const string& name) {
    DeviceNode* device = findDeviceByName(root, name);

    if (device == nullptr) {
        cout << "Khong tim thay thiet bi " << name << ".\n";
        return;
    }

    cout << "Ten thiet bi: " << device->data.name << '\n';
    cout << "Gia ban: " << device->data.price << " trieu dong\n";
}

// Giải phóng toàn bộ bộ nhớ động đã cấp phát cho BST.
void freeDeviceTree(DeviceNode*& root) {
    if (root == nullptr) {
        return;
    }

    freeDeviceTree(root->left);
    freeDeviceTree(root->right);
    delete root;
    root = nullptr;
}

// Thêm một cạnh vô hướng giữa hai kho bãi vào danh sách kề.
void addUndirectedEdge(Graph& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

// In số lượng và danh sách các kho kết nối trực tiếp với kho k.
void printDirectConnections(const Graph& graph, int k) {
    if (k < 0 || k >= static_cast<int>(graph.size())) {
        cout << "Kho bai khong hop le.\n";
        return;
    }

    cout << "Kho bai " << k << " ket noi truc tiep voi "
         << graph[k].size() << " kho: ";

    for (int neighbor : graph[k]) {
        cout << neighbor << ' ';
    }
    cout << '\n';
}

// Tìm một đường vận chuyển ngắn nhất giữa hai kho bằng BFS.
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

// In đường đi ngắn nhất và các kho trung gian giữa hai kho.
void printConnection(const Graph& graph, int start, int target) {
    vector<int> path = shortestPath(graph, start, target);

    if (path.empty()) {
        cout << "Khong co duong van chuyen tu kho "
             << start << " den kho " << target << ".\n";
        return;
    }

    cout << "Duong van chuyen ngan nhat: ";
    for (size_t i = 0; i < path.size(); ++i) {
        if (i > 0) {
            cout << " -> ";
        }
        cout << path[i];
    }
    cout << '\n';

    if (path.size() == 1) {
        cout << "Hai kho la cung mot kho; khong co kho trung gian.\n";
    } else if (path.size() == 2) {
        cout << "Hai kho ket noi truc tiep; khong co kho trung gian.\n";
    } else {
        cout << "So kho trung gian it nhat: " << path.size() - 2 << '\n';
        cout << "Cac kho trung gian: ";

        for (size_t i = 1; i + 1 < path.size(); ++i) {
            cout << path[i] << ' ';
        }
        cout << '\n';
    }
}

// Duyệt DFS để thu thập các kho thuộc cùng một thành phần liên thông.
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

// Tìm tất cả các thành phần liên thông của hệ thống logistics.
vector<vector<int>> connectedComponents(const Graph& graph) {
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

// In số lượng cụm kho bãi độc lập và các kho trong từng cụm.
void printComponents(const Graph& graph) {
    vector<vector<int>> components = connectedComponents(graph);

    cout << "So cum kho bai doc lap: " << components.size() << '\n';

    for (size_t i = 0; i < components.size(); ++i) {
        cout << "Cum " << i + 1 << ": ";
        for (int warehouse : components[i]) {
            cout << warehouse << ' ';
        }
        cout << '\n';
    }
}

// Tạo dữ liệu mẫu và thực hiện toàn bộ yêu cầu của đề số 01.
int main() {
    cout << "===== CAU 1: CAY TIM KIEM NHI PHAN =====\n";

    DeviceNode* root = nullptr;

    insertDevice(root, "Laptop Dell", 22);
    insertDevice(root, "May in HP", 6);
    insertDevice(root, "Router TP-Link", 3);
    insertDevice(root, "Man hinh Samsung", 7);
    insertDevice(root, "May chieu Epson", 25);

    cout << "Cac thiet bi co gia tu 5 den 20 trieu dong:\n";
    listDevicesInPriceRange(root);

    string deviceName;
    cout << "Nhap ten thiet bi can tim: ";
    getline(cin, deviceName);
    printDeviceInformation(root, deviceName);

    cout << "\n===== CAU 2: DO THI KHO BAI =====\n";

    Graph graph(7);

    addUndirectedEdge(graph, 0, 1);
    addUndirectedEdge(graph, 0, 2);
    addUndirectedEdge(graph, 1, 3);
    addUndirectedEdge(graph, 1, 4);
    addUndirectedEdge(graph, 2, 5);
    addUndirectedEdge(graph, 3, 6);
    addUndirectedEdge(graph, 4, 5);
    addUndirectedEdge(graph, 5, 6);

    printDirectConnections(graph, 5);
    printConnection(graph, 0, 6);
    printComponents(graph);

    freeDeviceTree(root);
    return 0;
}
