gtest = dependency('gtest', main: true)
test(
	'MyTests',
	executable(
		'tests',
		sources: ['tests.cpp'],
		include_directories: src,
		dependencies: [gtest]
	)
)
