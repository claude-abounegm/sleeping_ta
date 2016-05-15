sleepingTA:	main.h main.c queue.c queue.h rand_ts.c rand_ts.h signal_mutex.c signal_mutex.h
	gcc -Wall main.c queue.c rand_ts.c signal_mutex.c -o sleepingTA -lpthread

clean:
	rm -f sleepingTA