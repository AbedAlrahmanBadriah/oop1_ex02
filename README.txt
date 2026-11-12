================================================================
1. TITLE
================================================================

Digger - Object-Oriented Programming Exercise 2 (oop1_ex02)


================================================================
2. STUDENT DETAILS
================================================================

Full name: [FULL NAME AS IT APPEARS IN THE COLLEGE RECORDS]
ID:        [ID NUMBER]

(If submitted by a pair, add the second student's full name and ID
here as well. The same names must appear in MY_AUTHORS in the root
CMakeLists.txt, in the form first_last-first_last.)


================================================================
3. GENERAL EXPLANATION OF THE EXERCISE
================================================================

This project is a console implementation of the classic Digger game
in C++. The player controls the digger ('/'), whose goal is to eat
all the diamonds ('D') on the board while avoiding the guards ('!').
The digger may also eat a limited number of stones ('@') to clear a
path.

The game is turn-based. In each turn the player moves the digger
with the arrow keys, or presses space to skip the turn. After the
digger's turn, every guard moves one cell. Stepping on a diamond
scores 15 points, and clearing a level adds a 20-point bonus. The
digger starts with 3 lives. If a guard touches the digger, one life
is lost and the digger and all guards are returned to their starting
cells, while the board state (already eaten diamonds and stones) and
the score are preserved. Levels are read from resources/Board.txt.
Clearing the last level wins the game; losing all lives ends it in
failure.


================================================================
4. LIST OF FILES
================================================================

include/Location.h      Simple row/column coordinate struct with
                        equality and inequality operators.

include/board.h         Board class declaration: tile constants,
src/board.cpp           level loading, bounds-safe tile access,
                        stone allowance, tile counting.

include/digger.h        Digger class: position, lives, movement,
src/digger.cpp          reset, and losing a life.

include/guard.h         Guard class: current and starting position,
src/guard.cpp           reset, and the greedy chase algorithm.

include/controller.h    Controller class: owns the board, digger,
src/controller.cpp      and guards; game loop, input, turn order,
                        collisions, scoring, rendering, and level
                        transitions.

include/io.h            Course-provided keyboard constants (Keys,
src/io.cpp              SpecialKeys) and screen cursor helpers
                        (Screen). Not written by the students.

src/main.cpp            Entry point; creates a controller and calls
                        run().

resources/Board.txt     Level definitions: a header "size stones"
                        followed by exactly "size" rows of "size"
                        characters each.

CMakeLists.txt          Root CMake project; contains MY_AUTHORS.
include/CMakeLists.txt  CMake lists for the other directories.
src/CMakeLists.txt
resources/CMakeLists.txt


================================================================
5. MAIN DATA STRUCTURES AND THEIR ROLES
================================================================

Location (Location.h)
    A coordinate: int row, int col. Used everywhere positions are
    passed between objects.

board::m_board (std::vector<std::vector<char>>)
    The tile grid of the current level. Tiles are represented by the
    constants board::k_empty, k_wall, k_digger, k_guard, k_diamond,
    and k_stone.

board::m_size, m_max_stones, m_remaining_stones
    Board dimensions and the per-level stone allowance. m_max_stones
    is the value from the level header; m_remaining_stones is the
    live counter that decreases when the digger eats a stone.

controller::m_guards (std::vector<guard>)
    All guards of the current level. Rebuilt whenever a new level is
    loaded.

controller::m_digger_start (Location)
    The digger's starting cell for the current level, used when
    resetting characters after a hit.

controller::m_score (int)
    The accumulated score across all levels.

digger::m_lives (int)
    Remaining lives, initialised to digger::kInitialLives (3).

guard::m_location, guard::m_start (Location)
    The guard's current cell and its starting cell for the level.


================================================================
6. NOTABLE ALGORITHMS
================================================================

Level loading (board::load_next_level)
    Reads the header (size and maximum stones), then reads exactly
    size lines. Each line must be exactly size characters long; a
    wrong length is reported and the load fails. Returns false at end
    of file (no more levels) and on malformed input. The stone
    allowance is reset to the header value on every level.

