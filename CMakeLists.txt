cmake_minimum_required(VERSION 3.15)
project(UPS C)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS -pthread)

add_executable(UPS main.c globalVariable.h server.c server.h game.c game.h player.c player.h main.h globalVariable.c logger.c logger.h connectionHandler.c connectionHandler.h serverListener.c serverListener.h gameController.c gameController.h)