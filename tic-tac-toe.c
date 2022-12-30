#include <ncurses.h>
#include <stdio.h>
#include <stdbool.h>

#define ROWS 3
#define COLS 3

char board[ROWS][COLS];
int cursor_x, cursor_y;
bool player;

void init_game() {
  // Initialize the board to all spaces
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      board[i][j] = ' ';
    }
  }

  // Initialize the cursor position
  cursor_x = 0;
  cursor_y = 0;

  // Initialize the player to X
  player = true;
}

void draw_board() {
  // Clear the screen
  clear();

  // Draw the top row of the board
  printw("  0   1   2\n");

  // Draw the rows of the board
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      // Calculate the cell position
      int row = i * 2 + 1;
      int col = j * 4 + 2;

      // Highlight the cell that the cursor is over
      if (i == cursor_x && j == cursor_y) {
        attron(A_REVERSE);
        mvprintw(row, col, "%c", board[i][j]);
        attroff(A_REVERSE);
      } else {
        mvprintw(row, col, "%c", board[i][j]);
      }

      // Print the cell separator
      if (j < COLS - 1) {
        mvprintw(row, col + 3, "|");
      }
    }
    // Print the line break
    if (i < ROWS - 1) {
      mvprintw(i * 2 + 2, 0, " ----|---|----");
    }
  }

  // Calculate the number of rows that the board occupies
  int rows = ROWS * 2 + 1;

  // Move the cursor to the correct position
  move(rows, 0);

  // Draw the current player
  printw("\nCurrent player: %c\n", player ? 'X' : 'O');

  // Print the instructions
  printw("Use arrow keys to move, Enter to place your mark.\n");

  // Calculate the cursor position
  int row = cursor_x * 2 + 1;
  int col = cursor_y * 4 + 2;

  // Draw the cursor
  move(row, col);
}



void handle_input() {
  // Get the input character
  int ch = getch();

  // Handle the input character
  switch (ch) {
    case KEY_LEFT:
      // Move the cursor left
      if (cursor_y > 0) {
        cursor_y--;
      }
      break;
    case KEY_RIGHT:
      // Move the cursor right
      if (cursor_y < COLS - 1) {
        cursor_y++;
      }
      break;
    case KEY_UP:
      // Move the cursor up
      if (cursor_x > 0) {
        cursor_x--;
      }
      break;
    case KEY_DOWN:
      // Move the cursor down
      if (cursor_x < ROWS - 1) {
        cursor_x++;
      }
      break;
    case '\n':
      // Place the current player's symbol on the board
      if (board[cursor_x][cursor_y] == ' ') {
        board[cursor_x][cursor_y] = player ? 'X' : 'O';
        player = !player;
      }
      break;
  }
}

int check_winner() {
  // Check for a horizontal win
  for (int i = 0; i < ROWS; i++) {
    if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
      return board[i][0];
    }
  }

  // Check for a vertical win
  for (int i = 0; i < COLS; i++) {
    if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
      return board[0][i];
    }
  }

  // Check for a diagonal win
  if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
    return board[0][0];
  }
  if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
    return board[0][2];
  }

  // No winner yet
  return ' ';
}

int main() {
  // Initialize ncursed
  initscr();
  noecho();
  keypad(stdscr, TRUE);

  // Hide the cursor
  curs_set(0);

  // Initialize the game
  init_game();

  // Main game loop
  while (true) {
    // Draw the board
    draw_board();

    // Check for a winner
    int winner = check_winner();
    if (winner != ' ') {
      // Display the winner
      clear();
      printw("%c wins!\n", winner);
      printw("Press 'ESC' to exit.\n");

      // Wait for the 'ESC' key
      int ch;
      do {
        ch = getch();
      } while (ch != 27);

      break;
    }

    // Check for a draw
    bool draw = true;
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (board[i][j] == ' ') {
          draw = false;
          break;
        }
      }
      if (!draw) {
        break;
      }
    }

    if (draw) {
      // Display the draw
      clear();
      printw("It's a draw!\n");
      printw("Press 'ESC' to exit.\n");

      // Wait for the 'ESC' key
      int ch;
      do {
        ch = getch();
      } while (ch != 27);

      break;
    }

    // Handle player input
    handle_input();
  }

  // Show the cursor again
  curs_set(1);

  // Clean up ncursed
  endwin();
  return 0;
}
