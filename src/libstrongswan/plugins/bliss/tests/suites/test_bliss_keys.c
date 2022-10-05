/*
 * Copyright (C) 2015 Andreas Steffen
 *
 * Copyright (C) secunet Security Networks AG
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "test_suite.h"

#include <bliss_private_key.h>
#include <bliss_public_key.h>

static chunk_t privkey_chunk[] = {
	{NULL, 0},
	chunk_from_chars(0x30, 0x00),
	chunk_from_chars(0x30, 0x01),
	chunk_from_chars(0x30, 0x03, 0x06, 0x01, 0x01),
	chunk_from_chars(0x30, 0x0d, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82,
					 0xa0, 0x2a, 0x05, 0x02, 0x06),
	chunk_from_chars(0x30, 0x0f, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04, 0x01, 0x82,
					 0xa0, 0x2a, 0x05, 0x02, 0x05, 0x03, 0x00),
	chunk_from_chars(0x30, 0x82, 0x04, 0x9a, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04,
					 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x02, 0x05, 0x03, 0x82, 0x03,
					 0x81, 0x00, 0x81, 0xe5, 0xd2, 0x71, 0xeb, 0x98, 0xe5, 0x24,
					 0x34, 0xe4, 0x8a, 0x27, 0x23, 0x7d, 0x7d, 0x2c, 0xa3, 0xa7,
					 0x3f, 0x87, 0xad, 0xae, 0xfa, 0xe4, 0x66, 0x1c, 0xef, 0x69,
					 0x63, 0x5e, 0x91, 0xda, 0x41, 0x45, 0xd5, 0x8a, 0xb5, 0x26,
					 0x33, 0x32, 0xe0, 0xa2, 0x9b, 0x52, 0x5e, 0x49, 0x5d, 0x0d,
					 0x62, 0x72, 0x68, 0xa5, 0x94, 0x24, 0x03, 0x98, 0x48, 0x60,
					 0x4a, 0x98, 0x97, 0x0d, 0x60, 0x7d, 0x00, 0x4f, 0xb9, 0xaf,
					 0xcb, 0x6b, 0x41, 0x3d, 0x5b, 0xe4, 0x3e, 0x9a, 0xee, 0x06,
		/*  100 */	 0xa1, 0xd0, 0x93, 0x53, 0x88, 0x58, 0x83, 0xb2, 0x44, 0xa1,
					 0x16, 0x58, 0x3d, 0x32, 0xa1, 0x29, 0x85, 0x1a, 0x24, 0xc8,
					 0xb8, 0x8c, 0x1f, 0x43, 0xbb, 0x4b, 0xdd, 0x8e, 0x72, 0xd3,
					 0xf4, 0xfc, 0x02, 0x69, 0x47, 0xa5, 0x9d, 0xd0, 0xfc, 0xa6,
					 0x94, 0x2e, 0x02, 0x6d, 0x85, 0x2c, 0x6d, 0xe3, 0x91, 0xd5,
					 0xf1, 0x54, 0xbd, 0x1e, 0x63, 0x6b, 0xee, 0x28, 0xf9, 0xc6,
					 0xec, 0x05, 0x99, 0xd5, 0xdd, 0xe5, 0x72, 0x9b, 0xbc, 0xa7,
					 0x5a, 0x4a, 0x46, 0x3e, 0xec, 0xd7, 0x0b, 0xc5, 0x23, 0x00,
					 0xdc, 0x08, 0x09, 0x57, 0x44, 0x2e, 0x43, 0x0f, 0xea, 0xca,
					 0x2a, 0x31, 0xbe, 0xf3, 0x04, 0x8f, 0x8b, 0xa6, 0x3c, 0x35,
		/*  200 */	 0x80, 0x2b, 0xe2, 0x18, 0x22, 0xfd, 0xe9, 0x39, 0x57, 0xed,
					 0x77, 0x1d, 0x32, 0x02, 0x48, 0x2c, 0x85, 0x53, 0x9f, 0x4a,
					 0xd8, 0x86, 0x4d, 0xd2, 0x26, 0x19, 0x12, 0x19, 0xa2, 0xb5,
					 0xdf, 0x02, 0x50, 0xe4, 0x32, 0x9a, 0x27, 0xd0, 0x9e, 0x49,
					 0x4a, 0x13, 0x9a, 0xfc, 0x07, 0x98, 0x60, 0x65, 0xf4, 0xc1,
					 0x6c, 0x9a, 0x15, 0x28, 0x74, 0x5c, 0xd0, 0xa8, 0xe6, 0x2e,
					 0x1f, 0xe9, 0xe6, 0x2b, 0xc8, 0x46, 0xe9, 0x26, 0xb0, 0xf0,
					 0x8a, 0xe6, 0x8c, 0x9b, 0xbf, 0x64, 0xa0, 0x59, 0x33, 0x4f,
					 0xc0, 0x0c, 0x16, 0x72, 0x89, 0x79, 0x2a, 0x3a, 0x5e, 0x3d,
					 0x40, 0xbb, 0x73, 0xa9, 0xc0, 0x52, 0x70, 0x57, 0x06, 0xc1,
		/*  300 */	 0xe7, 0x70, 0xb8, 0x6d, 0x1b, 0x50, 0x61, 0x85, 0xee, 0x3e,
					 0xe5, 0x5a, 0x8a, 0x75, 0x9f, 0x1e, 0xb7, 0xea, 0x54, 0x5a,
					 0x8f, 0x52, 0xc2, 0xae, 0x2c, 0x7a, 0x58, 0xe6, 0xcb, 0xa6,
					 0x9b, 0x68, 0x84, 0x79, 0xf2, 0x82, 0x05, 0x57, 0xaa, 0xd5,
					 0x51, 0x82, 0xec, 0x84, 0x63, 0xce, 0xf4, 0xa7, 0xdf, 0x4e,
					 0xac, 0x7d, 0xdd, 0xc3, 0x02, 0x68, 0xe0, 0x35, 0xa1, 0x92,
					 0x29, 0x02, 0x2c, 0xa0, 0xe4, 0x29, 0x66, 0xd3, 0xe8, 0xd9,
					 0x52, 0x0f, 0x3b, 0xec, 0x53, 0x63, 0x57, 0xc3, 0xd2, 0x59,
					 0x38, 0xe7, 0x74, 0xf4, 0x1d, 0x03, 0x88, 0x3c, 0xe9, 0x97,
					 0x37, 0xd7, 0x12, 0x66, 0x2a, 0xb5, 0xf8, 0xcd, 0x10, 0x87,
		/*  400 */	 0x5d, 0x6a, 0x69, 0xbb, 0x9b, 0xc5, 0x55, 0x3c, 0x09, 0x46,
					 0x04, 0x57, 0xc0, 0x2f, 0x77, 0x89, 0xe2, 0x88, 0x15, 0x6b,
					 0x71, 0x56, 0xe1, 0xa2, 0x30, 0x71, 0x5f, 0x1d, 0x27, 0x12,
					 0xbf, 0xc3, 0x55, 0xde, 0xe5, 0x9c, 0x4e, 0xb8, 0xc6, 0xec,
					 0x96, 0x3a, 0x5d, 0x6d, 0xe9, 0xd3, 0xf8, 0x28, 0xda, 0x3f,
					 0x75, 0x24, 0xd0, 0x34, 0x50, 0xa6, 0x28, 0x65, 0x6a, 0xe9,
					 0xa6, 0x89, 0xe5, 0x5d, 0x45, 0xaf, 0x63, 0x34, 0xaf, 0x31,
					 0x29, 0x82, 0xe6, 0x03, 0x80, 0x5c, 0x34, 0x28, 0xd1, 0x9f,
					 0xca, 0xd3, 0x96, 0xcb, 0x31, 0xde, 0xdb, 0xf0, 0x07, 0x2b,
					 0xc5, 0xbc, 0x29, 0xd1, 0x11, 0xf4, 0x23, 0x3b, 0x14, 0xb5,
		/*  500 */	 0xa6, 0xf1, 0x02, 0x9e, 0x66, 0xbe, 0xdc, 0xc4, 0xca, 0xf7,
					 0xc0, 0x81, 0x92, 0x7c, 0xea, 0xe3, 0x42, 0x54, 0x8a, 0x6f,
					 0x0a, 0x2a, 0xa7, 0x2a, 0x92, 0xab, 0x09, 0xb1, 0x61, 0x91,
					 0xaa, 0x90, 0x54, 0xa3, 0x76, 0x64, 0xe2, 0xfd, 0x81, 0x9a,
					 0x4c, 0x35, 0x11, 0x28, 0xf3, 0x14, 0x97, 0x1b, 0x61, 0xa4,
					 0x67, 0x43, 0xae, 0x90, 0x6b, 0xe4, 0x29, 0x34, 0xec, 0x08,
					 0xbc, 0x6a, 0x82, 0x45, 0xc7, 0x7d, 0xdc, 0xd0, 0x03, 0x98,
					 0x29, 0x63, 0x05, 0x94, 0xb2, 0xb9, 0x04, 0xce, 0x34, 0x9a,
					 0x64, 0xae, 0x9a, 0xa9, 0x11, 0xa5, 0x13, 0x07, 0xcc, 0x92,
					 0xe9, 0xe5, 0x98, 0x13, 0x13, 0x8f, 0x8b, 0xb2, 0x77, 0x75,
		/*  600 */	 0x2a, 0x6f, 0xb1, 0xa6, 0x98, 0xbf, 0x50, 0xaf, 0xa7, 0x15,
					 0x2a, 0xe6, 0xdf, 0x41, 0xb6, 0x5e, 0x72, 0xb2, 0x74, 0xf2,
					 0x38, 0x88, 0x41, 0x56, 0x53, 0xea, 0x83, 0x23, 0x8a, 0x6d,
					 0x6c, 0x64, 0x6c, 0xa6, 0x04, 0x79, 0x51, 0x92, 0x89, 0xbe,
					 0x2a, 0x54, 0xd8, 0x5a, 0x8d, 0x5b, 0x9c, 0xfc, 0x62, 0x05,
					 0x0f, 0xbd, 0x85, 0x12, 0x57, 0x45, 0x96, 0x2e, 0x8f, 0x76,
					 0xd4, 0x33, 0xfb, 0x4a, 0xc2, 0x9f, 0x57, 0x96, 0xb3, 0xa2,
					 0xc6, 0xa6, 0x95, 0x3c, 0x9e, 0x7e, 0x15, 0x12, 0xd7, 0xe4,
					 0x65, 0x05, 0x5d, 0x72, 0xc2, 0x28, 0x10, 0xa9, 0x68, 0xa9,
					 0x01, 0xfe, 0x9e, 0x36, 0x07, 0x80, 0x41, 0xc8, 0xa3, 0x5f,
		/*  700 */	 0x18, 0x3b, 0x38, 0x09, 0x95, 0xe2, 0x87, 0xad, 0x03, 0xfd,
					 0xdd, 0xa6, 0xe9, 0x8e, 0xa8, 0x3a, 0xc9, 0x45, 0x7b, 0xdc,
					 0xc2, 0x6a, 0x30, 0x78, 0xaa, 0xba, 0x32, 0xe9, 0x8a, 0x65,
					 0x48, 0x13, 0x5b, 0x29, 0x18, 0x2e, 0x5c, 0x68, 0x8d, 0x71,
					 0x01, 0x09, 0xab, 0x7d, 0x1a, 0xe9, 0x09, 0x74, 0x1b, 0xe1,
					 0x90, 0x00, 0xb9, 0xda, 0xa3, 0x03, 0xb7, 0x6c, 0xdd, 0x40,
					 0xb6, 0xe3, 0xde, 0xa6, 0x7b, 0xe9, 0x3d, 0x41, 0x4d, 0xc7,
					 0xad, 0xa5, 0xf9, 0x8b, 0x88, 0xd4, 0x1a, 0x75, 0xb5, 0xb6,
					 0x9f, 0x51, 0x9b, 0x8b, 0xd7, 0xa4, 0x02, 0xb0, 0x62, 0x45,
					 0xdd, 0x6c, 0x11, 0x35, 0x03, 0x77, 0x1c, 0xdb, 0xc5, 0xac,
		/*  800 */	 0x60, 0x37, 0x20, 0x15, 0xaf, 0xbd, 0xae, 0x76, 0x51, 0xd2,
					 0xfb, 0x63, 0x23, 0x19, 0x81, 0xa6, 0x59, 0x7b, 0x68, 0x00,
					 0x3d, 0x68, 0x89, 0x6b, 0x5a, 0x29, 0xbd, 0x4f, 0xc1, 0x50,
					 0xe4, 0x98, 0x85, 0xe6, 0x1a, 0xdd, 0xc8, 0xe4, 0xa1, 0x2b,
					 0x99, 0x42, 0x81, 0x4d, 0x07, 0xf4, 0x24, 0x93, 0x88, 0xfe,
					 0x40, 0x90, 0x5a, 0x56, 0x0b, 0x7f, 0x8d, 0x14, 0x82, 0x6d,
					 0xaf, 0xf6, 0x0a, 0x3d, 0xe6, 0x64, 0xb5, 0x48, 0x01, 0x37,
					 0xfe, 0xf3, 0xba, 0x67, 0xcc, 0xd2, 0xba, 0x32, 0x76, 0xe8,
					 0xa7, 0x41, 0x1f, 0x2a, 0xfc, 0xa9, 0x72, 0x66, 0xc7, 0xd5,
					 0x76, 0x02, 0x6b, 0x77, 0xba, 0x6c, 0xd4, 0x84, 0x68, 0x0e,
		/*  900 */	 0x62, 0xc8, 0x43, 0xb0, 0x81, 0xd5, 0x8f, 0xdb, 0x42, 0xc9,
					 0xf4, 0xaf, 0x71, 0xbd, 0xb9, 0x6c, 0xd6, 0xdc, 0x03, 0x81,
					 0x81, 0x00, 0xc5, 0x10, 0x40, 0x33, 0x0f, 0xc0, 0x14, 0x01,
					 0x00, 0x03, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x4c, 0x0f, 0x10,
					 0x03, 0x10, 0x00, 0x00, 0x01, 0xc0, 0x43, 0x40, 0x03, 0x5c,
					 0x00, 0x07, 0xc0, 0x51, 0x34, 0x01, 0x30, 0x0c, 0x00, 0x00,
					 0x04, 0xc0, 0x3d, 0x40, 0x03, 0x07, 0x40, 0xd3, 0x50, 0x0c,
					 0x04, 0x03, 0x00, 0x11, 0x41, 0x30, 0x00, 0xc1, 0xc0, 0xc3,
					 0x03, 0x5f, 0x04, 0x30, 0x01, 0x40, 0x40, 0x00, 0x40, 0x40,
					 0x10, 0x40, 0x05, 0x05, 0x00, 0x53, 0x00, 0x04, 0x50, 0x00,
		/* 1000 */	 0x00, 0x00, 0x0c, 0x00, 0x51, 0x00, 0x00, 0x00, 0x04, 0xc7,
					 0x01, 0x50, 0xc0, 0x11, 0x00, 0x04, 0x03, 0xc0, 0x04, 0x00,
					 0x70, 0x4c, 0x31, 0x03, 0xc0, 0x40, 0xc4, 0x40, 0x40, 0xc0,
					 0x0c, 0x0c, 0xf1, 0x40, 0xc1, 0x31, 0x70, 0x17, 0xc0, 0x30,
					 0xc1, 0x04, 0x0c, 0x04, 0x00, 0xc4, 0x01, 0x00, 0x34, 0x00,
					 0x03, 0x81, 0x81, 0x00, 0xcc, 0x00, 0x50, 0x30, 0xc4, 0x13,
					 0x0f, 0xf0, 0x43, 0x01, 0x33, 0x40, 0x30, 0x01, 0x40, 0x10,
					 0x57, 0x04, 0x03, 0x04, 0x10, 0x00, 0xf0, 0x03, 0x04, 0x01,
					 0x00, 0x10, 0x34, 0x03, 0xf0, 0x1c, 0x01, 0x40, 0x30, 0xf4,
					 0x00, 0x40, 0x34, 0xc3, 0x00, 0x00, 0x01, 0x00, 0x01, 0x10,
		/* 1100 */	 0x3f, 0x03, 0x40, 0x00, 0x10, 0x10, 0x00, 0x40, 0x03, 0x00,
					 0x03, 0x04, 0x40, 0x03, 0x00, 0x13, 0x03, 0x00, 0xc0, 0x01,
					 0x34, 0x01, 0x00, 0x00, 0x10, 0xf4, 0x00, 0xf0, 0x30, 0x00,
					 0x00, 0xc3, 0x1c, 0x41, 0x00, 0x40, 0x30, 0x04, 0x10, 0xc4,
					 0x11, 0x03, 0x00, 0x10, 0x04, 0x4f, 0x17, 0xc0, 0x00, 0x30,
					 0xcd, 0x3c, 0x40, 0xc4, 0x00, 0xf0, 0x00, 0x00, 0x04, 0x30,
					 0x0f, 0x31, 0x34, 0xf0, 0x00, 0x07, 0x0c, 0x34, 0x00, 0x50,
					 0x05, 0x03, 0x10, 0x70, 0x00, 0x33, 0x0c, 0x00, 0xc4, 0x54,
					 0x07, 0x00)
};

