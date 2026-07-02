# Giải chi tiết đề thi Cấu trúc dữ liệu – Đề số 04

> Tài liệu này trình bày lại đề, kiến thức nền, cách suy luận và lời giải tham khảo bằng **C++17**.
> Trọng tâm thực tế của đề là **cây tìm kiếm nhị phân (BST)** và **đồ thị vô hướng**; đề không kiểm tra Binary Indexed Tree/Fenwick Tree.

## 1. Đọc nhanh cấu trúc đề

- Thời gian làm bài: **90 phút**.
- Tổng điểm: **10 điểm**.
- Câu 1: **5 điểm**, cây tìm kiếm nhị phân.
- Câu 2: **5 điểm**, đồ thị biểu diễn bằng danh sách kề.
- Sinh viên được tham khảo tài liệu.

Mỗi ý có 1 điểm. Vì vậy, ngay cả khi chưa hoàn thiện toàn bộ chương trình, nên viết rõ cấu trúc dữ liệu và từng hàm độc lập để lấy điểm từng phần.

---

# Phần I – Cây tìm kiếm nhị phân

## 2. Nội dung Câu 1

Mỗi nút của cây lưu một hành tinh gồm:

- Tên hành tinh.
- Khoảng cách từ hành tinh đến Mặt Trời, tính bằng triệu km.
- Khóa dùng để tổ chức cây là **khoảng cách**.

Đề yêu cầu:

1. Khai báo cấu trúc dữ liệu của cây tìm kiếm nhị phân.
2. Viết hàm thêm một hành tinh vào cây.
3. Liệt kê các hành tinh có khoảng cách nhỏ hơn 100 hoặc lớn hơn 500 triệu km.
4. Kiểm tra hành tinh có tên `x` có tồn tại trong cây không.
5. Viết `main` với năm hành tinh:

| Hành tinh | Khoảng cách (triệu km) |
|---|---:|
| Sao Thủy | 57 |
| Sao Kim | 108 |
| Trái Đất | 149 |
| Sao Hỏa | 227 |
| Sao Mộc | 778 |

Sau đó:

- Liệt kê các hành tinh thỏa điều kiện khoảng cách.
- Kiểm tra Sao Thổ có trong cây hay không.

## 3. Kiến thức nền về BST

### 3.1. Cây nhị phân là gì?

Mỗi nút của cây nhị phân có nhiều nhất hai cây con:

- Cây con trái.
- Cây con phải.

Một nút thường gồm dữ liệu và hai con trỏ:

```cpp
struct Node {
    Data data;
    Node* left;
    Node* right;
};
```

### 3.2. Điều kiện của cây tìm kiếm nhị phân

Với mỗi nút có khóa `key`:

- Các khóa trong cây con trái nhỏ hơn `key`.
- Các khóa trong cây con phải lớn hơn hoặc bằng `key` nếu ta quy ước khóa trùng được đưa sang phải.

Trong đề này, `key` chính là khoảng cách đến Mặt Trời.

Ví dụ, nếu chèn lần lượt khoảng cách `149, 57, 227, 108`, cây nhận được là:

```text
        149
       /   \
     57     227
       \
       108
```

### 3.3. Một điểm yếu trong dữ liệu mẫu

Đề chèn các khoảng cách theo thứ tự:

```text
57, 108, 149, 227, 778
```

Đây là thứ tự tăng dần, nên BST thông thường sẽ bị lệch hoàn toàn sang phải:

```text
57
  \
  108
     \
     149
        \
        227
           \
           778
```

Về hình dạng, cây này gần giống một danh sách liên kết. Do đó:

- BST không tự động cân bằng.
- Thao tác tìm/chèn trung bình thường được mô tả là `O(log n)` khi cây tương đối cân bằng.
- Trường hợp cây lệch có độ phức tạp `O(n)`.
- Với năm phần tử của đề, vấn đề hiệu năng không đáng kể, nhưng đây là một giả định ngầm cần nhận ra.

Nếu cần bảo đảm `O(log n)`, phải dùng cây tự cân bằng như AVL hoặc Red–Black Tree; đề không yêu cầu điều đó.

## 4. Câu 1a – Khai báo cấu trúc dữ liệu

