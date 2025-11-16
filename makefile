all_win: server_win run_win
all_lin: server_lin run_lin

server_win:
	@echo Building server...
	@g++ .\server.cpp .\src\HTTPparser.cpp .\src\HTTPReader.cpp .\src\Utils.cpp .\src\Socket.cpp .\src\ClientHandler.cpp -lws2_32 -o ./build/server.exe

server_lin:
	@echo Building server...
	@g++ ./server.cpp ./src/HTTPParser.cpp ./src/HTTPReader.cpp ./src/Utils.cpp ./src/Socket.cpp ./src/ClientHandler.cpp -o ./build/server.a

server_lin_ci:
	@echo Building server...
	@g++ ./server.cpp ./src/HTTPParser.cpp ./src/HTTPReader.cpp ./src/Utils.cpp ./src/Socket.cpp ./src/ClientHandler.cpp

run_win:
	@echo Running server...
	@./build/server.exe

run_lin:
	@echo Running server...
	@./build/server.a

cmake: 
	cd build 
	cmake -S . -B ./build 
	cmake --build ./build --config RelWithDebInfo

clean:
	@echo Cleaning build directory...
	@rmdir /s /q build
	@mkdir build