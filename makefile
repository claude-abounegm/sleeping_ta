sleepingTA:	main.h main.c queue.c queue.h rand_ts.c rand_ts.h signal_mutex.c signal_mutex.h Student.h
	gcc -Wall main.c queue.c rand_ts.c signal_mutex.c -o sleepingTA

clean:
	rm -f sleepingTA