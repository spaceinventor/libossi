project('ossi', 'c', version: '0.0.1', default_options: [
    'default_library=static',
    'b_lto=false',
    'b_lundef=false',
])

conf_data = configuration_data()

includes = include_directories('include', '.')
install_subdir('include', install_dir : '.')
sources = files([])
dependencies = []

subdir('src')

configure_file(output : 'libossi.h', configuration: conf_data)

ossi_lib = library('ossi',
    sources: sources,
    dependencies : dependencies,
    include_directories : includes,
)

ossi_dep = declare_dependency(dependencies: dependencies, include_directories: includes, link_with : ossi_lib)
