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

mini4ops: y.tab.c lex.yy.c syntax.h
	$(CC) -o $@ $^

