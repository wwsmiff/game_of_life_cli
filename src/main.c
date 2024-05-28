#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 64
#define HEIGHT 32
#define CHARS ".*"

#define UNUSED(x) (void)(x)

typedef int32_t board_t[HEIGHT][WIDTH];

void board_load_from_file(board_t board, const char *path) {
  FILE *input_file = fopen(path, "r");
  if (input_file == NULL) {
    fprintf(stderr, "Unable to open %s.\n", path);
    exit(1);
  } else {
    char *text_buffer = (char *)(malloc(WIDTH + 1));
    for (int32_t i = 0; i < HEIGHT; ++i) {
      fgets(text_buffer, WIDTH + 1, input_file);
      char _ = fgetc(input_file);
      UNUSED(_);
      for (int32_t j = 0; j < WIDTH; ++j) {
        if (text_buffer[j] == '0') {
          board[i][j] = 0;
        } else if (text_buffer[j] == '1') {
          board[i][j] = 1;
        } else if (text_buffer[j] == '\n' || text_buffer[j] == '\0') {
          break;
        } else {
          fprintf(stderr, "Unknown character '%c'", text_buffer[j]);
        }
      }
    }
    free(text_buffer);
  }
}

void print_board(board_t board) {
  system("clear");
  printf("\n");
  for (int32_t i = 0; i < HEIGHT; ++i) {
    printf("\t\t\t\t\t");
    for (int32_t j = 0; j < WIDTH; ++j) {
      printf("%c", CHARS[board[i][j]]);
    }
    printf("\n");
  }
}

int32_t count_neighbors(board_t board, int32_t x, int32_t y) {
  const int32_t N = ((y - 1) + HEIGHT) % HEIGHT;
  const int32_t S = ((y + 1) + HEIGHT) % HEIGHT;
  const int32_t W = ((x - 1) + WIDTH) % WIDTH;
  const int32_t E = ((x + 1) + WIDTH) % WIDTH;

  const int32_t count = board[N][x] + board[S][x] + board[y][W] + board[y][E] +
                        board[N][W] + board[N][E] + board[S][W] + board[S][E];
  return count;
}

void board_fill_random(board_t board, float probability) {
  if (probability > 1.0f) {
    fprintf(stderr, "Probability cannot be > 1.0\n");
    exit(1);
  } else if (probability < 0.0f) {
    fprintf(stderr, "Probability cannot be < 0.0\n");
    exit(1);
  }
  for (int32_t i = 0; i < HEIGHT; ++i) {
    for (int32_t j = 0; j < WIDTH; ++j) {
      if (((rand() % 101) / 100.0f) > (1.0f - probability)) {
        board[i][j] = 1;
      }
    }
  }
}

void board_update(board_t board, board_t buffer) {
  for (int32_t i = 0; i < HEIGHT; ++i) {
    for (int32_t j = 0; j < WIDTH; ++j) {
      int32_t neighbor_count = count_neighbors(board, j, i);
      if (neighbor_count < 2 || neighbor_count > 3) {
        buffer[i][j] = 0;
      } else if (neighbor_count == 3) {
        buffer[i][j] = 1;
      } else {
        buffer[i][j] = board[i][j];
      }
    }
  }
  memcpy(board, buffer, WIDTH * HEIGHT * sizeof(int32_t));
}

int main() {
  srand(time(NULL));
  board_t board = {0};
  board_t buffer = {0};
  board_load_from_file(board, "initial_state.txt");
  clock_t start = clock();
  float elapsed = 0.0f;
  print_board(board);
  while (1) {
    clock_t current = clock();
    const float delta = (float)(current - start) * 1000.0f / CLOCKS_PER_SEC;
    elapsed += delta;
    if (elapsed >= 1000.0f) {
      board_update(board, buffer);
      print_board(board);
      elapsed = 0.0f;
    }
    start = current;
  }
  return 0;
}
