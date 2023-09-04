/*
 * tomatoanon.c
 *
 * Copyright (C) 2012 shibby
 *
 *
 * Changes for FreshTomato AX branch 2023
 * 
 */


#include <rc.h>
#include <stdlib.h>
#include <bcmnvram.h>
#include <shared.h>

int tomatoanon_main(int argc, char *argv[])
{
	if (argc > 2) {
		usage_exit(argv[0], "None|-start|-stop");
		return 1;
	}

	if (argc == 1) { /* just start anon! */
		notify_rc("start_tomatoanon");
		return 0;
	}
	else if (strcmp(argv[1], "-start") == 0) {
		notify_rc("start_tomatoanon");
		return 0;
	}
	else if (strcmp(argv[1], "-stop") == 0) {
		notify_rc("stop_tomatoanon");
		return 0;
	}
	else {
		usage_exit(argv[0], "None|-start|-stop");
		return 1;
	}
}

void start_tomatoanon(void)
{
	/* only if enable... */
	if ((!nvram_match("tomatoanon_enable", "1")) || (!nvram_match("tomatoanon_answer", "1")))
		return;

	xstart("tomatoanon_serv");
	logmessage("tomatoanon", "- start TomatoAnon update %s", (nvram_match("tomatoanon_notify", "1") ? "and check version" : ""));
}

void stop_tomatoanon(void)
{
	xstart("cru", "d", "anonupdate");
	xstart("cru", "d", "checkver");
	logmessage("tomatoanon", "- stop TomatoAnon update and check version");
}
