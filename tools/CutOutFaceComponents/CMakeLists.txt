aux_source_directory(. COFC_CPPS)
add_executable(CutOutFaceComponents ${COFC_CPPS})
target_link_libraries(CutOutFaceComponents libfdd dm_global)
