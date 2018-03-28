/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Damien P. George
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

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mperrno.h"
#include "extmod/machine_i2c.h"
//#include "i2c.h"

#ifdef MICROPYTHON_USING_MACHINE_I2C

STATIC const mp_obj_type_t machine_hard_i2c_type;

#if defined(STM32F4)

typedef struct _machine_hard_i2c_obj_t {
    mp_obj_base_t base;
    const pyb_i2c_obj_t *pyb;
    uint32_t *timeout;
} machine_hard_i2c_obj_t;

STATIC uint32_t machine_hard_i2c_timeout[4];

STATIC const machine_hard_i2c_obj_t machine_hard_i2c_obj[] = {
    {{&machine_hard_i2c_type}, &pyb_i2c_obj[0], &machine_hard_i2c_timeout[0]},
    {{&machine_hard_i2c_type}, &pyb_i2c_obj[1], &machine_hard_i2c_timeout[1]},
    {{&machine_hard_i2c_type}, &pyb_i2c_obj[2], &machine_hard_i2c_timeout[2]},
    {{&machine_hard_i2c_type}, &pyb_i2c_obj[3], &machine_hard_i2c_timeout[3]},
};

STATIC void machine_hard_i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_hard_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "I2C(%u, freq=%u, timeout=%u)",
        self - &machine_hard_i2c_obj[0] + 1,
        i2c_get_baudrate(self->pyb->i2c),
        *self->timeout);
}

STATIC void machine_hard_i2c_init(const machine_hard_i2c_obj_t *self, uint32_t freq, uint32_t timeout) {
    *self->timeout = timeout;
    i2c_init_freq(self->pyb, freq);
}

// this function is based on STM code
STATIC bool I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c) {
    if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET) {
        /* Clear NACKF Flag */
        __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);
        return true;
    }
    return false;
}

// this function is based on STM code
STATIC bool I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart) {
    /* Wait until flag is set */
    while ((__HAL_I2C_GET_FLAG(hi2c, Flag) ? SET : RESET) == Status) {
        if (Timeout != HAL_MAX_DELAY) {
            if ((Timeout == 0U)||((HAL_GetTick() - Tickstart ) > Timeout)) {
                return false;
            }
        }
    }
    return true;
}

// this function is based on STM code
STATIC int I2C_WaitOnRXNEFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart) {
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_RXNE) == RESET) {
        /* Check if a STOPF is detected */
        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == SET) {
            /* Clear STOP Flag */
            __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
            return -MP_EBUSY;
        }

        /* Check for the Timeout */
        if ((Timeout == 0U) || ((HAL_GetTick()-Tickstart) > Timeout)) {
            return -MP_ETIMEDOUT;
        }
    }
    return 0;
}

// this function is based on STM code
STATIC int send_addr_byte(I2C_HandleTypeDef *hi2c, uint8_t addr_byte, uint32_t Timeout, uint32_t Tickstart) {
    /* Generate Start */
    hi2c->Instance->CR1 |= I2C_CR1_START;

    /* Wait until SB flag is set */
    if (!I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout, Tickstart)) {
        return -MP_ETIMEDOUT;
    }

    /* Send slave address */
    hi2c->Instance->DR = addr_byte;

    /* Wait until ADDR flag is set */
    while (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_ADDR) == RESET) {
        if (__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET) {
            // nack received for addr, release the bus cleanly
            hi2c->Instance->CR1 |= I2C_CR1_STOP;
            __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);
            return -MP_ENODEV;
        }

        /* Check for the Timeout */
        if (Timeout != HAL_MAX_DELAY) {
            if ((Timeout == 0U)||((HAL_GetTick() - Tickstart ) > Timeout)) {
                return -MP_ETIMEDOUT;
            }
        }
    }

    return 0;
}

// this function is based on STM code
int machine_hard_i2c_readfrom(mp_obj_base_t *self_in, uint16_t addr, uint8_t *dest, size_t len, bool stop) {
    machine_hard_i2c_obj_t *self = (machine_hard_i2c_obj_t*)self_in;

#if 0
    // TODO: for multi-master, here we could wait for the bus to be free
    // we'd need a flag to tell if we were in the middle of a set of transactions
    // (ie didn't send a stop bit in the last call)
    /* Wait until BUSY flag is reset */
    if (!I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart)) {
        return -MP_EBUSY;
    }
