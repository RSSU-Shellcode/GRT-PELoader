#ifndef BOOT_H
#define BOOT_H

#include "errno.h"
#include "pe_loader.h"

#define MODE_EMBED_IMAGE 1
#define MODE_LOCAL_FILE  2
#define MODE_HTTP_SERVER 3

#define ARG_ID_PE_IMAGE       1
#define ARG_ID_CMDLINE_A      2
#define ARG_ID_CMDLINE_W      3
#define ARG_ID_WAIT_MAIN      4
#define ARG_ID_ALLOW_SKIP_DLL 5
#define ARG_ID_STD_INPUT  	  6
#define ARG_ID_STD_OUTPUT 	  7
#define ARG_ID_STD_ERROR      8

#define EMBED_DISABLE_COMPRESS 0
#define EMBED_ENABLE_COMPRESS  1

#define ERR_INVALID_OPTION_STUB      0x7F000001
#define ERR_NOT_FOUND_PE_IMAGE       0x7F000101
#define ERR_EMPTY_PE_IMAGE_DATA      0x7F000102
#define ERR_NOT_FOUND_CMDLINE_A      0x7F000103
#define ERR_NOT_FOUND_CMDLINE_W      0x7F000104
#define ERR_NOT_FOUND_WAIT_MAIN      0x7F000105
#define ERR_INVALID_WAIT_MAIN        0x7F000106
#define ERR_NOT_FOUND_ALLOW_SKIP_DLL 0x7F000107
#define ERR_INVALID_ALLOW_SKIP_DLL   0x7F000108
#define ERR_NOT_FOUND_STD_INPUT      0x7F000109
#define ERR_INVALID_STD_INPUT        0x7F00010A
#define ERR_NOT_FOUND_STD_OUTPUT     0x7F00010B
#define ERR_INVALID_STD_OUTPUT       0x7F00010C
#define ERR_NOT_FOUND_STD_ERROR      0x7F00010D
#define ERR_INVALID_STD_ERROR        0x7F00010E
#define ERR_INVALID_IMAGE_CONFIG     0x7F000201
#define ERR_INVALID_LOAD_MODE        0x7F000202
#define ERR_INVALID_EMBED_CONFIG     0x7F000203
#define ERR_INVALID_COMPRESS_DATA    0x7F000204
#define ERR_INVALID_PE_IMAGE         0x7F000205
#define ERR_INVALID_HTTP_STATUS_CODE 0x7F000206
#define ERR_ERASE_ARGUMENTS          0x7F000301

PELoader_M* Boot();

#endif // BOOT_H
