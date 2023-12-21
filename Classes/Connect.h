#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <asio.hpp>

using std::vector;
using std::string;
using asio::ip::tcp;

class ConPort {

	asio::io_context io_context{};
	tcp::socket socket{ io_context };

	string addr;
	int port;

public:
	ConPort(string addr, int port);
	bool checkValid();
	char* getPack(size_t size);
	void connect();
};


inline ConPort::ConPort(string addr, int port) : addr(addr), port(port) {
	connect();
}


inline void ConPort::connect() {
	socket.connect(tcp::endpoint(asio::ip::address::from_string(addr), port));
}


inline char* ConPort::getPack(size_t size) {
	if (this->socket.available() >= size)
	{
		vector<char> buf(size, 0);
		this->socket.read_some(asio::buffer(buf));
		char* re = new char[size];
		for (size_t i = 0; i < size; i++)
			re[i] = buf[i];
		return re;
	}
	return nullptr;
}

inline bool ConPort::checkValid() {
	return this->socket.is_open();
}
