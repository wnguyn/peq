.PHONY: build run clean

build:
	verilator --cc --exe --build --top-module peq -Wall \
		rtl/peq.v cpp/main.cpp -CFLAGS "-std=c++17"

run: build
	./obj_dir/Vpeq

clean:
	rm -rf obj_dir
