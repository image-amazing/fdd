aux_source_directory(. DRIVERMONITOR_CPPS)
add_executable(DriverMonitor ${DRIVERMONITOR_CPPS}) 
target_link_libraries(DriverMonitor dm_global libfdd)
