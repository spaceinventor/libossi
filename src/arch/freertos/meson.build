sources += files([
	'message_queue.c',
	'critical_section.c',
	'si_notify.c',
])

dependencies += dependency('freertos', fallback: ['freertos', 'freertos_dep']).partial_dependency(links: false, includes: true)
dependencies += dependency('libc', fallback: ['picolibc', 'picolibc_dep']).partial_dependency(links: false, includes: true)
