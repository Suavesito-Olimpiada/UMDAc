CC              = gcc
CCFLAGS         = -I$(INCDIR) -I$(LINDIR) -O2 -Wall -Werror -Wextra \
				  -pedantic -std=c11 -fsanitize=address \
				  -fsanitize=undefined
CCFLAGSDBG      = -I$(INCDIR) -I$(LINDIR) -O0 -g3 -Wall -Werror -Wextra \
				  -pedantic -std=c11 -fsanitize=undefined
#				  -fsanitize=address # Doesn't work in gdb

LIBFLAGS        = -lm -pthread
# For math and thread libraries

BLDDIR          = ../build
OBJDIR          = ../build/obj
OBJDIRDBG       = ../build/objdbg
INCDIR          = ../include
LIBDIR          = ../lib
LINDIR          = ../lib/include
TSTDIR          = ../test

_LIBINCLUDES    =
LIBINCLUDES     = $(patsubst %,$(INCDIR)/%,$(_INCLUDES))

_INCLUDES       = random.h umdac.h statistics.h minmax.h logger.h
INCLUDES        = $(patsubst %,$(INCDIR)/%,$(_INCLUDES))

_OBJECTS        = main.o random.o umdac.o statistics.o minmax.o logger.o
OBJECTS         = $(patsubst %,$(OBJDIR)/%,$(_OBJECTS))
OBJECTSDBG      = $(patsubst %,$(OBJDIRDBG)/%,$(_OBJECTS))

TARGET          = program
TARGETDBG       = programdbg

$(OBJDIR)/%.o: %.c $(INCLUDES) $(LIBINCLUDES)
	$(CC) -c -o $@ $< $(CCFLAGS) $(LIBFLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $(BLDDIR)/$@ $^ $(CCFLAGS) $(LIBFLAGS)

$(OBJDIRDBG)/%.o: %.c $(INCLUDES) $(LIBINCLUDES)
	$(CC) -c -o $@ $< $(CCFLAGSDBG) $(LIBFLAGS)

$(TARGETDBG): $(OBJECTSDBG)
	$(CC) -o $(BLDDIR)/$@ $^ $(CCFLAGSDBG) $(LIBFLAGS)

.PHONY: clean

clean:
	rm -f $(BLDDIR)/$(OBJDIR)/*.o $(BLDDIR)/$(TARGET)

.PHONY: clean-debug

clean-debug:
	rm -f $(BLDDIR)/$(OBJDIRDBG)/*.o $(BLDDIR)/$(TARGETDBG)

.PHONY: clean-test

clean-test:
	rm -f $(TSTDIR)/out/* $(TSTDIR)/program.log

.PHONY: run

run: ./$(TARGET)
	$(BLDDIR)/$(TARGET) ${ARGS}

.PHONY: debug

debug: ./$(TARGETDBG)
	gdb -tui --args $(BLDDIR)/$(TARGETDBG) ${ARGS}

.PHONY: test

test: ./$(TARGET)
	sh $(TSTDIR)/test.sh &>> $(TSTDIR)/program.log
