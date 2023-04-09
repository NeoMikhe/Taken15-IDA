#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <climits>

const int N = 4;
const int MAX_DEPTH = 100;
const int INF = INT_MAX;

struct State {
    std::vector<std::vector<int>> board;
    int x;
    int y;

    State() {
        // inicializa el tablero en orden
        board.resize(N, std::vector<int>(N));
        int val = 1;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                board[i][j] = val;
                val += 1;
            }
        }
        board[N - 1][N - 1] = 0;
        x = N - 1;
        y = N - 1;
    }

    void print() const {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << board[i][j] << "   ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    bool operator==(const State& other) const {
        return board == other.board && x == other.x && y == other.y;
    }
};

int heuristic_Manhattan(const State& state) {
    int distance = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value = state.board[i][j];
            if (value != 0) {
                int target_x = (value - 1) / N;
                int target_y = (value - 1) % N;
                distance += std::abs(i - target_x) + std::abs(j - target_y);
            }
        }
    }
    return distance;
}

std::vector<State> successors(const State& state) {
    std::vector<State> successors;
    int x = state.x;
    int y = state.y;
    auto board = state.board; // create a new board vector
    if (x > 0) {
        State s;
        s.board = board; // use the new board vector
        s.x = x - 1;
        s.y = y;
        s.board[x][y] = board[x - 1][y];
        s.board[x - 1][y] = 0; // cambio aquí, pongo 0 en lugar de board[x][y]
        successors.push_back(s);
    }
    if (x < N - 1) {
        State s;
        s.board = board; // use the new board vector
        s.x = x + 1;
        s.y = y;
        s.board[x][y] = board[x + 1][y];
        s.board[x + 1][y] = 0; // cambio aquí, pongo 0 en lugar de board[x][y]
        successors.push_back(s);
    }
    if (y > 0) {
        State s;
        s.board = board; // use the new board vector
        s.x = x;
        s.y = y - 1;
        s.board[x][y] = board[x][y - 1];
        s.board[x][y - 1] = 0; // cambio aquí, pongo 0 en lugar de board[x][y]
        successors.push_back(s);
    }
    if (y < N - 1) {
        State s;
        s.board = board; // use the new board vector
        s.y = y + 1;
        s.x = x;
        s.board[x][y] = board[x][y + 1];
        s.board[x][y + 1] = 0; // cambio aquí, pongo 0 en lugar de board[x][y]
        successors.push_back(s);
    }
    return successors;
}


int search(State& state, int g, int depth, int bound, std::vector<State>& path) {
    int h = heuristic_Manhattan(state);
    int f = g + h;
    if (f > bound) {
        return f;
    }
    if (h == 0) {
        path.push_back(state);
        return -1;
    }
    int min_cost = INF;
    for (State& successor : successors(state)) {
        if (successor == State()) {  // incorrecto
            path.push_back(successor);
            int t = search(successor, g + 1, depth + 1, bound, path);
            if (t == -1) {
                return -1;
            }
            if (t < min_cost) {
                min_cost = t;
            }
            path.pop_back();
        } else if (successor == State()) {  // correcto
            path.push_back(successor);
            int t = search(successor, g + 1, depth + 1, bound, path);
            if (t == -1) {
                return -1;
            }
            if (t < min_cost) {
                min_cost = t;
            }
            path.pop_back();
        }
    }
    return min_cost;
}

std::vector<State> idaStar(const State& start) {
    std::vector<State> path;
    int bound = heuristic_Manhattan(start);
    while (true) {
        int t = search(const_cast<State&>(start), 0, 0, bound, path);
        if (t == -1) {
            return path;
        }
        if (t == INF) {
            return {};
        }
        bound = t;
    }
}

void solve(State& state) {
    std::vector<State> path = idaStar(state);
    if (path.empty()) {
        std::cout << "No se encontró solución dentro del límite de profundidad " << MAX_DEPTH << "." << std::endl;
    } else {
        std::cout << "Estado inicial:" << std::endl;
        state.print();
        for (int i = 0; i < path.size(); i++) {
            std::cout << "Movimiento " << i + 1 << ":" << std::endl;
            path[i].print();
        }
        std::cout << "Estado final:" << std::endl;
        path.back().print();
    }
}


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N * N - 1);

    State state;
    // mezcla el tablero inicial aleatoriamente
    for (int i = 0; i < N * N; i++) {
        int pos1 = dis(gen);
        int pos2 = dis(gen);
        int temp = state.board[pos1 / N][pos1 % N];
        state.board[pos1 / N][pos1 % N] = state.board[pos2 / N][pos2 % N];
        state.board[pos2 / N][pos2 % N] = temp;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state.board[i][j] == 0) {
                state.x = i;
                state.y = j;
            }
        }
    }
    std::cout << "Estado inicial:" << std::endl;
    state.print();

    solve(state);

    return 0;
}
