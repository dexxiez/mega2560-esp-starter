# No REQUIRES here: main implicitly depends on every component, which the
# SDK link order relies on (pthread for libstdc++).
idf_component_register(SRCS "main.c")
