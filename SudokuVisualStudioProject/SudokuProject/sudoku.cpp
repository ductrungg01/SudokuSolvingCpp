#include "sudoku.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "hidden_singles.h"
#include <string>

int main(int argc, char** argv)
{
    /*if (argc != 2)
    {
        printf("Usage: %s <sudoku string>\n", argv[0]);
        return 1;
    }*/

    SudokuBoard* board = (SudokuBoard*)malloc(sizeof(SudokuBoard));
    init_sudoku(board);
    load_sudoku(board, "000004028406000005100030600000301000087000140000709000002010003900000507670400000");

    Cell** p_solved_cells = board->solved_cells;
    int solved_counter = board->solved_counter;

    int ct = 0;

    while (board->solved_counter < BOARD_SIZE * BOARD_SIZE)
    {
        ct++;
        if (ct > 20) break;

        print_board(board, true);

        printf("SOLVED CELLS COUNTER = %d\n", board->solved_counter);

        solved_counter = check_solved_cells(board, &p_solved_cells);
        printf("check_solved_cells %d\n", solved_counter);

        bool is_show_possible = show_possible(board, p_solved_cells, solved_counter);
        if (is_show_possible)
        {
            printf("show_possible -> Yes\n");
            continue;
        }
        solved_counter = hidden_singles(board);
        if (solved_counter)
        {
            printf("hidden_singles %d\n", solved_counter);
            show_possible(board, p_solved_cells, solved_counter);
            continue;
        }
    }

    printf("Solution: ");
    print_solution(board);

    // clean up
    free_sudoku(board);
    free(board);
    return 0;
}