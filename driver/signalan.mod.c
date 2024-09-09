#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x828ce6bb, "mutex_lock" },
	{ 0x9618ede0, "mutex_unlock" },
	{ 0xff9166ce, "gpiod_set_value" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x800473f, "__cond_resched" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x647af474, "prepare_to_wait_event" },
	{ 0x49970de8, "finish_wait" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xec58aafb, "cdev_init" },
	{ 0xd0d97915, "cdev_add" },
	{ 0x282df941, "class_create" },
	{ 0x5d181af9, "device_create" },
	{ 0x828b0e2c, "of_find_node_by_name" },
	{ 0x7fe4f702, "gpiod_get" },
	{ 0x6089ded2, "kthread_create_on_node" },
	{ 0x3e68d7a9, "wake_up_process" },
	{ 0x2e7f922b, "gpiod_put" },
	{ 0x1c790c3b, "device_destroy" },
	{ 0xa796c9fe, "class_destroy" },
	{ 0x9b477d81, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x5f754e5a, "memset" },
	{ 0x637493f3, "__wake_up" },
	{ 0xdc9d45f9, "kthread_stop" },
	{ 0x77feb5be, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "35AC63349AFDAEA26ACFF01");
