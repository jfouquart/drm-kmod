/*
 * Autogenerated file by GPU Top : https://github.com/rib/gputop
 * DO NOT EDIT manually!
 *
 *
 * Copyright (c) 2015 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 */

#include <linux/sysfs.h>

#include "i915_drv.h"
#include "i915_oa_glk.h"

static const struct i915_oa_reg b_counter_config_test_oa[] = {
	{ _MMIO(0x2740), 0x00000000 },
	{ _MMIO(0x2744), 0x00800000 },
	{ _MMIO(0x2714), 0xf0800000 },
	{ _MMIO(0x2710), 0x00000000 },
	{ _MMIO(0x2724), 0xf0800000 },
	{ _MMIO(0x2720), 0x00000000 },
	{ _MMIO(0x2770), 0x00000004 },
	{ _MMIO(0x2774), 0x00000000 },
	{ _MMIO(0x2778), 0x00000003 },
	{ _MMIO(0x277c), 0x00000000 },
	{ _MMIO(0x2780), 0x00000007 },
	{ _MMIO(0x2784), 0x00000000 },
	{ _MMIO(0x2788), 0x00100002 },
	{ _MMIO(0x278c), 0x0000fff7 },
	{ _MMIO(0x2790), 0x00100002 },
	{ _MMIO(0x2794), 0x0000ffcf },
	{ _MMIO(0x2798), 0x00100082 },
	{ _MMIO(0x279c), 0x0000ffef },
	{ _MMIO(0x27a0), 0x001000c2 },
	{ _MMIO(0x27a4), 0x0000ffe7 },
	{ _MMIO(0x27a8), 0x00100001 },
	{ _MMIO(0x27ac), 0x0000ffe7 },
};

static const struct i915_oa_reg flex_eu_config_test_oa[] = {
};

static const struct i915_oa_reg mux_config_test_oa[] = {
	{ _MMIO(0x9888), 0x19800000 },
	{ _MMIO(0x9888), 0x07800063 },
	{ _MMIO(0x9888), 0x11800000 },
	{ _MMIO(0x9888), 0x23810008 },
	{ _MMIO(0x9888), 0x1d950400 },
	{ _MMIO(0x9888), 0x0f922000 },
	{ _MMIO(0x9888), 0x1f908000 },
	{ _MMIO(0x9888), 0x37900000 },
	{ _MMIO(0x9888), 0x55900000 },
	{ _MMIO(0x9888), 0x47900000 },
	{ _MMIO(0x9888), 0x33900000 },
};

static ssize_t
show_test_oa_id(struct device *kdev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "1\n");
}

void
i915_perf_load_test_config_glk(struct drm_i915_private *dev_priv)
{
	strncpy(dev_priv->perf.oa.test_config.uuid,
		"dd3fd789-e783-4204-8cd0-b671bbccb0cf",
		UUID_STRING_LEN);
	dev_priv->perf.oa.test_config.id = 1;

	dev_priv->perf.oa.test_config.mux_regs = mux_config_test_oa;
	dev_priv->perf.oa.test_config.mux_regs_len = ARRAY_SIZE(mux_config_test_oa);

	dev_priv->perf.oa.test_config.b_counter_regs = b_counter_config_test_oa;
	dev_priv->perf.oa.test_config.b_counter_regs_len = ARRAY_SIZE(b_counter_config_test_oa);

	dev_priv->perf.oa.test_config.flex_regs = flex_eu_config_test_oa;
	dev_priv->perf.oa.test_config.flex_regs_len = ARRAY_SIZE(flex_eu_config_test_oa);

	dev_priv->perf.oa.test_config.sysfs_metric.name = "dd3fd789-e783-4204-8cd0-b671bbccb0cf";
	dev_priv->perf.oa.test_config.sysfs_metric.attrs = dev_priv->perf.oa.test_config.attrs;

	dev_priv->perf.oa.test_config.attrs[0] = &dev_priv->perf.oa.test_config.sysfs_metric_id.attr;

	dev_priv->perf.oa.test_config.sysfs_metric_id.attr.name = "id";
	dev_priv->perf.oa.test_config.sysfs_metric_id.attr.mode = 0444;
	dev_priv->perf.oa.test_config.sysfs_metric_id.show = show_test_oa_id;
}