Turn order (controller::move_digger)
    A legal digger move is applied first. If the digger lands on a
    guard it is hit immediately, and the hit takes priority over the
    tile, so a diamond on that cell is not collected. Otherwise the
    tile is eaten (diamond or stone) and level completion is checked.
    Then every guard moves once, and a second collision check catches
    a guard that moved onto the digger. A blocked move (wall, or a
    stone when no allowance is left) does not consume a turn, so the
    guards do not move.

Guard AI (guard::choose_move)
    A deterministic greedy chase. The guard computes the row and
    column difference to the digger and tries four candidate steps in
    order: the dominant axis toward the digger, the other axis
    toward the digger, then the two opposite directions. The first
    candidate that is neither a wall nor a stone is chosen; if none
    is legal, the guard stays in place. This is cheap and always
    closes the distance when a straight path exists. It can get
    stuck behind concave walls; BFS shortest-path search would be a
    possible upgrade.

Collision detection (controller::digger_caught)
    A linear scan of all guards for the digger's current cell.

Hit handling (controller::handle_hit)
    Decrements a life. If lives remain, the digger and every guard
    are reset to their starting cells without touching the board, so
    eaten diamonds/stones and the score persist. If no lives remain,
    the game ends with a failure message.


================================================================
7. DESIGN
================================================================

Objects and responsibilities:

board
    Owns the tile grid and the level file. It is responsible for
    loading levels, validating row lengths, tracking the stone
    allowance, and providing bounds-safe tile access. It knows
    nothing about game rules beyond tiles.

digger
    Owns its position and its lives. It knows how to move, reset,
    and lose a life. It has no knowledge of the board.

guard
    Owns its current and starting position and the chase algorithm.
    It reads the board through a const reference when choosing a
    move and never modifies it.

controller
    The coordinator. It owns the board, the digger, the guards, the
    score, and the level start positions. It implements the game
    loop and the turn rules, handles input, detects collisions,
    updates the score, triggers level loading, and renders the board.

Interaction:

main creates a controller and calls run(). run() opens and loads a
level through the board, places the characters, renders once, and
then loops on handle_input(). handle_input() converts a key into a
target Location and calls move_digger(). move_digger() mutates the
digger and the board, asks every guard for a move (choose_move()
reads the board), detects collisions, updates the score, and
triggers the next level when the last diamond is eaten. The
controller performs all rendering, drawing the digger and the guards
as overlays on top of the board tiles.

The split is deliberate: game rules live in the controller, entity
state and entity-specific operations live in the entity classes, and
file I/O plus grid storage live in the board. This keeps each class
small and makes the dependencies one-directional (controller ->
board/digger/guard, guard -> board).


================================================================
8. KNOWN BUGS
================================================================

- The guard AI is greedy and can get stuck behind concave walls
  (for example an L-shaped obstacle) even when a longer path to the
  digger exists. It does not perform pathfinding.

- If the digger steps onto a guard that is standing on a diamond,
  the hit takes priority and the diamond is not collected. This is
  an intentional interpretation of the rule that the digger is hit
  "immediately"; the diamond remains on the board and can be
  collected after the guard moves away.

- The following members are currently unused leftovers and could be
  removed: board::print_board(), board::get_board(),
  board::get_max_stones(), and digger::set_lives().


================================================================
9. OTHER NOTES
================================================================

- Controls: arrow keys move the digger, space skips a turn, and Esc
  quits the game.

- The status line under the board shows the score, remaining lives,
  remaining stone allowance, and the current level number. It is
  redrawn after every turn.

- system("cls") is used only between levels. Inside a level,
  Screen::resetLocation() is used so the board is redrawn in place
  without flicker.

- Game values are named constants (static constexpr) instead of
  hard-coded literals: board tile characters, diamond points (15),
  level bonus (20), initial lives (3), and the space key.

- The project is built with CMake. The provided build setup copies
  resources/Board.txt next to the executable.

- Before submission, the student names must also be filled in
  MY_AUTHORS in the root CMakeLists.txt.
