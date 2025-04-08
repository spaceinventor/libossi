if host_machine.system() == 'linux'
	subdir('posix')
	conf_data.set('LIBOSSI_POSIX', 1)
elif host_machine.system() == 'freertos'
	subdir('freertos')
	conf_data.set('LIBOSSI_FREERTOS', 1)
else
	# If other architecture implementations are needed, please
	# implement the required functionality locally in the
	# application or submit a pull request to the maintainers.
endif
