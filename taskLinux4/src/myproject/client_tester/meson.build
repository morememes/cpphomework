myproject_src = files([
    'tester.cpp',
])
threads = dependency('threads')
executable(
    'tester',
    include_directories: src, # где ищутся заголовочные файлы
    sources: myproject_src, # список исходный файлов
    dependencies: [threads], # зависимости проекта (если имеются)
    install: true # устанавливать ли файл
)
