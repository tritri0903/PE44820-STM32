/*
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************

  File:		  tcpServerRAW.h
  Author:     ControllersTech.com
  Updated:    26-Jul-2021

  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ***************************************************************************************************************
*/
#include "lwip.h"
#include "lwip/tcp.h"


#ifndef TCP_SERVER_H
#define TCP_SERVER_H

/* structure for maintaining connection infos to be passed as argument
   to LwIP callbacks*/
struct tcp_server_struct
{
  u8_t state;             /* current connection state */
  u8_t retries;
  struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
  struct pbuf *p;         /* pointer on the received/to be transmitted pbuf */
};

extern char input[100];
extern struct tcp_pcb *server_pcb;

struct tcp_pcb* tcp_server_init(void);
void tcp_server_handle(struct tcp_pcb *tpcb, struct tcp_server_struct *es, const char* ans);

void tcp_server_send_message(struct tcp_pcb *tpcb, const char *message);

#endif /* TCP_SERVER_H */
