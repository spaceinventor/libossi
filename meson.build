project('ossi', 'c', version: '0.0.2', default_options: ['warning_level=3', 'werror=true'])
add_project_arguments(['-Wstrict-prototypes', '-Wmissing-prototypes', '-Wmissing-declarations', '-Wcast-align', '-Wpointer-arith', '-Wshadow'], language: 'c')

conf_data = configuration_data()

includes = include_directories('include', '.')
sources = files([])
dependencies = []

dependencies += dependency('param', fallback: ['param', 'param_dep'])
dependencies += dependency('csp', fallback: ['csp', 'csp_dep'])

subdir('src')

configure_file(output : 'libossi.h', configuration: conf_data, install_dir: 'include/', install_tag: 'devel')

ossi_lib = library('ossi',
    sources: sources,
    dependencies: dependencies,
    include_directories: includes,
    install: true,
    soversion: meson.project_version()
)
install_subdir('include', install_dir : '.', install_tag: 'devel')

pkg = import('pkgconfig')
pkg.generate(
	ossi_lib,
	subdirs: ['.'],
    requires: ['param', 'csp']
)

ossi_dep = declare_dependency(dependencies: dependencies, include_directories: includes, link_with : ossi_lib)
