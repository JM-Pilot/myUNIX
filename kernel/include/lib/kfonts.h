#include <stdint.h>
#ifndef KFONTS_H
#define KFONTS_H


/* these files are not to be changed, they are located at misc/ (kernel/misc/)*/
extern uint8_t _binary_misc_Lat2_Terminus16_psfu_start[];
extern uint8_t _binary_misc_ter_u16n_psfu_start[];
extern uint8_t _binary_misc_ter_v16n_psfu_start[];

#define FONT_LAT2_TERMINUS16 _binary_misc_Lat2_Terminus16_psfu_start
#define FONT_TER_U16N _binary_misc_ter_u16n_psfu_start
#define FONT_TER_V16B _binary_misc_ter_v16n_psfu_start

#endif