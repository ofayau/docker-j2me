/*
 * @(#)spi.h    1.0 11/01/05
 *
 * Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 *
 */

/**
 * @file spi.h
 * The API for the Service Provider Interface.
 * SPI is used to register new services with a VM on startup. Services
 * include functions for time, file-access, graphics and log-output.
 * SPI may be expanded in the future.
 *
 * During system startup, the VM tries to dynamically load a library 
 * called spi_impl (libspi_impl.so or spi_impl.dll on linux or windows,
 * according to accepted convention), which it looks for in the /lib
 * directory of the VM installation. Once loaded, the VM then looks 
 * for a function called SPI_OnLoad(SPIEnv)
 * and calls it, if it exists. Similarly, a call is made to 
 * SPI_OnUnload(SPIEnv) when the VM is shutting down.
 */

#ifndef _SPI_H
#define _SPI_H

#include "spi_io.h"
#include "spi_console.h"
#include "spi_time.h"

#ifdef CVM_USE_SPI

#define SPI_VERSION 0x10001 /**< the current version of the API */

/**
 * A struct containing pointers to the register and unregister
 * functions. The first item will always be a function taking no
 * parameters, that returns an int representing the version of the
 * API. It is highly recommended to test this against the version
 * used by the VM, and to exit if the two values are not compatible.
 */
typedef struct SPIEnv {

    int (*SPIgetSpiVersion) (void);

    int (*SPIioImplRegister) (SPIInterfaceIO * handle, int priority);
    int (*SPIioImplUnregister) (SPIInterfaceIO * handle);

    int (*SPIsetTimerImpl) (SPIInterfaceTime * handle);
    int (*SPIsetDefaultTimerImpl) (void);

    int (*SPIsetConsoleImpl) (SPIInterfaceConsole * handle);
    int (*SPIsetDefaultConsoleImpl) (void);

} SPIEnv;

/* prototypes of functions that will be called during CVM startup and
 * shutdown
 */

/**
 * Called by the VM during startup. This user-supplied method is used to
 * register the new services using the methods provided in the spi_env_p
 * struct.
 * @param spi_env_p a pointer to a struct of SPIEnv
 */
int
 SPI_OnLoad(SPIEnv * spi_env_p);

/**
 * Called by the VM during teardown. This user-supplied method can be used to
 * perform any cleanup needed by the user.
 * @param spi_env_p a pointer to a struct of SPIEnv
 */
void
 SPI_OnUnload(SPIEnv * spi_env_p);
#endif                          /* CVM_USE_SPI */
#endif
