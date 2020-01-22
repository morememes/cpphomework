myproject_src = files([
    'block_server.cpp'
])
threads = dependency('threads')
executable(
    'blockserver',
    include_directories: src, # где ищутся заголовочные файлы
    sources: myproject_src, # список исходный файлов
    dependencies: [threads], # зависимости проекта (если имеются)
    install: true # устанавливать ли файл
)
