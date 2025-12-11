#include "MySTL/Vector.h"
#include "MySTL/list.h"
#include "MySTL/Stack.h"
#include <iostream>
#include <climits>
#include <cstring>
#include <map>
using namespace std;

// 确保Queue基于List实现（若List.h中未定义，补充以下代码）
template <typename T>
class Queue {
private:
    List<T> list;
public:
    void enqueue(const T& e) { list.insertAsLast(e); }
    T dequeue() { return list.remove(list.first()); }
    T front() { return list.first()->data; }
    bool empty() { return list.empty(); }
};

class Graph {
private:
    int vertexNum;
    Vector<Vector<int>> adj;
    Vector<string> vertexs;
    map<string, int> vtxMap;

    void dfsHelper(int u, Vector<bool>& visited) {
        cout << vertexs[u] << " ";
        // 修复：自定义Vector的[]返回值需匹配类型
        visited.replace(u, true);
        for (int v = 0; v < vertexNum; v++) {
            if (adj[u][v] != INT_MAX && !visited[v]) {
                dfsHelper(v, visited);
            }
        }
    }

    void tarjan(int u, int parent, Vector<int>& dfn, Vector<int>& low, 
                Vector<bool>& isCut, int& time) {
        dfn.replace(u, ++time);
        low.replace(u, dfn[u]);
        int childCount = 0;
        for (int v = 0; v < vertexNum; v++) {
            if (adj[u][v] == INT_MAX) continue;
            if (dfn[v] == 0) {
                childCount++;
                tarjan(v, u, dfn, low, isCut, time);
                if (low[v] < low[u]) low.replace(u, low[v]);
                if (parent == -1 && childCount > 1) isCut.replace(u, true);
                if (parent != -1 && low[v] >= dfn[u]) isCut.replace(u, true);
            } else if (v != parent && dfn[v] < low[u]) {
                low.replace(u, dfn[v]);
            }
        }
    }

public:
    // 修复：Vector初始化改为逐个添加
    Graph(Vector<string>& vtxList) {
        vertexNum = vtxList.size();
        for (int i = 0; i < vertexNum; i++) {
            vtxMap[vtxList[i]] = i;
            vertexs.push_back(vtxList[i]);
        }
        // 初始化邻接矩阵
        for (int i = 0; i < vertexNum; i++) {
            Vector<int> row;
            for (int j = 0; j < vertexNum; j++) {
                if (i == j) row.push_back(0);
                else row.push_back(INT_MAX);
            }
            adj.push_back(row);
        }
    }

    void addEdge(string uName, string vName, int weight) {
        int u = vtxMap[uName];
        int v = vtxMap[vName];
        adj[u].replace(v, weight);
        adj[v].replace(u, weight);
    }

    void printAdjMatrix() {
        cout << "\n=== 图的邻接矩阵 ===" << endl;
        cout << "   ";
        for (int i = 0; i < vertexNum; i++) {
            cout << vertexs[i] << "  ";
        }
        cout << endl;
        for (int i = 0; i < vertexNum; i++) {
            cout << vertexs[i] << "  ";
            for (int j = 0; j < vertexNum; j++) {
                if (adj[i][j] == INT_MAX) cout << "∞  ";
                else cout << adj[i][j] << "  ";
            }
            cout << endl;
        }
    }

    void BFS(string startName) {
        int start = vtxMap[startName];
        // 修复：Vector<bool>初始化改为逐个赋值
        Vector<bool> visited;
        for (int i = 0; i < vertexNum; i++) visited.push_back(false);
        Queue<int> q;
        cout << "\n=== BFS 遍历（起点：" << startName << "）===" << endl;
        q.enqueue(start);
        visited.replace(start, true);
        while (!q.empty()) {
            int u = q.front();
            q.dequeue();
            cout << vertexs[u] << " ";
            for (int v = 0; v < vertexNum; v++) {
                if (adj[u][v] != INT_MAX && !visited[v]) {
                    visited.replace(v, true);
                    q.enqueue(v);
                }
            }
        }
        cout << endl;
    }

    void DFS(string startName) {
        int start = vtxMap[startName];
        Vector<bool> visited;
        for (int i = 0; i < vertexNum; i++) visited.push_back(false);
        cout << "\n=== DFS 遍历（起点：" << startName << "）===" << endl;
        dfsHelper(start, visited);
        cout << endl;
    }

