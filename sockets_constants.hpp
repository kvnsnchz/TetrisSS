#include <string>

using namespace std;

#ifndef SOCKETS_CONSTANTS
#define SOCKETS_CONSTANTS

namespace datatype {
    const string SERVER_INFO_REQUEST = "SERVER_INFORMATION_REQUEST";
    const string SERVER_INFO_RESPONSE = "SERVER_INFORMATION_RESPONSE";
    const string SERVER_CONN_REQUEST = "SERVER_CONNECTION_REQUEST";
    const string SERVER_CONN_RESPONSE_SUCCESS = "SERVER_CONNECTION_SUCCESS";
    const string SERVER_CONN_RESPONSE_ERROR = "SERVER_CONNECTION_ERROR";
    const string SERVER_DISCONNECTION = "SERVER_DISCONNECTION";
}

namespace ports_number {
    const unsigned short SERVER_PORT = 2004;
    const unsigned short CLIENT_PORT = 2005;
}

#endif
