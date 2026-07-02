# Giải chi tiết đề thi Cấu trúc dữ liệu – Đề số 01

> Năm học 2025–2026, thời gian 90 phút, không được tham khảo tài liệu.
> Lời giải dùng C++17. Mã nguồn hoàn chỉnh: [`giai-de-so-01.cpp`](./giai-de-so-01.cpp).

## 1. Cấu trúc đề

- Câu 1 – 5 điểm: cây tìm kiếm nhị phân quản lý thiết bị điện tử.
- Câu 2 – 5 điểm: đồ thị vô hướng lưu bằng danh sách kề.
- Mỗi ý a–e được 1 điểm.

Kiến thức cần dùng:

1. BST với khóa là giá bán.
2. Duyệt cây để lọc theo khoảng giá và tìm theo tên.
3. Danh sách kề cho đồ thị vô hướng.
4. BFS tìm đường ít kho trung gian nhất.
5. DFS tìm các thành phần liên thông.

---

# Phần I – BST quản lý thiết bị điện tử

## 2. Tóm tắt Câu 1

Mỗi thiết bị gồm:

- Tên thiết bị.
- Giá bán, đơn vị triệu đồng.
- Khóa dùng để tổ chức BST là **giá bán**.

Đề yêu cầu:

1. Khai báo BST.
2. Thêm thiết bị mới.
3. Liệt kê thiết bị có giá từ 5 đến 20 triệu đồng.
4. Tìm thiết bị theo tên; nếu có, in tên và giá.
5. Tạo cây mẫu và chạy các yêu cầu.

Dữ liệu mẫu:

| Thiết bị | Giá (triệu đồng) |
|---|---:|
| Laptop Dell | 22 |
| Máy in HP | 6 |
| Router TP-Link | 3 |
| Màn hình Samsung | 7 |
| Máy chiếu Epson | 25 |

## 3. Xác định đúng khóa BST

Đề nói rõ khóa là giá bán, nên quy tắc tổ chức cây là:

```text
giá mới < giá hiện tại  → sang trái
giá mới >= giá hiện tại → sang phải
```

Không được dùng tên thiết bị để quyết định hướng chèn.

Sau khi chèn dữ liệu theo thứ tự đề cho, cây có dạng:

```text
                 Laptop Dell (22)
                 /              \
         Máy in HP (6)       Máy chiếu Epson (25)
         /          \
Router TP-Link (3)  Màn hình Samsung (7)
```

Cây này không bị lệch hoàn toàn như dữ liệu đề số 04, nhưng cũng không phải cây tự cân bằng. BST thường không tự bảo đảm chiều cao `O(log n)`.

## 4. Câu 1a – Khai báo nút BST

```cpp
struct Device {
    string name;
    double price;
};

struct DeviceNode {
    Device data;
    DeviceNode* left;
    DeviceNode* right;
};
```

Giải thích:

- `Device` chứa dữ liệu nghiệp vụ.
- `DeviceNode` thêm hai liên kết cây con.
- `data.price` chính là khóa, không cần trường `key` riêng.
- `left` chứa các thiết bị giá thấp hơn.
- `right` chứa các thiết bị giá cao hơn hoặc bằng theo quy ước của lời giải.

## 5. Câu 1b – Thêm thiết bị

```cpp
void insertDevice(DeviceNode*& root,
                  const string& name,
                  double price) {
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
```

### Vì sao dùng `DeviceNode*&`?

Hàm cần thay đổi chính con trỏ đang rỗng. Khi tới vị trí chèn, lệnh:

```cpp
root = new DeviceNode{...};
```

phải cập nhật `root`, `root->left` hoặc `root->right` của cây bên ngoài. Tham chiếu tới con trỏ giúp việc gán có hiệu lực thật.

### Giá trùng nhau

