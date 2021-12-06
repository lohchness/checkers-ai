#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include<limits.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#include<string.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];

/* -------------------------------------------------------------------------- */

#define HORIZONTAL_DIVIDER "   +---+---+---+---+---+---+---+---+"
#define VERTICAL_DIVIDER      "|"
#define N_BLACKS (BOARD_SIZE*ROWS_WITH_PIECES)/2
#define N_WHITES (BOARD_SIZE*ROWS_WITH_PIECES)/2
#define PRINT_DIVIDER "====================================="
#define COMPUTED_ACTION "***"
#define WHITE_ROW_START (BOARD_SIZE - BOARD_SIZE)
#define BLACK_ROW_START (BOARD_SIZE)
#define DISTANCE_BETWEEN_PIECES 2
#define COLUMN_START 65
#define INPUT_MAX_LENGTH 5
#define ASCII_VAL 65
#define TABLE_DIFF 1
#define STARTING_TURN 1
#define WIN_MESSAGE_BLACK "BLACK WIN!"
#define WIN_MESSAGE_WHITE "WHITE WIN!"
#define ERROR -1

// Directions from NE clockwise, used to compute possible moves
#define NUM_DIRECTIONS 4
int Y_DIR[NUM_DIRECTIONS] = {-1, 1, 1, -1};
int X_DIR[NUM_DIRECTIONS] = {1, 1, -1, -1};

// Stage 1 variables 
typedef struct node node_t;

typedef struct {
    node_t *root;
    int (*cmp)(void*, void*);
} tree_t;

// Each node of the minimax tree
struct node {
    board_t board;
    int cost; // Board cost of current move
    int source_row , target_row;
    char source_col, target_col;
    node_t **child_list; // Array of pointers to their child nodes
    int child_list_length; // child_list buddy variable
};

void init_board(board_t board);
void read_input(board_t board, int turn);
void print_board(board_t board, int turn, int source_row, char source_col, 
    int target_row, char target_col, int if_computer);
int board_cost(board_t board);
int illegal_move_check(board_t board, 
                    int source_row, int source_col, 
                    int target_row, int target_col, 
                    int turn,
                    int mute);
void move_piece(board_t board,
                int source_row, int source_col,
                int target_row, int target_col,
                int turn,
                int mute);
int minimax(board_t board, int source_row, char source_col, int target_row, char target_col, int turn);
void insert_child_node(node_t* node, int child_num, int source_row, 
    char source_col, int target_row, char target_col, int tree_depth, int turn);
void init_tree_board_cost(node_t* node, int intial_turn, int current_turn);
void recursive_free_tree(node_t* node);

/* -------------------------------------------------------------------------- */

/* Traffic control */
int
main(int argc, char *argv[]) {
    // STAGE ZER0 - READING, ANALYZING, AND PRINTING INPUT DATA
    board_t board;
    init_board(board);
    int turn = STARTING_TURN;
    read_input(board, turn);

    // STAGE 1 - COMPUTE AND PRINT NEXT ACTION
    // if (result == 1) {
    //     // Stage 1
    //     // idk, non working code doesn't get marked anyway
        
    // }
    // STAGE 2 - MACHINES GAME
    // else if (result == 2) {
    //     // Stage 2
    //     // lol as if
    // }
    printf("\n");
    return EXIT_SUCCESS;
}

/* -------------------------------------------------------------------------- */

/**
 * @brief Initializes board with an ordering described as per the Assignment 
 * specs. Works with a board of arbitrary BOARD_SIZE, ROWS_WITH PIECES and
 * DISTANCE_BETWEEN_PIECES.
 * 
 * @param    board               board_t - 2-D Array of chars.
 */
void init_board(board_t board) {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            // Initalizes all cells to be empty at first
            board[row][col] = CELL_EMPTY;
            // Alternates cells as per the assignment handout.
            if ((col - row) % DISTANCE_BETWEEN_PIECES) {
                // Assigns cells its pieces within their boundaries.
                if (row < WHITE_ROW_START + ROWS_WITH_PIECES) {
                    board[row][col] = CELL_WPIECE;
                }
                else if (row >= BLACK_ROW_START - ROWS_WITH_PIECES) {
                    board[row][col] = CELL_BPIECE;
                }
            }
        }
    }
    // Prints out board in its initial state.
    print_board(board, 0, 0, 0, 0, 0, 0);
}

