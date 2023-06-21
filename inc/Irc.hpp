#pragma once
#include "ft_irc.hpp"

#include	"Command.hpp"
#include 	"AwayCommand.hpp"
#include 	"CapCommand.hpp"
#include	"InviteCommand.hpp"
#include	"JoinCommand.hpp"
#include	"KickCommand.hpp"
#include	"ModeCommand.hpp"
#include	"NickCommand.hpp"
#include	"PartCommand.hpp"
#include	"PassCommand.hpp"
#include	"PrivMsgCommand.hpp"
#include	"TopicCommand.hpp"
#include	"UserCommand.hpp"


class Command;
class Client;
class User;
class Channel;
class Reply;

class	Irc{
	Command *command;
	public:
		//Const & destr
		static Irc &getInstance(std::string port = "ircd", std::string pass = "123", std::string name = "ircserv") // Init and retrieve server instance
		{
			static Irc	instance(port, pass, name);
			return instance;
		}
		~Irc();

		//Utils
		void			printAi() const;

		//Setter
		void			addUser(int const &sfd);
		void			addClient(int const &sfd);

		//Getter
		const	int				&getSocket() const;
		const	struct addrinfo			*getAi() const;
		const	std::string			&getPass() const;
		std::map<int, Client *>			&getClients();
		void					addReply(Reply reply);
		const	std::string			&getName() const;
		const Client 				*getUserByNick(std::string const nick) const;
		void					promoteClient(Client *client);

		int	computeFdMax() const;
		int	main_loop();


		// Command manager
		void							initCommand();
		std::map<std::string, Command*>	getCommandList();


	private:
		// Const & destr
		Irc();
		Irc(std::string port, std::string passwdm, std::string name);

		// Private Copy and assignement to enforce singularity
		Irc(const Irc &);
		Irc &operator=(const Irc &);

		// Client, User 
		std::vector<Channel>	_chans;
		std::map<int, Client *>	_clients;

		// Client Management
		int			setup_fds();

		//Commands
		std::map<std::string, Command*> commandList;

		// Server info
		struct	addrinfo		*_net;
		std::string			_name;
		std::string			_pass;
		int				_sockfd;

		//// Receiving data
		int			accept_client();
		int			receive_client_data(Client *user);
		int			data_reception_handler();
		int			manage_incoming_connection();
		int			retrieve_clients_packet();
		int			read_client_socket(Client &user);
		int			manageCommand();

		//// Send data
		std::vector<Reply>	_replies;
		int			sendReplies();

		// Setup Server interface
		void			setupAddrInfo(std::string port);
		// Setup socket interface
		int			setup_socket();
		int			set_socket_option();

		// Operation on fds
		void				setupFds();
		fd_set				fds[4];
};
