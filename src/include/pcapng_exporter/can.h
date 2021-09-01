/* SPDX-License-Identifier: ((GPL-2.0-only WITH Linux-syscall-note) OR BSD-3-Clause) */
/*
 * linux/can.h
 *
 * Definitions for CAN network layer (socket addr / CAN frame / CAN filter)
 *
 * Authors: Oliver Hartkopp <oliver.hartkopp@volkswagen.de>
 *          Urs Thuermann   <urs.thuermann@volkswagen.de>
 * Copyright (c) 2002-2007 Volkswagen Group Electronic Research
 * All rights reserved.
 * 
 * See https://github.com/linux-can/can-utils/blob/master/include/linux/can.h
 */

#ifndef _CAN_H
#define _CAN_H

#include <stdint.h>

/* controller area network (CAN) kernel definitions */

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000U /* error message frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

/*
 * Controller Area Network Identifier structure
 *
 * bit 0-28	: CAN identifier (11/29 bit)
 * bit 29	: error message frame flag (0 = data frame, 1 = error message)
 * bit 30	: remote transmission request flag (1 = rtr frame)
 * bit 31	: frame format flag (0 = standard 11 bit, 1 = extended 29 bit)
 */

#define CAN_SFF_ID_BITS		11
#define CAN_EFF_ID_BITS		29

/*
 * defined bits for canfd_frame.flags
 *
 * The use of struct canfd_frame implies the FD Frame (FDF) bit to
 * be set in the CAN frame bitstream on the wire. The FDF bit switch turns
 * the CAN controllers bitstream processor into the CAN FD mode which creates
 * two new options within the CAN FD frame specification:
 *
 * Bit Rate Switch - to indicate a second bitrate is/was used for the payload
 * Error State Indicator - represents the error state of the transmitting node
 *
 * As the CANFD_ESI bit is internally generated by the transmitting CAN
 * controller only the CANFD_BRS bit is relevant for real CAN controllers when
 * building a CAN FD frame for transmission. Setting the CANFD_ESI bit can make
 * sense for virtual CAN interfaces to test applications with echoed frames.
 *
 * The struct can_frame and struct canfd_frame intentionally share the same
 * layout to be able to write CAN frame content into a CAN FD frame structure.
 * When this is done the former differentiation via CAN_MTU / CANFD_MTU gets
 * lost. CANFD_FDF allows programmers to mark CAN FD frames in the case of
 * using struct canfd_frame for mixed CAN / CAN FD content (dual use).
 * N.B. the Kernel APIs do NOT provide mixed CAN / CAN FD content inside of
 * struct canfd_frame therefore the CANFD_FDF flag is disregarded by Linux.
 */
#define CANFD_BRS 0x01 /* bit rate switch (second bitrate for payload data) */
#define CANFD_ESI 0x02 /* error state indicator of the transmitting node */
#define CANFD_FDF 0x04 /* mark CAN FD for dual use of struct canfd_frame */

/**
 * struct canfd_frame - CAN flexible data rate frame structure
 * @can_id: CAN ID of the frame and CAN_*_FLAG flags, see canid_t definition
 * @len:    frame payload length in byte (0 .. CANFD_MAX_DLEN)
 * @flags:  additional flags for CAN FD
 * @__res0: reserved / padding
 * @__res1: reserved / padding
 * @data:   CAN FD frame payload (up to CANFD_MAX_DLEN byte)
 */
struct canfd_frame {
	uint32_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
	uint8_t    len;     /* frame payload length in byte */
	uint8_t    flags;   /* additional flags for CAN FD */
	uint8_t    __res0;  /* reserved / padding */
	uint8_t    __res1;  /* reserved / padding */
	uint8_t    data[64];
};

#define CAN_MTU		16
#define CANFD_MTU	(sizeof(struct canfd_frame))

#endif
