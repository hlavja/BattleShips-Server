#
# Makefile server for Battleship game
#

CC = gcc

server : main.o game.o gameController.o globalVariable.o logger.o serverListener.o connectionHandler.o player.o server.o
	$(CC) -l pthread main.o game.o gameController.o globalVariable.o logger.o serverListener.o connectionHandler.o player.o server.o -o server

main.o : main.c
	$(CC)  -c -w main.c

game.o : game.c
	$(CC) -c -w game.c

player.o : player.c
	$(CC) -c -w player.c

gameController.o : gameController.c
	$(CC) -c -w gameController.c

globalVariable.o : globalVariable.c
	$(CC) -c -w globalVariable.c

logger.o : logger.c
	$(CC) -c -w logger.c -pthread

serverListener.o : serverListener.c
	$(CC) -c -w serverListener.c

server.o : server.c
	$(CC) -c -w server.c

connectionHandler.o : connectionHandler.c
	$(CC) -c -w connectionHandler.c -pthread

clean :
	rm main.o game.o gameController.o globalVariable.o logger.o serverListener.o connectionHandler.o player.o server.o