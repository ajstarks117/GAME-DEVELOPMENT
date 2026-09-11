import Phaser from "phaser";
import "./style.css";

class GameScene extends Phaser.Scene {
  constructor() {
    super("GameScene");

    this.board = [
      ["", "", ""],
      ["", "", ""],
      ["", "", ""],
    ];

    this.emptyLoc = 9;

    this.gameOver = false;
    this.aiThinking = false;

    this.cells = [[], [], []];
    this.marks = [[], [], []];

    this.titleText = null;
    this.statusText = null;
    this.restartButton = null;
  }

  create() {
    const cellSize = 150;
    const boardSize = cellSize * 3;

    const startX = (this.scale.width - boardSize) / 2 + cellSize / 2;

    const startY = (this.scale.height - boardSize) / 2 + cellSize / 2 + 20;

    this.cells = [[], [], []];
    this.marks = [[], [], []];

    this.titleText = this.add
      .text(this.scale.width / 2, 60, "TIC TAC TOE", {
        fontSize: "42px",
        fontStyle: "bold",
        color: "#ffffff",
      })
      .setOrigin(0.5);

    for (let row = 0; row < 3; row++) {
      for (let col = 0; col < 3; col++) {
        const x = startX + col * cellSize;
        const y = startY + row * cellSize;

        const cell = this.add
          .rectangle(x, y, cellSize - 8, cellSize - 8)
          .setStrokeStyle(3, 0x374151);

        this.cells[row][col] = cell;

        cell.setInteractive();

        cell.on("pointerover", () => {
          if (
            !this.gameOver &&
            !this.aiThinking &&
            this.board[row][col] === ""
          ) {
            cell.setStrokeStyle(3, 0x6b7280);
          }
        });

        cell.on("pointerout", () => {
          cell.setStrokeStyle(3, 0x374151);
        });

        cell.on("pointerdown", () => {
          if (this.gameOver || this.aiThinking) {
            return;
          }

          if (this.makeMove(row, col, "O")) {
            const mark = this.add
              .text(x, y, "O", {
                fontSize: "90px",
                fontStyle: "bold",
                color: "#e90f0f",
                setShadow: true,
                shadow: {
                  offsetX: 0,
                  offsetY: 0,
                  color: "#e11919",
                  blur: 4,
                  fill: true,
                },
              })
              .setOrigin(0.5);

            this.marks[row][col] = mark;

            if (this.checkGameOver()) {
              return;
            }

            this.statusText.setText("AI Thinking...");
            this.aiThinking = true;

            this.makeAIMove();

            if (!this.checkGameOver()) {
              this.statusText.setText("Your Turn");
              this.aiThinking = false;
            }
          }
        });
      }
    }

    this.statusText = this.add
      .text(this.scale.width / 2, 115, "Your Turn", {
        fontSize: "24px",
        fontStyle: "bold",
        color: "#2fff00",
      })
      .setOrigin(0.5);

    this.restartButton = this.add
      .text(this.scale.width / 2, this.scale.height - 70, "RESTART", {
        fontSize: "22px",
        fontstyle: "bold",
        color: "#ffffff",
        backgroundColor: "#6751e5",
        padding: {
          left: 24,
          right: 24,
          top: 12,
          bottom: 12,
        },
      })
      .setOrigin(0.5);

    this.restartButton.setInteractive();

    this.restartButton.on("pointerover", () => {
      this.restartButton.setStyle({ backgroundColor: "#4c0eeb" });
    });

    this.restartButton.on("pointerout", () => {
      this.restartButton.setStyle({ backgroundColor: "#6c48eb" });
    });

    this.restartButton.on("pointerdown", () => {
      this.restartGame();
    });
  }

  makeMove(row, col, player) {
    if (
      row >= 0 &&
      row < 3 &&
      col >= 0 &&
      col < 3 &&
      this.board[row][col] === ""
    ) {
      this.board[row][col] = player;

      if (this.emptyLoc > 0) {
        this.emptyLoc--;
      }

      return true;
    }

    return false;
  }

  eval() {
    // Check rows
    for (let i = 0; i < 3; i++) {
      if (
        this.board[i][0] === this.board[i][1] &&
        this.board[i][1] === this.board[i][2] &&
        this.board[i][0] !== ""
      ) {
        if (this.board[i][0] === "X") {
          return 10;
        } else {
          return -10;
        }
      }
    }

    // Check columns
    for (let i = 0; i < 3; i++) {
      if (
        this.board[0][i] === this.board[1][i] &&
        this.board[1][i] === this.board[2][i] &&
        this.board[0][i] !== ""
      ) {
        if (this.board[0][i] === "X") {
          return 10;
        } else {
          return -10;
        }
      }
    }

    // Check main diagonal
    if (
      this.board[0][0] === this.board[1][1] &&
      this.board[1][1] === this.board[2][2] &&
      this.board[0][0] !== ""
    ) {
      if (this.board[0][0] === "X") {
        return 10;
      } else {
        return -10;
      }
    }

    // Check other diagonal
    if (
      this.board[0][2] === this.board[1][1] &&
      this.board[1][1] === this.board[2][0] &&
      this.board[0][2] !== ""
    ) {
      if (this.board[0][2] === "X") {
        return 10;
      } else {
        return -10;
      }
    }

    return 0;
  }

