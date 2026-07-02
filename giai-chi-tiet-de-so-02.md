# Giải chi tiết đề thi Cấu trúc dữ liệu – Đề số 02

> Năm học 2025–2026, thời gian 90 phút, không được tham khảo tài liệu.
> Lời giải dùng C++17. Mã nguồn hoàn chỉnh: [`giai-de-so-02.cpp`](./giai-de-so-02.cpp).

## 1. Cấu trúc đề

- Câu 1 – 5 điểm: cây tổng quát quản lý hệ thống máy chủ.
- Câu 2 – 5 điểm: đồ thị vô hướng lưu bằng ma trận kề.
- Mỗi ý a–e được 1 điểm, vì vậy nên tách rõ từng hàm để có thể lấy điểm từng phần.

Hai kiến thức trọng tâm:

1. Cây tổng quát: một nút có thể có nhiều nút con.
2. Đồ thị không trọng số: BFS tìm đường ít cạnh nhất, DFS tìm thành phần liên thông.

---

# Phần I – Cây tổng quát quản lý máy chủ

## 2. Tóm tắt Câu 1

Mỗi máy chủ gồm:

- Mã máy chủ.
- Vai trò.
- Danh sách các máy chủ con do nó quản lý trực tiếp.

Đề yêu cầu:

1. Khai báo cây tổng quát.
2. Thêm máy chủ mới khi biết mã máy chủ quản lý trực tiếp.
3. Liệt kê máy chủ có vai trò khác `Máy chủ điều phối`.
4. Tìm máy chủ theo mã; nếu có, in mã và vai trò.
5. Tạo cây mẫu và chạy các chức năng trên.

## 3. Vì sao đây không phải cây nhị phân?

Cây nhị phân giới hạn mỗi nút có tối đa hai con. Đề nói một máy chủ có thể quản lý **nhiều** máy chủ con, nên phải dùng cây tổng quát.

Cây mẫu:

```text
SV01 – Máy chủ trung tâm
├── SV02 – Máy chủ điều phối
│   └── SV04 – Máy chủ xử lý
└── SV03 – Máy chủ điều phối
    └── SV05 – Máy chủ xử lý
```

Một cách biểu diễn tự nhiên trong C++ là cho mỗi nút chứa `vector` các con trỏ tới nút con.

## 4. Câu 1a – Khai báo nút cây tổng quát

```cpp
struct ServerNode {
    string code;
    string role;
    vector<ServerNode*> children;
};
```

Giải thích:

- `code`: mã duy nhất của máy chủ, ví dụ `SV01`.
- `role`: vai trò của máy chủ.
- `children`: các máy chủ do nút hiện tại quản lý trực tiếp.
- Gốc cây là máy chủ trung tâm `SV01`.

Khác với BST, cây này không có quy tắc “nhỏ sang trái, lớn sang phải”. Vì vậy muốn tìm một mã máy chủ, trong trường hợp tổng quát phải duyệt cây.

## 5. Hàm tìm máy chủ theo mã

Hàm tìm được viết trước vì hàm thêm máy chủ cũng cần tìm nút quản lý.

```cpp
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
```

Đây là DFS trên cây:

1. Kiểm tra nút hiện tại.
2. Nếu chưa đúng, lần lượt tìm trong từng cây con.
3. Gặp kết quả thì trả về ngay.
4. Duyệt hết mà không thấy thì trả về `nullptr`.

Độ phức tạp xấu nhất là `O(n)`, vì mã cần tìm có thể nằm ở cuối hoặc không tồn tại.

## 6. Câu 1b – Thêm máy chủ mới

Muốn thêm máy chủ, trước hết phải tìm máy chủ quản lý trực tiếp:

```cpp
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
```

Hàm trả về `false` khi:

- Mã máy chủ mới đã tồn tại.
- Không tìm thấy máy chủ quản lý.

Nếu tìm thấy máy chủ quản lý, nút mới được thêm vào `children` của nó. `push_back` thể hiện đúng quan hệ cấp dưới **trực tiếp**.

Điểm dễ sai: không được thêm nút mới vào `root->children` trong mọi trường hợp, vì SV04 phải là con của SV02 chứ không phải con của SV01.

## 7. Câu 1c – Liệt kê máy chủ không phải điều phối

```cpp
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
```

Hàm duyệt tiền tự:

```text
nút hiện tại → lần lượt các cây con
```

Với cây mẫu:

