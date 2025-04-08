subdir('arch')

sources += files([
])

dependencies = []
dependencies += dependency('csp', fallback: ['csp', 'csp_dep']).partial_dependency(links: false, includes: true)
dependencies += dependency('param', fallback: ['param', 'param_dep']).partial_dependency(links: false, includes: true)
