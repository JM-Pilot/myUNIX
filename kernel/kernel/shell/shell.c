#include <kernel/shell/shell.h>
#include <drivers/console/console.h>
#include <kstdio.h>
#include <kernel/kernel.h>
#include <string.h>
void kshell_init(void)
{
	console_enable_cursor(kcon);
	kprintf("Kernel Shell Initialized\n");
}
void kshell_interpret(void)
{
	char buf[2048];
	memset(buf, 0, sizeof(buf));
	kprintf("kernel $ ");
	kgets(buf, 2048);
	kprintf("\n");
	if (strcmp(buf, "help") == 0) {
		kprintf("========Kernel Shell=======\n");
		kprintf("Copyright (C) 2026 JM-Pilot\n");
		kprintf("List of Commands: \n"
				"help -- shows this menu\n"
				"hello -- hello world! :)\n"
				"clear -- clears the screen\n");
	}
	else if (strcmp(buf, "hello") == 0) {
		kprintf("Hello World!\n");
	}
	else if (strcmp(buf, "clear") == 0) {
		console_clear(kcon);
	}
	else {
		kprintf("[ERROR] Invalid Command \"%s\"\n", buf);
	}
}