Đề không nêu quy tắc cho hai thiết bị cùng giá. Lời giải quy ước khóa bằng nhau đi sang phải. Một cách khác là mỗi nút lưu danh sách thiết bị cùng giá. Cần có quy ước nhất quán, không được xử lý tùy ý.

## 6. Câu 1c – Liệt kê thiết bị giá từ 5 đến 20

Cụm “từ 5 đến 20” được hiểu là bao gồm hai đầu mút:

```text
5 <= price <= 20
```

Trong C++:

```cpp
price >= 5 && price <= 20
```

Hàm duyệt in-order:

```cpp
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
```

In-order có thứ tự:

```text
cây trái → nút hiện tại → cây phải
```

Vì đây là BST khóa theo giá, kết quả được in theo giá tăng dần.

Xét dữ liệu:

| Thiết bị | Giá | Kết quả |
|---|---:|---|
| Router TP-Link | 3 | Loại |
| Máy in HP | 6 | Chọn |
| Màn hình Samsung | 7 | Chọn |
| Laptop Dell | 22 | Loại |
| Máy chiếu Epson | 25 | Loại |

Kết quả: **Máy in HP và Màn hình Samsung**.

Hàm đơn giản trên duyệt cả cây nên có thời gian `O(n)`. Có thể cắt nhánh dựa trên khoảng giá, nhưng không cần thiết với đề nhỏ và dễ làm code phức tạp hơn trong phòng thi.

## 7. Câu 1d – Tìm theo tên thiết bị

Đây là điểm dễ sai nhất của Câu 1:

- Cây được sắp xếp theo **giá**.
- Đề yêu cầu tìm theo **tên**.

Tên không cho biết nên đi trái hay phải, nên trong trường hợp tổng quát phải duyệt cả cây.

```cpp
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
```

Độ phức tạp là `O(n)`, không phải `O(log n)`. BST chỉ hỗ trợ tìm nhanh theo khóa tổ chức cây.

Hàm in kết quả:

```cpp
void printDeviceInformation(DeviceNode* root, const string& name) {
    DeviceNode* device = findDeviceByName(root, name);

    if (device == nullptr) {
        cout << "Khong tim thay thiet bi " << name << ".\n";
        return;
    }

    cout << "Ten thiet bi: " << device->data.name << '\n';
    cout << "Gia ban: " << device->data.price << " trieu dong\n";
}
```

## 8. Nhập tên có khoảng trắng

Tên như `Man hinh Samsung` chứa khoảng trắng. Nếu dùng:

```cpp
cin >> deviceName;
```

chương trình chỉ đọc được `Man`. Phải dùng:

```cpp
getline(cin, deviceName);
```

Trong chương trình của đề, trước đó chưa có phép `cin >> ...`, nên có thể gọi `getline` trực tiếp. Nếu trước đó đã dùng `cin >>`, cần xử lý ký tự xuống dòng còn lại bằng `cin.ignore(...)`.

## 9. Giải phóng BST

```cpp
void freeDeviceTree(DeviceNode*& root) {
    if (root == nullptr) {
        return;
    }

    freeDeviceTree(root->left);
    freeDeviceTree(root->right);
    delete root;
    root = nullptr;
}
```

Đây là duyệt hậu tự: xóa hai cây con trước rồi mới xóa nút hiện tại.

---

# Phần II – Đồ thị logistics bằng danh sách kề

## 10. Tóm tắt Câu 2

- Có `n` kho bãi, đánh số `0` đến `n - 1`.
- Tuyến vận chuyển đi hai chiều nên đồ thị vô hướng.
- Không có trọng số, vì đề không cho quãng đường hoặc chi phí.
- Đồ thị được lưu bằng danh sách kề.

Danh sách mẫu:

```text
0: [1, 2]
1: [0, 3, 4]
2: [0, 5]
3: [1, 6]
4: [1, 5]
5: [2, 4, 6]
6: [3, 5]
```

## 11. Câu 2a – Khai báo danh sách kề