```cpp
struct Planet {
    string name;
    double distance;
};

struct PlanetNode {
    Planet data;
    PlanetNode* left;
    PlanetNode* right;

    PlanetNode(const string& name, double distance)
        : data{name, distance}, left(nullptr), right(nullptr) {}
};
```

Giải thích:

- `Planet` chứa thông tin nghiệp vụ của một hành tinh.
- `PlanetNode` là nút của BST.
- `left` trỏ đến cây con trái.
- `right` trỏ đến cây con phải.
- Hàm khởi tạo gán hai con trỏ bằng `nullptr`, nghĩa là lúc mới tạo, nút chưa có con.
- Không cần tạo một trường `key` riêng vì `data.distance` đã là khóa.

Có thể gộp tên, khoảng cách và hai con trỏ vào cùng một `struct`. Cách tách `Planet` và `PlanetNode` chỉ nhằm làm mô hình rõ nghĩa hơn.

## 5. Câu 1b – Thêm hành tinh vào cây

### 5.1. Ý tưởng

Muốn chèn một hành tinh mới:

1. Nếu cây đang rỗng, nút mới trở thành gốc.
2. Nếu khoảng cách mới nhỏ hơn khóa tại nút hiện tại, chèn vào cây con trái.
3. Ngược lại, chèn vào cây con phải.

### 5.2. Cài đặt đệ quy

```cpp
void insertPlanet(PlanetNode*& root,
                  const string& name,
                  double distance) {
    if (root == nullptr) {
        root = new PlanetNode(name, distance);
        return;
    }

    if (distance < root->data.distance) {
        insertPlanet(root->left, name, distance);
    } else {
        insertPlanet(root->right, name, distance);
    }
}
```

### 5.3. Vì sao tham số là `PlanetNode*& root`?

`root` là **tham chiếu đến con trỏ**. Khi gặp một vị trí rỗng, câu lệnh:

```cpp
root = new PlanetNode(name, distance);
```

phải thay đổi chính con trỏ `root`, `root->left` hoặc `root->right` ở bên ngoài lời gọi hàm. Nếu chỉ truyền `PlanetNode* root` theo giá trị, việc gán lại con trỏ cục bộ không cập nhật liên kết của cây.

### 5.4. Khóa trùng nhau thì sao?

Đề không nói cách xử lý hai hành tinh có cùng khoảng cách. Lời giải đang dùng quy ước:

- Khóa nhỏ hơn đi sang trái.
- Khóa lớn hơn **hoặc bằng** đi sang phải.

Đây là một quyết định cài đặt hợp lệ, nhưng nên nói rõ. Một phương án khác là mỗi nút chứa danh sách các hành tinh có cùng khoảng cách.

## 6. Câu 1c – Liệt kê theo điều kiện khoảng cách

Yêu cầu cần kiểm tra là:

```text
distance < 100 HOẶC distance > 500
```

Từ “hoặc” tương ứng toán tử `||` trong C++:

```cpp
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
```

Đây là phép duyệt **in-order**:

```text
cây trái → nút hiện tại → cây phải
```

Vì cây được khóa theo khoảng cách, kết quả được in theo khoảng cách tăng dần.

Hàm duyệt toàn bộ `n` nút nên có độ phức tạp:

- Thời gian: `O(n)`.
- Ngăn xếp đệ quy: `O(h)`, với `h` là chiều cao cây.

Có thể tận dụng tính thứ tự để cắt bớt một số nhánh, nhưng với điều kiện gồm hai khoảng rời nhau và dữ liệu nhỏ, duyệt in-order là cách đơn giản, rõ ràng và ít lỗi nhất trong phòng thi.

### 6.1. Kết quả với dữ liệu mẫu

- Sao Thủy: `57 < 100`, được chọn.
- Sao Kim: 108, không được chọn.
- Trái Đất: 149, không được chọn.
- Sao Hỏa: 227, không được chọn.
- Sao Mộc: `778 > 500`, được chọn.

Kết quả là **Sao Thủy và Sao Mộc**.

## 7. Câu 1d – Tìm hành tinh theo tên

```cpp
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
```

### 7.1. Vì sao phải tìm ở cả hai nhánh?

