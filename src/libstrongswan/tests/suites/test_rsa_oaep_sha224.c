/*
 * Copyright (C) 2021 Andreas Steffen, strongSec GmbH
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

#include <plugins/plugin_feature.h>

typedef struct rsa_oaep_test_t rsa_oaep_test_t;

struct rsa_oaep_test_t {
	chunk_t msg;
	chunk_t label;
	chunk_t ct;
};

/**
 * Private RSA key to load
 */
static chunk_t rsa_2048_key = chunk_from_chars( /* PKCS#8 format */
		0x30,0x82,0x04,0xbe,0x02,0x01,0x00,0x30,0x0d,0x06,0x09,0x2a,0x86,0x48,0x86,0xf7,
		0x0d,0x01,0x01,0x01,0x05,0x00,0x04,0x82,0x04,0xa8,0x30,0x82,0x04,0xa4,0x02,0x01,
		0x00,0x02,0x82,0x01,0x01,0x00,0xc3,0x2c,0xd0,0xe1,0x44,0x1f,0xde,0x8a,0x28,0x96,
		0xca,0x3a,0x13,0x37,0x35,0xbe,0x2d,0x10,0x10,0x77,0x7c,0xfc,0x73,0x9a,0xfc,0x77,
		0xb6,0xda,0xa6,0x6f,0x36,0x7d,0x48,0x76,0xdc,0xcb,0x30,0x21,0xfc,0x22,0xc2,0x54,
		0x50,0xa6,0x8d,0x6c,0xfb,0x11,0x91,0xd4,0x85,0xcb,0xfb,0xa5,0xec,0x45,0xb4,0x92,
		0x86,0xd7,0xca,0xe2,0xbd,0xae,0x55,0x3f,0x47,0xe1,0x0b,0x94,0xf8,0x67,0xab,0xcc,
		0x6d,0x0a,0xff,0xc7,0x33,0xba,0xcc,0x72,0x5e,0x5a,0xb4,0xde,0x1a,0xba,0x19,0xa3,
		0x9d,0x74,0x8b,0x4c,0x13,0x55,0xd5,0xa6,0xa7,0x10,0xa5,0x2b,0xd0,0x4c,0x0c,0x24,
		0xe7,0xbc,0x3b,0xda,0xb8,0xf3,0xce,0x3a,0xe8,0x6e,0xcb,0x31,0xc4,0xb4,0x5e,0x10,
		0xb4,0x0d,0xdb,0x5f,0xdd,0x40,0xcb,0x24,0x11,0xbc,0xf5,0xb1,0xd3,0x92,0xe1,0xee,
		0xf9,0x59,0xcf,0xf2,0x70,0x9a,0x6e,0x02,0xb2,0x0f,0xf3,0xb4,0x34,0x36,0x41,0xa6,
		0xb7,0x85,0x99,0x58,0x6e,0xdc,0x9b,0x67,0x3d,0x9f,0x3f,0x5e,0x9d,0x33,0x9c,0xee,
		0xbf,0x96,0xa1,0xa3,0x16,0x55,0x87,0x6c,0x39,0xfc,0xb0,0x0b,0x1c,0x3e,0x57,0x19,
		0x08,0xc9,0xb7,0x44,0x76,0x50,0x47,0xab,0xb5,0xc2,0x3e,0xcc,0x42,0xe5,0x51,0xe1,
		0x37,0x55,0xe3,0x8c,0xc9,0xa1,0x3e,0x1e,0x02,0xbc,0xd5,0xdc,0xec,0x9c,0x30,0x1f,
		0xab,0x75,0xbe,0x3e,0x1a,0x8e,0xe9,0xc4,0x29,0x81,0x60,0x7a,0xba,0x78,0x55,0xf4,
		0xbb,0xe7,0x6c,0x8c,0x16,0x0e,0x80,0x46,0x8b,0x54,0xbd,0xf9,0xf4,0x38,0xb1,0x77,
		0xc3,0x3d,0xee,0x30,0xb0,0xf5,0x02,0x03,0x01,0x00,0x01,0x02,0x82,0x01,0x00,0x56,
		0xd0,0x75,0x6c,0xed,0xdf,0x7b,0x1e,0x5b,0x25,0x8f,0x78,0x3b,0x99,0xe0,0x36,0xe2,
		0x56,0x75,0xec,0xa0,0x54,0xae,0x9b,0x6e,0xd7,0x55,0x27,0x76,0xc6,0x9b,0x27,0x28,
		0xf7,0x6e,0x08,0x97,0x35,0x56,0xb0,0xa3,0x5d,0xdb,0xad,0xe9,0xd4,0x62,0xed,0x12,
		0xbf,0xc4,0x6f,0xd2,0x54,0xa0,0x7e,0xf4,0xee,0x04,0x3a,0xb2,0x4d,0x1e,0xf0,0x0f,
		0x8d,0x21,0x4c,0xd1,0xd9,0x06,0x91,0x1e,0x92,0xc4,0xa2,0x12,0xd9,0xa9,0x81,0xda,
		0x74,0xb8,0xd1,0x82,0x08,0x15,0x3d,0x58,0x30,0x35,0xd6,0x64,0x2b,0x87,0xa2,0x33,
		0x71,0x78,0x78,0x67,0xef,0xd0,0x2c,0x33,0x6e,0xab,0x01,0x48,0x62,0x66,0xc8,0x53,
		0xa0,0x52,0x49,0x0d,0xea,0xea,0x43,0x0c,0x60,0x43,0xa6,0xb2,0x40,0xb6,0xe9,0xd7,
		0x1e,0x16,0xf2,0x92,0x55,0xf2,0xce,0xeb,0x35,0xd1,0xa4,0xae,0x25,0xae,0x0d,0xc9,
		0xa4,0x36,0xfb,0x5d,0xc3,0x03,0x81,0xcc,0xe9,0x82,0xac,0xc8,0x24,0x96,0x19,0x76,
		0xdf,0x68,0x31,0x73,0xa0,0x2a,0x54,0x0c,0x40,0x3f,0x3c,0x85,0x60,0x24,0x3c,0xeb,
		0x5b,0x79,0x8a,0xbc,0xdc,0x20,0xf3,0xc8,0x5d,0x95,0x32,0xb0,0xf0,0xb0,0x82,0x6f,
		0x1b,0x63,0x52,0xc5,0xad,0xac,0x75,0x7f,0xe3,0x22,0x4b,0x82,0x24,0x55,0xcc,0x52,
		0x9f,0xcd,0xc8,0xa2,0x20,0xb0,0x46,0x9f,0x32,0x1f,0x56,0xbd,0x18,0x53,0xd8,0xa7,
		0x0b,0x89,0x3f,0x40,0x4c,0xc0,0x63,0x17,0xe0,0x84,0x17,0x37,0x70,0xc7,0xd4,0xc8,
		0x36,0x28,0x1a,0xc2,0x51,0x35,0x3f,0xce,0xe4,0xac,0x39,0x38,0x38,0xa1,0xa1,0x02,
		0x81,0x81,0x00,0xea,0x60,0x10,0xa0,0x01,0x7d,0x40,0x73,0x19,0x86,0x10,0xd0,0xe7,
		0x61,0xf2,0x86,0xe3,0xa1,0x13,0x3e,0xee,0xd7,0x1b,0x4f,0xd4,0xdf,0xb0,0xd8,0xb5,
		0x2e,0x1e,0x1d,0x8f,0xfc,0x58,0xd9,0xc1,0x03,0x96,0x62,0x99,0x52,0xeb,0x00,0x23,
		0x69,0xb2,0x2b,0xb9,0xc3,0x31,0x3c,0x80,0x33,0x0c,0x1f,0x3a,0xb7,0xa3,0xfc,0xf8,
		0xd0,0xfb,0xf5,0xa1,0xc0,0xfb,0x79,0x36,0xef,0x20,0xd5,0x13,0x73,0xb2,0x1b,0xea,
		0x72,0xfc,0xb7,0xf7,0x02,0x75,0xac,0x41,0xd9,0x7b,0x56,0x6b,0x71,0xf7,0x27,0xa3,
		0x2f,0x00,0x06,0xde,0x15,0xe7,0xf9,0xbc,0xcf,0xcc,0x32,0x24,0x10,0x6f,0x6b,0x1a,
		0xe6,0x2d,0x59,0x38,0x00,0x65,0xa4,0x8e,0xf7,0xbd,0x01,0x5e,0xe6,0xda,0xba,0xe0,
		0xf9,0xd7,0x89,0x02,0x81,0x81,0x00,0xd5,0x2e,0xd8,0x08,0xd7,0x4a,0x79,0xb3,0xab,
		0x51,0x5b,0x4f,0x2f,0xe7,0xb4,0x10,0x27,0x47,0x2c,0x37,0x85,0xeb,0x25,0x58,0x7a,
		0xe3,0x1d,0x35,0x00,0xbf,0x3c,0x6e,0xf2,0x0c,0x42,0x7e,0x22,0x3e,0xfa,0x4f,0x29,
		0x11,0xcd,0x38,0xcf,0xc4,0x47,0x9f,0x28,0xae,0x49,0x5f,0xe3,0xe8,0x8e,0x94,0xc2,
		0x36,0xc0,0xbc,0x13,0x23,0xd7,0x10,0x27,0xab,0xa2,0x90,0x66,0x9f,0xfb,0xd7,0xe0,
		0xfb,0xb6,0xf6,0x15,0xdf,0x1c,0x9e,0x4f,0xe5,0xab,0xb5,0xa9,0x0d,0x4d,0x0b,0x6d,
		0x1d,0xea,0x0a,0xe2,0xb2,0x49,0xe6,0xa5,0x10,0x20,0x92,0xc9,0x98,0xb6,0x0d,0xd9,
		0x20,0xd0,0x9a,0x2e,0x6e,0xcf,0x2a,0x61,0x17,0x93,0x01,0x6b,0xc0,0xc9,0x2d,0xe3,
		0xb9,0xd9,0x27,0x44,0xbc,0x07,0x0d,0x02,0x81,0x81,0x00,0xcb,0x64,0xf6,0xc8,0x84,
		0xc2,0x57,0xae,0x0b,0x56,0x67,0x3d,0x83,0xaf,0x62,0xb3,0x60,0xd3,0xa6,0x4a,0x15,
		0x27,0xa3,0xd2,0x11,0xe0,0xd6,0x2e,0x1a,0x7d,0x9d,0x30,0xf6,0x85,0x7d,0xed,0xeb,
		0x2c,0xdb,0xd3,0x51,0x4f,0xbe,0x14,0xee,0xa6,0x89,0x32,0x9d,0x11,0x21,0xa7,0x69,
		0x71,0xe3,0x71,0x2e,0x99,0xb3,0xbc,0x93,0x89,0x79,0x3e,0xdf,0x53,0x04,0x69,0x5b,
		0x1d,0x06,0x97,0x23,0x3c,0x62,0x33,0x0b,0xb1,0x22,0x53,0xdc,0x0e,0xcc,0x63,0xe2,
		0xf9,0x83,0xa9,0xa9,0xb0,0xcb,0x56,0x20,0xad,0x67,0x0e,0x8e,0xa8,0xe0,0x19,0xc0,
		0x9b,0x6c,0x8f,0x8e,0xf0,0x9c,0x60,0x8c,0x85,0x78,0x91,0x56,0xa2,0x31,0x93,0x2f,
		0x67,0x1b,0x25,0x17,0x60,0xac,0x2d,0x45,0x94,0x4c,0x51,0x02,0x81,0x81,0x00,0xb6,
		0x16,0xbe,0xfc,0x3e,0x38,0x24,0xdf,0xac,0x53,0x5f,0x74,0xb1,0xec,0x6d,0x46,0xe6,
		0xe5,0xca,0x04,0xda,0xe4,0x51,0x0f,0xd4,0x57,0x2d,0x59,0x5a,0x7b,0xfd,0xf8,0x9d,
		0xd2,0x8e,0xf1,0x01,0xfb,0x5c,0xfe,0x44,0x8c,0x2a,0x08,0x7e,0x9e,0x9e,0xb6,0x79,
		0x9e,0xf4,0x99,0x6d,0x27,0xf4,0xb1,0x67,0x7a,0x31,0x01,0xf4,0x2f,0x46,0xc1,0x4b,
		0xc1,0x34,0xa7,0xb6,0xa0,0xac,0x12,0x66,0xdf,0x5a,0x15,0xb3,0xf4,0xd0,0x93,0x00,
		0x97,0xa2,0x25,0x16,0x72,0x7f,0xfe,0x64,0x83,0x8a,0xa7,0x25,0x9f,0x37,0xb4,0x44,
		0x05,0x14,0x6d,0x8c,0xb8,0x5d,0xb8,0x52,0x5f,0xcf,0x0e,0x02,0xdf,0x2f,0x20,0x79,
		0xb2,0x13,0x24,0xc1,0x8e,0xf7,0xc7,0xb4,0x9d,0xc7,0xb7,0xdc,0xe3,0xe5,0xf9,0x02,
		0x81,0x80,0x40,0xfb,0xda,0x2e,0x57,0x10,0x60,0x97,0x1c,0xc6,0xae,0xa0,0xc1,0xcc,
		0x82,0xbe,0xab,0x4f,0xa0,0x36,0x1d,0xe4,0x3a,0x55,0xbd,0x2b,0x39,0x9f,0x25,0xed,
		0x89,0xe3,0x3c,0x48,0x98,0x3f,0x40,0x9d,0x9f,0x27,0x29,0x27,0x70,0xd5,0xc0,0xac,
		0x1b,0x31,0xad,0xf7,0x66,0x04,0x5e,0x60,0xd6,0x7b,0x89,0x95,0x7c,0x81,0xc0,0xf3,
		0xb7,0x70,0x6c,0x3e,0x14,0x18,0x92,0x6b,0xfe,0xcb,0xa0,0x24,0x2b,0xa5,0x4e,0xd9,
		0x5a,0xe1,0xbc,0x73,0xb6,0x7d,0xdb,0x1e,0x3f,0x41,0x61,0x38,0x0c,0xbb,0xf1,0xdb,
		0x7d,0x3d,0xbe,0xf8,0x52,0xeb,0xb3,0x80,0x63,0xbc,0x25,0x44,0xc9,0xc2,0x9f,0x47,
		0xf4,0x16,0xf1,0x4e,0xb1,0xea,0x3c,0xf2,0xba,0xb9,0x33,0xd2,0x1c,0x2c,0x09,0x12,
		0x93,0xce);