```cpp
using Graph = vector<vector<int>>;
```

Với `Graph graph(n)`:

- `graph[u]` là danh sách các kho nối trực tiếp với `u`.
- `graph[u].size()` là bậc của đỉnh `u`.

Hàm thêm cạnh vô hướng:

```cpp
void addUndirectedEdge(Graph& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}
```

Phải thêm cả hai chiều. Nếu chỉ thêm `v` vào `graph[u]`, code đã biến bài toán thành đồ thị có hướng.

Danh sách kề dùng `O(V + E)` bộ nhớ, phù hợp khi đồ thị thưa.

## 12. Câu 2b – Các kho kết nối trực tiếp

```cpp
void printDirectConnections(const Graph& graph, int k) {
    cout << "Kho bai " << k << " ket noi truc tiep voi "
         << graph[k].size() << " kho: ";

    for (int neighbor : graph[k]) {
        cout << neighbor << ' ';
    }
    cout << '\n';
}
```

Với kho số 5:

```text
5: [2, 4, 6]
```

Kết luận:

- Kho 5 nối trực tiếp với 3 kho.
- Các kho đó là 2, 4, 6.

## 13. Câu 2c – Ít kho trung gian nhất

Đồ thị không trọng số nên BFS là lựa chọn đúng. DFS có thể tìm một đường nhưng không bảo đảm đường đó có ít cạnh nhất.

```cpp
vector<int> shortestPath(const Graph& graph, int start, int target) {
    int n = static_cast<int>(graph.size());
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
```

### Vai trò của các cấu trúc

- `queue`: duyệt theo từng lớp khoảng cách.
- `visited`: không đưa một đỉnh vào hàng đợi nhiều lần.
- `parent`: dựng lại đường đi sau BFS.

### Từ kho 0 tới kho 6

Có ít nhất hai đường ngắn nhất:

```text
0 → 1 → 3 → 6
0 → 2 → 5 → 6
```

Cả hai có:

- 3 cạnh.
- 4 đỉnh.
- 2 kho trung gian.

Với thứ tự danh sách kề của đề, BFS trong lời giải trả về:

```text
0 → 1 → 3 → 6
```

Các kho trung gian là `1, 3`.

Đề không nói đường ngắn nhất phải duy nhất. Một mảng `parent` chỉ lưu và trả về **một** đường ngắn nhất hợp lệ. Đường `0 → 2 → 5 → 6` cũng đúng.

### Công thức cần nhớ

Nếu `path.size()` là số đỉnh trên đường:

```text
số cạnh = path.size() - 1
số kho trung gian = path.size() - 2
```

## 14. Câu 2d – Các cụm kho bãi độc lập

Mỗi cụm kho bãi độc lập là một thành phần liên thông. Ta chạy DFS từ từng đỉnh chưa thăm.

```cpp
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

Toàn bộ bảy kho đều đi tới nhau, nên hệ thống có một cụm:

```text
{0, 1, 2, 3, 4, 5, 6}
```

Đỉnh cô lập, nếu có, vẫn được tính là một thành phần gồm một đỉnh.

## 15. Tạo đúng đồ thị mẫu

Các cạnh không lặp:

```cpp
addUndirectedEdge(graph, 0, 1);
addUndirectedEdge(graph, 0, 2);
addUndirectedEdge(graph, 1, 3);
addUndirectedEdge(graph, 1, 4);
addUndirectedEdge(graph, 2, 5);
addUndirectedEdge(graph, 3, 6);
addUndirectedEdge(graph, 4, 5);
addUndirectedEdge(graph, 5, 6);
```

Không thêm lại các cạnh đảo như `(1,0)`, vì `addUndirectedEdge` đã thêm cả hai chiều. Nếu thêm cả hai, danh sách kề sẽ bị lặp.

## 16. Kết quả chạy chương trình

Với tên tìm kiếm `Man hinh Samsung`:

```text
===== CAU 1: CAY TIM KIEM NHI PHAN =====
Cac thiet bi co gia tu 5 den 20 trieu dong:
May in HP - 6 trieu dong
Man hinh Samsung - 7 trieu dong
Nhap ten thiet bi can tim: Man hinh Samsung
Ten thiet bi: Man hinh Samsung
Gia ban: 7 trieu dong

