// SPDX-License-Identifier: GPL-2.0-or-later
/*
 */

#include <linux/module.h>
#include <linux/pci.h>

#include <drm/drm_drv.h>
#include <drm/drm_atomic_helper.h>

#include "bochs.h"

#ifdef __FreeBSD__
SYSCTL_NODE(_hw, OID_AUTO, bochs,
			CTLFLAG_RW | CTLFLAG_MPSAFE, 0,
			"bochs dispi vga interface (qemu stdvga) parameters");
#endif

static int bochs_modeset = -1;
module_param_named(modeset, bochs_modeset, int, 0444);
MODULE_PARM_DESC(modeset, "enable/disable kernel modesetting");

/* ---------------------------------------------------------------------- */
/* drm interface                                                          */

static void bochs_unload(struct drm_device *dev)
{
	struct bochs_device *bochs = dev->dev_private;

	bochs_kms_fini(bochs);
	bochs_mm_fini(bochs);
	bochs_hw_fini(dev);
	kfree(bochs);
	dev->dev_private = NULL;
}

static int bochs_load(struct drm_device *dev)
{
	struct bochs_device *bochs;
	int ret;

	bochs = kzalloc(sizeof(*bochs), GFP_KERNEL);
	if (bochs == NULL)
		return -ENOMEM;
	dev->dev_private = bochs;
	bochs->dev = dev;

	ret = bochs_hw_init(dev);
	if (ret)
		goto err;

	ret = bochs_mm_init(bochs);
	if (ret)
		goto err;

	ret = bochs_kms_init(bochs);
	if (ret)
		goto err;

	return 0;

err:
	bochs_unload(dev);
	return ret;
}

static const struct file_operations bochs_fops = {
	.owner		= THIS_MODULE,
	DRM_VRAM_MM_FILE_OPERATIONS
};

static struct drm_driver bochs_driver = {
	.driver_features	= DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	.fops			= &bochs_fops,
	.name			= "bochs-drm",
	.desc			= "bochs dispi vga interface (qemu stdvga)",
	.date			= "20130925",
	.major			= 1,
	.minor			= 0,
	DRM_GEM_VRAM_DRIVER,
};

/* ---------------------------------------------------------------------- */
/* pm interface                                                           */

#ifdef CONFIG_PM_SLEEP
static int bochs_pm_suspend(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	return drm_mode_config_helper_suspend(drm_dev);
}

static int bochs_pm_resume(struct device *dev)
{
	struct drm_device *drm_dev = dev_get_drvdata(dev);

	return drm_mode_config_helper_resume(drm_dev);
}
#endif

static const struct dev_pm_ops bochs_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(bochs_pm_suspend,
				bochs_pm_resume)
};

/* ---------------------------------------------------------------------- */
/* pci interface                                                          */

static int bochs_pci_probe(struct pci_dev *pdev,
			   const struct pci_device_id *ent)
{
	struct drm_device *dev;
	unsigned long fbsize;
	int ret;

	fbsize = pci_resource_len(pdev, 0);
	if (fbsize < 4 * 1024 * 1024) {
		DRM_ERROR("less than 4 MB video memory, ignoring device\n");
		return -ENOMEM;
	}

	ret = drm_fb_helper_remove_conflicting_pci_framebuffers(pdev, 0, "bochsdrmfb");
	if (ret)
		return ret;

	dev = drm_dev_alloc(&bochs_driver, &pdev->dev);
	if (IS_ERR(dev))
		return PTR_ERR(dev);

	ret = pci_enable_device(pdev);
	if (ret)
		goto err_free_dev;

	dev->pdev = pdev;
	pci_set_drvdata(pdev, dev);

	ret = bochs_load(dev);
	if (ret)
		goto err_free_dev;

	ret = drm_dev_register(dev, 0);
	if (ret)
		goto err_unload;

	drm_fbdev_generic_setup(dev, 32);
	return ret;

err_unload:
	bochs_unload(dev);
err_free_dev:
	drm_dev_put(dev);
	return ret;
}

static void bochs_pci_remove(struct pci_dev *pdev)
{
	struct drm_device *dev = pci_get_drvdata(pdev);

	drm_atomic_helper_shutdown(dev);
	drm_dev_unregister(dev);
	bochs_unload(dev);
	drm_dev_put(dev);
}

static const struct pci_device_id bochs_pci_tbl[] = {
	{
		.vendor      = 0x1234,
		.device      = 0x1111,
		.subvendor   = PCI_SUBVENDOR_ID_REDHAT_QUMRANET,
		.subdevice   = PCI_SUBDEVICE_ID_QEMU,
		.driver_data = BOCHS_QEMU_STDVGA,
	},
	{
		.vendor      = 0x1234,
		.device      = 0x1111,
		.subvendor   = PCI_ANY_ID,
		.subdevice   = PCI_ANY_ID,
		.driver_data = BOCHS_UNKNOWN,
	},
	{ /* end of list */ }
};

static struct pci_driver bochs_pci_driver = {
	.name =		"bochs-drm",
	.id_table =	bochs_pci_tbl,
	.probe =	bochs_pci_probe,
	.remove =	bochs_pci_remove,
	.driver.pm =    &bochs_pm_ops,
};

/* ---------------------------------------------------------------------- */
/* module init/exit                                                       */

static int __init bochs_init(void)
{
	if (vgacon_text_force() && bochs_modeset == -1)
		return -EINVAL;

	if (bochs_modeset == 0)
		return -EINVAL;

#ifdef __linux__
	return pci_register_driver(&bochs_pci_driver);
#elif defined(__FreeBSD__)
	int ret;
	bochs_pci_driver.bsdclass = drm_devclass;
	ret = linux_pci_register_drm_driver(&bochs_pci_driver);
	if (ret)
		DRM_ERROR("Failed initializing DRM.\n");
	return ret;
#endif
}

static void __exit bochs_exit(void)
{
#ifdef __linux__
	pci_unregister_driver(&bochs_pci_driver);
#elif defined(__FreeBSD__)
	linux_pci_unregister_drm_driver(&bochs_pci_driver);
#endif
}

#ifdef __linux__
module_init(bochs_init);
module_exit(bochs_exit);

MODULE_DEVICE_TABLE(pci, bochs_pci_tbl);
MODULE_AUTHOR("Gerd Hoffmann <kraxel@redhat.com>");
MODULE_LICENSE("GPL");

#else
LKPI_DRIVER_MODULE(bochs, bochs_init, bochs_exit);
LKPI_PNP_INFO(pci, bochs, bochs_pci_tbl);
MODULE_DEPEND(bochs, drmn, 2, 2, 2);
MODULE_DEPEND(bochs, ttm, 1, 1, 1);
MODULE_DEPEND(bochs, agp, 1, 1, 1);
MODULE_DEPEND(bochs, linuxkpi, 1, 1, 1);
MODULE_DEPEND(bochs, linuxkpi_gplv2, 1, 1, 1);
MODULE_DEPEND(bochs, debugfs, 1, 1, 1);
#endif
