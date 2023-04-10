#include <iostream>
#include <vector>
#include <climits>
#include <map>
#include <cmath>
#include <ctime>


const int N = 4;
const int MAX_DEPTH = 100;
const int INF = INT_MAX;

struct State {
    std::vector<std::vector<int>> board;
    int x;
    int y;

    State() {
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

    bool operator==(const State& other) const {
        return board == other.board && x == other.x && y == other.y;
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
    std::vector<State> successor;
    int x = state.x;
    int y = state.y;
    if (x > 0) {
        State s = state;
        std::swap(s.board[x][y], s.board[x-1][y]);
        s.x = x-1;
        successor.push_back(s);
    }
    if (x < N-1) {
        State s = state;
        std::swap(s.board[x][y], s.board[x+1][y]);
        s.x = x+1;
        successor.push_back(s);
    }
    if (y > 0) {
        State s = state;
        std::swap(s.board[x][y], s.board[x][y-1]);
        s.y = y-1;
        successor.push_back(s);
    }
    if (y < N-1) {
        State s = state;
        std::swap(s.board[x][y], s.board[x][y+1]);
        s.y = y+1;
        successor.push_back(s);
    }
    return successor;
}


void search(State state, int g, int bound, int (*Manhattan)(const State&), std::vector<State>& solution, bool& found_solution) {
    int f = g + Manhattan(state);
    if (f > bound || g >= MAX_DEPTH) {
        return;
    }
    if (Manhattan(state) == 0) {
        found_solution = true;
        solution.push_back(state);
        return;
    }
    int min_t = INF;
    std::vector<State> succ = successors(state);
    for (State& s : succ) {
        if (s == state) {
            continue;
        }
        int t = g + Manhattan(s);
        if (t <= bound) {
            search(s, g + 1, bound, Manhattan, solution, found_solution);
            if (found_solution) {
                solution.push_back(state);
                return;
            }
        }
        min_t = std::min(min_t, t);
    }
    bound = min_t;
}

std::vector<State> idaStar(const State& start) {
    int bound = heuristic_Manhattan(start);
    while (true) {
        std::vector<State> solution;
        bool found_solution = false;
        search(start, 0, bound, heuristic_Manhattan, solution, found_solution);
        if (found_solution) {
            reverse(solution.begin(), solution.end());
            return solution;
        } else if (bound == INF) {
            return solution;
        }
        bound++;
    }
}


void solve(State state) {
    std::cout << "Tablero inicial:" << std::endl;
    state.print();
    std::vector<State> solution = idaStar(state);
    if (solution.empty()) {
        std::cout << "No se encontro solucion" << std::endl;
    } else {
        std::cout << "Solucion encontrada con costo " << solution.size()-1 << std::endl;
        std::cout << " " << std::endl;
        State final_state = solution.back();
        std::cout << "Tablero final:" << std::endl;
        final_state.print();
    }
}



int main() {
    State state;
    srand(time(nullptr));

    for (int i = 0; i < 100; i++) {
        std::vector<State> successor = successors(state);
        int rand_index = rand() % successor.size();
        state = successor[rand_index];
    }

    solve(state);

    return 0;
}
