# Git 多账户 SSH Key 配置指南

当你需要在同一台机器上使用多个 Git 账户(比如工作账户和个人账户)时,可以通过配置不同的 SSH key 来实现。

## 1. 生成多个 SSH Key

首先为不同账户生成不同的 SSH key:

```bash
# 为工作账户生成 key
ssh-keygen -t ed25519 -C "work@company.com" -f ~/.ssh/id_ed25519_work

# 为个人账户生成 key
ssh-keygen -t ed25519 -C "personal@gmail.com" -f ~/.ssh/id_ed25519_personal
```

## 2. 配置 SSH config 文件

在 `~/.ssh/config` 文件中配置(如果文件不存在就创建):

```
# 工作账户 (GitHub)
Host github.com-work
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_ed25519_work
    IdentitiesOnly yes

# 个人账户 (GitHub)
Host github.com-personal
    HostName github.com
    User git
    IdentityFile ~/.ssh/id_ed25519_personal
    IdentitiesOnly yes

# 如果是 GitLab
Host gitlab.com-work
    HostName gitlab.com
    User git
    IdentityFile ~/.ssh/id_ed25519_work
    IdentitiesOnly yes
```

## 3. 添加公钥到 Git 服务

将生成的公钥添加到对应的 Git 账户:

```bash
# 查看公钥内容
cat ~/.ssh/id_ed25519_work.pub
cat ~/.ssh/id_ed25519_personal.pub
```

然后分别添加到 GitHub/GitLab 的 SSH keys 设置中。

## 4. 克隆仓库时使用对应的 Host

**关键点**: 使用你在 config 中定义的 Host 名称:

```bash
# 使用工作账户克隆
git clone git@github.com-work:company/repo.git

# 使用个人账户克隆
git clone git@github.com-personal:yourname/repo.git
```

## 5. 修改已有仓库的 remote

如果仓库已经存在,修改 remote URL:

```bash
# 查看当前 remote
git remote -v

# 修改为工作账户
git remote set-url origin git@github.com-work:company/repo.git

# 或修改为个人账户
git remote set-url origin git@github.com-personal:yourname/repo.git
```

## 6. 配置仓库的 Git 用户信息

在每个仓库目录下配置对应的用户信息:

```bash
# 进入工作仓库
cd work-repo
git config user.name "Your Work Name"
git config user.email "work@company.com"

# 进入个人仓库
cd personal-repo
git config user.name "Your Personal Name"
git config user.email "personal@gmail.com"
```

## 7. 测试连接

```bash
# 测试工作账户
ssh -T git@github.com-work

# 测试个人账户
ssh -T git@github.com-personal
```

成功的话会看到类似 "Hi username! You've successfully authenticated" 的消息。

## Push 代码时的操作

配置完成后,push 操作和平常完全一样,**不需要做任何特殊操作**:

```bash
# 1. 添加文件
git add .

# 2. 提交
git commit -m "your commit message"

# 3. push (和平常一样!)
git push origin main
```

### 工作原理

当你执行 `git push` 时:

1. Git 读取 remote URL,比如 `git@github.com-work:company/repo.git`
2. SSH 看到 host 是 `github.com-work`
3. SSH 查找 `~/.ssh/config`,找到对应配置并使用相应的 IdentityFile
4. 自动使用正确的 SSH key 进行认证
5. Push 成功!

### 实际示例

**工作仓库 push:**

```bash
cd ~/work/company-project
git remote -v
# origin  git@github.com-work:company/repo.git (fetch)
# origin  git@github.com-work:company/repo.git (push)

git add .
git commit -m "update feature"
git push origin main  # ✅ 自动使用工作账户的 key
```

**个人仓库 push:**

```bash
cd ~/personal/my-project
git remote -v
# origin  git@github.com-personal:username/repo.git (fetch)
# origin  git@github.com-personal:username/repo.git (push)

git add .
git commit -m "fix bug"
git push origin main  # ✅ 自动使用个人账户的 key
```

## 验证配置

查看当前仓库使用的是哪个账户:

```bash
# 查看 remote URL
git remote get-url origin

# 查看当前仓库的用户配置
git config user.name
git config user.email
```

## 故障排查

如果 push 失败,可以尝试:

```bash
# 1. 检查 remote URL 是否正确
git remote -v

# 2. 测试 SSH 连接
ssh -T git@github.com-work
ssh -T git@github.com-personal

# 3. 如果需要,修正 remote URL
git remote set-url origin git@github.com-work:company/repo.git
```

## 自动化技巧(可选)

你也可以使用 **目录级别的 Git 配置**。在 `~/.gitconfig` 中:

```ini
[includeIf "gitdir:~/work/"]
path = ~/.gitconfig-work

[includeIf "gitdir:~/personal/"]
path = ~/.gitconfig-personal
```

然后创建对应的配置文件:

**~/.gitconfig-work:**

```ini
[user]
name = Your Work Name
email = work@company.com
```

**~/.gitconfig-personal:**

```ini
[user]
name = Your Personal Name
email = personal@gmail.com
```

这样,在不同目录下的仓库会自动使用对应的用户信息,push 时只要 remote URL 配置正确,就会使用对应的 SSH key。

---

## 总结

配置好后,push 操作完全不变,一切都是自动的! Git 会根据 remote URL 中的 Host 自动选择正确的 SSH key。