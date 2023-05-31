#pragma once
#include "ft_irc.hpp"

class Client;
class User;
class Channel;

class	Irc{
	public:
		//Const & destr
		static Irc &getInstance() // Init and retrieve server instance
		{
			static Irc	instance("ircd", "");
			return instance;
		}
		~Irc();

		//Utils
		void			printAi() const;

		//Setter
		void			addUser(int const &sfd);
		void			addClient(int const &sfd);

		//Getter
		const	int			&getSocket() const;
		const	struct addrinfo		*getAi() const;
		std::map<int, Client *>		&getClients();
		int				computeFdMax() const;

		int				main_loop();


	private:
		// Const & destr
		Irc();
		Irc(std::string port, std::string passwd);

		// Private Copy and assignement to enforce singularity
		Irc(const Irc &);
		Irc &operator=(const Irc &);

		// Client, User 
		std::vector<Channel>	_chans;
		std::map<int, Client *>	_clients; // Unregistered User, got promoted to User after


		// Server info
		struct	addrinfo	*_net;
		std::string		_pass;
		int			_sockfd;

		//Commands
		// Client Management
		int			setup_fds();
		void			promote_client(std::map<int, Client *>::iterator &_client);

		//// Receiving data
		int			accept_client();
		int			receive_client_data(Client *user);
		int			data_reception_handler();
		int			manage_incoming_connection();
		int			retrieve_clients_packet();
		int			read_client_socket(Client &user);

		//Setup socket interface
		int			setup_socket();
		int			set_socket_option();

		// Operation on fds
		fd_set				*getFd_set();
		fd_set				fds[4];
};
