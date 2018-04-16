/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 SummerGift <zhangyuan@rt-thread.com>
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
#ifdef MICROPYTHON_USING_MACHINE_UART

#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mperrno.h"
#include "py/stream.h"

#include <stdarg.h>
#include "machine_uart.h"

/******************************************************************************/
/* MicroPython bindings                                                       */

STATIC void machine_uart_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_uart_obj_t *self = (machine_uart_obj_t*) self_in;
    mp_printf(print, "uart( device port : %s,baud_rate = %d, data_bits = %d, parity = %d, stop_bits = %d )",
            self->uart_device->parent.parent.name,
            self->uart_device->config.baud_rate,
            self->uart_device->config.data_bits,
            self->uart_device->config.parity,
            self->uart_device->config.stop_bits);
}

STATIC mp_obj_t machine_uart_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, MP_OBJ_FUN_ARGS_MAX, true);
    char uart_dev_name[RT_NAME_MAX];
    snprintf(uart_dev_name, sizeof(uart_dev_name), "uart%d", mp_obj_get_int(args[0]));

    struct rt_serial_device *rt_serial_device = (struct rt_serial_device *) rt_device_find(uart_dev_name);
    if (rt_serial_device == RT_NULL || rt_serial_device->parent.type != RT_Device_Class_Char) {
        rt_kprintf("ERROR: UART device %s not found!\n", uart_dev_name);
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError, "UART(%s) doesn't exist", uart_dev_name));
    }

    // create new uart object
    machine_uart_obj_t *self = m_new_obj(machine_uart_obj_t);
    self->base.type = &machine_uart_type;
    self->uart_device = rt_serial_device;
    return (mp_obj_t) self;
}

STATIC mp_obj_t machine_uart_init(size_t n_args, const mp_obj_t *args) {
    mp_obj_base_t *self = (mp_obj_base_t*) MP_OBJ_TO_PTR(args[0]);
    struct rt_serial_device *uart_p = ((machine_uart_obj_t *) self)->uart_device;
    struct serial_configure config;

    int baudrate  = mp_obj_get_int(args[1]);
    int bits = mp_obj_get_int(args[2]);
    int parity  = mp_obj_get_int(args[3]);
    int stop  = mp_obj_get_int(args[4]);

    config.baud_rate = baudrate;
    config.data_bits = bits;
    config.parity = parity;
    config.stop_bits = stop;

    rt_device_control((struct rt_device *) uart_p, RT_DEVICE_CTRL_CONFIG, &config);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(machine_uart_init_obj, 5, 5, machine_uart_init);


STATIC mp_obj_t machine_uart_deinit(mp_obj_t self_in) {
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_uart_deinit_obj, machine_uart_deinit);

STATIC mp_obj_t machine_uart_writechar(mp_obj_t self_in, mp_obj_t char_in) {
    machine_uart_obj_t *self = self_in;
    uint16_t data = mp_obj_get_int(char_in);
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write((struct rt_device *)(self->uart_device), 0, &data, 1);
        timeout++;
    }
    while (len != 1 && timeout < 500);

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(machine_uart_writechar_obj, machine_uart_writechar);

/* 串口接收事件标志 */
#define UART_RX_EVENT (1 << 0)
/* 事件控制块 */
static struct rt_event event;

STATIC mp_obj_t machine_uart_readchar(mp_obj_t self_in) {
    machine_uart_obj_t *self = self_in;
    rt_uint32_t e;
    rt_uint8_t ch;
    /* 读取1字节数据 */
    while (rt_device_read((struct rt_device *)(self->uart_device), 0, &ch, 1) != 1) {
        /* 接收事件 */
        rt_event_recv(&event, UART_RX_EVENT, RT_EVENT_FLAG_AND |
        RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &e);
    }

    return MP_OBJ_NEW_SMALL_INT(ch);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(machine_uart_readchar_obj, machine_uart_readchar);

STATIC const mp_rom_map_elem_t machine_uart_locals_dict_table[] = {
    // instance methods

    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&machine_uart_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&machine_uart_deinit_obj) },
//    { MP_ROM_QSTR(MP_QSTR_any), MP_ROM_PTR(&machine_uart_any_obj) },

    /// \method read([nbytes])
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mp_stream_read_obj) },
    /// \method readline()
    { MP_ROM_QSTR(MP_QSTR_readline), MP_ROM_PTR(&mp_stream_unbuffered_readline_obj)},
    /// \method readinto(buf[, nbytes])
    { MP_ROM_QSTR(MP_QSTR_readinto), MP_ROM_PTR(&mp_stream_readinto_obj) },
    /// \method write(buf)
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&mp_stream_write_obj) },

    { MP_ROM_QSTR(MP_QSTR_writechar), MP_ROM_PTR(&machine_uart_writechar_obj) },
    { MP_ROM_QSTR(MP_QSTR_readchar), MP_ROM_PTR(&machine_uart_readchar_obj) },
//    { MP_ROM_QSTR(MP_QSTR_sendbreak), MP_ROM_PTR(&pyb_uart_sendbreak_obj) },

//    class constants
//    { MP_ROM_QSTR(MP_QSTR_RTS), MP_ROM_INT(UART_HWCONTROL_RTS) },
//    { MP_ROM_QSTR(MP_QSTR_CTS), MP_ROM_INT(UART_HWCONTROL_CTS) },
};
STATIC MP_DEFINE_CONST_DICT(machine_uart_locals_dict, machine_uart_locals_dict_table);

STATIC mp_uint_t machine_uart_read(mp_obj_t self_in, void *buf_in, mp_uint_t size, int *errcode) {
    machine_uart_obj_t *self = self_in;
    byte *buf = buf_in;
    return rt_device_read((struct rt_device *)(self->uart_device), -1, buf, size);
}

STATIC mp_uint_t machine_uart_write(mp_obj_t self_in, const void *buf_in, mp_uint_t size, int *errcode) {
    machine_uart_obj_t *self = self_in;
    const byte *buf = buf_in;
    return rt_device_write((struct rt_device *)(self->uart_device), -1, buf, size);
}

STATIC mp_uint_t machine_uart_ioctl(mp_obj_t self_in, mp_uint_t request, mp_uint_t arg, int *errcode) {
    return NULL;
}

STATIC const mp_stream_p_t uart_stream_p = {
    .read = machine_uart_read,
    .write = machine_uart_write,
    .ioctl = NULL,
    .is_text = false,
};

const mp_obj_type_t machine_uart_type = {
    { &mp_type_type },
    .name = MP_QSTR_UART,
    .print = machine_uart_print,
    .make_new = machine_uart_make_new,
    .getiter = mp_identity_getiter,
    .iternext = mp_stream_unbuffered_iter,
    .protocol = &uart_stream_p,
    .locals_dict = (mp_obj_dict_t*)&machine_uart_locals_dict,
};

#endif // MICROPYTHON_USING_MACHINE_UART
