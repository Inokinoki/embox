/**
 * @file
 * @brief
 *
 * @author  Anton Kozlov
 * @date    13.11.2013
 */

#include <errno.h>
#include <util/err.h>
#include <kernel/printk.h>

#include <fs/file_operation.h>
#include <drivers/usb/usb_driver.h>
#include <drivers/usb/usb_dev_desc.h>
#include <drivers/usb/usb.h>

#include <embox/unit.h>

EMBOX_UNIT_INIT(usb_example_init);

static int usb_example_probe(struct usb_dev *dev) {
	printk("\tusb: probing: %04x:%04x\n", dev->dev_desc.id_vendor, dev->dev_desc.id_product);

	return 0;
}

static void usb_example_disconnect(struct usb_dev *dev, void *data) {
	printk("\tusb: disconnecting: %04x:%04x\n", dev->dev_desc.id_vendor, dev->dev_desc.id_product);
}

static struct idesc * example_open(struct inode *node, struct idesc *idesc) {
	/*
	struct usb_dev_desc *ddesc;
	int res;

	res = usb_driver_open_by_node(node, &ddesc);
	if (0 != res) {
		return err_ptr(-res);
	}

	assert(ddesc);

	file_desc_set_file_info(file_desc_from_idesc(idesc), ddesc);
	*/

	return idesc;
}

static int example_close(struct file_desc *desc) {

	/*
	usb_dev_desc_close(desc->file_info);
	*/

	return 0;
}

static size_t example_read(struct file_desc *desc, void *buf, size_t size) {
	return 0;
}

/*
static void example_write_hnd(struct usb_request *req, void *arg) {

}
*/

static size_t example_write(struct file_desc *desc, void *buf, size_t size) {

	return 0;
}
static struct file_operations example_file_ops = {
	.open = example_open,
	.close = example_close,
	.read = example_read,
	.write = example_write,
};

static struct usb_device_id example_id_table[] = {
	{ .vid=0x046d, .pid=0xc077},
	{ },
};

struct usb_driver usb_driver_example = {
	.probe = usb_example_probe,
	.disconnect = usb_example_disconnect,
	.file_ops = &example_file_ops,
	.id_table = example_id_table,
};

static int usb_example_init(void) {
	printk("\tusb: initing driver for: 046d:c077\n");

	return usb_driver_register(&usb_driver_example);
}
