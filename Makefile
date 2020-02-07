.PHONY:	tp5

tp5: 
	g++ -c -Wall -O3 *.cpp && g++ *.o -o output -lpthread -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system && ./output