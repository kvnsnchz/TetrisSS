.PHONY:	tp5

tp5: 
	g++ -c -Wall *.cpp && g++ *.o -o output -lpthread -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system && ./output