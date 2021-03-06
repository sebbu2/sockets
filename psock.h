/*
 * Copyright (C) 1999  Philippe Dubois (pdubois1@hotmail.com)
 * Copyright (C) 1999,2000  Ross Combs (rocombs@cs.nmsu.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifdef  __cplusplus
extern "C" {
#endif
  
#ifndef JUST_NEED_TYPES
#ifndef INCLUDED_PSOCK_PROTOS
#define INCLUDED_PSOCK_PROTOS

/*
 * This file is like the other compat header files in that it
 * tries to hide differences between systems. It is somewhat
 * different in that it doesn't present a "normal" interface
 * to the rest of the code, but instead defines its own macros
 * and functions. This is done since Win32 uses closesocket()
 * instead of close() for sockets, but it can't be remapped
 * since it already has a close() for file descriptors. Also,
 * this allows us to hide the fact that some systems use
 * fcntl() and some use ioctl(). The select() function is
 * included here even though it isn't strictly for sockets.
 * Just call this version if any of the file descriptors might
 * be sockets.
 *
 * This file must be included _after_ any other socket related
 * headers and any other compat socket fixup headers.
 */

#if defined(_WIN32)/*||defined(__WIN32__)||defined(__WINDOWS__)*/||defined(__CYGWIN__)||defined(__MINGW32__)
/* winsock2 */
#include <winsock2.h>
#include <windows.h>

#include <stdio.h>
  
/* protocol familes */
#define PSOCK_PF_INET PF_INET

/* address families */
#define PSOCK_AF_INET AF_INET

/* socket types */
#define PSOCK_SOCK_STREAM SOCK_STREAM
#define PSOCK_SOCK_DGRAM  SOCK_DGRAM

/* psock_[gs]etsockopt() flags */
#define PSOCK_SOL_SOCKET   SOL_SOCKET
#define PSOCK_SO_REUSEADDR SO_REUSEADDR
#define PSOCK_SO_KEEPALIVE SO_KEEPALIVE
#define PSOCK_SO_ERROR     SO_ERROR

/* psock_ctl() flags */
#define PSOCK_NONBLOCK FIONBIO

/* psock_errno() values */
#ifdef WSAEWOULDBLOCK
# define PSOCK_EWOULDBLOCK  WSAEWOULDBLOCK
#endif
#ifdef WSAEINPROGRESS
# define PSOCK_EINPROGRESS  WSAEINPROGRESS /* shouldn't this be WSAEWOULDBLOCK? */
#endif
#ifdef WSAEINTR
# define PSOCK_EINTR        WSAEINTR
#endif
#ifdef WSAECONNABORTED
# define PSOCK_ECONNABORTED WSAECONNABORTED
#endif
#ifdef WSATRY_AGAIN
# define PSOCK_EAGAIN       WSATRY_AGAIN
#endif
#ifdef WSAECONNRESET
# define PSOCK_ECONNRESET   WSAECONNRESET
#endif
#ifdef WSAENOTCONN
# define PSOCK_ENOTCONN     WSAENOTCONN
#endif
#ifdef WSAEPIPE
# define PSOCK_EPIPE        WSAEPIPE
#endif
#ifdef WSAEPROTO
# define PSOCK_EPROTO       WSAEPROTO
#endif
#ifdef WSAENOBUFS
# define PSOCK_ENOBUFS      WSAENOBUFS
#endif
#ifdef WSAENOMEM
# define PSOCK_ENOMEM       WSAENOMEM
#endif

/* psock_shutdown() flags */
#define PSOCK_SHUT_RD   0
#define PSOCK_SHUT_WR   1
#define PSOCK_SHUT_RDWR 2

/* psock types */
#define psock_t_socklen int
#define PSOCK_SOCKET SOCKET
#define PSOCK_INVALID_SOCKET INVALID_SOCKET
#define PSOCK_SOCKET_ERROR SOCKET_ERROR

/* psock_select() macros and types */
#define t_psock_fd_set fd_set
#define PSOCK_FD_ZERO  FD_ZERO
#define PSOCK_FD_CLR   FD_CLR
#define PSOCK_FD_SET   FD_SET
#define PSOCK_FD_ISSET FD_ISSET

/* psock functions */
extern int psock_init(void);               /* a real function in compat/psock.c */
extern int psock_exit(void);		   /* a real function in compat/psock.c */
#define psock_errno()                      WSAGetLastError()
#define psock_socket(pf, t, ps)            socket(pf, t, ps)
#define psock_getsockopt(s, l, o, v, size) getsockopt(s, l, o, (char*)v, size)
#define psock_setsockopt(s, l, o, v, size) setsockopt(s, l, o, (char*)v, size)
extern int psock_ctl(int sd, int mode);    /* a real function in compat/psock.c */
extern int psock_ctl_off(int sd,int mode); /* a real function in compat/psock.c */
#define psock_listen(s, b)                 listen(s, b)
#define psock_bind(s, a, l)                bind(s, a, l)
#define psock_accept(s, a, l)              accept(s, a, l)
#define psock_connect(s, a, l)             connect(s, a, l)
#define psock_send(s, b, l, f)             send(s, (char *)b, l, f)
#define psock_sendto(s, b, l, f, a, al)    sendto(s, (char *)b, l, f, a, al)
#define psock_recv(s, b, l, f)             recv(s, (char *)b, l, f)
#define psock_recvfrom(s, b, l, f, a, al)  recvfrom(s, (char *)b, l, f, a, al)
#define psock_shutdown(s, how)             shutdown(s, how)
#define psock_close(s)                     closesocket(s)
#define psock_select(s, r, w, e, t)        select(s, r, w, e ,t)
#define psock_getsockname(s, a, l)         getsockname(s, a, l)

