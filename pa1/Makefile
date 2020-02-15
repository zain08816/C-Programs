PROGRAMS = roman palindrome rle list mexp bst
MAKEFILES = $(foreach prog,$(PROGRAMS),src/$(prog)/Makefile)
MAINS = $(foreach prog,$(PROGRAMS),src/$(prog)/$(prog).c)
SOURCES = $(MAKEFILES) $(MAINS)

fulltest:
	./grader.py

1:
	./grader.py -1

clean:
	rm -rf build pa1.tar

tree: $(MAKEFILES) $(MAINS)

pa1.tar: $(SOURCES)
	tar -vzcf $@ $^

archive: pa1.tar
	./grader.py -a $^

$(MAINS):
	mkdir -p $(dir $@)
	sed '{s/PROGRAM/$(basename $(notdir $@))/; s/USER/$(LOGNAME)/}' stub.c > $@

$(MAKEFILES):
	mkdir -p $(dir $@)
	sed '{s/roman/$(word 2,$(subst /, ,$@))/}' Makefile_template > $@
