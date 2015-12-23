COMPILER = clang++

logger: ./src/logger.cpp
	rm -rf ./build
	mkdir ./build
	$(COMPILER) -c ./src/logger.cpp -o ./build/logger.o `llvm-config --cxxflags` -O0
	$(COMPILER) -shared -o ./build/logger.so ./build/logger.o `llvm-config --ldflags --libs --system-libs`
	
test01: ./test/test01.cpp
	rm -rf ./test.build
	mkdir ./test.build
	clang++ -v ./test/test01.cpp -o ./test.build/orign.out -O0
	clang++ -v -Xclang -load -Xclang ./build/logger.so ./test/test01.cpp -o ./test.build/custom.out -O0
	./test.build/orign.out
	./test.build/custom.out
	
ir:
	rm -rf ./test.ll
	mkdir ./test.ll
	clang++ -emit-llvm -S -Xclang -load -Xclang ./build/logger.so  ./test/test01.cpp -o ./test.ll/custom.ll -O0
	clang++ -emit-llvm -S ./test/test01.cpp -o ./test.ll/orign.ll -O0

clean:
	rm -rf ./test.ll
	rm -rf ./test.build
	rm -rf ./build
