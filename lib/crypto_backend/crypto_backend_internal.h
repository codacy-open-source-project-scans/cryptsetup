/*
 * crypto backend implementation
 *
 * Copyright (C) 2010-2024 Red Hat, Inc. All rights reserved.
 * Copyright (C) 2010-2024 Milan Broz
 *
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef _CRYPTO_BACKEND_INTERNAL_H
#define _CRYPTO_BACKEND_INTERNAL_H

#include "crypto_backend.h"

/* internal PBKDF2 implementation */
int pkcs5_pbkdf2(const char *hash,
		 const char *P, size_t Plen,
		 const char *S, size_t Slen,
		 unsigned int c,
		 unsigned int dkLen, char *DK,
		 unsigned int hash_block_size);

/* Argon2 implementation wrapper */
int argon2(const char *type, const char *password, size_t password_length,
	   const char *salt, size_t salt_length,
	   char *key, size_t key_length,
	   uint32_t iterations, uint32_t memory, uint32_t parallel);

/* Block ciphers: fallback to kernel crypto API */

struct crypt_cipher_kernel {
	int tfmfd;
	int opfd;
};

int crypt_cipher_init_kernel(struct crypt_cipher_kernel *ctx, const char *name,
			     const char *mode, const void *key, size_t key_length);
int crypt_cipher_encrypt_kernel(struct crypt_cipher_kernel *ctx,
				const char *in, char *out, size_t length,
				const char *iv, size_t iv_length);
int crypt_cipher_decrypt_kernel(struct crypt_cipher_kernel *ctx,
				const char *in, char *out, size_t length,
				const char *iv, size_t iv_length);
void crypt_cipher_destroy_kernel(struct crypt_cipher_kernel *ctx);
int crypt_bitlk_decrypt_key_kernel(const void *key, size_t key_length,
				   const char *in, char *out, size_t length,
				   const char *iv, size_t iv_length,
				   const char *tag, size_t tag_length);

/* Internal implementation for constant time memory comparison */
static inline int crypt_internal_memeq(const void *m1, const void *m2, size_t n)
{
	const unsigned char *_m1 = (const unsigned char *) m1;
	const unsigned char *_m2 = (const unsigned char *) m2;
	unsigned char result = 0;
	size_t i;

	for (i = 0; i < n; i++)
		result |= _m1[i] ^ _m2[i];

	return result;
}

#endif /* _CRYPTO_BACKEND_INTERNAL_H */
