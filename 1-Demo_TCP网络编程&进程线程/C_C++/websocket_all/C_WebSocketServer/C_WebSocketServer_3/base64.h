#ifndef __BASE64_H__
#define __BASE64_H__
 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
 
char* base64_encode(const char* data, int data_len); 
char *base64_decode(const char* data, int data_len); 
#endif
 
 