Cây được sắp xếp theo **khoảng cách**, nhưng đề lại yêu cầu tìm theo **tên**. Từ tên `Sao Thổ`, ta không biết nên đi trái hay phải vì thứ tự tên không quyết định cấu trúc cây.

Do đó, trong trường hợp tổng quát phải duyệt cả cây:

- Thời gian: `O(n)`.
- Không được khẳng định hàm tìm tên có độ phức tạp `O(log n)` chỉ vì dữ liệu đang nằm trong BST.

Đây là lỗi lập luận rất thường gặp: BST chỉ hỗ trợ tìm kiếm nhanh theo **khóa tổ chức cây**, không phải theo mọi thuộc tính lưu trong nút.

Nếu chương trình thường xuyên tìm theo tên, có thể bổ sung `unordered_map<string, PlanetNode*>`. Tuy nhiên, đó là cấu trúc phụ ngoài yêu cầu của đề.

## 8. Câu 1e – Hàm main và kết quả

```cpp
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
```

Kết luận:

- Sao Thủy và Sao Mộc thỏa điều kiện khoảng cách.
- Sao Thổ không được chèn, nên không tồn tại trong cây.

## 9. Giải phóng bộ nhớ cây

Đề không chấm trực tiếp phần này, nhưng chương trình dùng `new` nên nên có hàm `delete` tương ứng:

```cpp
void freeTree(PlanetNode*& root) {
    if (root == nullptr) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    delete root;
    root = nullptr;
}
```

Phải xóa cây con trước rồi mới xóa nút hiện tại. Đây là duyệt hậu tự:

```text
cây trái → cây phải → nút hiện tại
```

---

# Phần II – Đồ thị ga tàu

## 10. Nội dung Câu 2

Thành phố có `n` ga tàu, đánh số từ `0` đến `n - 1`. Hai ga có thể nối với nhau bằng đường sắt. Một số ga kết nối trực tiếp với nhau.

Đề yêu cầu:

1. Khai báo danh sách kề để lưu đồ thị.
2. Cho biết ga `k` nối trực tiếp với bao nhiêu ga và đó là các ga nào.
3. Tìm số ga trung gian ít nhất khi đi từ ga `x` tới ga `y`, đồng thời cho biết các ga trung gian.
4. Đếm số cụm kết nối và liệt kê các ga trong từng cụm.
5. Chạy với đồ thị mẫu `n = 7`:

```text
0: [1, 4]
1: [0, 2, 5]
2: [1, 3, 6]
3: [2, 4]
4: [0, 3, 5]
5: [1, 4, 6]
6: [2, 5]
```

## 11. Mô hình hóa đồ thị

### 11.1. Đỉnh và cạnh

- Mỗi ga là một **đỉnh**.
- Mỗi đường sắt nối hai ga là một **cạnh**.
- Đường sắt đi được hai chiều nên đây là **đồ thị vô hướng**.
- Đề không đưa trọng số như quãng đường hay thời gian, nên đây là **đồ thị không trọng số**.

### 11.2. Danh sách các cạnh của đồ thị mẫu

Từ danh sách kề, ta có thể rút ra các cạnh không lặp:

```text
(0,1), (0,4),
(1,2), (1,5),
(2,3), (2,6),
(3,4),
(4,5),
(5,6)
```

Vì đồ thị vô hướng, cạnh `(u,v)` phải được lưu ở cả hai phía:

- Thêm `v` vào `adj[u]`.
- Thêm `u` vào `adj[v]`.

## 12. Câu 2a – Khai báo danh sách kề

```cpp
using Graph = vector<vector<int>>;
```

Với `Graph graph(n)`:

- `graph.size()` bằng số đỉnh.
- `graph[u]` là danh sách các đỉnh kề trực tiếp với `u`.

Hàm thêm cạnh vô hướng:

```cpp
void addUndirectedEdge(Graph& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}
```

Độ phức tạp bộ nhớ của danh sách kề là `O(V + E)`, trong đó:

- `V` là số đỉnh.
- `E` là số cạnh.

Trong đồ thị vô hướng, mỗi cạnh xuất hiện hai lần trong danh sách kề, nhưng về ký hiệu tiệm cận vẫn là `O(V + E)`.

### 12.1. Giả định cần nói rõ

