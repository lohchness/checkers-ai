# checkers-ai

Implementation of minimax algorithm to calculate the optimal moves for a checkers game

# INTRODUCTION

Checkers is a simple strategy game involving two players with game pieces on a checkered board. These pieces can be moved diagonally to your opponent's side, and can capture your opponent's pieces by moving over their piece. When your piece reaches the opposite side, it turns into a Tower, which can move up to two spaces, diagonally in all directions (Fig 1).

While the rules are simple, there are a seemingly infinite possibilities that a game can turn out, depending on the moves you take. This program uses a simple AI to simulate this game using the Minimax or Min-Max algorithm.

![Figure 1](https://github.com/lohchness/checkers-ai/blob/main/fig1.png?raw=true)

### MINIMAX

Put simply, minimax finds the best outcome of the game for the player. Each game state is given a score, and when computing the best positions, the AI will seek the **minimum** score for the opponent while **maximizing** its own. If it encounters game state possibilities which have the same scores, it should always return the first game state with the same scores that it encountered.

To put it not simply, the minimax algorithm is a very big tree of all possible outcomes of the pieces on the board. The deepest nodes of the tree, called leaf nodes, are game states that have played out its states completely, until there are no black OR white pieces left. The scores of the leaf nodes will be compared against each other at the previous node, with the lowest or biggest score being stored at the previous node depending on the depth the node is on (Fig 2). This method will be propagated for all leaf nodes, up the tree until it reaches the original game state from which the minimax algorithm was called upon.

![Figure 2](https://github.com/lohchness/checkers-ai/blob/main/fig2.png?raw=true)

### PROGRAM

The C Program will read stdin (input needs to be redirected to a file) containing moves of pieces, for example "A6-B5" or "E2-F3" to move pieces. The command "A" will compute the *next* best move, while "P" will compute the *next 10* best moves, or until it reaches a winning game state, whichever comes first. If the game ends, the winner will be reported. The computation of moves uses the minimax algorithm.

### AN EXAMPLE FOR TIME AND SPACE COMPLEXITY

Here are 2 graphs for test3.txt, up to a tree depth of 8. Unfortunately, I do not have enough RAM to build a tree depth of 9. This is one of a few examples I personally saw of exponential time and space complexity.

RAM Usage vs Tree Depth

![RAM usage vs tree depth](https://github.com/lohchness/checkers-ai/blob/main/ram%20usage%20vs%20tree%20depth.png?raw=true)

Runtime vs Tree Depth

![Runtime vs tree depth](https://github.com/lohchness/checkers-ai/blob/main/runtime%20vs%20tree%20depth.png?raw=true)

