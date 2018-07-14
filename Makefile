#
# Copyright (c) 2018 Hirochika Asai
# All rights reserved.
#
# Authors:
#      Hirochika Asai  <asai@jar.jp>
#

all:
	$(MAKE) mini4ops

y.tab.c: mini4ops.y
	$(YACC) -d $^
y.tab.h: mini4ops.y
	$(YACC) -d $^
lex.yy.c: mini4ops.l y.tab.h
	$(LEX) $^
syntax.o: syntax.c syntax.h

mini4ops: y.tab.o lex.yy.o syntax.o
	$(CC) -o $@ $^

clean:
	rm -f *.o mini4ops y.y.tab.c y.tab.h lex.yy.c

