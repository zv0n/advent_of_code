CXX ?= c++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -pedantic -O2
PROJECT = PROJECT_NAME

all: ${PROJECT}

${PROJECT}: main.cpp
	${CXX} ${CXXFLAGS} -o $@ $^

test: ${PROJECT}
	./${PROJECT}

clean:
	${RM} *.o ${PROJECT}

.PHONY: all clean test