Hàm trên không tự ngăn:

- Cạnh trùng lặp.
- Cạnh khuyên `(u,u)`.
- Chỉ số đỉnh ngoài khoảng `[0, n-1]`.

Dữ liệu đề hợp lệ nên lời giải thi có thể không cần xử lý các trường hợp này. Trong phần mềm thực tế, đầu vào cần được kiểm tra.

## 13. Câu 2b – Các ga nối trực tiếp với ga k

```cpp
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
```

Số ga nối trực tiếp với `k` chính là **bậc của đỉnh `k`**:

```cpp
graph[k].size()
```

Với ga số 2:

```text
2: [1, 3, 6]
```

Vì vậy:

- Bậc của ga 2 là 3.
- Các ga nối trực tiếp là 1, 3 và 6.

Thời gian chạy là `O(deg(k))`, với `deg(k)` là bậc của đỉnh `k`.

## 14. Câu 2c – Đường đi có ít ga trung gian nhất

### 14.1. Vì sao dùng BFS?

Đồ thị không có trọng số. Thuật toán BFS duyệt theo từng lớp:

- Lớp 0: đỉnh xuất phát.
- Lớp 1: các đỉnh cách một cạnh.
- Lớp 2: các đỉnh cách hai cạnh.
- ...

Do đó, lần đầu BFS tới một đỉnh chính là lúc tìm được đường có **ít cạnh nhất** tới đỉnh đó.

Không cần Dijkstra vì mọi cạnh có cùng chi phí. DFS có thể tìm thấy một đường đi, nhưng đường đó không được bảo đảm là ngắn nhất.

### 14.2. Ba mảng quan trọng

Ta dùng:

```cpp
vector<bool> visited(n, false);
vector<int> parent(n, -1);
queue<int> q;
```

- `visited[v]`: đỉnh `v` đã được phát hiện chưa.
- `parent[v]`: đỉnh đứng ngay trước `v` trên đường BFS.
- `q`: hàng đợi giúp duyệt đúng theo từng lớp.

### 14.3. Cài đặt BFS và dựng lại đường đi

```cpp
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
    for (int current = target;
         current != -1;
         current = parent[current]) {
        path.push_back(current);
    }

    reverse(path.begin(), path.end());
    return path;
}
```

### 14.4. Vì sao phải đánh dấu ngay khi đưa vào hàng đợi?

Ta thực hiện:

```cpp
visited[v] = true;
q.push(v);
```

Nếu đợi đến lúc lấy `v` khỏi hàng đợi mới đánh dấu, nhiều đỉnh có thể cùng đưa `v` vào hàng đợi, gây xử lý lặp và làm `parent` khó kiểm soát.

### 14.5. Dựng lại đường đi

`parent[target]` cho biết đỉnh trước `target`. Tiếp tục lần ngược:

```text
target → parent[target] → ... → start
```

Chuỗi ban đầu bị ngược nên phải gọi:

```cpp
reverse(path.begin(), path.end());
```

### 14.6. Số cạnh và số ga trung gian khác nhau

Giả sử đường đi là:

```text
5 → 1 → 2
```

- Số đỉnh trên đường: 3.
- Số cạnh: 2.
- Số ga trung gian: 1, chỉ có ga 1.

Với một đường gồm `path.size()` đỉnh:

```text
số cạnh = path.size() - 1
số ga trung gian = path.size() - 2
```

Công thức số ga trung gian chỉ áp dụng trực tiếp khi hai đầu mút khác nhau và có đường đi. Code hoàn chỉnh bên dưới xử lý riêng trường hợp `start == target` và trường hợp không có đường.

### 14.7. Hàm in kết quả

```cpp
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
        cout << "So ga trung gian it nhat: "
             << path.size() - 2 << '\n';
        cout << "Cac ga trung gian: ";

        for (size_t i = 1; i + 1 < path.size(); ++i) {
            cout << path[i] << ' ';
        }
        cout << '\n';
    }
}
```

### 14.8. Kết quả từ ga 5 tới ga 2

Ga 5 không nối trực tiếp với ga 2. Có ít nhất hai đường ngắn nhất:

```text
5 → 1 → 2
5 → 6 → 2
```

Cả hai đều có:

- 2 cạnh.
- 1 ga trung gian.

Tùy thứ tự các đỉnh trong danh sách kề, BFS có thể trả về một trong hai đường. Với danh sách của đề, ga 1 được xét trước ga 6 nên lời giải trên thường trả về:

```text
5 → 1 → 2
```

Ga trung gian là ga 1.

Đây là chỗ đề hơi thiếu chặt chẽ: câu hỏi có thể khiến người đọc tưởng đường ngắn nhất là duy nhất. BFS dùng một mảng `parent` chỉ trả về **một** đường ngắn nhất hợp lệ. Muốn liệt kê mọi đường ngắn nhất phải lưu nhiều cha cho mỗi đỉnh và xử lý phức tạp hơn; đề không yêu cầu rõ việc đó.

### 14.9. Độ phức tạp BFS

- Thời gian: `O(V + E)`.
- Bộ nhớ: `O(V)` ngoài danh sách kề.

## 15. Câu 2d – Thành phần liên thông

### 15.1. Khái niệm

Một **thành phần liên thông** hay “cụm kết nối” là một nhóm đỉnh sao cho:

- Từ một đỉnh bất kỳ trong nhóm có thể đi tới mọi đỉnh khác trong nhóm.
- Không có đường đi từ nhóm đó tới đỉnh thuộc một thành phần khác.

### 15.2. Ý tưởng thuật toán

1. Tạo mảng `visited`, ban đầu mọi đỉnh đều chưa thăm.
2. Duyệt lần lượt các đỉnh từ `0` đến `n - 1`.
3. Nếu đỉnh `i` chưa thăm, bắt đầu một DFS từ `i`.
4. Tất cả đỉnh DFS tìm thấy thuộc cùng một thành phần.
5. Mỗi lần phải bắt đầu DFS mới, số thành phần tăng thêm 1.

### 15.3. Hàm DFS thu thập một thành phần

```cpp
void dfsComponent(const Graph& graph,
                  int u,
                  vector<bool>& visited,
                  vector<int>& component) {
    visited[u] = true;
    component.push_back(u);

    for (int v : graph[u]) {
        if (!visited[v]) {
            dfsComponent(graph, v, visited, component);
        }
    }
}
```

### 15.4. Tìm tất cả thành phần

```cpp
vector<vector<int>> connectedComponents(const Graph& graph) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<vector<int>> components;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            vector<int> component;
            dfsComponent(graph, i, visited, component);
            components.push_back(component);
        }
    }

    return components;
}
```

Lưu ý quan trọng: không được tạo lại `visited` trong mỗi lần gọi DFS. Tất cả lần DFS phải dùng chung một mảng để biết đỉnh nào đã thuộc thành phần trước đó.

### 15.5. Đỉnh cô lập

Một ga không nối với ga nào vẫn là một thành phần liên thông gồm đúng một đỉnh. Vòng lặp ngoài vẫn phát hiện được trường hợp này vì đỉnh cô lập chưa được thăm.

### 15.6. Kết quả của đồ thị mẫu

Từ ga 0 có thể lần lượt đi tới mọi ga khác. Chẳng hạn:

- `0 → 1 → 2 → 3`.
- `0 → 4 → 5 → 6`.

Vì cả bảy ga đều đi tới nhau, thành phố có đúng **1 cụm kết nối**:

```text
{0, 1, 2, 3, 4, 5, 6}
```

Thứ tự in của DFS có thể khác thứ tự trên, tùy thứ tự danh sách kề. Thành viên của tập không thay đổi.

### 15.7. Độ phức tạp

- Thời gian: `O(V + E)`.
- Bộ nhớ đánh dấu: `O(V)`.
- Ngăn xếp đệ quy: tối đa `O(V)` trong trường hợp xấu nhất.

---

# Phần III – Chương trình C++ hoàn chỉnh

## 16. Mã nguồn tham khảo

