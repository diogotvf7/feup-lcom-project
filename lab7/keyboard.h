#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include "utils.h"
#include "i8042.h"

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for using the i8042 kbd
 */
uint8_t (get_scancode)(); 

/**
 * @brief Subscribes and enables KBD 1 interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes KBD 1 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Handles keyboard interrupts (C implementation)
 * 
 * Reads the status register and the output buffer (OB).
 * If there was some error, the byte read from the OB should be discarded.
 * 
 * All communication with other code must be done via global variables, static if possible.
 */
void (kbc_ih)();

int (read_keyboard_status)(uint8_t *st);

int (read_kbc_scancode)(uint8_t st, uint8_t *scancode);

int (keyboard_write)(int port, uint8_t cmd);

int (keyboard_restore)();

int read_letter(int scancode, int* word_guess, int* number_letters);

#endif /* __KEYBOARD_H */
