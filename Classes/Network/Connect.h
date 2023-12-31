#pragma once


#include <iostream>
#include <vector>
#include <string>
#include <asio.hpp>

using std::vector;
using std::string;
using asio::ip::tcp;

class ConPort {

	asio::io_context* io_context = nullptr;
	tcp::socket* socket = nullptr;

	string addr;
	int port;

public:
	ConPort(string addr, int port);
	bool checkValid();
	char* getPack(size_t size);
	bool sendPack(const char* data, size_t size);
	void connect(bool isServer);
};


inline ConPort::ConPort(string addr, int port) : addr(addr), port(port) {
}


inline void ConPort::connect(bool isServer) {
	delete this->io_context;
	this->io_context = new asio::io_context{};
	delete this->socket;
	this->socket = new tcp::socket{ *(this->io_context) };

	if (!isServer)
		this->socket->connect(tcp::endpoint(asio::ip::address::from_string(addr), port));
	else
	{
		tcp::acceptor acceptor(*(this->io_context), tcp::endpoint(tcp::v4(), port));

		acceptor.accept(*(this->socket));
	}
}


inline char* ConPort::getPack(size_t size) {
	if (this->socket == nullptr)
		return nullptr;
	if (this->socket->available() >= size)
	{
		vector<char> buf(size, 0);
		this->socket->read_some(asio::buffer(buf));
		char* re = new char[size];
		for (size_t i = 0; i < size; i++)
			re[i] = buf[i];
		return re;
	}
	else
		std::cout << this->socket->available() << "\n";
	return nullptr;
}

inline bool ConPort::sendPack(const char* data, size_t size) {
	if (this->socket == nullptr)
		return false;
	try
	{
		std::cout << "Send:" << this->socket->write_some(asio::buffer(data, size)) << "\n";
	}
	catch (...)
	{
		return false; //·¢ËÍÊ§°Ü
	}
}

inline bool ConPort::checkValid() {
	return this->socket->is_open();
}
