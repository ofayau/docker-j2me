/*
 * @(#)spi_io.h 1.3 11/02/28
 *
 * Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
 * Use is subject to license terms.
 */

/**
 * @file spi_io.h
 * Allows to add/overide the default implementation of the IO functions.
 * 
 * int SPIioImplRegister(SPIInterfaceIO* handle, int priority);
 * Registers a number of functions to the JVM. Those funcions are defined
 * in this header file, as members of the JVM_SPIIOEnv struct. 
 * <p>
 * 
 * <p>
 * When the JVM needs to open a file, first the registered open function
 * shall be called. When it succeeds, the open function should return an int
 * "file descriptor"
 * If it fails, SPI_IO_ERR is returned and the original (default) IO function
 * can be called.
 */

#ifndef _SPI_IO_H
#define _SPI_IO_H

#include "jni.h"

#ifdef __cplusplus
extern "C" {
#endif

/* trying to reduce dependencies on external .h files .. */
#ifdef PATH_MAX
#define SPI_PATH_MAXLEN PATH_MAX
#else
#define SPI_PATH_MAXLEN 4096
#endif

#ifndef CVM_USE_SPI

#define SPIFD CVMInt32
#define SPI_IO_NULL 0
#define SPI_IO_ERR (-1)
#define SPI_IO_ERR_FD (-1)
#define JLONG_TO_SPIFD(x) CVMlong2Int(x)
#define SPIFD_TO_JLONG(x) CVMint2Long(x)

#else                           /* CVM_USE_SPI is defined */

#define JLONG_TO_SPIFD(x) jlong_to_ptr(x)
#define SPIFD_TO_JLONG(x) ptr_to_jlong(x)
#define SPI_IO_FILETYPE_REGULAR    (0)
#define SPI_IO_FILETYPE_DIRECTORY  (1)
/* #define SPI_IO_FILETYPE_HIDDEN     (???)*/
#define SPI_IO_FILETYPE_OTHER      (2)
#define SPI_IO_EEXIST              (-100)
#define SPI_IO_NULL                NULL
#define SPI_IO_ERR                 (-1)
#define SPI_IO_INTR                (-2)
#define SPI_IO_ENOENT              (2)
#define SPI_IO_RDONLY              (00) /* viz. fcntl.h */
#define SPI_IO_WRONLY              (01)
#define SPI_IO_RDWR                (02)
#define SPI_ACCESS_READ            (0x04)
#define SPI_ACCESS_WRITE           (0x02)
#define SPI_ACCESS_EXECUTE         (0x01)
#define SPI_IO_ERR_FD              (JLONG_TO_SPIFD(CVMint2Long(SPI_IO_ERR)))

#endif                          /* CVM_USE_SPI */

/** 
* Struct SPIioDirItr.
* Used for iterating over a directory and retreiving file names. 
* 
* Iteration starts by calling opendir using the directory 
* pathname. 
* The opendir should return a file descriptor for the directory. 
* 
* The iteration is done using the fd->readdir function.
* This function receives a pointer to a SPIioDirItr struct where 
* the next file name should be filled in fileName field. The 
* implementation can use the pUserCtxt pointer to place a context 
* struct (for example DIR in POSIX) to remember the 
* offset in the directory for the following readdir call. 
* 
*/
typedef struct SPIioDirItr {
    char fileName[SPI_PATH_MAXLEN + 1]; /**< The name of the directory file */
    void *pUserCtxt;            /**< internal - a common context */
} SPIioDirItr;

#define SPI_IO_TIME_ACCESSED_IDX 0
#define SPI_IO_TIME_MODIFIED_IDX 1

/** 
* struct SPIioFileTime. 
* 
* This structure is used as for getting/setting file times.  
*/
typedef struct SPIioFileTime {
    long sec;     /**< seconds */
    long usec;    /**< and microseconds */
} SPIioFileTime;

typedef struct SPIInterfaceIO {
    /** 
    * Close the stream
    * For failure return SPI_IO_ERR 
    */
    int (*close) (int fd);

    /** 
    * Close the directory stream.
    * 
    * For success return zero 
    * For failure return SPI_IO_ERR 
    */
    int (*closeDir) (void *dir);

    /** 
    * Move around in the file.
    * @param fd 
    * @param offset     [in] New position, relative to begin, current position 
    *                        or the end of the file
    * @param whence     [in] SEEK_SET, SEEK_CUR or SEEK_END, as defined in 
    *                        &lt;stdio.h&gt;.
    *
    * @return the new offset in the file, or SPI_IO_ERR 
    */
    jlong(*seek) (int fd, jlong offset, int whence);

    /** 
    * Read from the stream.
    * @param fd The file-system-specific file-descriptor
    * @param buf A buffer in which the read bytes are placed
    *            must be big enough to hold count bytes.
    * @param count The maximum number of bytes to read
    * 
    * @return the number of bytes read, or SPI_IO_ERR 
    */
    int (*read) (int fd, void *buf, unsigned int count);

    /** 
    * Write to the stream
    * 
    * Returns the number of bytes written
    * For failure return SPI_IO_ERR
    */
    int (*write) (int fd, const void *buf, unsigned int count);

    /** 
    * Set the length of the file
    * 
    * For success return zero
    * For failure return SPI_IO_ERR 
    */
    int (*setLength) (int fd, jlong length);

    /** 
    * clear the write buffer  
    * 
    * For success return zero
    * For failure return SPI_IO_ERR 
    */
    int (*sync) (int fd);

    /** 
    * How many more bytes are available from this stream.  
    * 
    * For success return number of available bytes
    * For failure return SPI_IO_ERR
    */
    int (*available) (int fd, jlong * bytes);

    /** 
    * Get file size from a file decriptor
    *
    * For success return the file size
    * For failure return SPI_IO_ERR
    */
    int (*fileSizeFD) (int fd, jlong * size);

    /** 
    * Get next directory entry
    * 
    * Use the struct SPIioDirItr declared earlier in this file for iteration.
    *
    * The next file name should be stored in the fileName string (unless no 
    * more files). 
    *
    * If directory entry is found and stored in *entry, return 1
    * If no more directory entries, return 0
    * For Failure, return SPI_IO_ERR
    */
    int (*readDir) (const void *dir, SPIioDirItr * entry,
                        SPIioDirItr ** result);

    /** 
    * Open a file. 
    * 
    * Returns an int file handle.
    * Return SPI_IO_ERR if open() fails.
    * 
    * Returns an int fd
    */
    int (*open) (const char *pathname, int openMode, int filePerm);

    /** 
    * Returns reason for a failure to open(). 
    * The return value need only be valid if the last API called was open(),
    * and open() returned SPI_IO_ERR.
    * The call must be thread safe (most unix/posix errno implementations are).
    * 
    * @return SPI_IO_EEXIST if an exclusive open request found that the file
    *         already exists, SPI_IO_ERR otherwise.
    * 
    */
    int (*getLastError) ();


    /** 
    * Copies the last error string into the buf, returning the length of the 
    * string.
    * The call must be thread safe and able to identify the last error.
    * @param buf a char buffer provided by the caller
    * @param len the length of the buffer provided by the caller.
    * @return the length of the null-terminated string copied into buf, or 0 
    *         if no string was copied.
    * 
    */
    int (*getLastErrorString) (char *buf, jint len);


    /** 
    * Open a directory. 
    * 
    * Returns a directory handle. 
    * Return SPI_IO_ERR if fails and dropback to default required
    * 
    * The returned handle is usually a DIR* , but is treated opaquely
    */
    void *(*openDir) (const char *pathname);

    /** 
    * Provide file attributes. 
    * 
    * For failure with dropback to default function, return SPI_IO_ERR
    *
    * Otherwise, the following flags can be returned (bitwise OR) 
    * SPI_IO_FILETYPE_REGULAR SPI_IO_FILETYPE_DIRECTORY SPI_IO_FILETYPE_HIDDEN
    *
    * TODO - need to both define and deal with HIDDEN, which IS
    * a file attribute in Win32
    * 
    * Note that REGULAR and DIRECTORY are mutually exclusive. Note also that
    * HIDDEN, in Unix filesystems, can be derived from the file name (first
    * character is . ) and this is tested for in the Java code, so there is
    * no need to return HIDDEN for Unix or unix-like filesystems.
    */
    int (*fileType) (const char *pathname);

    char *(*nativePath) (char *pathname);

    /** 
    * Returns the file access mode
    * 
    * @param pathname the path to the file being queried
    * @param mode Upon successful return, this will hold an int with bits
    * set and cleared as per the mode flags in unix stat(), that is,
    * identical to rwx unix flags R_OK, W_OK, X_OK defined in unistd.h
    * @return zero if success.
    * For failure with dropback to default function, return anything else
    * (suggested: SPI_IO_ENOENT)
    *
    */
    int (*getMode) (const char *pathname, int *mode);

    /** 
    * Sets the file access mode
    * 
    * The mode is a bit-set of flags as in unix stat(),
    * identical to rwx unix flags R_OK, W_OK, X_OK defined in unistd.h
    *
    * @param pathname the path to the file being queried
    * @param mode Upon successful return, the file access mode will be
    * set the given mode.
    *
    * @return zero if success.
    * For failure with dropback to default function, return anything else
    * (suggested: SPI_IO_ENOENT)
    */
    int (*setMode) (const char *pathname, int mode);

    /** 
    * Checks file read/write access permissions. 
    * 
    * For success return zero
    * If there are no permissions to access this file, return SPI_IO_ERR
    * For failure with dropback to default function, return anything else
    * (suggested: SPI_IO_ENOENT)
    *
    * One or more of the following mode flags can be supplied (defined
    * in FileSystem.java, but identical to rwx unix flags R_OK, W_OK,
    * X_OK defined in unistd.h)
    *   ACCESS_READ
    *   ACCESS_WRITE
    *   ACCESS_EXECUTE
    */
    int (*checkAccess) (const char *pathname, int mode);

    /** 
    * Sets a file attribute to read only. 
    * 
    * For success return zero
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    int (*setReadOnly) (const char *pathname);

    /** 
    * Get file size from a path
    * 
    * For success return the file size
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    jlong (*getSize) (const char *pathname);

    /** 
    * Creates a new directory
    * The direcory is created with full permissions to all users
    * 
    * For success return zero
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    int (*mkdir) (const char *pathname);

    /** 
    * Remove a file or a directory
    * 
    * For success return zero
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    int (*remove) (const char *pathname);

    /** 
    * Rename a file
    * 
    * For success return zero
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    int (*rename) (const char *frompathname, const char *topathname);

    /** 
    * Set the last modified/last accesed time of a file
    * First time is last accessed. Second is last modified
    * NOTE: If a time has a NULL value, value is IGNORED
    * 
    * For success return zero
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    int (*setFileTimes) (const char *pathname,
                             const SPIioFileTime times[2]);

    /** 
    * Get the last modified/last accesed time of a file
    * First time should be filled with last accessed
    * Second will be filled with last modified
    *
    * For success return zero
    * For failure with dropback to default function, return SPI_IO_ERR
    */
    int (*getFileTimes) (const char *pathname, SPIioFileTime times[2]);

    /** 
    * Get information about space on the disk.
    * The method must fill the four values - the fragment
    * size in bytes, the number of blocks on the disk, the number of free 
    * blocks and the number of blocks available to the program.
    * 
    * @param pathname the path to the filesystem being queried
    * @param frsize the size of each fragment on the filesystem
    * @param blocks the total number of blocks(fragments) in the filesystem
    * @param bfree the number of free blocks in the filesystem
    * @param bavail the number of blocks actually available for use
    * @return For success return zero, for failure with dropback to 
    * default function, return SPI_IO_ERR
    */
    int (*getSpace) (const char *pathname, jint * frsize, jint * blocks,
                         jint * bfree, jint * bavail);

    } SPIInterfaceIO;

#ifdef CVM_USE_SPI

    typedef struct _SPIFD {
        SPIInterfaceIO *fdHandle;
        jint fd;
    } *SPIFD;

    typedef struct _SPIDIR {
        SPIInterfaceIO *fdHandle;
        void *dir;
    } SPIDIR;

SPIInterfaceIO *
getDefaultIOSPI();

SPIFD
SPIioGetStdioFd(jlong fd);

SPIFD 
SPIcreateNewDefaultSpifd(int fd);

void 
SPIdestroySpifd(SPIFD spifd);

#endif                          /* CVM_USE_SPI */

#ifdef __cplusplus
}
#endif
#endif                          /* _SPI_IO_H */
