* 配置

```sh
cat > ~/.gitconfig << EOF
[alias]
    sb = submodule update --init --recursive
    sbr = submodule update --remote
    lg = log --graph --pretty=online --abbrev-commit
    lp = log -p
    cp = cherry-pick
    sw = checkout
    cm = commit -m
    rst = reset --hard
    rbi = rebase -i
    cb = checkout -b
    bc = branch
    ba = branch -a
    bd = branch -D
    pd = push origin --delete
    po = push origin
    ss = status
    mg = merge
    pl = pull
    ft = fetch
    td = tag -d
    ptd = "f() { git push origin :refs/tags/\"$1\"; }; f"
    tg = "f() { [ -z \"$1\" ] $$ n=10 || n=\"$1\";git tag --sort=-creatordate | head -n $n; }; f"
    ak = !sh -c 'git add . && git commit -m "up" && git push origin'
    ko = !sh -c 'git branch | tail -n +2 | xargs git branch -D'
[core]
	editor = code --wait
[user]
    name = downdemo
    email = downdemo@qq.com
EOF
```

* 添加远程仓库

```sh
# 远程仓库未创建也可以指定 origin 变量
git remote add origin git@github.com:downdemo/test.git
git remote add origin2 git@github.com:downdemo/test2.git

git remote
origin
origin2

git remote -v
origin  git@github.com:downdemo/test.git (fetch)
origin  git@github.com:downdemo/test.git (push)
origin2 git@github.com:downdemo/test2.git (fetch)
origin2 git@github.com:downdemo/test2.git (push)

git remote rm origin2
git remote
origin

git remote rename origin o 
git remote
o

git pull o master
# 等价于
git fetch o
git merge o/master
```

* 撤销工作区的修改

```sh
git checkout -- <filename>
```

* 撤销 git add

```sh
git restore --staged <filename>
git reset -- <filename>
```

* 分支

```sh
# 查看含有指定 commit 的分支
git branch --contains <commit>
# 如果远程没有 dev 分支则创建名为 dev 的远程分支
git push origin HEAD:dev
# 创建本地分支 b2 并关联远程分支 dev
git checkout -b b2 origin/dev
# 等价于
git checkout -b b2
git branch --set-upstream-to=orgin/dev b2
```

* 回退

```sh
# 回退到前 1 次提交
git reset --hard HEAD^
# 回退到前 2 次提交
git reset --hard HEAD^^
# 回退到前 5 次提交
git reset --hard HEAD~5
```

* 判断一个 commit 或分支是否为另一个的祖先

```sh
# A 是 B 的祖先则退出，不是则打印 1
git merge-base --is-ancestor <A> <B> || echo $?
# A 和 B 的公共祖先
git merge-base <A> <B>
```

* 查看某个文件的改动记录

```sh
git log -p <filename>
```

* 将某次提交合并到当前分支

```sh
git cherry-pick <commit>
git cherry-pick <A> <B> <C>
# 范围为 (A, B] 的提交
git cherry-pick <A>..<B>
# 范围为 [A, B] 的提交
git cherry-pick <A>^..<B>
```

* 暂存

```sh
# 保存，最新保存的 stash 编号为 0，之前保存的 stash 编号均加 1
git stash
# 保存时添加注释信息
git stash save <message>
# 列出所有 stash
git stash list
# 使用编号为 0 的 stash
git stash apply stash@{0}
# 使用并删除编号为 0 的 stash
git stash pop stash@{0}
# 删除编号为 0 的stash
git stash drop stash@{0}
# 删除所有 stash
git stash clear
```

* 打 tag

```sh
# 查看所有 tag
git tag
# 给当前分支最新的 commit 打 tag
git tag <tagname>
# 给某个 commit 打 tag
git tag <tagname> <commit>
# 给某个 commit 打 tag，并添加注释信息
git tag -a <tagname> -m <message> <commit>
# 显示某个 tag 的 commit
git show <tagname>
# 删除某个 tag
git tag -d <tagname>
# 上传某个 tag
git push origin <tagname>
# 上传所有 tag
git push origin --tags
# 从远程仓库删除某个 tag
git push origin :refs/tags/<tagname>
```

* 修改已提交 commit 的信息

```sh
git filter-branch -f --env-filter "GIT_AUTHOR_NAME=downdemo" -- --all
git filter-branch -f --env-filter "GIT_AUTHOR_EMAIL=downdemo@qq.com" -- --all
```

* 打印 commit 号

```sh
# 最新完整 commit
git rev-parse HEAD
# 短 commit
git rev-parse --short HEAD
# n 位 commit，若 n <= 4 则显示 4 位
git rev-parse --short=<n> HEAD
# 某个 branch 的 commit
git rev-parse <branch>
git rev-parse origin/<branch>
# branch hash
git rev-parse --branches
# tag hash
git rev-parse --tags
```

* 显示 .git 绝对路径

```sh
git rev-parse --git-dir
# 当前是否位于 .git 下
git rev-parse --is-inside-git-dir
```

* 显示 git repo 绝对路径

```sh
git rev-parse --show-toplevel
# 当前是否位于 repo 中
git rev-parse --is-inside-work-tree
```

* 添加 submodule

```sh
git submodule add -b <submodule_branch> <submodule_url> <submodule>
```

* 删除 submodule

```sh
rm -rf <submodule>
rm -rf .git/module/<submodule>
git rm --cached <submodule>
```

* 拉取时跳过某个 submodule

```sh
git -c submodule.<module_name>.update=none submodule update --init --recursive
```