/**
 * RSA OAEP SHA224 decryption test vectors from https://boringssl.googlesource.com/
 */
static rsa_oaep_test_t rsa_2048_oaep_sha224_tests[] = {
	{ chunk_from_chars(                                                          /* 2 */
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00),
	  chunk_from_chars(),
	  chunk_from_chars(
		0x9d,0xd1,0xed,0x2a,0xe7,0x25,0x7d,0x57,0x6d,0x79,0x3a,0x11,0x0e,0x0b,0x39,0xd8,
		0x5f,0xe2,0xb3,0x12,0xed,0xe2,0x86,0xf0,0xc1,0xbe,0xc4,0xfd,0x97,0x5f,0x10,0xf9,
		0xf5,0x1d,0x9c,0xb1,0xb4,0x67,0xde,0x23,0x46,0x97,0xc4,0x28,0x09,0x4a,0x87,0x87,
		0x94,0x34,0xf2,0x1f,0x91,0xe3,0x9f,0xbd,0x2e,0x1d,0x4b,0x87,0x73,0x13,0xaf,0x6d,
		0x9c,0x92,0xf4,0xa7,0xa1,0x6b,0xa1,0x48,0xfc,0x5b,0xd4,0x8c,0x54,0x05,0xe2,0x7a,
		0x9b,0xfd,0x6f,0x28,0x32,0xf0,0xf4,0x0f,0xdb,0x32,0xed,0x30,0x67,0xfe,0x29,0x52,
		0x06,0x0e,0x57,0x26,0xa9,0xa9,0x7d,0x55,0xe0,0x55,0xb6,0x23,0xcc,0x42,0xed,0x6a,
		0x4d,0xb8,0x70,0xf2,0x46,0xec,0xe6,0xe8,0x9c,0xe3,0x46,0x06,0x0a,0x4a,0x5c,0xca,
		0x21,0x4a,0x35,0xe8,0x68,0x23,0x15,0x28,0xc0,0xc1,0x84,0xa1,0x67,0xf1,0xd7,0xc3,
		0x31,0xcb,0x0c,0x1a,0xf9,0x5d,0x6a,0xbd,0x6d,0xda,0x25,0x02,0x62,0xaa,0x54,0xf2,
		0xf6,0x01,0x6d,0x12,0xd3,0xb5,0x30,0xdf,0xe0,0x9d,0x31,0xeb,0x18,0x2e,0xa3,0x71,
		0x8d,0x82,0x41,0xbf,0xa3,0xe6,0x01,0x37,0xdb,0x83,0x80,0x43,0xef,0x4c,0xce,0x8b,
		0xa2,0x8f,0xd3,0x2b,0x94,0x19,0x85,0x01,0xd8,0xd3,0xd8,0x1d,0xc9,0xcf,0x7e,0xb5,
		0x16,0x3c,0x41,0x75,0x3a,0xbe,0xe2,0xcc,0xbc,0xbd,0x36,0x8b,0x83,0x83,0xb3,0x9b,
		0xfe,0x07,0x39,0xfe,0x84,0xf3,0xe2,0xb3,0x26,0x12,0xd0,0x1b,0x32,0x4e,0x44,0x08,
		0xb7,0x27,0x31,0x83,0x56,0xee,0x38,0xc2,0x00,0xd5,0x14,0x8b,0x6e,0x24,0x6b,0xd3)
	},
	{ chunk_from_chars(                                                          /* 3 */
		0x54,0x65,0x73,0x74),
	  chunk_from_chars(),
	  chunk_from_chars(
		0xa0,0x42,0x22,0x6c,0x1e,0xac,0x49,0x48,0x27,0x97,0xac,0x70,0x65,0x94,0x0a,0x26,
		0x8b,0xc9,0xd7,0xb7,0x1a,0x84,0xec,0x7a,0xb4,0x3f,0x2f,0xec,0x5a,0xf2,0xe3,0x95,
		0x21,0x78,0x0c,0x74,0x10,0xcf,0xd7,0x53,0x8a,0xcb,0x5d,0x17,0xd2,0x13,0xd4,0xe9,
		0xee,0x1a,0xe1,0x4e,0x44,0xdb,0x6e,0x22,0x49,0xf9,0x7e,0xf1,0xee,0x97,0x98,0x3e,
		0x2b,0xab,0x3d,0x2e,0xa4,0x77,0x5c,0x4c,0x8c,0xb7,0x9f,0x25,0x2c,0xc7,0x87,0x5d,
		0xfc,0xa5,0x21,0x48,0x35,0xda,0xcb,0x1b,0xab,0x7d,0xa6,0xbc,0x77,0x81,0xc1,0x5b,
		0x56,0x54,0x6f,0x92,0x73,0xf7,0x90,0x25,0xfb,0x8c,0xfd,0x90,0xe6,0x32,0xda,0x17,
		0x43,0x11,0xaa,0xad,0x6c,0xc5,0xaf,0xfb,0xee,0x02,0x4f,0xf1,0x09,0x93,0x95,0xf5,
		0xcd,0x9f,0x31,0x4e,0xc3,0xcf,0xec,0xf0,0x89,0xc6,0x70,0x32,0x96,0xca,0xa4,0x33,
		0xd6,0xc3,0xc5,0x77,0x96,0x45,0xcc,0xd5,0xc2,0x70,0x2d,0xd9,0x16,0xaa,0x74,0x96,
		0x3c,0x1c,0x11,0xd3,0x0a,0xf3,0x34,0x8f,0xfc,0xd6,0xab,0x28,0xea,0x6b,0x80,0x76,
		0xa8,0xa8,0x2d,0xf8,0x90,0x18,0x6c,0x18,0x21,0xc1,0xaa,0x18,0x76,0x6d,0x80,0x24,
		0x88,0x50,0x83,0xb1,0xac,0x5a,0xde,0x07,0xaa,0xa0,0xbc,0x7c,0x5d,0x38,0xac,0xd6,
		0xfe,0x39,0x2f,0x70,0x37,0x8e,0x2a,0x64,0x40,0x3c,0x39,0xfd,0x4d,0xff,0x70,0xaf,
		0x9e,0xa9,0x6b,0x25,0xd2,0xdc,0xd4,0xea,0x4f,0x93,0x34,0xcc,0x24,0xa8,0x88,0xd1,
		0xcd,0xa5,0x8a,0xf7,0xeb,0x85,0xbf,0x04,0x75,0x2c,0x37,0x94,0x91,0xc9,0xd8,0xbe)
	},
	{ chunk_from_chars(                                                          /* 4 */
		0x31,0x32,0x33,0x34,0x30,0x30),
	  chunk_from_chars(),
	  chunk_from_chars(
		0x54,0xa3,0x14,0x9f,0xd9,0xa5,0x80,0x49,0x2b,0x16,0xab,0x91,0x40,0x6c,0x69,0x16,
		0x4f,0x80,0x91,0xce,0x0f,0x5f,0x57,0xe9,0xc2,0xc7,0x98,0xfb,0x1c,0xd9,0x85,0xce,
		0xa1,0xcb,0x7c,0x3d,0x39,0xe2,0xed,0x35,0xfd,0x54,0x43,0x14,0x0b,0x7f,0x28,0x68,
		0xe3,0x09,0x0c,0xb9,0x0a,0x4e,0x1f,0x4f,0x9e,0xfb,0x8e,0xaf,0xbe,0x93,0xe6,0xba,
		0x65,0xe2,0x67,0x07,0x78,0xf0,0xe8,0x2c,0x40,0x39,0x21,0xd2,0x9d,0x55,0xf3,0xba,
		0x5a,0x96,0x5e,0xd2,0xbb,0x62,0x3f,0x6b,0xd3,0x90,0x78,0xb8,0x59,0x94,0x8a,0x7f,
		0x0d,0xd9,0x29,0x21,0x0c,0x19,0x2e,0xf7,0xfd,0x28,0xd9,0xce,0x9d,0x04,0xac,0x56,
		0xf7,0x85,0xec,0xdf,0x5b,0x7c,0x98,0xcc,0x27,0x35,0xf0,0xaf,0x14,0x74,0x12,0x5f,
		0xef,0xc2,0xc0,0x63,0x89,0x38,0x68,0xea,0xff,0xc6,0xbc,0x61,0xc8,0x05,0x51,0x09,
		0x39,0x26,0xf2,0xb7,0x9c,0x3d,0x20,0x7c,0xe3,0x10,0xc3,0xcd,0x11,0x06,0x3a,0x27,
		0x47,0x60,0xe1,0x7f,0xfe,0xdb,0x5c,0xfa,0x64,0x00,0x80,0xa7,0xc2,0x33,0xba,0x38,
		0xb9,0x5c,0x39,0x76,0xa0,0xa2,0x84,0xec,0x90,0x4e,0xb3,0xac,0x75,0xfd,0x21,0x56,
		0x76,0x3a,0x39,0x68,0xd0,0x1a,0xa2,0x28,0x86,0xb3,0xee,0xf9,0x12,0xe3,0xa0,0xfe,
		0xd0,0x3e,0x26,0x52,0xeb,0x18,0xec,0xf0,0x82,0x4e,0x28,0x79,0xd7,0x0a,0x58,0x6d,
		0x92,0xcc,0xab,0x82,0x3d,0x16,0xfe,0x43,0x0f,0xe5,0xb5,0xa4,0x11,0x5f,0x07,0x9b,
		0x59,0x1c,0x33,0xd3,0x19,0x70,0xfa,0x80,0x81,0x97,0x2d,0x3f,0x1c,0x6f,0xdb,0xd5)
	},
	{ chunk_from_chars(                                                          /* 8 */
		0x31,0x32,0x33,0x34,0x30,0x30),
	  chunk_from_chars(
	  	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00),
	  chunk_from_chars(
	  	0xbf,0xfe,0x02,0x1e,0xb6,0x77,0x34,0x57,0x63,0xb2,0x0d,0xda,0x24,0xb4,0xf4,0x82,
	  	0xd0,0x2b,0x9c,0x42,0xa3,0x94,0x8f,0xe4,0xe9,0x03,0x91,0xf0,0x79,0xf5,0x27,0x47,
	  	0x17,0x95,0xb8,0xae,0x9c,0x8b,0xb1,0xb5,0x43,0x7f,0x56,0xb8,0xa6,0xf6,0x0e,0x56,
	  	0x23,0x43,0x3b,0x02,0x85,0x1a,0x38,0x77,0x61,0xe2,0x74,0x26,0xfe,0xa8,0x06,0xad,
	  	0x68,0xae,0x14,0x0c,0xd1,0x04,0xb0,0xfd,0xa5,0x42,0xb1,0xf0,0x01,0x30,0xbf,0x89,
	  	0xfd,0x56,0x5c,0x13,0xcb,0xdc,0x28,0xa2,0x7c,0x98,0x0a,0x07,0x07,0xd1,0xd5,0xa1,
	  	0xfe,0x27,0x5a,0xa2,0xac,0xec,0x52,0x90,0xc2,0x89,0x2a,0x0a,0x28,0xf6,0xb7,0xe5,
	  	0x46,0xa8,0x97,0x06,0x96,0x5d,0x3b,0xe0,0xea,0x55,0xa6,0x64,0x47,0x1b,0x03,0x01,
	  	0x7a,0x3c,0x44,0x14,0xc1,0xc6,0x94,0x48,0x4b,0x58,0xa3,0x8e,0xda,0x72,0x09,0x1c,
	  	0xbd,0xb2,0x16,0x0f,0x69,0x9f,0x5b,0xd1,0xf5,0x35,0x49,0xf4,0xe0,0x3d,0xe3,0xd7,
	  	0xff,0xfb,0x08,0x0b,0xc5,0x7b,0xb2,0x83,0x5f,0xa1,0xba,0xd1,0xc1,0x9e,0xee,0x3d,
	  	0x33,0x0f,0x3e,0x8e,0xaf,0xf8,0x11,0x0f,0x22,0x6e,0xa8,0x24,0x84,0x2c,0xa9,0x99,
	  	0x66,0x32,0xb8,0x32,0xa4,0x50,0x93,0x33,0xcc,0xff,0x3a,0xda,0x3c,0x6b,0x94,0x31,
	  	0x27,0x34,0x79,0xea,0x1d,0x0e,0x54,0x1f,0xe6,0x49,0x4c,0x9f,0x05,0x3a,0xd0,0x7d,
	  	0x6b,0xf9,0xa4,0xca,0x17,0xd6,0x4f,0xe5,0x05,0x46,0x42,0x8a,0xb7,0x2f,0xac,0x16,
	  	0x02,0x63,0x7c,0x7e,0xb7,0xdd,0xf6,0xad,0x08,0x1f,0x6c,0x9d,0x76,0x7b,0x41,0x77)
	},
	{ chunk_from_chars(                                                          /* 9 */
		0x31,0x32,0x33,0x34,0x30,0x30),
	  chunk_from_chars(
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
		0x10,0x11,0x12,0x13),
	  chunk_from_chars(
		0x10,0x0d,0xe3,0x6f,0x08,0x40,0x3f,0x5f,0x8d,0x3a,0x42,0x34,0x2c,0x94,0x71,0xb3,
		0xa5,0x9f,0x41,0x2d,0x64,0x1d,0x40,0x29,0x71,0xd0,0x20,0x8f,0xcd,0xc7,0xf2,0x8c,
		0xc5,0x51,0x7e,0x2d,0x6a,0xaf,0xc7,0x9d,0x59,0xa4,0x9c,0x65,0x4e,0xd1,0x02,0x85,
		0xe7,0x5f,0x8b,0x60,0x0f,0x5a,0xc4,0x90,0x6d,0x37,0x16,0xda,0xbf,0xd4,0x27,0x85,
		0xb6,0x69,0x81,0x47,0xdb,0xcc,0xa0,0x9e,0xef,0x28,0xee,0x96,0xc7,0x86,0x99,0xb6,
		0x88,0x42,0x8e,0x2d,0xe3,0x43,0x41,0xee,0x0c,0x81,0xfe,0xbc,0x6e,0xd4,0x35,0x12,
		0xcf,0xeb,0x93,0x04,0xe2,0xe4,0x07,0x38,0xa2,0x0d,0x7b,0x48,0x47,0x74,0x2f,0x9f,
		0xad,0x38,0x24,0xf0,0xcf,0x06,0xe4,0x37,0xb3,0xec,0x10,0x61,0xce,0x8a,0xad,0x08,
		0x92,0x68,0x9c,0x33,0x88,0xab,0x60,0x87,0xad,0xae,0x20,0x3e,0x23,0x01,0x98,0x75,
		0x76,0x8b,0xd5,0x3a,0x45,0x68,0x98,0x6f,0x9c,0xc9,0x1a,0x65,0xa5,0xe5,0xe5,0x0c,
		0x0f,0xfb,0xe0,0x7b,0xad,0x0f,0x00,0x2a,0x03,0xdb,0xb3,0xb3,0xc6,0x16,0x47,0xc3,
		0x5e,0x23,0xd8,0x65,0x88,0x79,0x95,0x6f,0xc2,0x3d,0x55,0x91,0xa6,0x14,0x24,0x21,
		0x49,0x0c,0xfb,0xe8,0xe1,0x2f,0x06,0x73,0xe2,0xe4,0x56,0xaf,0xd4,0x4b,0x6f,0x46,
		0x3e,0xf3,0xd2,0xd5,0xb5,0x17,0x55,0x30,0x5a,0xd8,0x31,0x81,0xbc,0xa0,0x8f,0x14,
		0xb2,0xa7,0x02,0xe4,0xef,0x2c,0xc0,0x78,0x6b,0x69,0xd9,0x18,0x87,0x14,0xe9,0xa4,
		0xe9,0xa1,0x85,0x30,0x11,0xc5,0x15,0xa2,0x2b,0xb1,0x78,0x9c,0x47,0xda,0x73,0x31)
	},
	{ chunk_from_chars(                                                         /* 10 */
		0x31,0x32,0x33,0x34,0x30,0x30),
	  chunk_from_chars(
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f),
	  chunk_from_chars(
		0xbb,0x47,0xd7,0x30,0xbd,0xcc,0x0f,0xbe,0xfa,0x8c,0xec,0xeb,0xd0,0x29,0x15,0x0f,
		0xe5,0xd3,0xc9,0x67,0xa4,0x29,0xc4,0x0f,0x16,0xb4,0x8c,0x8d,0x07,0x84,0x86,0x99,
		0xa3,0xab,0x5d,0x68,0xdd,0xb3,0x2f,0x76,0xe0,0xe7,0x4e,0x89,0x7b,0xac,0x94,0x05,
		0xa1,0x62,0x0d,0xbe,0x9f,0xfc,0x2e,0x0d,0x65,0x39,0x24,0xbf,0x5d,0xa3,0x81,0xa9,
		0x03,0xa2,0x78,0xed,0x74,0x89,0xa6,0x2c,0xa5,0xb9,0x4c,0x7f,0x96,0x3a,0x26,0x04,
		0xd5,0xcc,0xa6,0x1a,0x63,0x7d,0x43,0xdd,0x78,0xab,0x44,0x31,0x4b,0x4e,0xb3,0x54,
		0x7e,0x2e,0x4a,0x72,0xaf,0xba,0xbe,0xd8,0xcc,0x8d,0x48,0xe2,0x3f,0xae,0xb5,0xeb,
		0x03,0x6c,0x42,0xd5,0x42,0x57,0x8c,0x68,0xd7,0x21,0x34,0xfe,0xff,0x3c,0xbf,0xc5,
		0x61,0x34,0x8a,0x2d,0xb4,0x80,0x32,0xe0,0x8f,0x98,0x76,0x5e,0x87,0xdc,0x76,0x60,
		0x05,0x04,0xcb,0x72,0x17,0xfe,0xc0,0x4e,0xe9,0x05,0x0b,0xd3,0x76,0x66,0x57,0x71,
		0x97,0x57,0xf3,0x89,0xc0,0xb4,0xf0,0xbd,0xb2,0xfd,0xeb,0x8e,0x57,0x57,0xa3,0xa9,
		0xb2,0x39,0x1e,0xe6,0xeb,0x2e,0x6f,0xcf,0xe9,0x57,0x64,0x70,0x6c,0x07,0x78,0xe4,
		0xf0,0xf3,0xc7,0xfc,0x02,0x4f,0x35,0x6f,0x99,0x9e,0x5f,0x50,0xc6,0x37,0xca,0x9b,
		0xe7,0xff,0x20,0xf6,0x5e,0xdd,0x5c,0x9f,0x6e,0xc8,0x69,0xc2,0x96,0x57,0xb1,0x79,
		0x22,0x25,0xd8,0xa1,0x65,0x74,0xc1,0x83,0xdd,0x5f,0x15,0xf0,0x84,0xe9,0x7a,0x93,
		0x37,0xe5,0x3b,0x56,0xf5,0xd7,0xbc,0x54,0xbf,0x5e,0x03,0x19,0x22,0x95,0x9f,0x48)
	},
	{ chunk_from_chars(                                                         /* 11 */
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,0x78,
		0x78,0x78,0x78,0x78,0x78,0x78),
	  chunk_from_chars(),
	  chunk_from_chars(
		0xc2,0xed,0xf0,0x3b,0x94,0x6c,0xe5,0x71,0x41,0xfa,0xfb,0xc8,0x91,0x89,0xda,0xa7,
		0x34,0xd0,0xf3,0x88,0xc2,0x11,0xa3,0x22,0xad,0x4a,0xd9,0x42,0x3b,0xa5,0xf3,0x16,
		0x99,0xab,0xe6,0x8d,0x5c,0x03,0xc0,0xcc,0xd5,0x6b,0x1a,0x39,0xab,0xd3,0x14,0x5f,
		0x4c,0x0d,0x59,0x95,0x5e,0x6c,0xb5,0x02,0xd6,0x72,0x0c,0x1a,0xf3,0x1e,0xc5,0x11,
		0xfe,0xd5,0x41,0xe1,0xa6,0x67,0x61,0xa2,0x04,0x22,0xf5,0x49,0x0d,0x3d,0xad,0xd8,
		0x15,0x39,0x0a,0x01,0xda,0x61,0x6c,0xe4,0xd7,0x93,0x4c,0x68,0xe7,0xb1,0xc5,0x3d,
		0x94,0x78,0x03,0x26,0x8d,0x81,0xd2,0x64,0x29,0xdf,0x50,0x5c,0x22,0x57,0x8f,0x55,
		0xae,0x07,0x88,0xcc,0x61,0x95,0x67,0xf0,0x22,0x37,0x36,0x82,0x58,0xd2,0xf5,0x91,
		0xb5,0x1d,0x61,0xf0,0x88,0xd9,0x72,0xc4,0x0e,0x4f,0xbd,0x4f,0x50,0xf7,0x2a,0x67,
		0x4d,0x25,0xa1,0xcf,0xf3,0xb4,0x9e,0xdc,0x90,0x19,0xac,0x99,0x45,0xcf,0x2b,0x82,
		0x8d,0x43,0x45,0x39,0x8b,0x3a,0xef,0xb6,0x3c,0x9f,0x04,0x97,0xac,0xe4,0xb7,0xf9,
		0xcf,0x6c,0x08,0x7e,0xfc,0x93,0x80,0x2e,0x1c,0x19,0x32,0x65,0x85,0x78,0x8d,0xf3,
		0xae,0xef,0x36,0x30,0x0f,0x78,0xf7,0xad,0x01,0x6c,0xd4,0xba,0x6c,0x55,0xeb,0x49,
		0xd3,0x6e,0x0d,0x49,0xf9,0x7e,0x1b,0xb8,0xae,0x12,0xe9,0xd9,0xba,0x66,0xd1,0x8a,
		0xa7,0x68,0x12,0x00,0x8e,0xff,0x83,0xe0,0xd7,0xd4,0x52,0x12,0xee,0x97,0x29,0x57,
		0xc3,0x55,0x30,0x78,0x23,0xfb,0x69,0x2b,0xb2,0x04,0x1e,0x6a,0xcc,0x81,0x90,0x2d)
	}
};

