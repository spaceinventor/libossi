#include <stdlib.h>
#include <vmem/vmem.h>
#include "miniz.h"
#include <stdio.h>

// OUT_BUF_SIZE must be a power of 2 >= TINFL_LZ_DICT_SIZE (because the low-level decompressor not only writes, but reads from the output buffer as it decompresses)
#define OUT_BUF_SIZE TINFL_LZ_DICT_SIZE
//  must be >= 1
#define IN_BUF_SIZE (TINFL_LZ_DICT_SIZE / 2)

static uint8_t out_buf[OUT_BUF_SIZE];
static uint8_t in_buf[IN_BUF_SIZE];

tinfl_decompressor inflator;
#define my_max(a, b) (((a) > (b)) ? (a) : (b))
#define my_min(a, b) (((a) < (b)) ? (a) : (b))

/* Not reentrant! tinfl_decompressor size too big for stack, vmem server currenly protects this */
int __attribute__((weak)) ossi_decompressor(uint64_t dst_addr, uint32_t * dst_len, uint64_t src_addr, uint32_t src_len) {

	tinfl_init(&inflator);

	uint64_t src_remaining = src_len;
	uint64_t total_written = 0;

	size_t avail_in = 0;
	const uint8_t * next_in = in_buf;
	void * next_out = out_buf;
	size_t avail_out = OUT_BUF_SIZE;

	for (;;) {
		size_t in_bytes, out_bytes;
		tinfl_status status;

		if (!avail_in && src_remaining > 0) {
			// Input buffer is empty read more bytes from input file
			uint32_t n = my_min(IN_BUF_SIZE, src_remaining);

			vmem_read(in_buf, src_addr + (src_len - src_remaining), n);

			next_in = in_buf;
			avail_in = n;
			src_remaining -= n;
		}

		in_bytes = avail_in;
		out_bytes = avail_out;
		mz_uint32 decompflag = (src_remaining ? TINFL_FLAG_HAS_MORE_INPUT : 0) | TINFL_FLAG_PARSE_ZLIB_HEADER;
		status = tinfl_decompress(&inflator, (const mz_uint8 *)next_in, &in_bytes, out_buf, (mz_uint8 *)next_out, &out_bytes, decompflag);

		avail_in -= in_bytes;
		next_in = (const mz_uint8 *)next_in + in_bytes;

		avail_out -= out_bytes;
		next_out = (mz_uint8 *)next_out + out_bytes;

		if ((status <= TINFL_STATUS_DONE) || (!avail_out)) {
			// Output buffer is full, or decompression is done, so write buffer to output file
			uint32_t n = OUT_BUF_SIZE - (uint32_t)avail_out;
			if (n > 0) {
				vmem_write(dst_addr + total_written, out_buf, n);
				total_written += n;
			}
			next_out = out_buf;
			avail_out = OUT_BUF_SIZE;
		}

		// If status is <= TINFL_STATUS_DONE then either decompression is done or something went wrong
		if (status <= TINFL_STATUS_DONE) {
			if (status == TINFL_STATUS_DONE) {
				// Decompression completed successfully.
				break;
			} else {
				// Decompression failed.
				printf("tinfl_decompress() failed with status %i!\n", status);
				return EXIT_FAILURE;
			}
		}
	}

	*dst_len = total_written;
	return EXIT_SUCCESS;
}
