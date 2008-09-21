#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#define NO_SYS 1
#define MEM_ALIGNMENT 4
#define MEM_SIZE 64000
#define MEMP_OVERFLOW_CHECK 1
#define MEMP_SANITY_CHECK 1
#define MEMP_NUM_PBUF 2048
#define MEMP_NUM_UDP_PCB 512
#define MEMP_NUM_TCP_PCB 512

#define LWIP_ARP 0
#define IP_REASSEMBLY 0
#define IP_FRAG 0

/*#define SYS_LIGHTWEIGHT_PROT           1 */

/* TCP */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         1

/* TCP Maximum segment size. */
#define TCP_MSS                 1024

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             2048

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
#define TCP_SND_QUEUELEN        (4 * TCP_SND_BUF/TCP_MSS)

/* TCP writable space (bytes). This must be less than or equal
   to TCP_SND_BUF. It is the amount of space which must be
   available in the tcp snd_buf for select to return writable */
#define TCP_SNDLOWAT		(TCP_SND_BUF/2)

/* TCP receive window. */
#define TCP_WND                 8096

/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              12

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           4

#undef LWIP_EVENT_API
#define LWIP_CALLBACK_API 1

#define LWIP_STATS	0

#define PPP_SUPPORT      0      /* Set > 0 for PPP */

#define ARP_QUEUEING 0
#define LWIP_SOCKET 0
#define LWIP_NETCONN 0

#define LWIP_DEBUG
#define LWIP_DBG_TYPES_ON  LWIP_DBG_ON
#define LWIP_DBG_MIN_LEVEL 0

#define PPP_DEBUG        LWIP_DBG_OFF
#define MEM_DEBUG        LWIP_DBG_ON
#define MEMP_DEBUG       LWIP_DBG_ON
#define PBUF_DEBUG       LWIP_DBG_ON
#define API_LIB_DEBUG    LWIP_DBG_ON
#define API_MSG_DEBUG    LWIP_DBG_ON
#define TCPIP_DEBUG      LWIP_DBG_ON
#define NETIF_DEBUG      LWIP_DBG_ON
#define SOCKETS_DEBUG    LWIP_DBG_ON
#define DEMO_DEBUG       LWIP_DBG_ON
#define IP_DEBUG         LWIP_DBG_ON
#define IP_REASS_DEBUG   LWIP_DBG_ON
#define RAW_DEBUG        LWIP_DBG_ON
#define ICMP_DEBUG       LWIP_DBG_ON
#define UDP_DEBUG        LWIP_DBG_ON
#define TCP_DEBUG        LWIP_DBG_ON
#define TCP_INPUT_DEBUG  LWIP_DBG_ON
#define TCP_OUTPUT_DEBUG LWIP_DBG_ON
#define TCP_RTO_DEBUG    LWIP_DBG_ON
#define TCP_CWND_DEBUG   LWIP_DBG_ON
#define TCP_WND_DEBUG    LWIP_DBG_ON
#define TCP_FR_DEBUG     LWIP_DBG_ON
#define TCP_QLEN_DEBUG   LWIP_DBG_ON
#define TCP_RST_DEBUG    LWIP_DBG_ON

/*extern unsigned char debug_flags;
  #define LWIP_DBG_TYPES_ON debug_flags*/


#endif	/* LWIPOPTS_H */
