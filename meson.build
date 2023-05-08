project('libshmrb', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=3', 'cpp_std=c++14'])

version = meson.project_version()
components = version.split('.')
version_major = components[0]
version_minor = components[1]
version_patch = components[2]

glib_dep = dependency('glib-2.0', version: '>= 2.38')
gobject_dep = dependency('gobject-2.0', version: '>= 2.38')
gmodule_dep = dependency('gmodule-2.0', version: '>= 2.38')

deps = [
    glib_dep,
    gobject_dep,
    gmodule_dep,
]

include_dir = include_directories('include')
header_dir = 'shmrb-@0@'.format(version_major)
header_subdir = join_paths(header_dir, 'shmrb')

subdir('src')
subdir('include')
