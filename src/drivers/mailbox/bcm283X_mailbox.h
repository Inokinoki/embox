/**
 * @file
 * @brief Mailbox Communication mechanism header for BCM283X chip
 *
 * @date 01.04.21
 * @author Weixuan XIAO
 */

#ifndef BCM283X_MAILBOX_H_
#define BCM283X_MAILBOX_H_

#include <stdint.h>

#define BCM283X_FB_MAX_RES    4096
#define BCM283X_FB_MAX_BPP    32

/**
 * The Mailbox gives the ability to send/receive messages to/from the processor
 * and the graphic card.
 */
#define BCM283X_MAILBOX_BASE            0x3F00B880
#define BCM283X_MAILBOX_DATA_MASK       0xFFFFFFF0
#define BCM283X_MAILBOX_CHANNEL_MASK    0x0000000F

/* BCM283X Mailbox Status Flags */
#define BCM283X_MAILBOX_EMPTY           0x40000000
#define BCM283X_MAILBOX_FULL            0x80000000

/* BCM283X Mailbox Channels */
#define BCM283X_POWER_MGMT_CHANNEL      0
#define BCM283X_FRAMEBUFFER_CHANNEL     1

/**
 * Layout of the Mailbox Registers.
 */
struct bcm283X_mailbox_regs {
	uint32_t Read;              /* Receiving mail.              R- */
	uint32_t unused1;
	uint32_t unused2;
	uint32_t unused3;
	uint32_t Poll;              /* Receive without retrieving.  R- */
	uint32_t Sender;            /* Sender information.          R- */
	uint32_t Status;            /* Information.                 R- */
	uint32_t Configuration;     /* Settings.                    RW */
	uint32_t Write;             /* Sending mail.                -W */
};

/**
 * Format of the messages to the graphics processor.
 */
struct bcm283X_fb_info {
	uint32_t width_p;       /* width of the physical display */
	uint32_t height_p;      /* height of the physical display */
	uint32_t width_v;       /* width of the virtual display */
	uint32_t height_v;      /* height of the virtual display */
	uint32_t gpu_pitch;     /* 0 upon request; in response: num bytes between rows */
	uint32_t bit_depth;     /* number of bits to allocate in each pixel */
	uint32_t x;             /* x offset */
	uint32_t y;             /* y offset */
	uint32_t gpu_pointer;   /* actual pointer to the frame buffer */
	uint32_t gpu_size;      /* size of the frame buffer in bytes */
} __attribute__((aligned(16)));

/**
 * Routine to send a message to a particular mailbox.
 * @param datas - message to be sent
 * @param channel - what mailbox to write the message to
 *
 * @return 0 - in case of success
 * @return < 0 - in case of error
 */
int bcm283X_mailbox_write(uint32_t data, uint32_t channel);

/**
 * Routine to read a message from a particular mailbox.
 * @param channel - what mailbox to receive the message from
 *
 * @return data read - in case of success
 * @return < 0 - in case of error
 */
uint32_t bcm283X_mailbox_read(uint32_t channel);

#endif /* BCM283X_MAILBOX_H_ */