```cpp
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

// ============================================================
// CAU 1: BINARY SEARCH TREE
// ============================================================

struct Planet {
    string name;
    double distance;  // Don vi: trieu km
};

struct PlanetNode {
    Planet data;
    PlanetNode* left;
    PlanetNode* right;

    PlanetNode(const string& name, double distance)
        : data{name, distance}, left(nullptr), right(nullptr) {}
};

void insertPlanet(PlanetNode*& root,
                  const string& name,
                  double distance) {
    if (root == nullptr) {
        root = new PlanetNode(name, distance);
        return;
    }

    if (distance < root->data.distance) {
        insertPlanet(root->left, name, distance);
    } else {
        insertPlanet(root->right, name, distance);
    }
}

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

void freeTree(PlanetNode*& root) {
    if (root == nullptr) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);
    delete root;
    root = nullptr;
}

// ============================================================
// CAU 2: UNDIRECTED GRAPH
// ============================================================

using Graph = vector<vector<int>>;

void addUndirectedEdge(Graph& graph, int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

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
    for (int current = target;
         current != -1;
         current = parent[current]) {
        path.push_back(current);
    }

    reverse(path.begin(), path.end());
    return path;
}

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
        cout << "So ga trung gian it nhat: "
             << path.size() - 2 << '\n';
        cout << "Cac ga trung gian: ";

        for (size_t i = 1; i + 1 < path.size(); ++i) {
            cout << path[i] << ' ';
        }
        cout << '\n';
    }
}

void dfsComponent(const Graph& graph,
                  int u,
                  vector<bool>& visited,
                  vector<int>& component) {
    visited[u] = true;
    component.push_back(u);

    for (int v : graph[u]) {
        if (!visited[v]) {
            dfsComponent(graph, v, visited, component);
        }
    }
}

vector<vector<int>> connectedComponents(const Graph& graph) {
    int n = static_cast<int>(graph.size());
    vector<bool> visited(n, false);
    vector<vector<int>> components;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            vector<int> component;
            dfsComponent(graph, i, visited, component);
            components.push_back(component);
        }
    }

    return components;
}

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

int main() {
    // -------------------- Cau 1 --------------------
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

    // -------------------- Cau 2 --------------------
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
```

## 17. Kết quả dự kiến

```text
===== CAU 1: CAY TIM KIEM NHI PHAN =====
Cac hanh tinh co khoang cach < 100 hoac > 500:
Sao Thuy - 57 trieu km
Sao Moc - 778 trieu km
Sao Tho khong co trong cay

===== CAU 2: DO THI GA TAU =====
Ga 2 ket noi truc tiep voi 3 ga: 1 3 6
Duong di ngan nhat: 5 -> 1 -> 2
So ga trung gian it nhat: 1
Cac ga trung gian: 1
So cum ket noi: 1
Cum 1: 0 1 2 3 4 5 6
```

Thứ tự của các ga trong kết quả DFS có thể khác nhưng vẫn đúng nếu tập đỉnh của từng thành phần không đổi.

---

# Phần IV – Những lỗi dễ mất điểm

## 18. Lỗi ở phần BST

1. **So sánh tên để chèn cây:** sai yêu cầu vì khóa là khoảng cách.
2. **Tìm tên bằng cách chỉ đi một nhánh:** không hợp lệ vì cây không được sắp theo tên.
3. **Dùng `&&` thay vì `||`:** không có khoảng cách nào vừa nhỏ hơn 100 vừa lớn hơn 500.
4. **Không xử lý cây rỗng:** đệ quy sẽ truy cập con trỏ null.
5. **Truyền con trỏ theo giá trị khi chèn:** nút mới không được gắn vào cây bên ngoài hàm.
6. **Khẳng định BST luôn `O(log n)`:** chỉ đúng khi chiều cao cây là `O(log n)`; dữ liệu mẫu tạo cây lệch.

## 19. Lỗi ở phần Graph

1. **Chỉ thêm một chiều của cạnh:** khi đó code đang tạo đồ thị có hướng, trái với đề.
2. **Nhầm `graph[k].size()` với số đỉnh toàn đồ thị.**
3. **Dùng DFS để khẳng định đường tìm được là ngắn nhất:** DFS không có bảo đảm đó trên đồ thị không trọng số.
4. **Không lưu `parent`:** biết khoảng cách nhưng không dựng lại được các ga trung gian.
5. **Tính số ga trung gian bằng số cạnh:** đường `5 → 1 → 2` có hai cạnh nhưng chỉ một ga trung gian.
6. **In cả hai đầu mút như ga trung gian:** phải bỏ `start` và `target`.
7. **Quên trường hợp không có đường:** không được lần `parent` từ một đỉnh chưa được thăm.
8. **Khởi tạo lại `visited` cho từng đỉnh khi đếm thành phần:** sẽ đếm sai hoặc lặp lại các cụm.
9. **Cho rằng đường ngắn nhất là duy nhất:** đồ thị mẫu có hai đường ngắn nhất từ 5 đến 2.