| Máy chủ | Vai trò | Có được in? |
|---|---|---|
| SV01 | Máy chủ trung tâm | Có |
| SV02 | Máy chủ điều phối | Không |
| SV03 | Máy chủ điều phối | Không |
| SV04 | Máy chủ xử lý | Có |
| SV05 | Máy chủ xử lý | Có |

Kết quả là `SV01`, `SV04`, `SV05`.

## 8. Câu 1d – Kiểm tra và in thông tin

```cpp
void printServerInformation(ServerNode* root, const string& code) {
    ServerNode* server = findServer(root, code);

    if (server == nullptr) {
        cout << "Khong tim thay may chu co ma " << code << ".\n";
        return;
    }

    cout << "Ma may chu: " << server->code << '\n';
    cout << "Vai tro: " << server->role << '\n';
}
```

Không nên gọi `findServer` hai lần: tìm một lần, lưu con trỏ kết quả rồi dùng lại.

## 9. Câu 1e – Tạo dữ liệu mẫu

```cpp
ServerNode* root = new ServerNode{"SV01", "May chu trung tam", {}};

addServer(root, "SV01", "SV02", "May chu dieu phoi");
addServer(root, "SV01", "SV03", "May chu dieu phoi");
addServer(root, "SV02", "SV04", "May chu xu ly");
addServer(root, "SV03", "SV05", "May chu xu ly");
```

Giả định được dùng ở đây: cả SV02 và SV03 đều có vai trò máy chủ điều phối; cách trình bày của đề gom hai máy chủ trước cùng một mô tả vai trò.

## 10. Giải phóng cây tổng quát

```cpp
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
```

Phải xóa toàn bộ cây con trước khi xóa nút cha. Nếu xóa cha trước, ta mất các con trỏ dẫn tới cây con.

---

# Phần II – Đồ thị tàu điện ngầm bằng ma trận kề

## 11. Tóm tắt Câu 2

- Có `n` ga, đánh số `0` đến `n - 1`.
- Tuyến tàu chạy hai chiều nên đồ thị vô hướng.
- Không có trọng số khoảng cách hay thời gian.
- Đồ thị được lưu bằng ma trận kề.

Ma trận mẫu:

```text
    0 1 2 3 4 5 6
0 [ 0 1 1 0 0 1 0 ]
1 [ 1 0 0 1 0 0 1 ]
2 [ 1 0 0 0 1 0 1 ]
3 [ 0 1 0 0 0 1 0 ]
4 [ 0 0 1 0 0 0 1 ]
5 [ 1 0 0 1 0 0 0 ]
6 [ 0 1 1 0 1 0 0 ]
```

## 12. Câu 2a – Khai báo ma trận kề

```cpp
using AdjacencyMatrix = vector<vector<int>>;
```

Ý nghĩa:

```text
graph[u][v] = 1  → u nối trực tiếp với v
graph[u][v] = 0  → u không nối trực tiếp với v
```

Vì đồ thị vô hướng:

```text
graph[u][v] == graph[v][u]
```

Đường chéo chính bằng 0 vì một ga không được xem là nối trực tiếp với chính nó.

Ma trận dùng `O(V²)` bộ nhớ. Đây là khác biệt quan trọng so với danh sách kề có bộ nhớ `O(V + E)`.

## 13. Câu 2b – Các ga không nối trực tiếp với k

Đề hỏi **không kết nối trực tiếp**, đồng thời không tính chính ga `k`.

```cpp
for (int i = 0; i < n; ++i) {
    if (i != k && graph[k][i] == 0) {
        stations.push_back(i);
    }
}
```

Hàng của ga 1:

```text
[1, 0, 0, 1, 0, 0, 1]
```

- Nối trực tiếp với 0, 3, 6.
- Không nối trực tiếp với 2, 4, 5.
- Không tính ga 1 dù `graph[1][1] == 0`.

Kết quả: ga 1 không nối trực tiếp với **3 ga: 2, 4, 5**.

## 14. Câu 2c – Tìm ít ga trung gian nhất

Đồ thị không trọng số nên dùng BFS. BFS duyệt theo số cạnh tăng dần, do đó lần đầu tới ga đích là một đường có ít cạnh nhất.

```cpp
vector<int> shortestPath(const AdjacencyMatrix& graph,
                         int start,
                         int target) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

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
```

Mảng `parent` dùng để lần ngược từ đích về nguồn. Sau khi đảo lại, ta nhận đường theo đúng chiều.

Nếu đường có `k` đỉnh:

```text
số cạnh = k - 1
số ga trung gian = k - 2
```

Với ga 1 và ga 6:

```text
graph[1][6] = 1
```

