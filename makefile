all: server run

server:
	@echo Building server...
	@g++ .\server.cpp .\src\HTTPparser.cpp .\src\HTTPReader.cpp .\src\Utils.cpp .\src\Socket.cpp .\src\ClientHandler.cpp -lws2_32 -o ./build/server.exe

run:
	@echo Running server...
	@./build/server.exe

clean:
	@echo Cleaning build directory...
	@rmdir /s /q build
	@mkdir build