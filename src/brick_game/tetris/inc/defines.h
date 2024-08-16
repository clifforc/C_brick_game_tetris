/**
 * @file defines.h
 * @brief Contains the definitions of various constants used in the Tetris game.
 */

#ifndef DEFINES_H
#define DEFINES_H

#define TICKS_START 30 /**< Initial number of ticks. */

#define FIGURE_SIZE 5   /**< Size of tetris figure. */
#define FIGURES_COUNT 7 /**< Total number of tetris figures. */

#define WIDTH_FACTOR 2 /**< Factor used to calculate width of game field.*/

#define FIELD_WIDTH 10  /**< Width of the game field. */
#define FIELD_HEIGHT 20 /**< Height of the game field. */
#define FIELD_BORDERS 2 /**< Number of border lines around the game field. */

#define FIELD_START_X 17 /**< Starting x-coordinate of the game field. */
#define FIELD_START_Y 2  /**< Starting y-coordinate of the game field. */

#define NEXT_FIELD_WIDTH 12 /**< Width of the next figure field. */
#define NEXT_FIELD_HEIGHT 7 /**< Height of the next figure field. */
#define NEXT_FIELD_X 0      /**< x-coordinate of the next figure field. */
#define NEXT_FIELD_Y 10     /**< y-coordinate of the next figure field. */

#define FIGURE_START_X                 \
  (((FIELD_WIDTH - FIGURE_SIZE) / 2) + \
   1)                    /**< Starting x-coordinate of each Tetris figure. */
#define FIGURE_START_Y 0 /**< Starting y-coordinate of each Tetris figure. */

#define BASE_SPEED 3000000 /**< Base speed to decrese speed for each level. */

#endif
