/*
 * @(#)spi_time.h       1.3 11/02/28
 *
 * Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 *
 */

/**
 *  @file spi_time.h
 *  The Service Provider Interface API for Time.
 *
 * 
*/

#ifndef _SPI_TIME_H
#define _SPI_TIME_H

#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* NOTE: when adding members to this struct matching rules needs to be added to postbuildchecks in cdc/build/share/rules.mk */

    /**
     * This struct contains a pointer to the custom implementation of 
     * jlong timeMillis(void). User code should populate a SPIInterfaceTime
     * struct with a pointer to a compliant function, and then use the SPI
     * registration method SPIsetTimerImpl() to register it with the VM. See
     * spi.h for more detailed instructions for the registration sequence.
     */
    typedef struct SPIInterfaceTime {
    /**
    * Returns the current time in milliseconds. 
    *
    * Returns:
    * Return time in milliseconds, as defined by Java System.currentTimeMillis()
    **/
        jlong(*timeMillis) (void);

    } SPIInterfaceTime;

#ifdef __cplusplus
}
#endif
#endif                          /* _SPI_TIME_H */
