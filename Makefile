.PHONY: queue_tester tests
all: queue_tester tests

CC = g++
OPTIONS = -O2 -Wall

queue_tester: 
	${CC} ${OPTIONS} -o queue_tester main.cpp queue.cpp

tests:
	${CC} ${OPTIONS} -o tests tests.cpp queue.cpp

