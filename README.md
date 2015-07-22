# docker-j2me

This project create docker images for `Oracle Java ME`.

As a subset of Java SE for embedded, it is probably the smallest JVM ever made by Oracle.

## Supported tags and respective `Dockerfile` links

-	[`latest` (*Dockerfile*)](https://github.com/ofayau/docker-j2me/blob/master/Dockerfile)

## Details

### What are these?

tl;dr : Busybox + libc 64bit + libc 32 bits + Embedded JRE = Small Java Container

All image are based on Busybox with 32 bits (and 64 bits) libs (see [docker-busybox-jvm](https://github.com/ofayau/docker-busybox-jvm) and [docker-busybox-lib32](https://github.com/ofayau/docker-busybox-lib32) ).

The overhead on top of a JRE is around 8 MB.

### Size matters

The biggest advantage of this image is its size : only 21 MB !!!

It's 24 times smaller than the official java image (not really fair cause it's a full 64 bits JVM and based on a full Debian) !

Here is a list of some java image.

```sh
REPOSITORY                  TAG          IMAGE ID        CREATED         VIRTUAL SIZE
ofayau/j2me                 latest       f56c31b1cc20    26 hours ago    21.73 MB
frolvlad/alpine-oraclejdk8  latest       8e87306ea37d    7 weeks ago     170.4 MB
jeanblanchard/busybox-java  8            f9b532dbdd9f    3 months ago    162 MB
java                        8-jre        b0f21df5333b    5 months ago    478.7 MB
```

### Drawback

- rather old truncated JRE : based on a J2SE 1.4, it doesn't contain every java package.

- 32 bits only : you can't run 64 bits code.

- the command is `cvm`, not `java`.

- JRE, not JDK : hence there is no compiler (`javac`) included, only runtime executor (`cvm`). You have to compile somewhere else (i.e. a standard jdk).


## License

- *Busybox* : see [license information](http://www.busybox.net/license.html).

- *Debian libc-i386* : see [licence](http://ftp-master.metadata.debian.org/changelogs/main/g/glibc/glibc_2.19-18_copyright) of [debian package](https://packages.debian.org/jessie/libc6-i386).

- *Oracle and Java* : see [OTN licence](http://www.oracle.com/technetwork/licenses/standard-license-152015.html).

Oracle and Java are registered trademarks of Oracle and/or its affiliates.

The Java softwares (including JDK or JRE) belong to Oracle and this redistribution is not for commercial use.

The ejdk directory is the unzipped version of `ejdk-8u51-linux-i586.tar.gz` available (with login) on [Oracle](http://www.oracle.com/technetwork/java/embedded/embedded-se/downloads/index.html)


## Installation & Usage

Download or update image :

```sh
docker pull ofayau/j2me
```

Showing java version of every image :

```sh
docker run --rm -it ofayau/j2me cvm -version
Product version "Oracle Java Micro Edition Embedded Client 1.1.1"
*Java(TM) ME CDC 1.1.2 Runtime Environment (build 1.1.1-b02)*
Components: CDC JSR218, FP JSR219, JAAS JSR219, JCE JSR219, JSSE JSR219, RMI JSR66, JDBC JSR169, XML JSR280
JVM:        CVM (mixed mode)
```

Simple runs

```sh
# Run a fat jar from current dir with full JRE
docker run --rm -v "$PWD":/tmp/myapp -w /tmp/myapp ofayau/j2me cvm -jar myFatJar.jar
```

Compile and run for compact1

```sh
# Compile with jdk (outside container)
javac -target 1.4 HelloWorld.java
# Run a "HelloWorld" class from current dir
docker run --rm -v "$PWD":/tmp/myapp -w /tmp/myapp ofayau/j2me cvm HelloWorld.class
```

