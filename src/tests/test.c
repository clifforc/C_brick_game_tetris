#include <check.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../brick_game/tetris/inc/backend.h"
#include "../brick_game/tetris/inc/defines.h"
#include "../brick_game/tetris/inc/fsm.h"

START_TEST(test_game_init) {
  GameInfo_t* game = game_init();
  ck_assert_ptr_ne(game, NULL);
  ck_assert_ptr_ne(game->field, NULL);
  ck_assert_ptr_ne(game->next_figure, NULL);
  ck_assert_int_eq(game->score, 0);
  ck_assert_int_eq(game->level, 1);
  ck_assert_int_eq(game->speed, 0);
  ck_assert_ptr_ne(game->figure, NULL);
  ck_assert_int_eq(game->status, Pause);
  ck_assert_int_eq(game->action, IDLE);
  ck_assert_int_eq(game->ticks_left, TICKS_START);
  free_game_init(game);
  ck_assert(game->field == NULL);
  ck_assert(game->next_figure == NULL);
  ck_assert(game->figure == NULL);
}
END_TEST

START_TEST(test_init_game_field) {
  int** field = init_game_field();
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ck_assert_int_eq(field[i][j], 0);
    }
  }
  free_game_field(field);
}

START_TEST(test_init_figure) {
  Figure* figure = init_figure(FIGURE_START_X, FIGURE_START_Y);
  ck_assert_int_ge(figure->figure_num, 0);
  ck_assert_int_lt(figure->figure_num, FIGURES_COUNT);
  ck_assert_int_eq(figure->x, FIGURE_START_X);
  ck_assert_int_eq(figure->y, FIGURE_START_Y);
  free_figure(figure);
}

START_TEST(test_init_figure_valid_inputs) {
  int figure_x = 5;
  int figure_y = 5;
  Figure* f = init_figure(figure_x, figure_y);

  ck_assert_ptr_nonnull(f);
  ck_assert_int_eq(f->x, figure_x);
  ck_assert_int_eq(f->y, figure_y);
  ck_assert_ptr_nonnull(f->figure);

  for (int i = 0; i < FIGURE_SIZE; i++) ck_assert_ptr_nonnull(f->figure[i]);

  free_figure(f);
}
END_TEST

START_TEST(test_init_figure_invalid_inputs) {
  int figure_x = -1;
  int figure_y = -1;
  Figure* f = init_figure(figure_x, figure_y);

  ck_assert_ptr_nonnull(f);
  ck_assert_int_eq(f->x, figure_x);
  ck_assert_int_eq(f->y, figure_y);
  ck_assert_ptr_nonnull(f->figure);

  for (int i = 0; i < FIGURE_SIZE; i++) ck_assert_ptr_nonnull(f->figure[i]);

  free_figure(f);
}
END_TEST

START_TEST(test_get_random_figure) {
  Figure* figure = init_figure(0, 0);
  get_random_figure(figure);

  ck_assert_int_ge(figure->figure_num, 0);
  ck_assert_int_le(figure->figure_num, 6);

  int figures[7][FIGURE_SIZE][FIGURE_SIZE] = {
      // 0. z
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {1, 1, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0}},
      // 1. s
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 2, 2, 0, 0},
       {2, 2, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      // 2. T
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 3, 0, 0, 0},
       {3, 3, 3, 0, 0},
       {0, 0, 0, 0, 0}},
      // 3. L
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 4, 0, 0},
       {4, 4, 4, 0, 0},
       {0, 0, 0, 0, 0}},
      // 4. J
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {5, 0, 0, 0, 0},
       {5, 5, 5, 0, 0},
       {0, 0, 0, 0, 0}},
      // 5. square
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 6, 6, 0, 0},
       {0, 6, 6, 0, 0},
       {0, 0, 0, 0, 0}},
      // 6. bar
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {7, 7, 7, 7, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
  };

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      ck_assert_int_eq(figure->figure[i][j], figures[figure->figure_num][i][j]);
    }
  }

  free_figure(figure);
}
END_TEST

START_TEST(test_place_figure_on_field_valid) {
  GameInfo_t* game = game_init();

  place_figure_on_field(game);

  ck_assert_int_eq(game->field[2][9], 0);
  clear_figure_from_field(game);
  ck_assert_int_eq(game->field[2][9], 0);

  free_game_init(game);
}
END_TEST

START_TEST(test_collision_within_field) {
  GameInfo_t* game = game_init();

  ck_assert_int_eq(collision(game), 0);
  free_game_init(game);
}
END_TEST

