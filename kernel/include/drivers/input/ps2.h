#ifndef DRIVERS_INPUT_PS2_H
#define DRIVERS_INPUT_PS2_H

/* Initialize the PS/2 Port for keyboard */
void ps2_init();
char ps2_getc(void);
uint8_t ps2_getsc_rel(void);
uint8_t ps2_getsc(void);

#endif /* DRIVERS_INPUT_PS2_H */
