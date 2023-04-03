# amdgpu-tools

## amdgpu_ips.py
Enumerates amdgpu [IP blocks](https://www.kernel.org/doc/html/v5.10/gpu/amdgpu.html#ip-blocks), printing the IP blocks name, version and base address(es).

Risk: **Harmless** - running this tool is completely safe, all operations are read-only.

```
$ ./amdgpu_ips.py -h
usage: amdgpu_ips.py [-h] [--gpu [CARD_INDEX]] [--enumerate] [-u]

Enumerate AMDGPU IPs

options:
  -h, --help          show this help message and exit
  --gpu [CARD_INDEX]  Card index, first by default
  --enumerate         Enumerate IPs
  -u                  Print only unique IPs (based on name and version)
```
 
```
$ ./amdgpu_ips.py -u
     NBIF 2.5.0      0x00000000
      DMU 2.1.0      0x00000012
DBGU_NBIO 3.0.0      0x000001C0
  DBGU_IO 3.0.0      0x000001E0
    ATHUB 1.5.0      0x00000C20
SYSTEMHUB 2.5.0      0x00000EA0
      HDP 4.1.2      0x00000F20
   OSSSYS 4.3.0      0x000010A0
    SDMA0 4.1.2      0x00001260
       GC 9.3.0      0x00002000
       DF 3.5.2      0x00007000
      UVD 2.2.0      0x00007800
     IOHC 6.0.0      0x00010000
      MP1 12.0.1     0x00016000
      MP2 11.0.0     0x00016000
      MP0 12.0.1     0x00016000
      THM 10.1.1     0x00016600
    SMUIO 10.0.2     0x00016800
     CLKA 10.0.2     0x00016E00
     FUSE 10.1.1     0x00017400
     CLKB 10.0.2     0x00017E00
      ISP 2.1.0      0x00018000
    MMHUB 1.5.0      0x0001A000
      UMC 7.6.0      0x00054000
      ACP 3.1.0      0x02403800
      DIO 127.127.63 0x02404000
      DAZ 127.127.63 0x02404800
      FCH 7.0.0      0x0240C000
     PCIE 5.1.0      0x02411800
      PCS 5.1.0      0x02414C00
      SST 1.1.0      0x02416C00
    IOAGR 6.0.0      0x02419000
   IOAPIC 6.0.0      0x0241F000
      USB 4.6.0      0x0242A800
```

## metrics
Parses [`gpu_metrics`](https://dri.freedesktop.org/docs/drm/gpu/amdgpu.html#gpu-metrics).
A list of available metrics can be found at https://elixir.bootlin.com/linux/v6.3-rc5/source/drivers/gpu/drm/amd/include/kgd_pp_interface.h#L831.

Risk: **Harmless** - running this tool is completely safe, all operations are read-only.

```
$ meson setup build
$ meson compile -C build
```

```
$ ./build/metrics
format_revision = 2, content_revision = 2
Parsing gpu_metrics_v2_2
average_socket_power: 8 W
average_cpu_power: 1.68 W
average_soc_power: 3.60 W
```
