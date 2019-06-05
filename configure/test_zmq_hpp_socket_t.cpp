#include <zmq.hpp>

int main(int argc, char** argv)
{
  zmq::context_t c;
  zmq::socket_t s(c, ZMQ_REQ);
}