/* PDubois - 991111 */
#define inet_aton(cp, pAdd)                    (((*pAdd).s_addr = inet_addr(cp)) != INADDR_NONE)





#else /* assume POSIX */
/* posix */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/* protocol familes */
#define PSOCK_PF_INET PF_INET

/* address families */
#define PSOCK_AF_INET AF_INET

/* socket types */
#define PSOCK_SOCK_STREAM SOCK_STREAM
#define PSOCK_SOCK_DGRAM  SOCK_DGRAM

/* psock_[gs]etsockopt() flags */
#define PSOCK_SOL_SOCKET   SOL_SOCKET
#define PSOCK_SO_REUSEADDR SO_REUSEADDR
#define PSOCK_SO_KEEPALIVE SO_KEEPALIVE
#define PSOCK_SO_ERROR     SO_ERROR

/* psock_ctl() flags */
#define PSOCK_NONBLOCK O_NONBLOCK

/* psock_errno() values */
#ifdef EWOULDBLOCK
# define PSOCK_EWOULDBLOCK  EWOULDBLOCK
#endif
#ifdef EINPROGRESS
# define PSOCK_EINPROGRESS  EINPROGRESS
#endif
#ifdef EINTR
# define PSOCK_EINTR        EINTR
#endif
#ifdef ECONNABORTED
# define PSOCK_ECONNABORTED ECONNABORTED
#endif
#ifdef EAGAIN
# define PSOCK_EAGAIN       EAGAIN
#endif
#ifdef ECONNRESET
# define PSOCK_ECONNRESET   ECONNRESET
#endif
#ifdef ENOTCONN
# define PSOCK_ENOTCONN     ENOTCONN
#endif
#ifdef EPIPE
# define PSOCK_EPIPE        EPIPE
#endif
#ifdef EPROTO
# define PSOCK_EPROTO       EPROTO
#endif
#ifdef ENOBUFS
# define PSOCK_ENOBUFS      ENOBUFS
#endif
#ifdef ENOMEM
# define PSOCK_ENOMEM       ENOMEM
#endif

/* psock_shutdown() flags */
#ifdef SHUT_RD
# define PSOCK_SHUT_RD   PSOCK_SHUT_RD
#else
# define PSOCK_SHUT_RD   0
#endif
#ifdef SHUT_WR
# define PSOCK_SHUT_WR   SHUT_WR
#else
# define PSOCK_SHUT_WR   1
#endif
#ifdef SHUT_RDWR
# define PSOCK_SHUT_RDWR SHUT_RDWR
#else
# define PSOCK_SHUT_RDWR 2
#endif

/* psock types */
#ifdef HAVE_SOCKLEN_T
# define psock_t_socklen socklen_t
#else
# define psock_t_socklen unsigned int
#endif
#define PSOCK_SOCKET int
#define PSOCK_INVALID_SOCKET -1
#define PSOCK_SOCKET_ERROR -1
  
/* psock_select() macros and types */
#define t_psock_fd_set fd_set
#define PSOCK_FD_ZERO  FD_ZERO
#define PSOCK_FD_CLR   FD_CLR
#define PSOCK_FD_SET   FD_SET
#define PSOCK_FD_ISSET FD_ISSET

/* psock functions */
extern int psock_init(void);               /* a real function in compat/psock.c */
extern int psock_exit(void);		   /* a real function in compat/psock.c */
#define psock_errno()                      (errno)
#define psock_socket(pf, t, ps)            socket(pf, t, ps)
#define psock_getsockopt(s, l, o, v, size) getsockopt(s, l, o, (void*), size)
#define psock_setsockopt(s, l, o, v, size) setsockopt(s, l, o, (const void*)v, size)
extern int psock_ctl(int sd, long int mode);    /* a real function in compat/psock.c */
extern int psock_ctl_off(int sd,int mode); /* a real function in compat/psock.c\   */
#define psock_listen(s, b)                 listen(s, b)
#define psock_bind(s, a, l)                bind(s, a, l)
#define psock_accept(s, a, l)              accept(s, a, l)
#define psock_connect(s, a, l)             connect(s, a, l)
#define psock_send(s, b, l, f)             send(s, (void *)b, l, f)
#define psock_sendto(s, b, l, f, a, al)    sendto(s, (void *)b, l, f, a, al)
#define psock_recv(s, b, l, f)             recv(s, (void *)b, l, f)
#define psock_recvfrom(s, b, l, f, a, al)  recvfrom(s, (void *)b, l, f, a, al)
#define psock_shutdown(s, how)             shutdown(s, how)
#define psock_close(s)                     close(s)
#define psock_select(s, r, w, e, t)        select(s, r, w, e ,t)
#define psock_getsockname(s, a, l)         getsockname(s, a, l)

#endif

#endif
#endif

#ifdef  __cplusplus
}
#endif
