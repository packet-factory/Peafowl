/*
 * ipv6_reassembly.h
 *
 * Created on: 05/10/2012
 *
 * =========================================================================
 *  Copyright (C) 2012-2013, Daniele De Sensi (d.desensi.software@gmail.com)
 *
 *  This file is part of Peafowl.
 *
 *  Peafowl is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 3 of the License, or (at your option) any later version.

 *  Peafowl is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with Peafowl.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
 * =========================================================================
 */

#ifndef IPV6_REASSEMBLY_H_
#define IPV6_REASSEMBLY_H_


#ifdef __cplusplus
extern "C" {
#endif

typedef struct dpi_ipv6_fragmentation_state
			   dpi_ipv6_fragmentation_state_t;

/**
 * Enables the IPv6 defragmentation.
 * @param table_size  The size of the table used to store the fragments.
 * @return            A pointer to the IPv6 defragmentation handle.
 */
dpi_ipv6_fragmentation_state_t*
		dpi_reordering_enable_ipv6_fragmentation(u_int16_t table_size);

/**
 * Sets the maximum amount of memory that can be used to store
 * fragments generated by the same source.
 * @param frag_state A pointer to the IPv6 degragmentation handle.
 * @param per_host_memory_limit  The maximum amount of memory that can be
 *                               used to store fragments generated by the
 *                               same source.
 */
void dpi_reordering_ipv6_fragmentation_set_per_host_memory_limit(
		dpi_ipv6_fragmentation_state_t *frag_state,
		u_int32_t per_host_memory_limit);

/**
 * Sets the maximum (global) amount of memory that can be used for
 * defragmentation purposes.
 * @param frag_state A pointer to the IPv6 defragmentation handle.
 * @param total_memory_limit   The global memory limit.
 */
void dpi_reordering_ipv6_fragmentation_set_total_memory_limit(
		dpi_ipv6_fragmentation_state_t *frag_state,
		u_int32_t total_memory_limit);

/**
 * Sets the maximum amount of time (seconds) which can elapse before the
 * complete defragmentation of the datagram.
 * @param frag_state        A pointer to the IPv6 defragmentation handle.
 * @param timeout_seconds   The timeout (seconds).
 */
void dpi_reordering_ipv6_fragmentation_set_reassembly_timeout(
		dpi_ipv6_fragmentation_state_t *frag_state,
		u_int8_t timeout_seconds);

/**
 * Disables the IPv6 fragmentation and deallocates the handle.
 * @param frag_state  A pointer to the IPv6 defragmentation handle.
 */
void dpi_reordering_disable_ipv6_fragmentation(
		dpi_ipv6_fragmentation_state_t* frag_state);

/**
 * Reassemble the IP datagram if it is fragmented.
 * @param state The state for fragmentation support.
 * @param unfragmentable_start A pointer to the beginning of
 * unfragmentable part. We suppose that unfragmentable and
 * fragmentable data are in the same buffer.
 * @param unfragmentable_size The size of the unfragmentable part.
 * In principle this could obtained by difference (fragmentable_start-
 * unfragmentable_start). By the way this additional level of flexibility
 * allows to store only a part of the unfragmentable header (e.g. only the
 * IPv6 header without additional headers).
 * @param fragmentable_start A pointer to the beginning of the
 * fragmentable part.
 * @param fragmentable_size Size of the fragmentable part.
 * @param offset The data offset specified in the IPv6 fragmentation
 * header.
 * @param more_fragments 1 if the MF flag is set, 0 otherwise.
 * @param identification The identification of the datagram.
 * @param next_header The header which follows the fragmentation
 * header.
 * @param current_time The current time, in seconds.
 * @param tid The thread identifier.
 * @return Returns NULL if the datagram is a fragment but doesn't fill
 * an hole. In this case, the content of the datagram has been copied,
 * so if the user wants, he can release the resources used to store the
 * received packet.
 * Returns a pointer to the recomposed datagram if the datagram is the
 * last fragment of a bigger datagram. This pointer will be different
 * from data. The user should free() this pointer when it is no more
 * needed.
 */
unsigned char* dpi_reordering_manage_ipv6_fragment(
		dpi_ipv6_fragmentation_state_t *state,
		const unsigned char* unfragmentable_start,
		u_int16_t unfragmentable_size,
		const unsigned char* fragmentable_start,
		u_int16_t fragmentable_size,
		u_int16_t offset, u_int8_t more_fragments,
		u_int32_t identification, u_int8_t next_header,
		u_int32_t current_time, int tid);

#ifdef __cplusplus
}
#endif
#endif /* IPV6_REASSEMBLY_H_ */
