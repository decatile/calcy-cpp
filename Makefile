all: clean compile
	build/out file.calc

compile:
	mkdir -p build
	bison -o src/parser.gen.cpp -d src/res/parser.y
	flex -o src/lexer.gen.cpp src/res/lexer.lex
	clang++ src/*.cpp -o build/out

clean:
	rm -rf build

bear:
	bear -- make compile
