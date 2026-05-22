#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <fstream>

using namespace std;

constexpr int ROWS = 10;
constexpr int COLS = 10;

// State node within the graph search space
struct Node {
    int x, y;       // Spatial grid coordinates
    int g;          // Accumulated historical movement cost from origin
    int h;          // Admissible heuristic distance (Manhattan L1 metric) to destination
    Node* parent;   // Raw pointer link to ancestor state for backtracking

    int f() const { return g + h; } // Evaluates total priority cost function: f(n) = g(n) + h(n)
};

// Strict-weak-ordering functor to configure the min-heap priority queue
struct CompareNodes {
    bool operator()(Node* const& n1, Node* const& n2) {
        return n1->f() > n2->f(); // Lower f(n) values receive higher priority extraction
    }
};

// Generates an ASCII representation of the structural state matrix
void printMap(const vector<vector<int>>& grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] == 1) cout << "# ";      // Warehouse Shelving Obstacle
            else if (grid[i][j] == 2) cout << "X "; // Terminal Sorting Bin Destination
            else if (grid[i][j] == 3) cout << "* "; // Evaluated Optimal Traversal Path
            else cout << ". ";                      // Unchecked Floor Allocation
        }
        cout << endl;
    }
}

// Perimeter boundary validating and structural collision verification check
bool isValid(int r, int c, const vector<vector<int>>& grid) {
    return (r >= 0 && r < ROWS && c >= 0 && c < COLS && grid[r][c] != 1);
}

// Manhattan distance heuristic tracking toward bottom-right boundary terminal point
int getH(int r, int c) {
    return abs(r - (ROWS - 1)) + abs(c - (COLS - 1));
}

int main() {
    // 1. Grid Configuration & Environment Initialization
    vector<vector<int>> grid(ROWS, vector<int>(COLS, 0));
    for (int i = 2; i < 8; i++) {
        grid[i][3] = 1;
        grid[i][7] = 1;
    }
    grid[ROWS - 1][COLS - 1] = 2;

    // 2. Search Infrastructure Setup
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false)); // Guarantees O(1) state verification

    Node* startNode = new Node{0, 0, 0, getH(0, 0), nullptr};
    pq.push(startNode);

    Node* finalNode = nullptr;

    // 3. Asymptotic Expansion Loop (Weighted A* Search Engine)
    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        if (visited[current->x][current->y]) continue; // Safe closed-set state pruning
        visited[current->x][current->y] = true;

        if (current->x == ROWS - 1 && current->y == COLS - 1) {
            finalNode = current;
            break; // Goal state reached
        }

        // Discrete orthogonal transformation direction vectors
        int dx[] = {-1, 1, 0, 0};
        int dy[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (isValid(nx, ny, grid) && !visited[nx][ny]) {
                // --- HUMAN-CENTRIC LOGISTICS METRICS (DYNAMIC STEP EXPENDITURE) ---
                int stepCost = 1; 

                // Constraint A: High-Density Congestion Zones (Conveyor belt area bottlenecks)
                if (ny == 4 || ny == 5) {
                    stepCost = 10; 
                }

                // Constraint B: Ergonometrical Fatigue Penalty curve calculation
                if (current->g > 15) {
                    stepCost += 2;
                }

                int newG = current->g + stepCost;
                // ------------------------------------------------------------------

                Node* neighbor = new Node{nx, ny, newG, getH(nx, ny), current};
                pq.push(neighbor);
            }
        }
    }

    // 4. Pointer-Based Backtracking & State Rendering
    if (finalNode) {
        cout << "Path found! Visualizing the route:" << endl;

        Node* curr = finalNode->parent;
        while (curr != nullptr && (curr->x != 0 || curr->y != 0)) {
            grid[curr->x][curr->y] = 3; 
            curr = curr->parent;
        }
        printMap(grid);
    } else {
        cout << "No path could be found to the bin." << endl;
    }

    // 5. Inter-Process File-Stream Pipeline (CSV Grid Serialization for Python Data Node)
    ofstream outFile("path_data.csv");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            outFile << grid[i][j] << (j == COLS - 1 ? "" : ",");
        }
        outFile << "\n";
    }
    outFile.close();
    cout << "Data exported to path_data.csv" << endl;

    return 0;
}