/* -------------------------------------------------------------------------- */

/**
 * @brief Reads input from stdin. If input follows the format "%c%d-%c%d\n",
 * illegal_move_check will be called to check whether the input is valid. If input
 * is either 'A' or 'P', it returns 1 or 2 for Stage 1 and 2 function calls.
 * If there is an error with the input file, it will exit immediately.
 * 
 * @param    board               2-D Array of chars.
 * @param    turn                Current turn number.
 * @return   int                 1 if stdin reads 'A', 2 if stdin reads 'P' and
 *                               0 if there are no more inputs from stdin.
 */
void read_input(board_t board, int turn) {
    int source_row = 0, target_row = 0;
    char source_col = '\0', target_col = '\0';
    int result;
    // Reads format from stdin
    while (scanf("%c%d-%c%d\n", &source_col, &source_row, &target_col, &target_row)) {
        if (source_row) {
            // Accounts for ASCII values and array indexing differences.
            source_row -= TABLE_DIFF;
            source_col -= ASCII_VAL;
            target_row -= TABLE_DIFF;
            target_col -= ASCII_VAL;
            int illegal = illegal_move_check(board, 
                source_row, source_col, 
                target_row, target_col, 
                turn, 0);
            if (!illegal) {
                move_piece(board, source_row, source_col, target_row, 
                target_col, turn, 0);
            }
            else {
                exit(EXIT_FAILURE);
            }
            turn++;
        }
        else if (source_col == 'A') {
            if (minimax(board, source_row, source_col, target_row, target_col, turn)) {
                move_piece(board, source_row, source_col, target_row, target_col, turn, 0);
                print_board(board, turn, source_row, source_col, target_row, target_col, 1);
            }
            else {
                if (turn % 2) {
                    printf("\n%s", WIN_MESSAGE_BLACK);
                } 
                else {
                    printf("\n%s", WIN_MESSAGE_WHITE);
                }
            }
            exit(EXIT_SUCCESS);
        }
        else if (source_col == 'P') {
            int actions=0;
            while((result=minimax(board, source_row, source_col, target_row, target_col, turn)) && actions < COMP_ACTIONS) { // TO-DO: CAUTION
                move_piece(board, source_row, source_col, target_row, target_col, turn, 0);
                turn++;
            }
            if (!(result) && actions < COMP_ACTIONS) {
                if (turn % 2) {
                    printf("\n%s", WIN_MESSAGE_BLACK);
                } 
                else {
                    printf("\n%s", WIN_MESSAGE_WHITE);
                }
            }
            exit(EXIT_SUCCESS);
        }
        // Reached end of input file without 'A' or 'P'.
        else {
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        source_row = 0;
        target_row = 0;
        source_col = '\0';
        target_col = '\0';
    }
}

/* -------------------------------------------------------------------------- */

/**
 * @brief Prints out board_t board formatted in a BOARD_SIZE x BOARD_SIZE grid.
 * If this is called in the process of Stage 1 or 2, the string "***" will be
 * printed to show that the minimax algorithm has computed this move. If there
 * are no more white or black pieces, the game is won, and a win message is
 * printed out.
 * 
 * @param    board               board_t - 2-D Array of chars.
 * @param    turn                Current turn number.
 * @param    source_row          Source row of piece.
 * @param    source_col          Source column of piece.
 * @param    target_row          Target row of piece.
 * @param    target_col          Target column of piece.
 * @param    if_computer         Bool value if S1 minimax algorithm called this.
 */
void print_board(board_t board, int turn, int source_row, char source_col, 
int target_row, char target_col, int if_computer) {
    char *turn_color = '\0';
    int b_piece_count = 0, w_piece_count = 0;

    // If this was NOT called from its initial state.
    if (source_row) {
        if (if_computer) {
            printf("%s", COMPUTED_ACTION);
        }

        if (turn % 2) {
            turn_color = "BLACK";
        }
        else {
            turn_color = "WHITE";
        }

        printf("\n%s\n", PRINT_DIVIDER);
        printf("%s ACTION #%d: %c%d-%c%d\n", turn_color, turn, source_col, 
        source_row, target_col, target_row);
        printf("BOARD COST: %d\n", board_cost(board));
    }
    // Called from init_board().
    else {
        printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
        printf("#BLACK PIECES: %d\n", N_BLACKS);
        printf("#WHITE PIECES: %d\n", N_WHITES);
    }

    // Print column header (A  B  C  D ...)
    printf("  ");
    for (int i = COLUMN_START; i < COLUMN_START + BOARD_SIZE; i++) {
        printf("   %c", i);
    }
    printf("\n");

    // Prints formatted grid
    for (int row = 0; row < BOARD_SIZE; row++) {
        printf("%s\n %d", HORIZONTAL_DIVIDER, row + 1);
        for (int col = 0; col < BOARD_SIZE; col++) {
            printf(" %s %c", VERTICAL_DIVIDER, board[row][col]);

            // Counts each black and white piece to check if the game is over.
            if (board[row][col] == CELL_BPIECE || 
            board[row][col] == CELL_BTOWER) {
                b_piece_count++;
            }
            else if (board[row][col] == CELL_WPIECE || 
            board[row][col] == CELL_WTOWER) {
                w_piece_count++;
            }
        }
        // Closing vertical divider
        printf(" %s\n", VERTICAL_DIVIDER);
    }
    // Closing horizontal divider
    printf("%s", HORIZONTAL_DIVIDER);

    /* Prints win messages if the game is won. Only the logic of Stage 1 or 2
    should identify the winner. */
    if (if_computer) {
        if (!w_piece_count) {
            printf("\n%s", WIN_MESSAGE_BLACK);
        }
        else if (!b_piece_count) {
            printf("\n%s", WIN_MESSAGE_WHITE);
        }
    }
}

/* -------------------------------------------------------------------------- */

/**
 * @brief Counts black and white pieces and towers, and prints out an integer
 * for the result of the Board Cost formula: b + 3B - w - 3W.
 * 
 * @param    board               board_t - 2-D Array of chars.
 * @return   int                 Result of Board Cost formula.
 */
int board_cost(board_t board) {
    int N_B_PIECES = 0, N_B_TOWERS = 0, N_W_PIECES = 0, N_W_TOWERS = 0;
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == CELL_BPIECE) {
                N_B_PIECES++;
            }
            else if (board[row][col] == CELL_BTOWER) {
                N_B_TOWERS++;
            }
            else if (board[row][col] == CELL_WPIECE) {
                N_W_PIECES++;
            }
            else if (board[row][col] == CELL_WTOWER) {
                N_W_TOWERS++;
            }
        }
    }
    return N_B_PIECES + (3 * N_B_TOWERS) - N_W_PIECES - (3 * N_W_TOWERS);
}