---

# Phần V – Cách làm bài trong 90 phút

## 20. Phân bổ thời gian đề xuất

| Thời gian | Công việc |
|---:|---|
| 5 phút | Đọc đề, xác định khóa BST và loại đồ thị |
| 30 phút | Làm Câu 1, kiểm tra điều kiện `||` và tìm theo tên |
| 40 phút | Làm Câu 2, ưu tiên danh sách kề, BFS, DFS |
| 10 phút | Viết/hoàn thiện `main`, đối chiếu kết quả mẫu |
| 5 phút | Soát null, chỉ số, `visited`, `parent`, số ga trung gian |

## 21. Khung tư duy cần nhớ

### BST

```text
Chèn theo khóa distance
    ├── nhỏ hơn: sang trái
    └── lớn hơn hoặc bằng: sang phải

Lọc theo distance: duyệt cây
Tìm theo name: duyệt cả cây vì name không phải khóa
```

### Graph

```text
Danh sách kề
    ├── Hàng xóm trực tiếp: adj[k]
    ├── Đường ít cạnh nhất: BFS + parent
    └── Thành phần liên thông: DFS/BFS + visited
```

## 22. Bộ câu hỏi tự kiểm tra

Nếu trả lời được các câu sau mà không nhìn tài liệu, bạn đã nắm phần lõi của đề:

1. Vì sao cây trong đề là BST theo khoảng cách chứ không phải theo tên?
2. Vì sao tìm `Sao Thổ` phải duyệt cả hai nhánh?
3. Vì sao thứ tự chèn của đề làm cây bị lệch?
4. `graph[2]` biểu diễn điều gì?
5. Vì sao cạnh vô hướng phải thêm hai lần?
6. Vì sao BFS tìm được đường ít ga trung gian nhất?
7. Mảng `parent` dùng để làm gì?
8. Đường có `k` đỉnh thì có bao nhiêu đỉnh trung gian?
9. Vì sao phải có vòng lặp ngoài DFS khi tìm các thành phần liên thông?
10. Ga cô lập có được tính là một thành phần không?

## 23. Đáp án ngắn cho bộ tự kiểm tra

1. Vì đề chỉ định khoảng cách là khóa.
2. Vì tên không quyết định nút nằm bên trái hay bên phải.
3. Vì các khóa được chèn theo thứ tự tăng dần.
4. Danh sách các ga nối trực tiếp với ga 2.
5. Vì từ `u` đi được tới `v` và từ `v` cũng đi được tới `u`.
6. Vì BFS duyệt đồ thị theo số cạnh tăng dần.
7. Để lần ngược và dựng lại đường đi.
8. `k - 2`, nếu hai đầu mút khác nhau và đường đi tồn tại.
9. Một lần DFS chỉ thăm được thành phần chứa đỉnh bắt đầu.
10. Có, đó là thành phần gồm một đỉnh.

---

# 24. Kết luận

Để làm được đề này, không cần học toàn bộ lý thuyết cây và đồ thị. Phần cốt lõi gồm:

- BST: khai báo nút, chèn theo khóa, duyệt cây và hiểu giới hạn của khóa tìm kiếm.
- Graph: danh sách kề của đồ thị vô hướng.
- BFS: tìm một đường đi ngắn nhất và dựng lại đường bằng `parent`.
- DFS/BFS: tìm các thành phần liên thông.

Hai kết luận quan trọng nhất của dữ liệu mẫu là:

- Các hành tinh thỏa điều kiện: **Sao Thủy, Sao Mộc**; **Sao Thổ không có trong cây**.
- Ga 2 nối trực tiếp với **1, 3, 6**; từ ga 5 đến ga 2 cần ít nhất **một ga trung gian**; toàn bộ đồ thị có **một thành phần liên thông**.
