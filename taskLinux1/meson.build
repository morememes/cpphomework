 project(
    'myproject',
    'cpp',
    version: '0.1.0',
    meson_version: '>=0.46',
    default_options: ['cpp_std=c++17']
)
subdir('src')

doxygen = find_program('doxygen')
run_target('docs', command : ['doxygen', meson.source_root() + '/Doxyfile'])