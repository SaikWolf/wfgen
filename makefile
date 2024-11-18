

all:

MAKEWIDTH	:= 20
SOURCES 	:= $(wildcard src/*.cc)
HEADERS 	:= $(wildcard include/*.hh)
APPS 		:= $(wildcard apps/*.cc)
OBJECTS 	:= $(patsubst src/%.cc, build/src/%.o, ${SOURCES})
PROGRAMS	:= $(patsubst apps/%.cc, build/apps/%, ${APPS})

OMP_FLAGS 	:= -D ENABLE_OMP -fopenmp
CXXFLAGS	:= -std=c++17 -g -Wall -Wno-deprecated-declarations -I./include -I${VIRTUAL_ENV}/include ${OPT_FLAGS} -Wno-class-memaccess ${OMP_FLAGS}
LDFLAGS		:= -L${VIRTUAL_ENV}/lib -L./build/lib
LIBS		:= -lliquid -lfftw3f -pthread -lzmq -lczmq -luhd -lboost_system -lyaml

.phony: clean

builddir				:
	if [ ! -d "./build/src" ]; then \
		mkdir -p ./build/src; \
		mkdir -p ./build/apps; \
		mkdir -p ./build/include; \
		mkdir -p ./build/lib; \
	fi

clean					:
	rm -rf ./build

check-virtualenv		:
ifndef VIRTUAL_ENV
	$(error VIRTUAL_ENV is not set)
endif
	if [ ! -d "${VIRTUAL_ENV}/include" ]; then mkdir -p ${VIRTUAL_ENV}/include; fi

check-pybombs:
ifndef PYBOMBS_PREFIX
	$(error PYBOMBS_PREFIX is not set)
endif

${OBJECTS} : build/%.o : %.cc ${HEADERS} builddir
	g++ ${CXXFLAGS} -c -o $@ $<

${PROGRAMS} : build/% : %.cc ${OBJECTS} builddir
	g++ -I${PYBOMBS_PREFIX}/include ${CXXFLAGS} -L${PYBOMBS_PREFIX}/lib ${LDFLAGS} -o $@ $< ${OBJECTS} ${LIBS}

all						: check-virtualenv check-pybombs ${PROGRAMS}




install					: check-virtualenv all
	cp ./build/apps/* ${VIRTUAL_ENV}/bin/

uninstall				: check-virtualenv
	rm $(patsubst apps/%.cc, ${VIRTUAL_ENV}/bin/%, ${APPS})

