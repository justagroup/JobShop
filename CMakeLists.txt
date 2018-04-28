cmake_minimum_required(VERSION 3.9)
project(jss C)

set(CMAKE_C_STANDARD 11)

add_executable(dev src/io/file_classic.c src/core/jss_algo.h src/io/file.h src/core/algos/greedy/greedy.c src/core/algos/greedy/greedy.h src/main.c src/utils/dump_excange_s.h src/utils/dump_exchange_s.c)