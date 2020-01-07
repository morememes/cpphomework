myproject_src = files([
    'task8.cpp'
])
threads = dependency('threads')
executable(
    'myproject',
    include_directories: src, # где ищутся заголовочные файлы
    sources: myproject_src, # список исходный файлов
    dependencies: [threads], # зависимости проекта (если имеются)
    install: true # устанавливать ли файл
)
