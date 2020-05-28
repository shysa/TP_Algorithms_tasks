#include <iostream>
#include <cassert>
#include <array>
#include <queue>
#include <unordered_map>
#include <algorithm>

// ------------------------------- GAME FIELD --------------------------------------------
const char SIZE = 9;
const std::array<char, SIZE> FINAL = {1, 2, 3, 4, 5, 6, 7, 8, 0};

struct GameStateHash;

class GameState {
private:
    std::array<char, SIZE> field;
    char emptyPos;

    bool CanMoveLeft() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;
    bool CanMoveUp() const;

    GameState MoveLeft() const;
    GameState MoveRight() const;
    GameState MoveDown() const;
    GameState MoveUp() const;

    bool InversionParity(const std::array<char, SIZE>& array);
    friend std::ostream& operator<<(std::ostream &os, const GameState &state);
    friend struct GameStateHash;
    friend std::string GetSolution(const std::array<char, SIZE> &_field);
public:
    GameState(const std::array<char, SIZE> &array): field(array) {
        emptyPos = -1;
        for (int i = 0; i < SIZE; i++) {
            if (field[i] == 0) {
                emptyPos = i;
                break;
            }
        }
        assert(emptyPos != -1);
    }
    GameState(const GameState &_source): field(_source.field), emptyPos(_source.emptyPos) {};

    bool IsComplete() const {
        return field == FINAL;
    }

    bool IsSolvable() {
        return InversionParity(field);
    }

    bool operator==(const GameState &other) const {
        return other.field == field;
    }
};
// ------------------------------------------------------------------------------------
bool GameState::CanMoveLeft() const {
    return emptyPos % 3 != 2;
}
bool GameState::CanMoveRight() const {
    return emptyPos % 3 != 0;
}
bool GameState::CanMoveDown() const {
    return emptyPos > 2;
}
bool GameState::CanMoveUp() const {
    return emptyPos < 6;
}
// ------------------------------------------------------------------------------------
GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());

    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
    newState.emptyPos++;

    return newState;
}
GameState GameState::MoveRight() const {
    assert(CanMoveRight());

    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
    newState.emptyPos--;

    return newState;
}
GameState GameState::MoveDown() const {
    assert(CanMoveDown());

    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos - 3]);
    newState.emptyPos -= 3;

    return newState;
}
GameState GameState::MoveUp() const {
    assert(CanMoveUp());

    GameState newState(*this);
    std::swap(newState.field[emptyPos], newState.field[emptyPos + 3]);
    newState.emptyPos += 3;

    return newState;
}
// ------------------------------------------------------------------------------------
// чтобы имелось решение, нужно четное количество инверсий
bool GameState::InversionParity(const std::array<char, SIZE> &array) {
    int countWrongPairs = 0;

    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = i + 1; j < SIZE; j++) {
            if (array[i] && array[j] && array[i] < array[j]) {
                countWrongPairs++;
            }
        }
    }

    return countWrongPairs % 2 == 0;
}
// ------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream &os, const GameState &state) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            os << static_cast<int>(state.field[i * 3 + j]) << " ";
        }
        os << std::endl;
    }
    return os;
}
// ------------------------------------------------------------------------------------
struct GameStateHash {
    size_t operator()(const GameState &state) const {
        return std::hash<std::string>{}(state.field.data());
    }
};
// ------------------------------------------------------------------------------------
std::string GetSolution(const std::array<char, SIZE> &_field) {
    GameState startState(_field);

    if (!startState.IsSolvable()) {
        return "-1";
    }

    std::queue<GameState> queue;
    // {field, L/R/D/U} - как попали в field
    std::unordered_map<GameState, char, GameStateHash> way;

    way[startState] = 'S';      // S = start
    queue.push(startState);

    while (true) {
        GameState currentState = queue.front();
        queue.pop();

        if (currentState.IsComplete()) {
            break;
        }

        if (currentState.CanMoveLeft()) {
            GameState nextState = currentState.MoveLeft();
            if (way.find(nextState) == way.end()) {
                way[nextState] = 'L';
                queue.push(nextState);
            }
        }
        if (currentState.CanMoveRight()) {
            GameState nextState = currentState.MoveRight();
            if (way.find(nextState) == way.end()) {
                way[nextState] = 'R';
                queue.push(nextState);
            }
        }
        if (currentState.CanMoveDown()) {
            GameState nextState = currentState.MoveDown();
            if (way.find(nextState) == way.end()) {
                way[nextState] = 'D';
                queue.push(nextState);
            }
        }
        if (currentState.CanMoveUp()) {
            GameState nextState = currentState.MoveUp();
            if (way.find(nextState) == way.end()) {
                way[nextState] = 'U';
                queue.push(nextState);
            }
        }
    }

    std::string path;
    GameState state(FINAL);

    std::cout << state << std::endl;

    while (way[state] != 'S') {
        switch (way[state]) {
            case 'L': {
                path += 'L';
                state = state.MoveRight();
                break;
            }
            case 'R': {
                path += 'R';
                state = state.MoveLeft();
                break;
            }
            case 'D': {
                path += 'D';
                state = state.MoveUp();
                break;
            }
            case 'U': {
                path += 'U';
                state = state.MoveDown();
                break;
            }
        }
        std::cout << state << std::endl;
    }

    std::reverse(path.begin(), path.end());
    return path;
}
// ------------------------------------------------------------------------------------



int main() {
    std::array<char, SIZE> field = {2, 3, 1, 4, 5, 6, 7, 8, 0};

    std::cout << GetSolution(field) << std::endl;

    return 0;
}