    void dijkstra(string startName) {
        int start = vtxMap[startName];
        Vector<int> dist;
        Vector<bool> visited;
        for (int i = 0; i < vertexNum; i++) {
            dist.push_back(INT_MAX);
            visited.push_back(false);
        }
        dist.replace(start, 0);

        for (int i = 0; i < vertexNum - 1; i++) {
            int minDist = INT_MAX, u = -1;
            for (int v = 0; v < vertexNum; v++) {
                if (!visited[v] && dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
            }
            if (u == -1) break;
            visited.replace(u, true);
            for (int v = 0; v < vertexNum; v++) {
                if (!visited[v] && adj[u][v] != INT_MAX && dist[u] != INT_MAX) {
                    if (dist[u] + adj[u][v] < dist[v]) {
                        dist.replace(v, dist[u] + adj[u][v]);
                    }
                }
            }
        }

        cout << "\n=== Dijkstra 单源最短路径（起点：" << startName << "）===" << endl;
        for (int i = 0; i < vertexNum; i++) {
            cout << startName << " -> " << vertexs[i] << "：";
            if (dist[i] == INT_MAX) cout << "不可达";
            else cout << dist[i];
            cout << endl;
        }
    }

    void prim(string startName) {
        int start = vtxMap[startName];
        Vector<int> key;
        Vector<bool> inMST;
        Vector<int> parent;
        for (int i = 0; i < vertexNum; i++) {
            key.push_back(INT_MAX);
            inMST.push_back(false);
            parent.push_back(-1);
        }
        key.replace(start, 0);
        int totalWeight = 0;

        for (int i = 0; i < vertexNum - 1; i++) {
            int minKey = INT_MAX, u = -1;
            for (int v = 0; v < vertexNum; v++) {
                if (!inMST[v] && key[v] < minKey) {
                    minKey = key[v];
                    u = v;
                }
            }
            if (u == -1) break;
            inMST.replace(u, true);
            totalWeight += key[u];
            for (int v = 0; v < vertexNum; v++) {
                if (!inMST[v] && adj[u][v] != INT_MAX && adj[u][v] < key[v]) {
                    key.replace(v, adj[u][v]);
                    parent.replace(v, u);
                }
            }
        }

        cout << "\n=== Prim 最小生成树（起点：" << startName << "）===" << endl;
        cout << "边（起点-终点）：权重" << endl;
        for (int i = 0; i < vertexNum; i++) {
            if (parent[i] != -1) {
                cout << vertexs[parent[i]] << " - " << vertexs[i] << "：" << adj[parent[i]][i] << endl;
            }
        }
        cout << "最小生成树总权重：" << totalWeight << endl;
    }

    void findCutVertices() {
        Vector<int> dfn;
        Vector<int> low;
        Vector<bool> isCut;
        for (int i = 0; i < vertexNum; i++) {
            dfn.push_back(0);
            low.push_back(0);
            isCut.push_back(false);
        }
        int time = 0;

        cout << "\n=== Tarjan 算法找关节点 ===" << endl;
        for (int i = 0; i < vertexNum; i++) {
            if (dfn[i] == 0) {
                tarjan(i, -1, dfn, low, isCut, time);
            }
        }

        cout << "图中的关节点：";
        bool hasCut = false;
        for (int i = 0; i < vertexNum; i++) {
            if (isCut[i]) {
                cout << vertexs[i] << " ";
                hasCut = true;
            }
        }
        if (!hasCut) cout << "无";
        cout << endl;
    }
};

int main() {
    cout << "===== 图算法实验（exp3）=====" << endl;
    Vector<string> vertexList;
    vertexList.push_back("A");
    vertexList.push_back("B");
    vertexList.push_back("C");
    vertexList.push_back("D");
    vertexList.push_back("E");
    vertexList.push_back("F");
    vertexList.push_back("G");
    vertexList.push_back("H");
    Graph graph(vertexList);

    graph.addEdge("A", "B", 12);
    graph.addEdge("A", "C", 4);
    graph.addEdge("B", "E", 9);
    graph.addEdge("B", "F", 10);
    graph.addEdge("C", "D", 13);
    graph.addEdge("C", "E", 1);
    graph.addEdge("D", "G", 2);
    graph.addEdge("E", "F", 5);
    graph.addEdge("E", "G", 11);
    graph.addEdge("E", "H", 14);
    graph.addEdge("F", "H", 3);
    graph.addEdge("G", "H", 8);

    graph.printAdjMatrix();
    graph.BFS("A");
    graph.DFS("A");
    graph.dijkstra("A");
    graph.prim("A");
    graph.findCutVertices();

    cout << "\n===== 实验结束 =====" << endl;
    return 0;
}

