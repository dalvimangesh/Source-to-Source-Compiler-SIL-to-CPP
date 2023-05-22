build : lex yacc all
lex : ./src/compiler.l
	@mkdir -p bin
	@lex -o ./bin/lex.yy.c ./src/compiler.l
yacc : ./src/compiler.y
	@mkdir -p bin
	@yacc ./src/compiler.y -d -o ./bin/y.tab.c
all : ./src/compiler.l ./src/compiler.y
	@g++ -w ./bin/*.c ./src/*.cpp -o compiler.out
test : build compiler.out test.txt
	@./compiler.out test.txt test
tests : build compiler.out
	@mkdir -p AST_output
	@mkdir -p CPP_output
	@chmod +x run.sh
	@./run.sh
clean :
	if [ -d bin ]; then \
		rm -f bin/*; \
		rmdir bin; \
	fi
	if [ -e *.out ]; then \
		rm *.out; \
		fi
	if [ -e test.ast ]; then \
		rm test.ast; \
	fi
	if [ -e test.cpp ]; then \
		rm test.cpp; \
	fi
	if [ -d AST_output ]; then \
		rm -f AST_output/*; \
		rmdir AST_output; \
	fi
	if [ -d CPP_output ]; then \
		rm -f CPP_output/*; \
		rmdir CPP_output; \
	fi