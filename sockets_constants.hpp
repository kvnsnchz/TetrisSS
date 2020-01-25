#include <string>

using namespace std;

#ifndef DATATYPE
#define DATATYPE
namespace datatype {
    const string REQUEST_CONNECTION = "REQUEST_CONNECTION";
    const string RESPONSE_CONNECTION = "RESPONSE_CONNECTION";
}
#endif

#ifndef PORTS_NUMBER
#define PORTS_NUMBER
namespace ports_number {
    const unsigned short SERVER_PORT = 2004;
    const unsigned short CLIENT_PORT = 2005;
}
#endif