static void test_rsa_oaep(chunk_t key, encryption_scheme_t encryption_scheme,
						  rsa_oaep_test_t *test)
{
	private_key_t *privkey;
	public_key_t *pubkey;
	chunk_t msg = chunk_empty, ct = chunk_empty;
	bool success;

	privkey = lib->creds->create(lib->creds, CRED_PRIVATE_KEY, KEY_ANY,
								 BUILD_BLOB_ASN1_DER, key, BUILD_END);
	ck_assert(privkey != NULL);
	ck_assert(privkey->get_type(privkey) == KEY_RSA);

	pubkey = privkey->get_public_key(privkey);
	ck_assert(pubkey != NULL);

	/* decrypt */
	success = privkey->decrypt(privkey, encryption_scheme, &test->label,
							   test->ct, &msg);
	if (test->label.len > 0 && !success)
	{
		warn("RSA OAEP labels might not be supported");
		goto end;
	}
	ck_assert(success);
	ck_assert_chunk_eq(msg, test->msg);
	chunk_free(&msg);

	/* encrypt and decrypt again */
	ck_assert(pubkey->encrypt(pubkey, encryption_scheme, &test->label,
							  test->msg, &ct));
	ck_assert(privkey->decrypt(privkey, encryption_scheme, &test->label,
							   ct, &msg));
	ck_assert_chunk_eq(msg, test->msg);
	chunk_free(&ct);
	chunk_free(&msg);

end:
	pubkey->destroy(pubkey);
	privkey->destroy(privkey);
}

START_TEST(test_rsa_2048_oaep_sha224)
{
	test_rsa_oaep(rsa_2048_key, ENCRYPT_RSA_OAEP_SHA224,
				 &rsa_2048_oaep_sha224_tests[_i]);
}
END_TEST

Suite *rsa_oaep_sha224_suite_create()
{
	Suite *s;
	TCase *tc;

	s = suite_create("rsa_oaep_sha224");

	tc = tcase_create("rsa_2048_oaep_sha224");
	tcase_add_loop_test(tc, test_rsa_2048_oaep_sha224,0, countof(rsa_2048_oaep_sha224_tests));
	suite_add_tcase(s, tc);

	return s;
}
