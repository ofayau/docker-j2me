/*
 * @(#)spi_console.h   1.3 07/02/28
 *
 * Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 *
 */

/**
 * @file spi_console.h
 * The Service Provider Interface console API. Provides the means to
 * register an alternative console output.
 */

#ifndef _SPI_CONSOLE_H
#define _SPI_CONSOLE_H

#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* NOTE: when adding members to this struct matching rules needs to be added
     * to postbuildchecks in cdc/build/share/rules.mk 
     */
/**
 * @fn jint consolePrint(const char *text);
 * Outputs the given char string to the console. In order to override the 
 * default stdout/stderr output of the VM, register your own implementation 
 * of this function using SPI. See the file spi.h for more details.
 * @param text the text to be output to the console
 * @return the number of chars that should have been written, or a negative 
 * value if failure. (see C99 printf documentation for full documentation)
 */

/**
 * Contains a pointer to a consolePrint function.
 */
    typedef struct SPIInterfaceConsole {
        jint(*consolePrint) (const char *text);
    } SPIInterfaceConsole;

#ifdef __cplusplus
}
#endif
#endif                          /* _SPI_CONSOLE_H */
