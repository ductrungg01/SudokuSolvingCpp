#include "hidden_singles.h"
#include <stdlib.h>
#include <stdio.h>

int find_hidden_single_values(Cell** p_cells, int* hidden_single_values) {
    int counter = 0;

    for (int value = 1; value <= BOARD_SIZE; ++value) {
        int occurrence = 0;
        Cell* singleCell = nullptr;

        bool thisValueIsUsed = false;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (p_cells[i]->num_candidates > 1 && is_candidate(p_cells[i], value)) {
                ++occurrence;
                singleCell = p_cells[i];
            }
            else if (p_cells[i]->num_candidates == 1 && is_candidate(p_cells[i], value)) {
                thisValueIsUsed = true;
                break;
            }
        }

        if (occurrence == 1 && !thisValueIsUsed) {
            hidden_single_values[counter++] = value;
            singleCell->value = value;
        }
    }

    return counter;
}

void find_hidden_single(Cell** p_cells, HiddenSingle* p_hidden_singles, int* p_counter) {
    int hidden_single_values[BOARD_SIZE];
    int values_count = find_hidden_single_values(p_cells, hidden_single_values);

    for (int i = 0; i < values_count; ++i) {
        p_hidden_singles[*p_counter].p_cell = nullptr;
        p_hidden_singles[*p_counter].value = hidden_single_values[i];

        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (p_cells[j]->num_candidates > 0 && is_candidate(p_cells[j], hidden_single_values[i])) {
                p_hidden_singles[*p_counter].p_cell = p_cells[j];
                break;
            }
        }

        (*p_counter)++;
    }
}

// Set the solved cells and unset candidates
void modify_candidates(SudokuBoard* p_board, HiddenSingle* p_hidden_singles, int* p_counter) {
    int duplicateCounter = 0;
    
    for (int i = 0; i < *p_counter; ++i) {
        HiddenSingle* hidden_single = &p_hidden_singles[i];
        hidden_single->p_cell->value = hidden_single->value;

        if (is_in_list(p_board->solved_cells, p_board->solved_counter, hidden_single->p_cell)) {
            duplicateCounter++;
        }
        else {
            int* candidates = get_candidates(hidden_single->p_cell);
            int num_candidates = hidden_single->p_cell->num_candidates;
            for (int c = 0; c < num_candidates; c++) {
                unset_candidate(hidden_single->p_cell, candidates[c]);
            }
            set_candidate(hidden_single->p_cell, hidden_single->value);
            free(candidates);

            p_board->solved_cells[p_board->solved_counter++] = hidden_single->p_cell;

            printf("\n%d(%d:%d)", hidden_single->p_cell->value,
                hidden_single->p_cell->row_index + 1,
                hidden_single->p_cell->col_index + 1);
        }
    }

    (*(p_counter)) -= duplicateCounter;
}

int hidden_singles(SudokuBoard* p_board) {
    //printf("-----------------------\nHIDDEN_SINGLE: ");

    int counter = 0;
    HiddenSingle hidden_singles[BOARD_SIZE * 3];

    // Check hidden singles in rows
    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_single(p_board->p_rows[i], hidden_singles, &counter);
    }

    //modify_candidates(p_board, hidden_singles, &counter);
    //printf("\nRow done");

    // Check hidden singles in columns
    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_single(p_board->p_cols[i], hidden_singles, &counter);
    }

    /*modify_candidates(p_board, hidden_singles, &counter);
    printf("\nColumn done");*/

    // Check hidden singles in boxes
    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_single(p_board->p_boxes[i], hidden_singles, &counter);
    }

    modify_candidates(p_board, hidden_singles, &counter);
    //printf("\nBox done");

    //printf("\n---------------------\n");

    return counter;
}




