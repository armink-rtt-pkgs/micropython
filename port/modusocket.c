/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Armink (armink.ztl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include "py/objtuple.h"
#include "py/objlist.h"
#include "py/runtime.h"
#include "py/mperrno.h"
#include "lib/netutils/netutils.h"
#include "modnetwork.h"

#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <dfs_posix.h>

#if MICROPY_PY_USOCKET

/******************************************************************************/
// socket class

STATIC const mp_obj_type_t socket_type;

typedef struct _posix_socket_obj_t {
    mp_obj_base_t base;
    int fd;
} posix_socket_obj_t;

// constructor socket(family=AF_INET, type=SOCK_STREAM, proto=0)
STATIC mp_obj_t socket_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    uint8_t domain = AF_INET;
    uint8_t style = SOCK_STREAM;
    uint8_t proto = 0;
    int fd = -1;
    posix_socket_obj_t *s = MP_OBJ_NULL;

    mp_arg_check_num(n_args, n_kw, 0, 4, false);

    if (n_args >= 1) {
        domain = mp_obj_get_int(args[0]);
        if (n_args >= 2) {
            style = mp_obj_get_int(args[1]);
        }
    }

    fd = socket(domain, style, proto);

    if (fd != -1) {
        s = m_new_obj_with_finaliser(posix_socket_obj_t);
        if (s) {
            s->base.type = (mp_obj_t)&socket_type;
            s->fd = fd;
        }
    }

    return s;
}

