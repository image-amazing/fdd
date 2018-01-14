aux_source_directory(. COFC_CPPS)
add_executable(DrawRects ${COFC_CPPS})
target_link_libraries(DrawRects libfdd dm_global)