#endif


    return 0;
}

// this function is based on STM code
int machine_hard_i2c_writeto(mp_obj_base_t *self_in, uint16_t addr, const uint8_t *src, size_t len, bool stop) {
    machine_hard_i2c_obj_t *self = (machine_hard_i2c_obj_t*)self_in;
    I2C_HandleTypeDef *hi2c = self->pyb->i2c;
    uint32_t Timeout = *self->timeout;

#if 0
    // TODO: for multi-master, here we could wait for the bus to be free
    // we'd need a flag to tell if we were in the middle of a set of transactions
    // (ie didn't send a stop bit in the last call)
    /* Wait until BUSY flag is reset */
    if (!I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY_FLAG, tickstart)) {
        return -MP_EBUSY;
    }
#endif

    return num_acks;

timeout:
    // timeout, release the bus cleanly
    hi2c->Instance->CR1 |= I2C_CR1_STOP;
    return -MP_ETIMEDOUT;
}

#endif

/******************************************************************************/
/* MicroPython bindings for machine API                                       */

mp_obj_t machine_hard_i2c_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    // parse args
    enum { ARG_id, ARG_scl, ARG_sda, ARG_freq, ARG_timeout };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_id, MP_ARG_REQUIRED | MP_ARG_OBJ },
        { MP_QSTR_scl, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_sda, MP_ARG_KW_ONLY | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_freq, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 400000} },
        { MP_QSTR_timeout, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = 1000} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // work out i2c bus
    int i2c_id = 0;
    if (MP_OBJ_IS_STR(args[ARG_id].u_obj)) {
        const char *port = mp_obj_str_get_str(args[ARG_id].u_obj);
        if (0) {
        #ifdef MICROPY_HW_I2C1_NAME
        } else if (strcmp(port, MICROPY_HW_I2C1_NAME) == 0) {
            i2c_id = 1;
        #endif
        #ifdef MICROPY_HW_I2C2_NAME
        } else if (strcmp(port, MICROPY_HW_I2C2_NAME) == 0) {
            i2c_id = 2;
        #endif
        #ifdef MICROPY_HW_I2C3_NAME
        } else if (strcmp(port, MICROPY_HW_I2C3_NAME) == 0) {
            i2c_id = 3;
        #endif
        } else {
            nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                "I2C(%s) doesn't exist", port));
        }
    } else {
        i2c_id = mp_obj_get_int(args[ARG_id].u_obj);
        if (i2c_id < 1 || i2c_id > MP_ARRAY_SIZE(machine_hard_i2c_obj)
            || machine_hard_i2c_obj[i2c_id - 1].base.type == NULL) {
            nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_ValueError,
                "I2C(%d) doesn't exist", i2c_id));
        }
    }

    // get static peripheral object
    machine_hard_i2c_obj_t *self = (machine_hard_i2c_obj_t*)&machine_hard_i2c_obj[i2c_id - 1];

    // here we would check the scl/sda pins and configure them, but it's not implemented
    if (args[ARG_scl].u_obj != MP_OBJ_NULL || args[ARG_sda].u_obj != MP_OBJ_NULL) {
        mp_raise_ValueError("explicit choice of scl/sda is not implemented");
    }

    // initialise the I2C peripheral
    machine_hard_i2c_init(self, args[ARG_freq].u_int, args[ARG_timeout].u_int);

    return MP_OBJ_FROM_PTR(self);
}

STATIC const mp_machine_i2c_p_t machine_hard_i2c_p = {
    .readfrom = machine_hard_i2c_readfrom,
    .writeto = machine_hard_i2c_writeto,
};

STATIC const mp_obj_type_t machine_hard_i2c_type = {
    { &mp_type_type },
    .name = MP_QSTR_I2C,
    .print = machine_hard_i2c_print,
    .make_new = machine_hard_i2c_make_new,
    .protocol = &machine_hard_i2c_p,
    .locals_dict = (mp_obj_dict_t*)&mp_machine_soft_i2c_locals_dict,
};

#endif // MICROPYTHON_USING_MACHINE_I2C