// method socket.close()
STATIC mp_obj_t socket_close(mp_obj_t self_in) {
    posix_socket_obj_t *self = self_in;

    closesocket(self->fd);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(socket_close_obj, socket_close);

// method socket.bind(address)
STATIC mp_obj_t socket_bind(mp_obj_t self_in, mp_obj_t addr_in) {
    posix_socket_obj_t *self = self_in;
    int _errno;
    uint8_t ip[MOD_NETWORK_IPADDR_BUF_SIZE];
    mp_uint_t port = netutils_parse_inet_addr(addr_in, ip, NETUTILS_BIG);
    struct sockaddr_in sockaddr;

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_aton((char *)ip, (struct in_addr* )&(sockaddr.sin_addr));
    memset(&(sockaddr.sin_zero), 0, sizeof(sockaddr.sin_zero));

    if ((_errno = bind(self->fd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr))) < 0) {
        mp_raise_OSError(_errno);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_bind_obj, socket_bind);

// method socket.listen(backlog)
STATIC mp_obj_t socket_listen(mp_obj_t self_in, mp_obj_t backlog) {
    posix_socket_obj_t *self = self_in;
    int _errno;

    if ((_errno = listen(self->fd, mp_obj_get_int(backlog))) < 0) {
        mp_raise_OSError(_errno);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_listen_obj, socket_listen);

// method socket.accept()
STATIC mp_obj_t socket_accept(mp_obj_t self_in) {
    posix_socket_obj_t *self = self_in;

    // create new socket object
    posix_socket_obj_t *socket2 = m_new_obj_with_finaliser(posix_socket_obj_t);
    socket2->base.type = (mp_obj_t)&socket_type;

    // accept incoming connection
    int new_client;
    struct sockaddr_in addr;
    socklen_t addrlen;

    if ((new_client = accept(self->fd, (struct sockaddr *)&addr, &addrlen)) < 0) {
        mp_raise_OSError(new_client);
    }

    // make the return value
    mp_obj_tuple_t *client = mp_obj_new_tuple(2, NULL);
    client->items[0] = socket2;
    client->items[1] = netutils_format_inet_addr((uint8_t *)inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), NETUTILS_BIG);

    return client;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(socket_accept_obj, socket_accept);

// method socket.connect(address)
STATIC mp_obj_t socket_connect(mp_obj_t self_in, mp_obj_t addr_in) {
    posix_socket_obj_t *self = self_in;

    // get address
    uint8_t ip[MOD_NETWORK_IPADDR_BUF_SIZE];
    mp_uint_t port = netutils_parse_inet_addr(addr_in, ip, NETUTILS_BIG);

    int _errno;
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_aton((char *)ip, (struct in_addr* )&(sockaddr.sin_addr));
    memset(&(sockaddr.sin_zero), 0, sizeof(sockaddr.sin_zero));

    if ((_errno = connect(self->fd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr))) <= 0) {
        mp_raise_OSError(_errno);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_connect_obj, socket_connect);

// method socket.send(bytes)
STATIC mp_obj_t socket_send(mp_obj_t self_in, mp_obj_t buf_in) {
    posix_socket_obj_t *self = self_in;
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);
    int _errno;

    _errno = send(self->fd, bufinfo.buf, bufinfo.len, 0);
    if (_errno == -1) {
        mp_raise_OSError(_errno);
    }
    return mp_obj_new_int_from_uint(_errno);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_send_obj, socket_send);

// method socket.recv(bufsize)
STATIC mp_obj_t socket_recv(mp_obj_t self_in, mp_obj_t len_in) {
    posix_socket_obj_t *self = self_in;
    mp_int_t len = mp_obj_get_int(len_in);
    vstr_t vstr;
    vstr_init_len(&vstr, len);
    int ret;

    ret = recv(self->fd, (byte*)vstr.buf, len, 0);
    if (ret == -1) {
        mp_raise_OSError(ret);
    }
    if (ret == 0) {
        return mp_const_empty_bytes;
    }
    vstr.len = ret;
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_recv_obj, socket_recv);

// method socket.sendto(bytes, address)
STATIC mp_obj_t socket_sendto(mp_obj_t self_in, mp_obj_t data_in, mp_obj_t addr_in) {
    posix_socket_obj_t *self = self_in;

    // get the data
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(data_in, &bufinfo, MP_BUFFER_READ);

    // get address
    uint8_t ip[MOD_NETWORK_IPADDR_BUF_SIZE];
    mp_uint_t port = netutils_parse_inet_addr(addr_in, ip, NETUTILS_BIG);
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_aton((char *)ip, (struct in_addr* )&(sockaddr.sin_addr));
    memset(&(sockaddr.sin_zero), 0, sizeof(sockaddr.sin_zero));

    mp_int_t ret = sendto(self->fd, bufinfo.buf, bufinfo.len, MSG_DONTWAIT, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr));
    if (ret == -1) {
        mp_raise_OSError(ret);
    }

    return mp_obj_new_int(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(socket_sendto_obj, socket_sendto);

// method socket.recvfrom(bufsize)
STATIC mp_obj_t socket_recvfrom(mp_obj_t self_in, mp_obj_t len_in) {
    posix_socket_obj_t *self = self_in;
    vstr_t vstr;
    vstr_init_len(&vstr, mp_obj_get_int(len_in));
    byte ip[4];
    mp_uint_t port;
    int ret;
    struct sockaddr_in sockaddr;
    uint32_t addr_len;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_aton((char *)ip, (struct in_addr* )&(sockaddr.sin_addr));
    memset(&(sockaddr.sin_zero), 0, sizeof(sockaddr.sin_zero));

    ret = recvfrom(self->fd, vstr.buf, vstr.len, MSG_DONTWAIT, (struct sockaddr *)&sockaddr, &addr_len);
    if (ret == -1) {
        mp_raise_OSError(ret);
    }
    mp_obj_t tuple[2];
    if (ret == 0) {
        tuple[0] = mp_const_empty_bytes;
    } else {
        vstr.len = ret;
        tuple[0] = mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
    }
    tuple[1] = netutils_format_inet_addr(ip, port, NETUTILS_BIG);
    return mp_obj_new_tuple(2, tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_recvfrom_obj, socket_recvfrom);

// method socket.setsockopt(level, optname, value)
STATIC mp_obj_t socket_setsockopt(size_t n_args, const mp_obj_t *args) {
    posix_socket_obj_t *self = args[0];

    mp_int_t level = mp_obj_get_int(args[1]);
    mp_int_t opt = mp_obj_get_int(args[2]);

    const void *optval;
    mp_uint_t optlen;
    mp_int_t val;
    if (mp_obj_is_integer(args[3])) {
        val = mp_obj_get_int_truncated(args[3]);
        optval = &val;
        optlen = sizeof(val);
    } else {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(args[3], &bufinfo, MP_BUFFER_READ);
        optval = bufinfo.buf;
        optlen = bufinfo.len;
    }

    int _errno;
    if ((_errno = setsockopt(self->fd, level, opt, optval, optlen)) < 0) {
        mp_raise_OSError(_errno);
    }

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(socket_setsockopt_obj, 4, 4, socket_setsockopt);

// method socket.settimeout(value)
// timeout=0 means non-blocking
// timeout=None means blocking
// otherwise, timeout is in seconds
STATIC mp_obj_t socket_settimeout(mp_obj_t self_in, mp_obj_t timeout_in) {
    posix_socket_obj_t *self = self_in;
    int timeout;

    if (timeout_in == mp_const_none) {
        timeout = -1;
    } else {
        #if MICROPY_PY_BUILTINS_FLOAT
        timeout = 1000 * mp_obj_get_float(timeout_in);
        #else
        timeout = 1000 * mp_obj_get_int(timeout_in);
        #endif
    }
    int _errno;

    int flags = ioctl(self->fd, F_GETFL, NULL);
    if (timeout < 0) {
        _errno = ioctl(self->fd, F_SETFL, (void *)(flags & (~O_NONBLOCK)));
    } else if (timeout > 0) {
        //TODO
        MP_RTT_NOT_IMPL_PRINT;
    } else {
        _errno = ioctl(self->fd, F_SETFL, (void *)(flags | O_NONBLOCK));
    }

    if (_errno < 0) {
        mp_raise_OSError(_errno);
    }
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_settimeout_obj, socket_settimeout);

// method socket.setblocking(flag)
STATIC mp_obj_t socket_setblocking(mp_obj_t self_in, mp_obj_t blocking) {
    if (mp_obj_is_true(blocking)) {
        return socket_settimeout(self_in, mp_const_none);
    } else {
        return socket_settimeout(self_in, MP_OBJ_NEW_SMALL_INT(0));
    }
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(socket_setblocking_obj, socket_setblocking);

STATIC const mp_rom_map_elem_t socket_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&socket_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&socket_close_obj) },
    { MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&socket_bind_obj) },
    { MP_ROM_QSTR(MP_QSTR_listen), MP_ROM_PTR(&socket_listen_obj) },
    { MP_ROM_QSTR(MP_QSTR_accept), MP_ROM_PTR(&socket_accept_obj) },
    { MP_ROM_QSTR(MP_QSTR_connect), MP_ROM_PTR(&socket_connect_obj) },
    { MP_ROM_QSTR(MP_QSTR_send), MP_ROM_PTR(&socket_send_obj) },
    { MP_ROM_QSTR(MP_QSTR_recv), MP_ROM_PTR(&socket_recv_obj) },
    { MP_ROM_QSTR(MP_QSTR_sendto), MP_ROM_PTR(&socket_sendto_obj) },
    { MP_ROM_QSTR(MP_QSTR_recvfrom), MP_ROM_PTR(&socket_recvfrom_obj) },
    { MP_ROM_QSTR(MP_QSTR_setsockopt), MP_ROM_PTR(&socket_setsockopt_obj) },
    { MP_ROM_QSTR(MP_QSTR_settimeout), MP_ROM_PTR(&socket_settimeout_obj) },
    { MP_ROM_QSTR(MP_QSTR_setblocking), MP_ROM_PTR(&socket_setblocking_obj) },
};

STATIC MP_DEFINE_CONST_DICT(socket_locals_dict, socket_locals_dict_table);

mp_uint_t socket_ioctl(mp_obj_t self_in, mp_uint_t request, mp_uint_t arg, int *errcode) {
    posix_socket_obj_t *self = self_in;

    return ioctl(self->fd, request, (void *)arg);
}

STATIC const mp_stream_p_t socket_stream_p = {
    .ioctl = socket_ioctl,
    .is_text = false,
};

STATIC const mp_obj_type_t socket_type = {
    { &mp_type_type },
    .name = MP_QSTR_socket,
    .make_new = socket_make_new,
    .protocol = &socket_stream_p,
    .locals_dict = (mp_obj_dict_t*)&socket_locals_dict,
};

/******************************************************************************/
// usocket module

// function usocket.getaddrinfo(host, port)
STATIC mp_obj_t mod_usocket_getaddrinfo(mp_obj_t host_in, mp_obj_t port_in) {
    size_t hlen;
    const char *host = mp_obj_str_get_data(host_in, &hlen);
    const char port_in_s[6] = { 0 };
    mp_int_t port = mp_obj_get_int(port_in);
    int ret;

    snprintf(port_in_s, sizeof(port_in_s), "%d", port);

    struct sockaddr_in sockaddr, *res = NULL;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    memset(&(sockaddr.sin_zero), 0, sizeof(sockaddr.sin_zero));

    ret = getaddrinfo(host, port_in_s, &sockaddr, &res);

    if (!ret) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "no available netif"));
    }

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(5, NULL);
    tuple->items[0] = MP_OBJ_NEW_SMALL_INT(MOD_NETWORK_AF_INET);
    tuple->items[1] = MP_OBJ_NEW_SMALL_INT(MOD_NETWORK_SOCK_STREAM);
    tuple->items[2] = MP_OBJ_NEW_SMALL_INT(0);
    tuple->items[3] = MP_OBJ_NEW_QSTR(MP_QSTR_);
    tuple->items[4] = netutils_format_inet_addr((uint8_t *)inet_ntoa(res->sin_addr), res->sin_port, NETUTILS_BIG);

    freeaddrinfo(res);

    return mp_obj_new_list(1, (mp_obj_t*)&tuple);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_usocket_getaddrinfo_obj, mod_usocket_getaddrinfo);

