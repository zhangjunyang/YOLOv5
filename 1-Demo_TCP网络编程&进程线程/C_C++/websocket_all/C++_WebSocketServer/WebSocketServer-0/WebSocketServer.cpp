#include "stdafx.h"
#include <iostream>

using namespace std;

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> WebsocketServer;
typedef WebsocketServer::message_ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

void OnOpen(WebsocketServer *server, websocketpp::connection_hdl hdl)
{
    cout << "have client connected" << endl;
}

void OnClose(WebsocketServer *server, websocketpp::connection_hdl hdl)
{
    cout << "have client disconnected" << endl;
}

void OnMessage(WebsocketServer *server, websocketpp::connection_hdl hdl, message_ptr msg)
{
    string strMsg = msg->get_payload();
    cout << strMsg << endl;

    string strRespon = "receive: ";
    strRespon.append(strMsg);

    server->send(hdl, strRespon, websocketpp::frame::opcode::text);
}

int main()
{
    WebsocketServer                    server;

    // Set logging settings
    server.set_access_channels(websocketpp::log::alevel::all);
    server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize ASIO
    server.init_asio();

    // Register our open handler
    server.set_open_handler(bind(&OnOpen, &server, ::_1));

    // Register our close handler
    server.set_close_handler(bind(&OnClose, &server, _1));

    // Register our message handler
    server.set_message_handler(bind(&OnMessage, &server, _1, _2));

    //Listen on port 2152
    server.listen(8090);

    //Start the server accept loop
    server.start_accept();

    //Start the ASIO io_service run loop
    server.run();

    return 0;
}