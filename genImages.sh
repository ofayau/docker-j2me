#!/bin/sh

# Version
ejdk_zip=ojec-1.1.1-rr-bin-b02-linux-x86-generic-product-22_apr_2013.zip
ejdk=ojec1.1.1

# unzip ejdk downloaded from Oracle.com
unzip -o $ejdk_zip -x index*

# Build docker image
docker build -t j2me .

# Clean unzip jdk
#rm -rf $ejdk

echo "Image were builded OK"

