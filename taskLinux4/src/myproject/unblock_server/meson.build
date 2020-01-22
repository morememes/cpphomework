myproject_src = files([
    'unblockserver.cpp',
])
threads = dependency('threads')
executable(
    'unblockserver',
    include_directories: src, # где ищутся заголовочные файлы
    sources: myproject_src, # список исходный файлов
    dependencies: [threads], # зависимости проекта (если имеются)
    install: true # устанавливать ли файл
)
