#include <bits/stdc++.h>
using namespace std;

int dirX[8] = {1, -1, 0, 0, 1, 1, -1, -1};
int dirY[8] = {0, 0, 1, -1, 1, -1, 1, -1};

struct Cell {
    int cx, cy;
    double gScore, fScore;
    Cell(int _cx=0, int _cy=0, double _g=0, double _f=0) { 
        cx=_cx; cy=_cy; gScore=_g; fScore=_f; 
    }
};

// Comparator for priority queue (min-heap on fScore)
struct Compare {
    bool operator()(const Cell &a, const Cell &b) const {
        return a.fScore > b.fScore;
    }
};

double calcHeuristic(int x1, int y1, int x2, int y2) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return sqrt(dx * dx + dy * dy); // Euclidean distance
}

bool insideGrid(int i, int j, int rows, int cols) {
    return (i >= 0 && i < rows && j >= 0 && j < cols);
}

vector<pair<int,int>> computePath(vector<string>& board, pair<int,int> src, pair<int,int> dest) {
    int rows = board.size(), cols = board[0].size();

    vector<vector<double>> dist(rows, vector<double>(cols, 1e18));
    vector<vector<pair<int,int>>> parent(rows, vector<pair<int,int>>(cols, {-1,-1}));
    priority_queue<Cell, vector<Cell>, Compare> pq;

    dist[src.first][src.second] = 0;
    pq.push(Cell(src.first, src.second, 0, calcHeuristic(src.first, src.second, dest.first, dest.second)));

    while(!pq.empty()) {
        Cell cur = pq.top();
        pq.pop();
        int ux = cur.cx, uy = cur.cy;

        if (ux == dest.first && uy == dest.second) {
            vector<pair<int,int>> path;
            while (!(ux == -1 && uy == -1)) {
                path.push_back({ux, uy});
                auto p = parent[ux][uy];
                ux = p.first; uy = p.second;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        for(int d = 0; d < 8; d++) {
            int vx = ux + dirX[d];
            int vy = uy + dirY[d];

            if(!insideGrid(vx, vy, rows, cols)) continue;
            if(board[vx][vy] == '#') continue;

            // Move cost (diagonal = 1.5, straight = 1.0)
            double moveCost = (abs(dirX[d]) + abs(dirY[d]) == 2 ? 1.5 : 1.0);
            double tentativeG = dist[ux][uy] + moveCost;

            if(tentativeG < dist[vx][vy]) {
                dist[vx][vy] = tentativeG;
                parent[vx][vy] = {ux, uy};
                double fScore = tentativeG + calcHeuristic(vx, vy, dest.first, dest.second);
                pq.push(Cell(vx, vy, tentativeG, fScore));
            }
        }
    }
    return {};
}

void processInput() {
    int n, m;
    cout << "Enter Grid Size : ";
    cin >> n >> m;

    vector<string> board(n, string(m, '.'));

    int sx, sy;
    cout << "\nEnter Start Coordinates : ";
    cin >> sx >> sy;
    pair<int,int> start = {sx, sy};
    board[sx][sy] = 'S';

    int gx, gy;
    cout << "\nEnter Destination Coordinates : ";
    cin >> gx >> gy;
    pair<int,int> goal = {gx, gy};
    board[gx][gy] = 'D';

    int r;
    cout << "Enter number of rivers : ";
    cin >> r;

    for(int i = 0; i < r; i++) {
        int rx, ry;
        cout << "Enter River " << i << " coordinates : ";
        cin >> rx >> ry;
        board[rx][ry] = '#';
    }

    auto path = computePath(board, start, goal);

    if(path.empty()) {
        cout << "No path found\n";
    } else {
        cout << "Path found:\n";
        for(auto [px, py] : path) {
            cout << "(" << px << "," << py << ") ";
            if(board[px][py] == '.') board[px][py] = '*';
        }
        cout << "\n\nGrid with path:\n";
        for(auto &row : board) cout << row << "\n";
    }
}

int main() {
    processInput();
    return 0;
}
