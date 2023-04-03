// Compile:
// $ gcc ./metrics.c -I/usr/include/libdrm -ldrm -ldrm_amdgpu -lpciaccess -O2 -o metrics

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/param.h>
#include <assert.h>
#include <errno.h>

#include <xf86drm.h>
#include <libdrm/amdgpu_drm.h>
#include <libdrm/amdgpu.h>

#define NUM_HBM_INSTANCES 4

typedef struct metrics_table_header {
    uint16_t            structure_size;
    uint8_t             format_revision;
    uint8_t             content_revision;
} metrics_table_header_t;

// https://elixir.bootlin.com/linux/latest/source/drivers/gpu/drm/amd/include/kgd_pp_interface.h#L599
struct gpu_metrics_v1_3 {
    struct metrics_table_header common_header;

    /* Temperature */
    uint16_t            temperature_edge;
    uint16_t            temperature_hotspot;
    uint16_t            temperature_mem;
    uint16_t            temperature_vrgfx;
    uint16_t            temperature_vrsoc;
    uint16_t            temperature_vrmem;

    /* Utilization */
    uint16_t            average_gfx_activity;
    uint16_t            average_umc_activity; // memory controller
    uint16_t            average_mm_activity; // UVD or VCN

    /* Power/Energy */
    uint16_t            average_socket_power;
    uint64_t            energy_accumulator;

    /* Driver attached timestamp (in ns) */
    uint64_t            system_clock_counter;

    /* Average clocks */
    uint16_t            average_gfxclk_frequency;
    uint16_t            average_socclk_frequency;
    uint16_t            average_uclk_frequency;
    uint16_t            average_vclk0_frequency;
    uint16_t            average_dclk0_frequency;
    uint16_t            average_vclk1_frequency;
    uint16_t            average_dclk1_frequency;

    /* Current clocks */
    uint16_t            current_gfxclk;
    uint16_t            current_socclk;
    uint16_t            current_uclk;
    uint16_t            current_vclk0;
    uint16_t            current_dclk0;
    uint16_t            current_vclk1;
    uint16_t            current_dclk1;

    /* Throttle status */
    uint32_t            throttle_status;

    /* Fans */
    uint16_t            current_fan_speed;

    /* Link width/speed */
    uint16_t            pcie_link_width;
    uint16_t            pcie_link_speed; // in 0.1 GT/s

    uint16_t            padding;

    uint32_t            gfx_activity_acc;
    uint32_t            mem_activity_acc;

    uint16_t            temperature_hbm[NUM_HBM_INSTANCES];

    /* PMFW attached timestamp (10ns resolution) */
    uint64_t            firmware_timestamp;

    /* Voltage (mV) */
    uint16_t            voltage_soc;
    uint16_t            voltage_gfx;
    uint16_t            voltage_mem;

    uint16_t            padding1;

    /* Throttle status (ASIC independent) */
    uint64_t            indep_throttle_status;
};


// https://elixir.bootlin.com/linux/latest/source/drivers/gpu/drm/amd/include/kgd_pp_interface.h#L775
struct gpu_metrics_v2_2 {
    metrics_table_header_t common_header;

    /* Temperature */
    uint16_t            temperature_gfx; // gfx temperature on APUs
    uint16_t            temperature_soc; // soc temperature on APUs
    uint16_t            temperature_core[8]; // CPU core temperature on APUs
    uint16_t            temperature_l3[2];

    /* Utilization */
    uint16_t            average_gfx_activity;
    uint16_t            average_mm_activity; // UVD or VCN

    /* Driver attached timestamp (in ns) */
    uint64_t            system_clock_counter;

    /* Power/Energy */
    uint16_t            average_socket_power; // dGPU + APU power on A + A platform
    uint16_t            average_cpu_power;
    uint16_t            average_soc_power;
    uint16_t            average_gfx_power;
    uint16_t            average_core_power[8]; // CPU core power on APUs

    /* Average clocks */
    uint16_t            average_gfxclk_frequency;
    uint16_t            average_socclk_frequency;
    uint16_t            average_uclk_frequency;
    uint16_t            average_fclk_frequency;
    uint16_t            average_vclk_frequency;
    uint16_t            average_dclk_frequency;

    /* Current clocks */
    uint16_t            current_gfxclk;
    uint16_t            current_socclk;
    uint16_t            current_uclk;
    uint16_t            current_fclk;
    uint16_t            current_vclk;
    uint16_t            current_dclk;
    uint16_t            current_coreclk[8]; // CPU core clocks
    uint16_t            current_l3clk[2];

    /* Throttle status (ASIC dependent) */
    uint32_t            throttle_status;

    /* Fans */
    uint16_t            fan_pwm;

    uint16_t            padding[3];

