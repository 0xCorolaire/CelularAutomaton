# AutoCelle in C++

# Celular automaton
What is a cellular automaton?
 
A cellular automaton consists of a grid of cells. A cell can have multiple states. Cells change state over time. The state of a cell at t + 1 depends on the state of the cells of its neighborhood at time t and the rule of the automaton. To advance the controller of a unit of time, the transition rule is applied to all cells of the start state simultaneously. We then obtain a new state, on which we can apply this transition again, and so on ... There are many types of cellular automata with dimensions, number of states, types of neighborhoods and different transition rules. In this project, we implemented five cellular automata.

# Game of Life

The automaton of the Game of Life is one of the best known cellular automata, because very interesting behaviors emerge from simple rules. It is an automaton on two dimensions, and its cells can take two states (dead or alive). The future state of a cell is determined by its current state and by the number of living cells among the eight surrounding it (Moore's neighborhood). Originally, the Game of Life as it was imagined by J. Conway, had the following rules: a living cell only survives if it is surrounded by 2 or 3 living cells, and a cell is born if she has exactly 3 neighbors. The remarkable aspect of this automaton is that it is Turing-complete: any algorithm can then be calculated, it is possible to program the game in the game. In our implementation, it is possible to choose the conditions by entering four numbers which represent the minimum and the maximum of neighboring cells necessary for the birth or survival of a cell. Initially this configuration is that of the traditional Game of Life

# Forest Fire
This cellular automaton is a simple representation of how a fire spreads in a forest. It is in two dimensions, and its cells can take four different states (tree, fire, ash and vacuum). The neighborhood chosen is the neighborhood of Von Neumann, that is to say that we look at the cells on the left, right, up and down of the cell that interests us. The transition rules are simple. A tree box with at least one neighbor on fire becomes on fire, a burning box becomes ash, an ash box becomes empty if it does not have a neighbor on fire, and an empty box remains empty. For this example the trees are green, the fire is red, the ashes are gray and the emptiness is white.

![alt text](https://raw.githubusercontent.com/hugofloter/AutoCell-C-/master/Photos/FireForest.png)

# D. Griffeath's cyclic automaton
This auto-reproductive cell automaton is two-dimensional, and its cells can take four states (red, brown, purple, purple). The state of a cell at time t + 1 depends on its state at time t and the state of its 8 neighbors (Moore's neighborhood). A cell moves from a state i to a state i + 1 in the state cycle when the state i + 1 is present in at least three neighboring cells.

![alt text](https://raw.githubusercontent.com/hugofloter/AutoCell-C-/master/Photos/Griffith.png)

# Brian's Brain
The "Brian's Brain" cellular automaton is a two-dimensional automaton, and its cells can take three states (living, dying and dead). The calculation of the state after transition of a cell is made with Moore's neighborhood. A cell is born if it has exactly two living neighbors, it becomes dying if it were alive, and dies if it were dying. In this example, the living cells are red, the dying cells are yellow, and the dead cells are black.

![alt text](https://raw.githubusercontent.com/hugofloter/AutoCell-C-/master/Photos/BB.png)
