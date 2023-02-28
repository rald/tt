tt: main.c common.h ini.h irc.h strutil.h texttwist.h
	gcc main.c -o tt -lm -g -Wall -Wextra -pedantic

clean:
	rm tt