START_TEST(test_drop_filled_lines_empty_field) {
  GameInfo_t* game = game_init();

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 0;
    }
  }
  drop_filled_lines(0, game);
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ck_assert_int_eq(game->field[i][j], 0);
    }
  }
  free_game_init(game);
}
END_TEST

START_TEST(test_drop_filled_lines_filled_field) {
  GameInfo_t* game = game_init();

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  drop_filled_lines(FIELD_HEIGHT - 1, game);
  for (int i = 0; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ck_assert_int_eq(game->field[i][j], 1);
    }
  }
  for (int j = 0; j < FIELD_WIDTH; j++) {
    ck_assert_int_eq(game->field[FIELD_HEIGHT - 1][j], 1);
  }
  free_game_init(game);
}
END_TEST

START_TEST(test_erase_and_score_one_line) {
  GameInfo_t* game = game_init();

  for (int j = 0; j < FIELD_WIDTH; j++) {
    game->field[FIELD_HEIGHT - 1][j] = 1;
  }
  erase_and_score(game);
  ck_assert_int_eq(game->score, 100);
  free_game_init(game);
}
END_TEST

START_TEST(test_erase_and_score_two_lines) {
  GameInfo_t* game = game_init();

  for (int i = FIELD_HEIGHT - 2; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  erase_and_score(game);
  ck_assert_int_eq(game->score, 300);
  free_game_init(game);
}
END_TEST

START_TEST(test_erase_and_score_three_lines) {
  GameInfo_t* game = game_init();

  for (int i = FIELD_HEIGHT - 3; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  erase_and_score(game);
  ck_assert_int_eq(game->score, 700);
  free_game_init(game);
}
END_TEST

START_TEST(test_erase_and_score_four_lines) {
  GameInfo_t* game = game_init();

  for (int i = FIELD_HEIGHT - 4; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  erase_and_score(game);
  ck_assert_int_eq(game->score, 1500);
  free_game_init(game);
}
END_TEST

START_TEST(test_check_ticks_no_collision) {
  GameInfo_t* game = game_init();

  game->ticks_left = 0;
  game->status = Start;

  check_ticks(game);

  ck_assert_int_eq(game->ticks_left, TICKS_START);
  ck_assert_int_eq(game->status, Start);
  free_game_init(game);
}
END_TEST

START_TEST(test_check_ticks_ticks_left_positive) {
  GameInfo_t* game = game_init();

  game->ticks_left = 5;
  game->status = Start;

  check_ticks(game);

  ck_assert_int_eq(game->ticks_left, 5);
  ck_assert_int_eq(game->status, Start);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_level) {
  GameInfo_t* game = game_init();

  game->score = 1200;
  game->level = 1;

  calculate_level(game);

  ck_assert_int_eq(game->level, 3);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_level_max) {
  GameInfo_t* game = game_init();

  game->score = 6000;
  game->level = 1;

  calculate_level(game);

  ck_assert_int_eq(game->level, 10);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_speed) {
  GameInfo_t* game = game_init();

  game->level = 2;

  calculate_speed(game);

  ck_assert_int_eq(game->speed, 2 * BASE_SPEED);
  free_game_init(game);
}
END_TEST

START_TEST(test_update_max_score) {
  GameInfo_t* game = game_init();

  game->score = 100;
  game->high_score = 50;

  update_max_score(game);
  ck_assert_int_eq(game->high_score, 100);

  game->score = 40;
  update_max_score(game);
  ck_assert_int_eq(game->high_score, 100);
  free_game_init(game);
}
END_TEST

START_TEST(test_save_max_score) {
  GameInfo_t* game = game_init();
  plant_check_collision_and_score(game);

  game->high_score = 200;

  save_max_score(game);

  FILE* file = fopen("max_score.txt", "r");
  ck_assert_msg(file != NULL, "File not found");

  int max_score;
  fscanf(file, "%d", &max_score);
  fclose(file);

  ck_assert_int_eq(max_score, 200);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_up) {
  GameInfo_t* game = game_init();
  game->action = Up;
  calculate_game(game);
  ck_assert_int_eq(game->figure->x, 3);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_left) {
  GameInfo_t* game = game_init();

  game->action = Left;
  calculate_game(game);
  ck_assert_int_eq(game->figure->x, 2);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_right) {
  GameInfo_t* game = game_init();

  game->action = Right;
  calculate_game(game);
  ck_assert_int_eq(game->figure->x, 4);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_down) {
  GameInfo_t* game = game_init();

  game->action = Down;
  calculate_game(game);
  ck_assert_int_eq(game->figure->y, 1);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_pause) {
  GameInfo_t* game = game_init();

  game->action = Pause;
  calculate_game(game);
  ck_assert_int_eq(game->status, Pause);

  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_terminate) {
  GameInfo_t* game = game_init();

  game->action = Terminate;
  calculate_game(game);
  ck_assert_int_eq(game->status, Terminate);
  free_game_init(game);
}
END_TEST

START_TEST(test_calculate_game_start) {
  GameInfo_t* game = game_init();

  game->action = Start;
  calculate_game(game);
  ck_assert_int_eq(game->status, Start);

  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_up) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, KEY_UP);
  ck_assert_int_eq(game->action, Up);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_left) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, KEY_LEFT);
  ck_assert_int_eq(game->action, Left);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_right) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, KEY_RIGHT);
  ck_assert_int_eq(game->action, Right);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_down) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, KEY_DOWN);
  ck_assert_int_eq(game->action, Down);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_action) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, 'x');
  ck_assert_int_eq(game->action, Action);
  get_user_action(game, 'X');
  ck_assert_int_eq(game->action, Action);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_pause) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, 'p');
  ck_assert_int_eq(game->action, Pause);
  get_user_action(game, 'P');
  ck_assert_int_eq(game->action, Pause);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_terminate) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, 'q');
  ck_assert_int_eq(game->action, Terminate);
  get_user_action(game, 'Q');
  ck_assert_int_eq(game->action, Terminate);
  free_game_init(game);
}
END_TEST

