#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#include "lwipopts_conf.h"

// Workaround for Linux timeval
//#if defined (TOOLCHAIN_GCC)
//#define LWIP_TIMEVAL_PRIVATE 0
//#include <sys/time.h>
//#endif

// Operating System 
#define NO_SYS                      1
#define LWIP_IPV4                   1
#define LWIP_IPV6                   0
#define MEM_LIBC_MALLOC				1

#define LWIP_DEBUG					1

#define ADDR_TIMEOUT                0
#define PREF_IPV4                   1
#define PREF_IPV6                   2
#define IP_VERSION_PREF             PREF_IPV4

// 32-bit alignment
#define MEM_ALIGNMENT               4

#define LWIP_RAM_HEAP_POINTER       lwip_ram_heap

// Number of pool pbufs.
// Each requires 684 bytes of RAM.
#ifndef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE              5
#endif

// One tcp_pcb_listen is needed for each TCPServer.
// Each requires 72 bytes of RAM.
#define MEMP_NUM_TCP_PCB_LISTEN     4

// One is tcp_pcb needed for each TCPSocket.
// Each requires 196 bytes of RAM.
#define MEMP_NUM_TCP_PCB            4

// One udp_pcb is needed for each UDPSocket.
// Each requires 84 bytes of RAM (total rounded to multiple of 512).
#define MEMP_NUM_UDP_PCB            4

// Number of non-pool pbufs.
// Each requires 92 bytes of RAM.
#ifndef MEMP_NUM_PBUF
#define MEMP_NUM_PBUF               8
#endif

// Each netbuf requires 64 bytes of RAM.
#ifndef MEMP_NUM_NETBUF
#define MEMP_NUM_NETBUF             8
#endif

// One netconn is needed for each UDPSocket, TCPSocket or TCPServer.
// Each requires 236 bytes of RAM (total rounded to multiple of 512).
#define MEMP_NUM_NETCONN            4

#define TCP_QUEUE_OOSEQ             0
#define TCP_OVERSIZE                0

#define LWIP_DHCP                   0
#define LWIP_DNS                    0
#define LWIP_UDP					1
#define LWIP_TCP					0
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0

#define SO_REUSE                    1

// Support Multicast
#include "stdlib.h"
//#define LWIP_IGMP                   LWIP_IPV4
#define LWIP_RAND()                 rand()

#define LWIP_COMPAT_SOCKETS         0
#define LWIP_POSIX_SOCKETS_IO_NAMES 0
#define LWIP_SO_RCVTIMEO            1
#define LWIP_TCP_KEEPALIVE          1

// Fragmentation on, as per IPv4 default
#define LWIP_IPV6_FRAG              LWIP_IPV6

// Queuing "disabled", as per IPv4 default (so actually queues 1)
#define LWIP_ND6_QUEUEING           0

// Debug Options
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define API_LIB_DEBUG               LWIP_DBG_ON
#define API_MSG_DEBUG               LWIP_DBG_ON
#define SOCKETS_DEBUG               LWIP_DBG_ON
#define ICMP_DEBUG                  LWIP_DBG_ON
#define IGMP_DEBUG                  LWIP_DBG_OFF
#define INET_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define IP_REASS_DEBUG              LWIP_DBG_ON
#define RAW_DEBUG                   LWIP_DBG_OFF
#define MEM_DEBUG                   LWIP_DBG_OFF
#define MEMP_DEBUG                  LWIP_DBG_OFF
#define SYS_DEBUG                   LWIP_DBG_ON
#define TIMERS_DEBUG                LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_ON
#define TCP_INPUT_DEBUG             LWIP_DBG_ON
#define TCP_FR_DEBUG                LWIP_DBG_ON
#define TCP_RTO_DEBUG               LWIP_DBG_ON
#define TCP_CWND_DEBUG              LWIP_DBG_ON
#define TCP_WND_DEBUG               LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG            LWIP_DBG_ON
#define TCP_RST_DEBUG               LWIP_DBG_ON
#define TCP_QLEN_DEBUG              LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define TCPIP_DEBUG                 LWIP_DBG_OFF
#define SLIP_DEBUG                  LWIP_DBG_ON
#define DHCP_DEBUG                  LWIP_DBG_OFF
#define AUTOIP_DEBUG                LWIP_DBG_ON
#define DNS_DEBUG                   LWIP_DBG_OFF
#define IP6_DEBUG                   LWIP_DBG_ON

#define PPP_DEBUG                   LWIP_DBG_ON
#define ETHARP_DEBUG                LWIP_DBG_OFF
#define UDP_LPC_EMAC                LWIP_DBG_ON

#ifdef LWIP_DEBUG
#define MEMP_OVERFLOW_CHECK         1
#define MEMP_SANITY_CHECK           1
#else
#define LWIP_NOASSERT               1
#define LWIP_STATS                  0
#endif

#define LWIP_DBG_TYPES_ON           LWIP_DBG_ON
#define LWIP_DBG_MIN_LEVEL          LWIP_DBG_LEVEL_ALL

#define LWIP_PLATFORM_BYTESWAP      1

#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS 1

#if LWIP_TRANSPORT_ETHERNET

// Broadcast
#define IP_SOF_BROADCAST            1
#define IP_SOF_BROADCAST_RECV       1

#define LWIP_BROADCAST_PING         1

#define LWIP_CHECKSUM_ON_COPY       1

#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

#else
#error A transport mechanism (Ethernet or PPP) must be defined
#endif

#endif /* LWIPOPTS_H_ */

