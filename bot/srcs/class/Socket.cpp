# include "Socket.hpp"

Sock::Sock():
_fd(),
_bi(0),
_buffer()
{
	std::cout << "Create Socket !" << std::endl;
}

int Sock::link(const char *host, int port) {
	struct sockaddr_in si;
	struct hostent *hostess_twinkies = gethostbyname(host);

	this->_fd = socket(PF_INET, SOCK_STREAM, 0);
	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr = *((struct in_addr*)hostess_twinkies->h_addr);
	memset(&(si.sin_zero), ' ', 8);

	if (connect(this->_fd, (struct sockaddr *)&si, sizeof(struct sockaddr)) == -1)
		this->_fd = 0;
	return (this->_fd);
}

int Sock::unlink() {
	return close(this->_fd);
}

int
	Sock::in(char* buf) {
	int	len;

	for (len = 0; recv(this->_fd, buf, 1, 0); len++, buf++) {
		if (*buf == '\n' && *(buf-1) == '\r') {
			*(buf+1) = ' ';
			return (len);
		}
	}
	return (0);
}

int
	Sock::out(const char* buf) {
	int	i;

	for (i = 0; *(short*)buf != 0x0a0d; i++, buf++);
	buf -= i;
	i += 2;
	return send(this->_fd, buf, i, 0);
}

Sock &
	Sock::operator <<(const std::string &msg) {
	const char *buf = msg.c_str();
	int	len = strlen(buf);

	memmove(this->_buffer + this->_bi, buf, len);
	this->_bi += len;
	if (buf[len-1] == '\n' && buf[len - 2] == '\r') {
		send(this->_fd, this->_buffer, this->_bi, 0);
		this->_bi = 0;
	}
	return (*this);
}
