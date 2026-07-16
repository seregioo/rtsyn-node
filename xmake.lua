local project_name = "rtsyn-node"

set_license("GPL-3.0-or-later")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

add_repositories("rtsyn-xmake-repo https://github.com/seregioo/rtsyn-xmake-repo.git")

set_languages("c23")

add_requires("gtest")

local rtsyn_dependencies = { "rtsyn-port" }
for i, rtsyn_dependency in ipairs(rtsyn_dependencies) do
	add_requires(rtsyn_dependency)
end

target(project_name)
set_kind("static")
add_files("src/*.c")
add_files("src/**/*.c")
for i, rtsyn_dependency in ipairs(rtsyn_dependencies) do
	add_packages(rtsyn_dependency)
end
add_includedirs("include", { public = true })
add_includedirs("src")
add_headerfiles("include/(rtsyn/**.h)")

local rtsyn_modules = {
	{ path = "node", name = "node" },
	{ path = "node/aggregation", name = "aggregation" },
}

for _, rtsyn_module in ipairs(rtsyn_modules) do
	local tests_name = "tests/" .. rtsyn_module.path .. "-tests"
	target(tests_name)
	set_kind("binary")
	add_deps(project_name)
	add_packages("gtest")
	for i, rtsyn_dependency in ipairs(rtsyn_dependencies) do
		add_packages(rtsyn_dependency)
	end
	add_links("gtest_main")
	add_includedirs("src")
	add_files("tests/" .. rtsyn_module.path .. ".cpp")
	add_tests(rtsyn_module.name)
end
--
-- If you want to known more usage about xmake, please see https://xmake.io
--