STATIC const mp_rom_map_elem_t mp_module_usocket_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_usocket) },

    { MP_ROM_QSTR(MP_QSTR_socket), MP_ROM_PTR(&socket_type) },
    { MP_ROM_QSTR(MP_QSTR_getaddrinfo), MP_ROM_PTR(&mod_usocket_getaddrinfo_obj) },

    // class constants
    { MP_ROM_QSTR(MP_QSTR_AF_INET), MP_ROM_INT(MOD_NETWORK_AF_INET) },
    { MP_ROM_QSTR(MP_QSTR_AF_INET6), MP_ROM_INT(MOD_NETWORK_AF_INET6) },

    { MP_ROM_QSTR(MP_QSTR_SOCK_STREAM), MP_ROM_INT(MOD_NETWORK_SOCK_STREAM) },
    { MP_ROM_QSTR(MP_QSTR_SOCK_DGRAM), MP_ROM_INT(MOD_NETWORK_SOCK_DGRAM) },
    { MP_ROM_QSTR(MP_QSTR_SOCK_RAW), MP_ROM_INT(MOD_NETWORK_SOCK_RAW) },

    /*
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_IP), MP_ROM_INT(MOD_NETWORK_IPPROTO_IP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_ICMP), MP_ROM_INT(MOD_NETWORK_IPPROTO_ICMP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_IPV4), MP_ROM_INT(MOD_NETWORK_IPPROTO_IPV4) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_TCP), MP_ROM_INT(MOD_NETWORK_IPPROTO_TCP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_UDP), MP_ROM_INT(MOD_NETWORK_IPPROTO_UDP) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_IPV6), MP_ROM_INT(MOD_NETWORK_IPPROTO_IPV6) },
    { MP_ROM_QSTR(MP_QSTR_IPPROTO_RAW), MP_ROM_INT(MOD_NETWORK_IPPROTO_RAW) },
    */
};

STATIC MP_DEFINE_CONST_DICT(mp_module_usocket_globals, mp_module_usocket_globals_table);

const mp_obj_module_t mp_module_usocket = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_usocket_globals,
};

#endif  // MICROPY_PY_USOCKET
