#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "message.hxx"
#include "send.hxx"

int send(const message &reference_to_message)
{
  auto descriptor_socket{socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)};

  if (descriptor_socket < 0)
  {
    return 1;
  }

  struct sockaddr_in address{};

  std::memset(&address, '\0', sizeof(struct sockaddr_in));
  address.sin_family = AF_INET;

  if (inet_pton(AF_INET, "1.1.1.1", &address.sin_addr) != 1) {
    close(descriptor_socket);
  }

  char packet[sizeof(struct icmp) + 56]{'\0'};
  struct icmp* pointer_to_packet{reinterpret_cast<struct icmp*>(&packet[0])};

  pointer_to_packet->icmp_type = ICMP_ECHO;
  pointer_to_packet->icmp_code = 0;
  pointer_to_packet->icmp_id = 1;
  pointer_to_packet->icmp_seq = 1;
  pointer_to_packet->icmp_cksum = 0;

  memset(pointer_to_packet->icmp_data, '\0', 56u);

  ssize_t amounts_sent_bytes{sendto(descriptor_socket, packet, sizeof(packet), 0, reinterpret_cast<struct sockaddr*>(&address), sizeof(address))};

  if (amounts_sent_bytes <= 0) {
    close(descriptor_socket);
    return 1;
  }

  close(descriptor_socket);
  return 0;
}