/* -------------------------------------------------------------------------- */

/**
 * @brief Checks move of piece against 6 classifications of illegal moves. If 
 * there is an illegal move, it returns 1-6 as well as printing out a message if
 * required.
 * If move is LEGAL, returns 0.
 * 
 * @param    board               board_t - 2-D Array of chars.
 * @param    source_row          Source row of piece.
 * @param    source_col          Source column of piece.
 * @param    target_row          Target row of piece.
 * @param    target_col          Target column of piece.
 * @param    turn                Current turn number.
 * @param    mute                Bool value to print out error message.
 * @return   int                 0 if it is an illegal move, 1 if it is valid.
 */
int illegal_move_check(board_t board, 
                    int source_row, int source_col, 
                    int target_row, int target_col, 
                    int turn,
                    int mute) {    
    int apparent_turn = turn % 2; // Odd = Black, Even = White
    int error_code = 0;

    /* Vectors are mirrored on the x-axis, since board is indexed by (row,col)
    order. */
    int vector_row = target_row - source_row;
    int vector_col = target_col - source_col;

    /* ERROR 1 : Source cell outside of board  */
    if (source_row >= BOARD_SIZE || source_col >= BOARD_SIZE) {
        error_code = 1;
    }

    /* ERROR 2 : Target cell is outside of the board */
    else if (target_row >= BOARD_SIZE || target_col >= BOARD_SIZE) {
        error_code = 2;
    }

    /* Error 3: Source cell is empty */
    else if (board[source_row][source_col] == CELL_EMPTY) {
        error_code = 3;
    }

    /* Error 4: Target cell is not empty */
    else if(board[target_row][target_col] != CELL_EMPTY) {
        error_code = 4;
    }

    /* Error 5: Source cell holds opponent's piece/tower */
    // White's turn
    else if (!apparent_turn && 
    (board[source_row][source_col] == CELL_BPIECE || 
    board[source_row][source_col] == CELL_BTOWER)) {
        error_code = 5;
    }
    // Black's turn
    else if (apparent_turn && 
    (board[source_row][source_col] == CELL_WPIECE || 
    board[source_row][source_col] == CELL_WTOWER)) {
        error_code = 5;
    }

    /* ERROR 6 - Illegal moves */
    /* If piece does not move 1 cell or 2 cells in both directions, meaning the 
    move did not execute a standard move or a capturing move respectively. */
    else if (!((abs(vector_row) == 1 && abs(vector_col) == 1) || 
    (abs(vector_row) == 2 && abs(vector_col) == 2))) {
        error_code = 6;
    }
    else if (source_col == target_col) {
        error_code = 6;
    }

    // If a piece is moving in the wrong direction.
    // White's turn
    else if (board[source_row][source_col] == CELL_WPIECE &&
    vector_row < 0) {
            error_code = 6;
    }
    // Black's turn
    else if (board[source_row][source_col] == CELL_BPIECE &&
    vector_row > 0) {
            error_code = 6;
    }

    // If the move is a capturing move
    else if (abs(vector_row) + abs(vector_col) == 4) {
        
        // Piece that the move will capture
        char piece_to_capture = board[source_row + (vector_row/2)][source_col + (vector_col/2)];
        
        // If the piece to capture is an Empty Cell
        if (piece_to_capture == CELL_EMPTY) {
            error_code = 6;
        }

        /* If the Piece/Tower is White and is attempting to capture a White 
        Piece/Tower */
        else if ((board[source_row][source_col] == CELL_WPIECE 
        || board[source_row][source_col] == CELL_WTOWER)
        && 
        (piece_to_capture == CELL_WPIECE || 
        piece_to_capture == CELL_WTOWER)) {
            error_code = 6;
        }
        /* If the Piece/Tower is Black and is attempting to capture a Black 
        Piece/Tower */
        else if ((board[source_row][source_col] == CELL_BPIECE 
        || board[source_row][source_col] == CELL_BTOWER) 
        && 
        (piece_to_capture == CELL_BPIECE || 
        piece_to_capture == CELL_BTOWER)) {
            error_code = 6;
        }
    }

    // Prints out error code and message if mute == 0
    if (!mute) {
        switch (error_code) {
            case 1:
                printf("\nERROR: Source cell is outside of the board.");
                break;
            case 2:
                printf("\nERROR: Target cell is outside of the board.");
                break;
            case 3:
                printf("\nERROR: Source cell is empty.");
                break;
            case 4:
                printf("\nERROR: Target cell is not empty.");
                break;
            case 5:
                printf("\nERROR: Source cell holds opponent's piece/tower.");
                break;
            case 6:
                printf("\nERROR: Illegal action.");
                break;
        }
    }
    if (error_code && !mute) {
        printf("\n");
        exit(EXIT_FAILURE);
    }
    else if (error_code && mute) {
        return error_code;
    }
    return 0;
}

