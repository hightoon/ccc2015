all:
	gcc -Wall -g -o ut ut_main.c ../src/msg_prop_reader.c ../src/rtm_log_parser.c -lcmocka -DUNIT_TESTING -I../src/
	./ut

clean:
	rm -f ./ut