START_TEST(test_bliss_keys_priv)
{
	private_key_t *privkey;

	privkey = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_BLISS,
								 BUILD_BLOB, privkey_chunk[_i], BUILD_END);
	if (_i == countof(privkey_chunk) - 1)
	{
		ck_assert(privkey);
		privkey->destroy(privkey);
	}
	else
	{
		ck_assert(!privkey);
	}
}
END_TEST

typedef struct privkey_mod_t privkey_mod_t;

struct privkey_mod_t {
	int offset;
	char byte;
};

static privkey_mod_t privkey_mod[] = {
	{   20, 0x80 },
	{   22, 0xc1 },
	{  920, 0x80 },
	{  922, 0x85 },
	{ 1052, 0x80 },
	{ 1054, 0x8c }
};

START_TEST(test_bliss_keys_priv_mod)
{
	private_key_t *privkey;
	chunk_t data;

	data = chunk_clone(privkey_chunk[countof(privkey_chunk) - 1]);
	data.ptr[privkey_mod[_i].offset] = privkey_mod[_i].byte;

	privkey = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_BLISS,
								 BUILD_BLOB, data, BUILD_END);
	ck_assert(!privkey);
	chunk_free(&data);
}
END_TEST

static chunk_t pubkey_chunk[] = {
	{NULL, 0},
	chunk_from_chars(0x30, 0x00),
	chunk_from_chars(0x30, 0x01),
	chunk_from_chars(0x30, 0x02, 0x30, 0x00),
	chunk_from_chars(0x30, 0x05, 0x30, 0x03, 0x06, 0x01, 0x01),
	chunk_from_chars(0x30, 0x11, 0x30, 0x0F, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04,
					 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x01, 0x01, 0x04, 0x00),
	chunk_from_chars(0x30, 0x12, 0x30, 0x10, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04,
					 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x01, 0x01, 0x06, 0x01, 0x01),
	chunk_from_chars(0x30, 0x1c, 0x30, 0x1a, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04,
					 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x01, 0x01, 0x06, 0x0b, 0x2b,
					 0x06, 0x01, 0x04, 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x02, 0x06),
	chunk_from_chars(0x30, 0x1e, 0x30, 0x1a, 0x06, 0x0b, 0x2b, 0x06, 0x01, 0x04,
					 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x01, 0x01, 0x06, 0x0b, 0x2b,
					 0x06, 0x01, 0x04, 0x01, 0x82, 0xa0, 0x2a, 0x05, 0x02, 0x05,
					 0x03, 0x00)
};

START_TEST(test_bliss_keys_pub)
{
	public_key_t *pubkey;

	pubkey = lib->creds->create(lib->creds, CRED_PUBLIC_KEY, KEY_ANY,
								 BUILD_BLOB, pubkey_chunk[_i], BUILD_END);
	ck_assert(!pubkey);
}
END_TEST

Suite *bliss_keys_suite_create()
{
	Suite *s;
	TCase *tc;

	s = suite_create("bliss_keys");

	tc = tcase_create("keys_priv");
	tcase_add_loop_test(tc, test_bliss_keys_priv, 0, countof(privkey_chunk));
	suite_add_tcase(s, tc);

	tc = tcase_create("keys_priv_mod");
	tcase_add_loop_test(tc, test_bliss_keys_priv_mod, 0, countof(privkey_mod));
	suite_add_tcase(s, tc);

	tc = tcase_create("keys_pub");
	tcase_add_loop_test(tc, test_bliss_keys_pub, 0, countof(pubkey_chunk));
	suite_add_tcase(s, tc);

	return s;
}