START_TEST(test_get_user_action_start) {
  GameInfo_t* game = game_init();

  game->status = Start;
  get_user_action(game, '\n');
  ck_assert_int_eq(game->action, Start);
  free_game_init(game);
}
END_TEST

Suite* backend_suite() {
  Suite* suite = suite_create("Backend");
  TCase* tc_init = tcase_create("Backend test case");
  tcase_add_test(tc_init, test_game_init);
  tcase_add_test(tc_init, test_init_game_field);
  tcase_add_test(tc_init, test_init_figure);
  tcase_add_test(tc_init, test_init_figure_valid_inputs);
  tcase_add_test(tc_init, test_init_figure_invalid_inputs);
  tcase_add_test(tc_init, test_get_random_figure);
  tcase_add_test(tc_init, test_place_figure_on_field_valid);
  tcase_add_test(tc_init, test_collision_within_field);
  tcase_add_test(tc_init, test_drop_filled_lines_empty_field);
  tcase_add_test(tc_init, test_drop_filled_lines_filled_field);
  tcase_add_test(tc_init, test_erase_and_score_one_line);
  tcase_add_test(tc_init, test_erase_and_score_two_lines);
  tcase_add_test(tc_init, test_erase_and_score_three_lines);
  tcase_add_test(tc_init, test_erase_and_score_four_lines);
  tcase_add_test(tc_init, test_check_ticks_no_collision);
  tcase_add_test(tc_init, test_check_ticks_ticks_left_positive);
  tcase_add_test(tc_init, test_calculate_level);
  tcase_add_test(tc_init, test_calculate_level_max);
  tcase_add_test(tc_init, test_calculate_speed);
  tcase_add_test(tc_init, test_update_max_score);
  tcase_add_test(tc_init, test_save_max_score);
  tcase_add_test(tc_init, test_calculate_game_up);
  tcase_add_test(tc_init, test_calculate_game_left);
  tcase_add_test(tc_init, test_calculate_game_right);
  tcase_add_test(tc_init, test_calculate_game_down);
  tcase_add_test(tc_init, test_calculate_game_pause);
  tcase_add_test(tc_init, test_calculate_game_terminate);
  tcase_add_test(tc_init, test_calculate_game_start);
  tcase_add_test(tc_init, test_get_user_action_up);
  tcase_add_test(tc_init, test_get_user_action_left);
  tcase_add_test(tc_init, test_get_user_action_right);
  tcase_add_test(tc_init, test_get_user_action_down);
  tcase_add_test(tc_init, test_get_user_action_action);
  tcase_add_test(tc_init, test_get_user_action_pause);
  tcase_add_test(tc_init, test_get_user_action_terminate);
  tcase_add_test(tc_init, test_get_user_action_start);

  suite_add_tcase(suite, tc_init);
  return suite;
}

int run_test_suite(Suite* (*suite_func)(), int* number_failed) {
  Suite* s = suite_func();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_VERBOSE);
  *number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return *number_failed;
}

int main() {
  int number_failed = 0;

  printf("\nBACKEND TESTS\n");
  run_test_suite(backend_suite, &number_failed);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}