  minMax(isMax, depth) {
    const score = this.eval();

    // AI wins
    if (score === 10) {
      return score - depth;
    }

    // Human wins
    if (score === -10) {
      return score + depth;
    }

    // Draw
    if (this.emptyLoc === 0) {
      return 0;
    }

    // MAX - AI
    if (isMax) {
      let best = -1000;

      for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
          if (this.board[i][j] === "") {
            this.board[i][j] = "X";
            this.emptyLoc--;

            const value = this.minMax(false, depth + 1);

            best = Math.max(best, value);

            // Undo move
            this.board[i][j] = "";
            this.emptyLoc++;
          }
        }
      }

      return best;
    }

    // MIN - Human
    else {
      let best = 1000;

      for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
          if (this.board[i][j] === "") {
            this.board[i][j] = "O";
            this.emptyLoc--;

            const value = this.minMax(true, depth + 1);

            best = Math.min(best, value);

            // Undo move
            this.board[i][j] = "";
            this.emptyLoc++;
          }
        }
      }

      return best;
    }
  }

  findBestMove() {
    let bestVal = -1000;

    let bestRow = -1;
    let bestCol = -1;

    for (let i = 0; i < 3; i++) {
      for (let j = 0; j < 3; j++) {
        if (this.board[i][j] === "") {
          // Try AI move
          this.board[i][j] = "X";
          this.emptyLoc--;

          // Start depth at 1 because AI has already made a move
          const moveVal = this.minMax(false, 1);

          // Undo AI move
          this.board[i][j] = "";
          this.emptyLoc++;

          // Keep best move
          if (moveVal > bestVal) {
            bestVal = moveVal;
            bestRow = i;
            bestCol = j;
          }
        }
      }
    }

    return {
      row: bestRow,
      col: bestCol,
    };
  }

  makeAIMove() {
    const bestMove = this.findBestMove();

    if (bestMove.row !== -1 && bestMove.col !== -1) {
      const row = bestMove.row;
      const col = bestMove.col;

      this.board[row][col] = "X";
      this.emptyLoc--;

      const cellSize = 150;
      const boardSize = cellSize * 3;

      const startX = (this.scale.width - boardSize) / 2 + cellSize / 2;

      const startY = (this.scale.height - boardSize) / 2 + cellSize / 2 + 20;

      const x = startX + col * cellSize;
      const y = startY + row * cellSize;

      const mark = this.add
        .text(x, y, "X", {
          fontSize: "90px",
          fontStyle: "bold",
          color: "#2020e9",
          setShadow: true,
          shadow: {
            offsetX: 0,
            offsetY: 0,
            color: "#2020e9",
            blur: 4,
            fill: true,
          },
        })
        .setOrigin(0.5);

      this.marks[row][col] = mark;
    }
  }

  checkGameOver() {
    const score = this.eval();

    if (score === 10) {
      this.statusText.setText("AI Wins!");

      this.gameOver = true;

      return true;
    }

    if (score === -10) {
      this.statusText.setText("You Win!");

      this.gameOver = true;

      return true;
    }

    if (this.emptyLoc === 0) {
      this.statusText.setText("Draw!");

      this.gameOver = true;

      return true;
    }

    return false;
  }

  restartGame() {
    this.board = [
      ["", "", ""],
      ["", "", ""],
      ["", "", ""],
    ];

    this.emptyLoc = 9;
    this.gameOver = false;
    this.aiThinking = false;

    for (let row = 0; row < 3; row++) {
      for (let col = 0; col < 3; col++) {
        if (this.marks[row][col]) {
          this.marks[row][col].destroy();
          this.marks[row][col] = null;
        }
      }
    }

    this.statusText.setText("Your Turn");
  }
}

const config = {
  type: Phaser.AUTO,

  scale: {
    mode: Phaser.Scale.RESIZE,
    autoCenter: Phaser.Scale.CENTER_BOTH,
  },

  width: window.innerWidth,
  height: window.innerHeight,

  backgroundColor: "#111827",

  scene: GameScene,
};

new Phaser.Game(config);