    /* Throttle status (ASIC independent) */
    uint64_t            indep_throttle_status;
};

// https://elixir.bootlin.com/linux/latest/source/drivers/gpu/drm/amd/include/kgd_pp_interface.h#L828
struct gpu_metrics_v2_3 {
    struct metrics_table_header common_header;

    /* Temperature */
    uint16_t            temperature_gfx; // gfx temperature on APUs
    uint16_t            temperature_soc; // soc temperature on APUs
    uint16_t            temperature_core[8]; // CPU core temperature on APUs
    uint16_t            temperature_l3[2];

    /* Utilization */
    uint16_t            average_gfx_activity;
    uint16_t            average_mm_activity; // UVD or VCN

    /* Driver attached timestamp (in ns) */
    uint64_t            system_clock_counter;

    /* Power/Energy */
    uint16_t            average_socket_power; // dGPU + APU power on A + A platform
    uint16_t            average_cpu_power;
    uint16_t            average_soc_power;
    uint16_t            average_gfx_power;
    uint16_t            average_core_power[8]; // CPU core power on APUs

    /* Average clocks */
    uint16_t            average_gfxclk_frequency;
    uint16_t            average_socclk_frequency;
    uint16_t            average_uclk_frequency;
    uint16_t            average_fclk_frequency;
    uint16_t            average_vclk_frequency;
    uint16_t            average_dclk_frequency;

    /* Current clocks */
    uint16_t            current_gfxclk;
    uint16_t            current_socclk;
    uint16_t            current_uclk;
    uint16_t            current_fclk;
    uint16_t            current_vclk;
    uint16_t            current_dclk;
    uint16_t            current_coreclk[8]; // CPU core clocks
    uint16_t            current_l3clk[2];

    /* Throttle status (ASIC dependent) */
    uint32_t            throttle_status;

    /* Fans */
    uint16_t            fan_pwm;

    uint16_t            padding[3];

    /* Throttle status (ASIC independent) */
    uint64_t            indep_throttle_status;

    /* Average Temperature */
    uint16_t            average_temperature_gfx; // average gfx temperature on APUs
    uint16_t            average_temperature_soc; // average soc temperature on APUs
    uint16_t            average_temperature_core[8]; // average CPU core temperature on APUs
    uint16_t            average_temperature_l3[2];
};

int main(void)
{
    void *buf[MAX(sizeof(struct gpu_metrics_v1_3), MAX(sizeof(struct gpu_metrics_v2_2), sizeof(struct gpu_metrics_v2_3)))];
    metrics_table_header_t* header = (metrics_table_header_t*)buf;

    // To get the correct path, run the following:
    // $ find /sys/devices/ | grep gpu_metrics
    #error "FIXME: UPDATE PATH:" // Comment this out
    FILE *f = fopen("/sys/devices/pci0000:00/0000:00:08.1/0000:08:00.0/gpu_metrics", "rb");

    while(true)
    {
        if (!fread(buf, sizeof(buf), 1, f) == 0)
        {
            perror("fread");
            return 1;
        }
        rewind(f);

        printf("format_revision = %d, content_revision = %d\n", header->format_revision, header->content_revision);

        if (header->format_revision == 1)
        {
            if (header->content_revision == 3)
            {
                printf("Parsing gpu_metrics_v1_3\n");
                struct gpu_metrics_v1_3 *amdgpu_metrics = (struct gpu_metrics_v1_3 *) buf;

                printf("average_socket_power: %d W\n", amdgpu_metrics->average_socket_power);
            }
        }
        else
        if (header->format_revision == 2)
        {
            if (header->content_revision == 2)
            {
                printf("Parsing gpu_metrics_v2_2\n");
                struct gpu_metrics_v2_2 *amdgpu_metrics = (struct gpu_metrics_v2_2 *) buf;

                printf("average_socket_power: %d W\n", amdgpu_metrics->average_socket_power);
                printf("average_cpu_power: %.2f W\n", amdgpu_metrics->average_cpu_power / 1000.f);
                printf("average_soc_power: %.2f W\n", amdgpu_metrics->average_soc_power / 1000.f);
            }
            else
            if (header->content_revision == 3)
            {
                printf("Parsing gpu_metrics_v2_3\n");
                struct gpu_metrics_v2_3 *amdgpu_metrics = (struct gpu_metrics_v2_3 *) buf;

                printf("average_socket_power: %d W\n", amdgpu_metrics->average_socket_power);
                printf("average_cpu_power: %.2f W\n", amdgpu_metrics->average_cpu_power / 1000.f);
                printf("average_soc_power: %.2f W\n", amdgpu_metrics->average_soc_power / 1000.f);
            }
        }

        printf("\e[1;1H\e[2J");

        usleep(500000);
    }
    fclose(f);

    return 0;
}
