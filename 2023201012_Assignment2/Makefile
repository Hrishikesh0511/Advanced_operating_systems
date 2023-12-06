output: main.o cd.o echo.o display.o ls.o pwd.o search.o pinfo.o redirection.o commands.o history.o
	g++ main.o cd.o echo.o display.o ls.o pwd.o search.o pinfo.o redirection.o commands.o history.o -o output
main.o: main.cpp
	g++ -c main.cpp
cd.o: cd.cpp
	g++ -c cd.cpp
echo.o: echo.cpp
	g++ -c echo.cpp
display.o: display.cpp
	g++ -c display.cpp
pwd.o: pwd.cpp
	g++ -c pwd.cpp
search.o: search.cpp
	g++ -c search.cpp
pinfo.o: pinfo.cpp
	g++ -c pinfo.cpp
redirection.o: redirection.cpp
	g++ -c redirection.cpp
commands.o: commands.cpp
	g++ -c commands.cpp
history.o: history.cpp
	g++ -c history.cpp
clean:
	rm *.o output