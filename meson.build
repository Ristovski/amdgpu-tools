project('metrics', 'c',
    version: '0.1',
    default_options: ['warning_level=2'])

executable('metrics', 'metrics.c',
dependencies: [
    dependency('libdrm'),
    dependency('libdrm_amdgpu'),
    dependency('pciaccess')
], install: true)
