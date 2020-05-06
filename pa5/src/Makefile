TARGET     = cachesim
CC         = clang
SANITIZERS = -fsanitize=address $(if $(findstring clang,$(CC)),-fsanitize=undefined)
OPT        = -o2
CFLAGS     = -g -std=c99 -Wall -Wvla -Werror $(SANITIZERS) $(OPT)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM
