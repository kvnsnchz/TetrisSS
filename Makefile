.PHONY:	tp5

tp5: 
	g++ -c -Wall *.cpp && g++ *.o -o output -lsfml-graphics -lsfml-window -lsfml-system && ./output