Hai ga nối trực tiếp:

```text
1 → 6
```

Số ga trung gian bằng 0.

### Độ phức tạp BFS với ma trận kề

Mỗi khi lấy một đỉnh ra khỏi hàng đợi, thuật toán phải quét toàn bộ một hàng gồm `V` ô. Do đó:

- Thời gian: `O(V²)`.
- Bộ nhớ phụ: `O(V)`.

Không nên ghi `O(V + E)` như khi dùng danh sách kề; cách lưu đồ thị làm thay đổi chi phí duyệt hàng xóm.

## 15. Câu 2d – Thành phần liên thông

Mỗi lần bắt đầu DFS từ một đỉnh chưa thăm, ta phát hiện một thành phần liên thông mới.

```cpp
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
```

Vòng lặp ngoài:

```cpp
for (int i = 0; i < n; ++i) {
    if (!visited[i]) {
        vector<int> component;
        collectComponent(graph, i, visited, component);
        components.push_back(component);
    }
}
```

Đồ thị mẫu có một thành phần:

```text
{0, 1, 2, 3, 4, 5, 6}
```

Nói cách khác, toàn bộ hệ thống tàu điện ngầm là một tuyến/cụm hoạt động liên thông.

## 16. Kết quả chạy chương trình

Với mã tìm kiếm `SV04`:

```text
===== CAU 1: CAY TONG QUAT MAY CHU =====
Cac may chu khong phai may chu dieu phoi:
SV01 - May chu trung tam
SV04 - May chu xu ly
SV05 - May chu xu ly
Nhap ma may chu can tim: SV04
Ma may chu: SV04
Vai tro: May chu xu ly

===== CAU 2: DO THI TAU DIEN NGAM =====
Ga 1 khong ket noi truc tiep voi 3 ga: 2 4 5
Duong di ngan nhat: 1 -> 6
Hai ga ket noi truc tiep; khong co ga trung gian.
So tuyen hoat dong doc lap: 1
Tuyen 1: 0 1 2 3 4 5 6
```

## 17. Độ phức tạp tổng hợp

| Hàm | Độ phức tạp |
|---|---:|
| Tìm máy chủ | `O(n)` |
| Thêm máy chủ | `O(n)` |
| Liệt kê theo vai trò | `O(n)` |
| BFS trên ma trận kề | `O(V²)` |
| DFS toàn đồ thị bằng ma trận | `O(V²)` |
| Bộ nhớ ma trận kề | `O(V²)` |

## 18. Những lỗi dễ mất điểm

1. Dùng cây nhị phân cho hệ thống máy chủ dù mỗi nút có thể có nhiều con.
2. Thêm mọi máy chủ vào gốc thay vì đúng máy chủ quản lý trực tiếp.
3. Không kiểm tra mã máy chủ mới bị trùng.
4. Chỉ duyệt một nhánh khi tìm mã trong cây tổng quát.
5. Ở câu 2b, đếm cả chính ga `k` là ga không kết nối.
6. Đọc sai yêu cầu “không kết nối trực tiếp” thành “kết nối trực tiếp”.
7. Dùng DFS rồi khẳng định đường tìm được là ngắn nhất.
8. Không lưu `parent`, nên không in được các ga trung gian.
9. Lấy `path.size() - 1` làm số ga trung gian; đó là số cạnh.
10. Ghi độ phức tạp BFS là `O(V + E)` dù đang duyệt ma trận kề.

## 19. Chiến thuật làm bài trong 90 phút

| Thời gian | Công việc |
|---:|---|
| 5 phút | Đọc đề, xác định cây tổng quát và ma trận kề |
| 30 phút | Làm Câu 1 và dựng đúng quan hệ cha–con |
| 40 phút | Làm Câu 2: hàng ma trận, BFS, DFS |
| 10 phút | Viết `main`, đối chiếu kết quả mẫu |
| 5 phút | Soát `nullptr`, `visited`, `parent`, loại trừ ga `k` |

## 20. Tóm tắt cần nhớ

```text
Cây tổng quát:
    mỗi nút có vector<ServerNode*> children
    tìm mã bằng DFS
    thêm nút vào children của đúng manager

Ma trận kề:
    graph[u][v] = 1 nếu có cạnh
    đồ thị vô hướng → ma trận đối xứng
    ga không nối k → graph[k][i] == 0 và i != k

Đường ngắn nhất:
    BFS + parent
    số ga trung gian = path.size() - 2

Thành phần liên thông:
    chạy DFS từ mỗi đỉnh chưa thăm
```
