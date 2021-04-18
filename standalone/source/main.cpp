/*
 * Copyright (C) 2011 Fredi Machado <https://github.com/fredimachado>
 * IRCClient is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "irc/IRCClient.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <signal.h>

void msgCommand(std::string arguments, IRCClient *client) {
    std::string to = arguments.substr(0, arguments.find(" "));
    std::string text = arguments.substr(arguments.find(" ") + 1);

    std::cout << "To " + to + ": " + text << std::endl;
    client->SendIRC("PRIVMSG " + to + " :" + text);
}

void joinCommand(std::string channel, IRCClient *client) {
    if (channel[0] != '#')
        channel = "#" + channel;

    client->SendIRC("JOIN " + channel);
}

void partCommand(std::string channel, IRCClient *client) {
    if (channel[0] != '#')
        channel = "#" + channel;

    client->SendIRC("PART " + channel);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Insuficient parameters: host port [nick] [user]"
                  << std::endl;
        return 1;
    }

    char *host = argv[1];
    int port = atoi(argv[2]);
    std::string nick("MyIRCClient");
    std::string user("IRCClient");

    if (argc >= 4)
        nick = argv[3];
    if (argc >= 5)
        user = argv[4];

    IRCClient client;

    client.Debug(true);

    if (client.InitSocket()) {
        std::cout << "Socket initialized. Connecting..." << std::endl;

        if (client.Connect(host, port)) {
            std::cout << "Connected. Loggin in..." << std::endl;

            if (client.Login(nick, user)) {
                std::cout << "Logged." << std::endl;

                while (client.Connected())
                    client.ReceiveData();
            }

            if (client.Connected())
                client.Disconnect();

            std::cout << "Disconnected." << std::endl;
        }
    }
}