/* -------------------------------------------------------------------------- */

/**
 * @brief This function changes the pieces of the board around to reflect the
 * current state. This function assumes that each move put in the function is
 * valid. If a piece reaches the opposite end of the board, it will turn into
 * a tower. If a piece captures an opponent's piece, the opponent's piece will
 * be captured and disappear.
 * 
 * @param    board               board_t - 2-D Array of chars.
 * @param    source_row          Source row of piece.
 * @param    source_col          Source column of piece.
 * @param    target_row          Target row of piece.
 * @param    target_col          Target column of piece.
 * @param    turn                Current turn number.
 * @param    mute                Decides whether to print the result.
 */
void move_piece(board_t board,
                int source_row, int source_col,
                int target_row, int target_col,
                int turn,
                int mute) {
    int apparent_turn = turn % 2; // Odd = Black, Even = White
    /* Vectors are mirrored on the x-axis, since board is indexed by (row,col)
    order. */
    int vector_row = target_row - source_row;
    int vector_col = target_col - source_col;

    // Classifies the source piece.
    int piece_code = 0;
    if (board[source_row][source_col] == CELL_WPIECE) {
        piece_code = 1;
    }
    else if (board[source_row][source_col] == CELL_BPIECE) {
        piece_code = 2;
    }
    else if (board[source_row][source_col] == CELL_WTOWER) {
        piece_code = 3;
    }
    else { // SOURCE CELL IS BTOWER
        piece_code = 4;
    }

    board[source_row][source_col] = CELL_EMPTY;

    // Standard, non-capturing move.
    // WHITE'S TURN
    if (!apparent_turn) {
        if (piece_code == 1) {
            board[target_row][target_col] = CELL_WPIECE;
        }
        // SOURCE CELL IS WTOWER
        else { 
            board[target_row][target_col] = CELL_WTOWER;
        }
    }
    // BLACK'S TURN
    else {
        if (piece_code == 2) {
            board[target_row][target_col] = CELL_BPIECE;
        }
        // SOURCE CELL IS BTOWER
        else { 
            board[target_row][target_col] = CELL_BTOWER;
        }
    }

    // Capturing move.
    // Replaces piece to capture with an empty cell.
    if (abs(vector_row) + abs(vector_col) == 4) {
        board[source_row + (vector_row/2)][source_col + (vector_col/2)] = 
        CELL_EMPTY;
    }

    // If piece has reached the end of the board, transform it into a tower
    if (board[target_row][target_col] == CELL_WPIECE && 
    target_row == (BOARD_SIZE - TABLE_DIFF)) {
        board[target_row][target_col] = CELL_WTOWER;
    }
    else if (board[target_row][target_col] == CELL_BPIECE &&
    target_row == (BOARD_SIZE - BOARD_SIZE)) {
        board[target_row][target_col] = CELL_BTOWER;
    }

    // Prints out result after moving the pieces.
    if (!mute) print_board(board, turn, source_row + TABLE_DIFF, 
    (char)(source_col + ASCII_VAL), target_row + TABLE_DIFF, 
    (char)(target_col + ASCII_VAL), 0);
}

