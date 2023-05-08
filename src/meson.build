sources = [
    'ring-buffer.cpp',
]

lib = library('shmrb',
    sources: sources,
    dependencies: deps,
    version: version,
    soversion: version_major,
    include_directories: include_dir,
    install: true,
)

lib_dep = declare_dependency(
    link_with: lib,
    include_directories: include_dir,
    dependencies: deps
)

pkg = import('pkgconfig')

pkg.generate(
    libraries: [lib],
    version: version,
    name: 'shmrb',
    description: 'Library for a SharedMemory RingBuffer',
    requires: [
        'glib-2.0',
        'gobject-2.0',
        'gmodule-2.0',
    ],
    subdirs: [
        header_dir
    ],
)
