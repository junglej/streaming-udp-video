// This program opens a socket that listens to incoming UDP packets. When a
// video frame packet is received, it will be decoded and displayed in a GUI
// window.

#include <iostream>
#include <vector>

#include "protocols/basic_protocol.h"
#include "receiver/receiver_socket.h"
#include "util/util.h"

using udp_streaming_video::BasicProtocolData;
using udp_streaming_video::ReceiverSocket;

int main(int argc, char** argv) {
  const int port = udp_streaming_video::util::ProcessPortParam(argc, argv);
  if (port < 0) {
    return -1;
  }
  const ReceiverSocket socket(port);
  if (!socket.BindSocketToListen()) {
    std::cerr << "Could not bind socket." << std::endl;
    return -1;
  }
  std::cout << "Listening on port " << port << "." << std::endl;
  BasicProtocolData protocol_data;

  clock_t last_cycle = clock();
  while (true) {  // TODO: break out cleanly when done.
    std::cout << "####################################################" << std::endl;
    clock_t next_cycle = clock();
    protocol_data.UnpackData(socket.GetPacket());
    protocol_data.GetImage().Display();

    double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
    std::cout << "effective FPS:" << (1 / duration) << " kbps" << std::endl;

    std::cout << "Time difference is " << next_cycle - last_cycle <<std::endl;
    last_cycle = next_cycle;
  }
  return 0;
}