int n_pieces(board_t board, char piece) {
    int n=0;
    for (int row=0; row<BOARD_SIZE; row++) {
        for (int col=0; col<BOARD_SIZE; col++) {
            if (board[row][col] == piece) {
                n++;
            }
        }
    }
    return n;
}

void make_empty_tree(node_t* node, int tree_depth, int turn) {
    int row, col, child_nodes=0, direction, illegal_move;
    
    if (turn % 2 != 1) {
        node->child_list_length = NUM_DIRECTIONS * 
                                n_pieces(node->board, CELL_WPIECE) +
                                n_pieces(node->board, CELL_WTOWER);
    }
    else {
        node->child_list_length = NUM_DIRECTIONS * 
                                n_pieces(node->board, CELL_BPIECE) +
                                n_pieces(node->board, CELL_BTOWER);
    }
    
    node->child_list = (node_t**)malloc((node->child_list_length)*
                                sizeof(node_t*));
    
    for (int row=0; row<node->child_list_length; row++) {
        node->child_list[row] = NULL;
    }

    if (tree_depth <= 0) return;

    int source_row = 0, target_row = 0;
    char source_col = '\0', target_col = '\0';
    for (row=0; row<BOARD_SIZE; row++) {
        for (col=(row+1)%2; col<BOARD_SIZE; col+=2) { // row-major order, skipping 
            // check if it is the wrong piece for black/white's turn
            if ((node->board[row][col] == CELL_EMPTY) ||
                ((node->board[row][col] == CELL_BPIECE || 
                node->board[row][col]==CELL_BTOWER)
                && !(turn%2)) ||
                (node->board[row][col] == CELL_EMPTY) ||
                ((node->board[row][col]==CELL_BPIECE || 
                node->board[row][col]==CELL_BTOWER)
                && turn%2)) {
                    continue;
                }

            // if turn is valid, start playing pieces to see if it is valid
            // row major order, playing pieces from NE clockwise.
            for (direction=0; direction<NUM_DIRECTIONS; direction++) {
                source_row = row;
                source_col = col;
                target_row = row + Y_DIR[direction];
                target_col = col + X_DIR[direction];
                illegal_move = illegal_move_check(node->board, source_row, 
                source_col, target_row, target_col, turn, 1);
                if (!illegal_move) {
                    insert_child_node(node, NUM_DIRECTIONS*child_nodes+direction,
                        source_row, source_col, target_row, target_col, tree_depth, turn);
                }
                // If piece is a tower, it can still move in 1 square in all
                // directions
                else if (illegal_move==4) {
                    target_row += Y_DIR[direction];
                    target_col += X_DIR[direction];
                    if (!illegal_move_check(node->board, source_row, 
                    source_col, target_row, target_col, turn, 1)) {
                        insert_child_node(node, NUM_DIRECTIONS*child_nodes+direction,
                        source_row, source_col, target_row, target_col, tree_depth, turn);
                    }

                }
            }
            // Increment children
            child_nodes++;
        }
    }
}