===== CAU 2: DO THI KHO BAI =====
Kho bai 5 ket noi truc tiep voi 3 kho: 2 4 6
Duong van chuyen ngan nhat: 0 -> 1 -> 3 -> 6
So kho trung gian it nhat: 2
Cac kho trung gian: 1 3
So cum kho bai doc lap: 1
Cum 1: 0 1 2 3 4 5 6
```

## 17. Độ phức tạp

Gọi `h` là chiều cao BST, `n` là số thiết bị, `V` là số đỉnh và `E` là số cạnh.

| Hàm | Độ phức tạp |
|---|---:|
| Chèn thiết bị | `O(h)` |
| Lọc khoảng giá bằng duyệt toàn cây | `O(n)` |
| Tìm theo tên | `O(n)` |
| Lấy các kho kề của k | `O(deg(k))` |
| BFS bằng danh sách kề | `O(V + E)` |
| DFS toàn đồ thị | `O(V + E)` |
| Bộ nhớ danh sách kề | `O(V + E)` |

Nếu BST cân bằng tương đối, `h = O(log n)`. Nếu cây lệch, `h = O(n)`. Không được khẳng định BST luôn chèn hoặc tìm trong `O(log n)`.

## 18. Những lỗi dễ mất điểm

1. Chèn BST theo tên dù đề nói khóa là giá bán.
2. Dùng điều kiện `price > 5 && price < 20`, vô tình loại hai đầu mút.
3. Dùng `||` thay cho `&&` khi kiểm tra một giá nằm trong đoạn.
4. Tìm theo tên bằng cách chỉ chọn một nhánh như tìm theo giá.
5. Dùng `cin >>` nên không đọc được tên có khoảng trắng.
6. Chỉ thêm một chiều khi tạo cạnh vô hướng.
7. Thêm cả `(u,v)` và `(v,u)` dù hàm đã tự thêm hai chiều, làm trùng hàng xóm.
8. Dùng DFS để khẳng định đã tìm được đường ngắn nhất.
9. Không lưu `parent`, nên không dựng lại được các kho trung gian.
10. Cho rằng từ 0 tới 6 chỉ có một đường ngắn nhất.
11. Nhầm số cạnh với số kho trung gian.
12. Khởi tạo lại `visited` trong mỗi lần DFS khi đếm thành phần.

## 19. Chiến thuật làm bài trong 90 phút

| Thời gian | Công việc |
|---:|---|
| 5 phút | Gạch chân khóa BST và kiểu lưu đồ thị |
| 30 phút | Câu 1: struct, chèn, lọc, tìm tên |
| 40 phút | Câu 2: danh sách kề, BFS, DFS |
| 10 phút | Viết `main`, kiểm tra kết quả mẫu |
| 5 phút | Soát điều kiện đoạn giá, `getline`, `visited`, `parent` |

## 20. Tóm tắt cần nhớ

```text
BST thiết bị:
    khóa = giá bán
    giá nhỏ → trái
    giá lớn hoặc bằng → phải
    lọc [5, 20] bằng price >= 5 && price <= 20
    tìm theo tên phải duyệt cả cây

Graph logistics:
    vector<vector<int>> graph
    cạnh vô hướng thêm hai chiều
    graph[k] là các kho nối trực tiếp với k

Đường ít kho trung gian nhất:
    BFS + parent
    số kho trung gian = path.size() - 2

Cụm độc lập:
    mỗi DFS mới từ đỉnh chưa thăm là một thành phần
```
