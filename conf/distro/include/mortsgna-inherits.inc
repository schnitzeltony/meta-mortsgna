# use debian style naming
INHERIT += "debian"

# activate config checker
INHERIT += "sanity"

# make devshell available as task
INHERIT += "devshell"

# run QA tests on builds and packages and log them
INHERIT += "insane"

# run QA tests on recipes
#INHERIT += "recipe_sanity"

# have a shared state a.k.a package-staging2
INHERIT += "sstate"

# drop buildinfo to /etc/build
INHERIT += "image-buildinfo-mortsgna"

