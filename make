ser:Server.o Acceptor.o Channel.o EventLoop.o EventLoopThreadPool.o PollPoller.o Socket.o TcpConnection.o ThreadPool.o TcpServer.o
	g++ -o ser Server.o Acceptor.o Channel.o EventLoop.o EventLoopThreadPool.o PollPoller.o Socket.o TcpConnection.o ThreadPool.o TcpServer.o -lpthread
GDB=-g

Server.o:Server.cpp
	g++ -c Server.cpp -std=c++11 ${GDB}
Acceptor.o: Acceptor.cpp
	g++ -c Acceptor.cpp -std=c++11 ${GDB}

Channel.o: Channel.cpp
	g++ -c Channel.cpp -std=c++11 ${GDB}

PollPoller.o:PollPoller.cpp
	g++ -c PollPoller.cpp -std=c++11 ${GDB}

EventLoop.o: EventLoop.cpp
	g++ -c EventLoop.cpp -std=c++11 ${GDB}

EventLoopThreadPool.o:EventLoopThreadPool.cpp
	g++ -c EventLoopThreadPool.cpp -std=c++11 ${GDB}

Socket.o:Socket.cpp
	g++ -c Socket.cpp -std=c++11 ${GDB}

TcpConnection.o:TcpConnection.cpp
	g++ -c TcpConnection.cpp -std=c++11 ${GDB}

TcpServer.o:TcpServer.cpp
	g++ -c TcpServer.cpp -std=c++11 ${GDB}

ThreadPool.o:ThreadPool.cpp
	g++ -c ThreadPool.cpp -std=c++11 ${GDB}

clean:
	rm -rf *.o ser
