
# Git 规范

## 1. 克隆

develop 分支是用于开发的公共分支，克隆代码时指定为开发分支:

```sh
# 仅克隆 develop 分支
git clone xxx.git -b develop
```

## 2. 创建分支

不应该在公共分支 develop 中直接修改代码，因此在自己的本地仓库中创建一个基于 develop 分支的 dev 分支:

```sh
# 创建本地用于开发的分支
git branch dev

# 切换到 dev 分支
git checkout dev
```

## 3. 合并分支

当在自己的 dev 分支完成某一部分的开发工作后需要提交到远程仓库时, 需要将 dev 合并到公共分支 develop.

**在自己本地仓库合并时请使用 rebase, 在公共分支不要使用 rebase**

合并时可随时通过 git log 操作随时查看分支状态:

```sh
git log --all --pretty=oneline --abbrev-commit --graph
```

##### 3.1 更新本地公共分支 

合并前先同步更新本地的 develop 公共分支并将其合并到自己的 dev 分支

```sh
# 确保当前操作是在自己的本地分支
git checkout dev

git fetch origin develop
git rebase origin/develop

# 或者直接一个命令
git pull origin develop --rebase
```

如果存在冲突，则需解决冲突.

如果冲突很小, 容易解决, 将冲突解决后执行下面操作来继续合并远程分支

```
git rebase --continue 
```

如果冲突太多, 不好解决不要强行合并, 检查是否对 develop 公共分支直接进行了修改

这时可以先终止合并, 并和他人讨论如何处理

```
git rebase --abort
```

##### 3.2 合并到公共分支并提交

同步更新公共分支的最新代码后, 通过 git log 应该可以看到自己的  dev 分支已经在 develop 分支的前面

这时再对 develop 分支进行合并操作:

```sh
git checkout develop
git merge dev

git push origin develop

# 立刻切回本地的 dev 分支, 防止误操作公共分支
git checkout dev
```

**注**: merge 合并失败不要强行合并,并通过 

```
merge --abort 
```

来终止合并, 然后检查自己是否直接修改了公共分支的代码。


## 4. 版本发布

develop 分支开发到某一个节点时再将 develop 分支合并到 master 并在 master 分支打上版本标签.

