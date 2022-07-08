#pragma once

#include <sstream>


typedef void (*B64Fun)(void *pOpaque, const unsigned char b);

size_t b64_decod_length(size_t srclen);


bool b64_encode (const unsigned char *, size_t ,std::stringstream &ss);
bool b64_encode (const unsigned char *, size_t , B64Fun call,void *pOpaque);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base64 decoded string.
 */
bool b64_decode (const char *, size_t, B64Fun call,void *pOpaque);

/**
 * Dencode `char *' source with `size_t' size.
 * Returns a `unsigned char *' base64 decoded string + size of decoded string.
 */
unsigned char *
b64_decode_ex (const char *, size_t, size_t *);
 
 