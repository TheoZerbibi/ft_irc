use strict;
use Irssi;
use Irssi::Irc;

my $nickname = "hugin";
my $username = "my_user";
my $realname = "My Real Name";
my $channel = "#hello123";
my $host = "localhost";
my $port = 6667;


sub register {
	my ($data, $server, $witem) = @_;

	Irssi::Server::command($server, "NICK $nickname");
	Irssi::Server::command($server, "USER $username 0 * :$realname");
}

sub JOIN {
	my ($data, $server, $witem) = @_;
	
	Irssi::Server::command($server, "JOIN $channel");
}

sub event_connected {
	my ($data, $server, $witem) = @_;

	register($server);
	JOIN($server);
}


Irssi::command_bind register => \&event_connected;

# use strict;
# use Irssi;
# use Irssi::Irc;
# 
# my $nickname = "my_nick";
# my $username = "my_user";
# my $realname = "My Real Name";
# my $server   = "irc.example.com";
# my $port     = 6667;
# 
# sub register {
#     my ($server) = @_;
#     
#     $server->command("NICK $nickname");
#     $server->command("USER $username 0 * :$realname");
# }
# 
# sub event_connected {
#     my ($server) = @_;
#     
#     register($server);
# }
# 
# Irssi::signal_add_last('event connected', 'event_connected');

