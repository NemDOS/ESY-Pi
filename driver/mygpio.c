
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h> // for kernel 6.8
#include <linux/of_device.h> // for kernel 6.6

static dev_t tl_dev_nr;
static struct cdev driver_object;
static struct class *tl_class;
static struct device *tl_dev;

static struct gpio_desc *gpio_my;

static int config_gpios( void )
{
	struct device_node *nodeptr = tl_dev->of_node;

        nodeptr = of_find_node_by_name(nodeptr, "mygpio");
	tl_dev->of_node = nodeptr;

	gpio_my = gpiod_get( tl_dev, "my", GPIOD_OUT_LOW );
	if (IS_ERR(gpio_my)) {
		pr_err("gpiod_get failed for led-my\n");
		return -EIO;
	}
	pr_info("all gpios reserved and configured...");
	return 0;
}

static int free_gpios( void )
{
	if (gpio_my)
		gpiod_put( gpio_my );
	return 0;
}

static ssize_t device_write( struct file *instanz, const char __user *user,
		size_t count, loff_t *offset )
{
	unsigned long not_copied=0, to_copy=0;
	int value=0;

	to_copy = min( count, sizeof(value) );
	not_copied=copy_from_user(&value,user,to_copy);
	pr_info("device_write( %d )\n", value);
	if (value & 0x01)
		gpiod_set_value( gpio_my, value );
	*offset += to_copy-not_copied;
	return to_copy-not_copied;
}

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.write   = device_write,
};

static int __init mod_init( void )
{
	if (alloc_chrdev_region(&tl_dev_nr,0,1,"mygpio")<0)
		return -EIO;
	cdev_init( &driver_object, &fops );
	driver_object.owner = THIS_MODULE;
	if (cdev_add(&driver_object,tl_dev_nr,1))
		goto free_device_number;
	tl_class = class_create( "mygpio" );
	if (IS_ERR( tl_class )) {
		pr_err( "mygpio: no udev support\n");
		goto free_cdev;
	}
	tl_dev = device_create(tl_class,NULL,tl_dev_nr,
		NULL, "%s", "mygpio" );
	if (IS_ERR( tl_dev )) {
		pr_err( "mygpio: device_create failed\n");
		goto free_class;
	}
	if ( config_gpios() )
		goto free_device;
	return 0;
free_device:
	device_destroy( tl_class, tl_dev_nr );
free_class:
	class_destroy( tl_class );
free_cdev:
	kobject_put( &driver_object.kobj );
free_device_number:
	unregister_chrdev_region( tl_dev_nr, 1 );
	return -EIO;
}

static void __exit mod_exit( void )
{
	free_gpios();
	device_destroy( tl_class, tl_dev_nr );
	class_destroy( tl_class );
	cdev_del( &driver_object );
	unregister_chrdev_region( tl_dev_nr, 1 );
	return;
}

module_init( mod_init );
module_exit( mod_exit );

MODULE_LICENSE("GPL");
