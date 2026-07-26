#ifndef BOOT_REQUESTS_H
#define BOOT_REQUESTS_H

#include <stdint.h>
#include <boot/limine.h>

extern volatile uint64_t limine_base_revision[];
extern volatile struct limine_framebuffer_request framebuffer_request; 

#endif