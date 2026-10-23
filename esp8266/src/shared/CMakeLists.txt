# Header-only component exposing the repo-level shared/include/ to the ESP build.
# main sees it automatically; other components need REQUIRES shared.
idf_component_register(INCLUDE_DIRS "../../../shared/include")
