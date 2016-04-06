/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/memory.h>
#include <linux/msm_tsens.h>
#include <linux/msm_thermal.h>
#include <linux/clk/msm-clk-provider.h>
#include <linux/regulator/rpm-smd-regulator.h>
#include <asm/mach/map.h>
#include <asm/mach/arch.h>
#include <mach/board.h>
#include <mach/gpiomux.h>
#include <mach/msm_iomap.h>
#include <mach/msm_memtypes.h>
#include <mach/msm_smd.h>
#include <mach/rpm-smd.h>
#include <mach/restart.h>
#include <mach/socinfo.h>
#include <soc/msm/smem.h>
#include "board-dt.h"
#include "clock.h"
#include "spm.h"

static struct of_dev_auxdata msmkrypton_auxdata_lookup[] __initdata = {
	OF_DEV_AUXDATA("qcom,sdhci-msm", 0xF98A4900, "msm_sdcc.2", NULL),
	OF_DEV_AUXDATA("qti,msm_pcie", 0xFC520000, "msm_pcie", NULL),
	{}
};

/*
 * Used to satisfy dependencies for devices that need to be
 * run early or in a particular order. Most likely your device doesn't fall
 * into this category, and thus the driver should not be added here. The
 * EPROBE_DEFER can satisfy most dependency problems.
 */
void __init msmkrypton_add_drivers(void)
{
	msm_smd_init();
	msm_rpm_driver_init();
	rpm_smd_regulator_driver_init();
	msm_spm_device_init();
	msm_clock_init(&msmkrypton_clock_init_data);
	tsens_tm_init_driver();
	msm_thermal_device_init();
}

void __init msmkrypton_reserve(void)
{
	of_scan_flat_dt(dt_scan_for_memory_reserve, NULL);
}
static void __init msmkrypton_early_memory(void)
{
	of_scan_flat_dt(dt_scan_for_memory_hole, NULL);
}
static void __init msmkrypton_map_io(void)
{
	msm_map_msmkrypton_io();
}

void __init msmkrypton_init(void)
{
	/*
	 * populate devices from DT first so smem probe will get called as part
	 * of msm_smem_init.  socinfo_init needs smem support so call
	 * msm_smem_init before it.
	 */
	board_dt_populate(msmkrypton_auxdata_lookup);

	msm_smem_init();

	if (socinfo_init() < 0)
		pr_err("%s: socinfo_init() failed\n", __func__);

	msmkrypton_init_gpiomux();
	msmkrypton_add_drivers();
}

static const char *msmkrypton_dt_match[] __initconst = {
	"qcom,msmkrypton",
	NULL
};

DT_MACHINE_START(MSMKRYPTON_DT, "Qualcomm MSM Krypton (Flattened Device Tree)")
	.map_io = msmkrypton_map_io,
	.init_irq = msm_dt_init_irq,
	.init_machine = msmkrypton_init,
	.dt_compat = msmkrypton_dt_match,
	.reserve = msmkrypton_reserve,
	.init_very_early = msmkrypton_early_memory,
	.restart = msm_restart,
MACHINE_END
