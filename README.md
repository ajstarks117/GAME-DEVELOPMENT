# Game Development Projects

A collection of college game-development projects, experiments, and algorithm implementations.

## Projects

### Snake Game

A browser-based Snake game built with JavaScript, HTML, CSS, and Phaser loaded from a CDN.

- Folder: [`Snake-Game/`](Snake-Game/)
- Entry point: [`Snake-Game/index.html`](Snake-Game/index.html)

Open `Snake-Game/index.html` in a browser to play. A local static server is recommended for consistent browser behavior.

### Tic-Tac-Toe with Phaser

A Vite-powered Tic-Tac-Toe game using Phaser.

- Folder: [`TICTACTOE_PHASER/tictactoe-phaser-game/`](TICTACTOE_PHASER/tictactoe-phaser-game/)
- Entry point: [`TICTACTOE_PHASER/tictactoe-phaser-game/src/main.js`](TICTACTOE_PHASER/tictactoe-phaser-game/src/main.js)

Run it with:

```powershell
cd TICTACTOE_PHASER/tictactoe-phaser-game
npm install
npm run dev
```

Use the local URL printed by Vite in your browser.

### C++ Tic-Tac-Toe

Console-based Tic-Tac-Toe implementations using minimax and alpha-beta pruning:

- [`TIC_TAC_TOE.cpp`](TIC_TAC_TOE.cpp)
- [`DP.cpp`](DP.cpp), which includes dynamic-programming memoization

Compile and run either file with a C++17 compiler:

```powershell
g++ -std=c++17 TIC_TAC_TOE.cpp -o tic-tac-toe.exe
.\tic-tac-toe.exe
```

Replace `TIC_TAC_TOE.cpp` with `DP.cpp` to build the other implementation.

### Unity 2D Project

A Unity 2D project containing scenes, prefabs, and C# scripts.

- Folder: [`UNITY PROJECTS/My project/`](UNITY%20PROJECTS/My%20project/)
- Solution: [`UNITY PROJECTS/My project/My project.slnx`](UNITY%20PROJECTS/My%20project/My%20project.slnx)

Open the project folder in Unity Hub with a compatible Unity Editor version.

## Repository Structure

```text
.
├── DP.cpp
├── TIC_TAC_TOE.cpp
├── Snake-Game/
├── TICTACTOE_PHASER/
└── UNITY PROJECTS/
```

## Requirements

- A modern web browser
- Node.js and npm for the Phaser/Vite project
- A C++17 compiler for the console projects
- Unity Hub and a compatible Unity Editor for the Unity project
