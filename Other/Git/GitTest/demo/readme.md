

```shell
# 不要在当前文件夹使用, 最好在桌面新建文件夹来测试

mkdir test
cd test

git init .
echo Hello > hello.txt
git add .
git commit -m "Add hello.txt"
git log

mkdir subtest
cd subtest
    git init .
    echo Good > subtext.txt
    git add .
    git commit -m "Add subtext.txt in submodule"
    git log
cd ..

# not used
cd ..
git status --ignored
git add --force subtest

git submodule add --name subtest


# Test 
# 从本地添加子模块, url 为本地路径 ./subtest
git submodule add ./subtest sub

# 这时就会复制 ./subtest 到 sub 文件夹, 然后 .gitmodules 
# 我们提交本次更改
git add .
git commit -m "Add submodule sub"

git push 
git clone 
```


# Define origin to be the absolute path to this repository - we'll remove
# this later:
$ cd /tmp/main/
$ git remote add origin /tmp/main/

# Now add the submodule:
$ git submodule add -b gh-pages ./ gh-pages
Initialized empty Git repository in /tmp/main/gh-pages/.git/
Branch gh-pages set up to track remote branch gh-pages from origin.

# Now .gitmodules looks sensible:
$ cat .gitmodules 
[submodule "gh-pages"]
    path = gh-pages
    url = ./

# However, the origin for the submodule isn't what we want:
$ cd gh-pages
$ git remote -v
origin  /tmp/main/ (fetch)
origin  /tmp/main/ (push)

# So remove it and add the right origin (just ".."):
$ git remote rm origin
$ git remote add origin ..

# Change back to the main repository and commit:
$ cd ..
$ git commit -m "Added the gh-pages branch as a submodule of this repository"
[master 6849d53] Added the gh-pages branch as a submodule of this repository
 2 files changed, 4 insertions(+), 0 deletions(-)
 create mode 100644 .gitmodules
 create mode 160000 gh-pages