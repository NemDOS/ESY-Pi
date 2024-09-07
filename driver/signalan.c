#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h> // for kernel 6.8
#include <linux/of_device.h> // for kernel 6.6
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/mutex.h>

static dev_t tl_dev_nr;
static struct cdev driver_object;
static struct class *tl_class;
static struct device *tl_dev;

static struct gpio_desc *gpio_my;
static struct task_struct *blinker_thread;  // Kernel-Thread für das Blinken
static int freqPanagou = 1;  // Initiale Blinkfrequenz (in Hz)
static DECLARE_WAIT_QUEUE_HEAD(wait_queue);  // Warteschlange
static DEFINE_MUTEX(freq_lock);  // Mutex für den Zugriff auf freqPanagou

// Thread-Funktion für das Blinken
static int blinker_function(void *data) {
    while (!kthread_should_stop()) {
        int ret;
        int period_ms;

        mutex_lock(&freq_lock);  // Zugriff auf freqPanagou schützen
        if(freqPanagou == 0) {
		printk("frequency cannot be 0!\n");
		continue;
	}
	period_ms = 1000 / freqPanagou;  // Periode in ms berechnen
        mutex_unlock(&freq_lock);

        gpiod_set_value(gpio_my, 1);

        ret = wait_event_interruptible_timeout(wait_queue, 
                                                kthread_should_stop(),
                                                msecs_to_jiffies(period_ms / 2));

        if (ret) 
            break;

        gpiod_set_value(gpio_my, 0);

        ret = wait_event_interruptible_timeout(wait_queue, 
                                                kthread_should_stop(),
                                                msecs_to_jiffies(period_ms / 2));

        if (ret) 
            break;
    }
    return 0;
}

static int config_gpios(void) 
{
    struct device_node *nodeptr = tl_dev->of_node;

    nodeptr = of_find_node_by_name(nodeptr, "led_onoff_an");
    tl_dev->of_node = nodeptr;

    gpio_my = gpiod_get(tl_dev, "my", GPIOD_OUT_LOW);
    if (IS_ERR(gpio_my)) {
        pr_err("gpiod_get failed for led-my\n");
        return -EIO;
    }
    pr_info("all gpios reserved and configured...");
    return 0;
}

static int free_gpios(void) {
    if (gpio_my)
        gpiod_put(gpio_my);
    return 0;
}

// Schreiben auf die Gerätedatei zur Steuerung der Blinkfrequenz
static ssize_t device_write(struct file *instanz, const char __user *user,
                            size_t count, loff_t *offset) {
    unsigned long not_copied = 0, to_copy = 0;
    int value = 0;

    to_copy = min(count, sizeof(value));
    not_copied = copy_from_user(&value, user, to_copy);

    pr_info("device_write( %d )\n", value);

    if (value > 0) {
        mutex_lock(&freq_lock);  // Frequenzänderung schützen
        freqPanagou = value;  // Neue Frequenz setzen
        mutex_unlock(&freq_lock);
        wake_up_interruptible(&wait_queue);  // Wecke den Thread auf
    }

    *offset += to_copy - not_copied;
    return to_copy - not_copied;
}

static ssize_t device_read(struct file *instanz, char __user *user,
                           size_t count, loff_t *offset) {
    int value;
    char buffer[16];
    ssize_t len;

    if (*offset > 0)
        return 0;

    mutex_lock(&freq_lock);
    value = freqPanagou;
    mutex_unlock(&freq_lock);

    len = snprintf(buffer, sizeof(buffer), "%d\n", value);

    if (count < len)
        return -EINVAL;

    if (copy_to_user(user, buffer, len))
        return -EFAULT;

    *offset = len;
    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = device_write,
    .read = device_read,
};

static int __init mod_init(void) {
    int ret;

    // Geräte-Nummer reservieren
    ret = alloc_chrdev_region(&tl_dev_nr, 0, 1, "led_onoff_an");
    if (ret < 0) {
        pr_err("led_onoff_an: Failed to allocate chrdev region\n");
        return -EIO;
    }

    // Zeichen-Device initialisieren
    cdev_init(&driver_object, &fops);
    driver_object.owner = THIS_MODULE;

    // Gerät zu Kernel hinzufügen
    ret = cdev_add(&driver_object, tl_dev_nr, 1);
    if (ret) {
        pr_err("led_onoff_an: Failed to add cdev\n");
        goto unregister_device_number;
    }

    // Klasse erstellen
    tl_class = class_create("led_onoff_an");
    if (IS_ERR(tl_class)) {
        pr_err("led_onoff_an: Failed to create class\n");
        ret = PTR_ERR(tl_class);
        goto del_cdev;
    }

    // Gerät erstellen
    tl_dev = device_create(tl_class, NULL, tl_dev_nr, NULL, "%s", "led_onoff_an");
    if (IS_ERR(tl_dev)) {
        pr_err("led_onoff_an: Failed to create device\n");
        ret = PTR_ERR(tl_dev);
        goto destroy_class;
    }

    // GPIO konfigurieren
    ret = config_gpios();
    if (ret) {
        pr_err("led_onoff_an: Failed to configure GPIOs\n");
        goto destroy_device;
    }

    // Kernel-Thread für das LED-Blinken starten
    blinker_thread = kthread_run(blinker_function, NULL, "blinker_thread");
    if (IS_ERR(blinker_thread)) {
        pr_err("led_onoff_an: Failed to create blinker thread\n");
        ret = PTR_ERR(blinker_thread);
        goto free_gpios;
    }

    pr_info("led_onoff_an: Module loaded successfully\n");
    return 0;

free_gpios:
    free_gpios();
destroy_device:
    device_destroy(tl_class, tl_dev_nr);
destroy_class:
    class_destroy(tl_class);
del_cdev:
    cdev_del(&driver_object);
unregister_device_number:
    unregister_chrdev_region(tl_dev_nr, 1);
    return ret;
}

static void __exit mod_exit(void) {
    if (blinker_thread) {
        kthread_stop(blinker_thread);  // Stoppe den Kernel-Thread
    }

    free_gpios();
    device_destroy(tl_class, tl_dev_nr);
    class_destroy(tl_class);
    cdev_del(&driver_object);
    unregister_chrdev_region(tl_dev_nr, 1);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
