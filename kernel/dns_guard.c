#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#define DEVICE_NAME "dns_guard"
#define CLASS_NAME  "dns_guard"
#define BUFFER_SIZE 256

static dev_t dns_guard_dev;
static struct cdev dns_guard_cdev;
static struct class *dns_guard_class;
static struct device *dns_guard_device;

static char dns_buffer[BUFFER_SIZE];
static size_t dns_buffer_size;
static DEFINE_MUTEX(dns_guard_lock);

static int dns_guard_open(struct inode *inode, struct file *file)
{
    pr_info("DNS Guard: device opened\n");
    return 0;
}

static int dns_guard_release(struct inode *inode, struct file *file)
{
    pr_info("DNS Guard: device closed\n");
    return 0;
}

static ssize_t dns_guard_read(struct file *file,
                              char __user *buffer,
                              size_t count,
                              loff_t *offset)
{
    size_t bytes_to_copy;

    if (*offset >= dns_buffer_size)
        return 0;

    mutex_lock(&dns_guard_lock);

    bytes_to_copy = min(count, dns_buffer_size - (size_t)*offset);

    if (copy_to_user(buffer, dns_buffer + *offset, bytes_to_copy)) {
        mutex_unlock(&dns_guard_lock);
        return -EFAULT;
    }

    *offset += bytes_to_copy;

    mutex_unlock(&dns_guard_lock);

    return bytes_to_copy;
}

static ssize_t dns_guard_write(struct file *file,
                               const char __user *buffer,
                               size_t count,
                               loff_t *offset)
{
    size_t bytes_to_copy;

    bytes_to_copy = min(count, (size_t)(BUFFER_SIZE - 1));

    mutex_lock(&dns_guard_lock);

    if (copy_from_user(dns_buffer, buffer, bytes_to_copy)) {
        mutex_unlock(&dns_guard_lock);
        return -EFAULT;
    }

    dns_buffer[bytes_to_copy] = '\0';
    dns_buffer_size = bytes_to_copy;

    mutex_unlock(&dns_guard_lock);

    pr_info("DNS Guard: received %zu bytes\n", bytes_to_copy);

    return bytes_to_copy;
}

static const struct file_operations dns_guard_fops = {
    .owner = THIS_MODULE,
    .open = dns_guard_open,
    .read = dns_guard_read,
    .write = dns_guard_write,
    .release = dns_guard_release,
};

static int __init dns_guard_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dns_guard_dev, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("DNS Guard: failed to allocate device number\n");
        return ret;
    }

    cdev_init(&dns_guard_cdev, &dns_guard_fops);
    dns_guard_cdev.owner = THIS_MODULE;

    ret = cdev_add(&dns_guard_cdev, dns_guard_dev, 1);
    if (ret < 0) {
        pr_err("DNS Guard: failed to add cdev\n");
        unregister_chrdev_region(dns_guard_dev, 1);
        return ret;
    }

    dns_guard_class = class_create(CLASS_NAME);
    if (IS_ERR(dns_guard_class)) {
        pr_err("DNS Guard: failed to create device class\n");
        cdev_del(&dns_guard_cdev);
        unregister_chrdev_region(dns_guard_dev, 1);
        return PTR_ERR(dns_guard_class);
    }

    dns_guard_device = device_create(
        dns_guard_class,
        NULL,
        dns_guard_dev,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(dns_guard_device)) {
        pr_err("DNS Guard: failed to create device\n");
        class_destroy(dns_guard_class);
        cdev_del(&dns_guard_cdev);
        unregister_chrdev_region(dns_guard_dev, 1);
        return PTR_ERR(dns_guard_device);
    }

    pr_info("DNS Guard: character device loaded\n");
    pr_info("DNS Guard: device /dev/%s created\n", DEVICE_NAME);

    return 0;
}

static void __exit dns_guard_exit(void)
{
    device_destroy(dns_guard_class, dns_guard_dev);
    class_destroy(dns_guard_class);
    cdev_del(&dns_guard_cdev);
    unregister_chrdev_region(dns_guard_dev, 1);

    pr_info("DNS Guard: character device unloaded\n");
}

module_init(dns_guard_init);
module_exit(dns_guard_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anshuman Bisoi");
MODULE_DESCRIPTION("Edge DNS Security Gateway Linux character device");
MODULE_VERSION("1.0");