int minimax(board_t board, int source_row, char source_col, int target_row, char target_col, int turn) {
    node_t* tree_root = (node_t*)malloc(sizeof(*tree_root));
    for (int row=0; row<BOARD_SIZE; row++) {
        for (int col=0; col<BOARD_SIZE; col++) {
            board[row][col] = tree_root->board[row][col];
        }
    }
    make_empty_tree(tree_root, TREE_DEPTH, turn);
    init_tree_board_cost(tree_root, turn, turn);
    for (int child_num=0; child_num < tree_root->child_list_length; child_num++) {
        // If propagated board cost of root and each leaf matches 
        // (from tree_board_cost), and they are not NULL. 
        if (tree_root->cost == tree_root->child_list[child_num]->cost && tree_root->child_list[child_num] != NULL) {
            source_row = tree_root->child_list[child_num]->source_row;
            source_col = tree_root->child_list[child_num]->source_col;
            target_row = tree_root->child_list[child_num]->target_row;
            target_col = tree_root->child_list[child_num]->target_col;
            return 1;
        }
    }
    // Releases all memory space associated with the tree structure.
    recursive_free_tree(tree_root);

    return 0;
}

void init_tree_board_cost(node_t* node, int intial_turn, int current_turn) {
    if (current_turn-intial_turn==TREE_DEPTH) {
        node->cost = board_cost(node->board);
        return;
    }

    int apparent_turn = current_turn % 2;

    if (apparent_turn) {
        // Calculates best moves for white
        node->cost = INT_MAX;
    }
    else {
        // Calculates best move for black
        node->cost = INT_MIN;
    }

    for (int child_num=0; child_num < node->child_list_length; child_num++) {
        if (node->child_list[child_num] != NULL) {
            init_tree_board_cost(node->child_list[child_num], intial_turn, 
            current_turn+1);
            // Propagates optimal board cost from the best-case left-most 
            // node/leaf up the tree, to the root
            if ((apparent_turn && node->child_list[child_num]->cost < node->cost) || (!(apparent_turn) && node->child_list[child_num]->cost > node->cost)) {
                node->cost = node->child_list[child_num]->cost;
            }
        }
    }
}

void insert_child_node(node_t* node, int child_num, int source_row, 
    char source_col, int target_row, char target_col, int tree_depth, int turn) {
    node->child_list[child_num] = (node_t*)malloc(sizeof(node_t));
    for (int row=0; row<BOARD_SIZE; row++) {
        for (int col=0; col<BOARD_SIZE; col++) {
            node->board[row][col]=node->child_list[child_num]->board[row][col];
        }
    }
    node->child_list[child_num]->source_row = source_row;
    node->child_list[child_num]->source_col = source_col;
    node->child_list[child_num]->target_row = target_row;
    node->child_list[child_num]->target_col = target_col;
    move_piece(node->board,
    node->child_list[child_num]->source_row,
    node->child_list[child_num]->source_col,
    node->child_list[child_num]->target_row,
    node->child_list[child_num]->target_col,
    0, 1);
    make_empty_tree(node->child_list[child_num], tree_depth-1, turn+1);
}

void recursive_free_tree(node_t* node) {
    for (int child_num=0; child_num < node->child_list_length; child_num++) {
        if (node->child_list[child_num] != NULL) {
            recursive_free_tree(node->child_list[child_num]);
        }
    }
    free(node->child_list);
    free(node);
}

/* Algorithms are fun :') */
/* THE END -------------------------------------------------------------------*/