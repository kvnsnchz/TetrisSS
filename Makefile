.PHONY:	tp5

tp5: 
	g++ -c -O3 -Wall *.cpp && g++ *.o -o output -lpthread -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system && ./output