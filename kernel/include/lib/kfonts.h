#ifndef KFONTS_H
#define KFONTS_H

#include <stdint.h>


/* these files they are built in and not to be changed, they are located at misc/ (kernel/misc/)*/
extern uint8_t _binary_misc_Lat2_Terminus16_psfu_start[];
extern uint8_t _binary_misc_ter_u16n_psfu_start[];
extern uint8_t _binary_misc_ter_u18n_psfu_start[];

#define FONT_LAT2_TERMINUS16 _binary_misc_Lat2_Terminus16_psfu_start
#define FONT_TER_U16N _binary_misc_ter_u16n_psfu_start
#define FONT_TER_U18N _binary_misc_ter_u18n_psfu_start

#endif
