# GitHub 上传与日常维护指南

本指南适用于当前的“嵌入式软件面试宝典”目录。首次上传时需要先初始化 Git；如果目录已经有 `.git`，跳过 `git init`。以下命令在项目根目录运行；把 `你的用户名` 和仓库名替换为实际值。

## 首次上传

1. 安装 Git，在终端运行 `git --version` 确认安装成功。首次使用时设置提交身份；邮箱可用 GitHub 提供的 noreply 地址，避免公开私人邮箱。

   ```sh
   git config --global user.name "你的名字"
   git config --global user.email "你的邮箱"
   ```

2. 在 GitHub 网页点击 **New repository**，输入仓库名，例如 `embedded-interview-guide`，选择 **Public（公开）**。创建时不要勾选 README、`.gitignore` 或 License，保持远端仓库为空。公开仓库的代码和提交历史任何人都能查看。

3. 公开仓库上传前，检查文件中的个人信息。项目文件以当前目录为基准使用相对路径；`./制作与核验记录.txt` 已移除旧制作电脑的绝对路径。不要提交密码、令牌、私人笔记备份、编译出的 `.exe` 或其他临时文件。学习进度保存在浏览器本地，不会因为上传项目文件自动同步。

4. 在本项目目录执行：

   ```sh
   git init -b main
   git status
   git add .
   git diff --cached --stat
   git diff --cached --check
   git commit -m "docs: add embedded interview guide"
   git remote add origin https://github.com/你的用户名/embedded-interview-guide.git
   git push -u origin main
   ```

   `git diff --cached --stat` 用来核对将要公开的文件；发现不应提交的文件时，用 `git restore --staged 文件名` 撤出暂存，然后处理该文件。`git push -u` 建立本地 `main` 与远端分支的关联，以后可直接运行 `git push`。本指南默认使用 HTTPS，无需 SSH。已安装 GitHub CLI 时，可先执行以下命令，在浏览器中完成授权；不要把一次性代码、密码或令牌提交到仓库。

   ```sh
   gh auth login --hostname github.com --git-protocol https --web
   gh auth setup-git
   gh auth status
   ```

   如果使用其他凭据管理器，按其提示登录。GitHub 账户密码不能直接用作 Git HTTPS 密码。

## 邀请协作者与保护主分支

上传后，在仓库网页进入 **Settings → Collaborators**（界面也可能显示为 **Access → Collaborators and teams**），点击 **Add people**，输入你批准的 GitHub 用户名并发送邀请。对方接受邀请后才能直接推送到该仓库；公开仓库的其他用户可以查看和 fork，也可以提交 Pull Request，但不能直接推送。个人账户下的仓库通常由所有者管理协作者；不再需要某人维护时，在同一页面撤销其访问权限。

建议在 **Settings → Rules → Rulesets** 为 `main` 新建并启用分支规则：要求通过 Pull Request 合并，并要求至少 1 次审核批准。这样协作者从自己的分支提交修改，再经审核合并到 `main`。如需确保协作者的改动**由你本人审核**，可创建 `.github/CODEOWNERS`，写入 `* @你的用户名`，并启用“Require review from Code Owners”；仅设置“1 次批准”允许其他有审核权限的人批准。检查规则的绕过权限，确保协作者不能直接绕过审核。若你也会独自维护，请保留所有者的合理绕过权限，否则自己的 PR 无人可审核时可能无法合并。

## 日常修改与同步

开始工作前获取他人已上传的修改。完成修改后，检查差异，再提交和推送：

```sh
git switch main
git pull --ff-only
git switch -c docs/clarify-volatile
git status
git diff
git add "01_分章讲义/01_C 语言与代码语义.md"
git diff --cached
git commit -m "docs: clarify volatile and interrupt safety"
git push -u origin docs/clarify-volatile
```

上面的分支名和 `git add` 文件只是示例；换成实际改动。已有工作分支上继续修改时，切回该分支，提交后直接运行 `git push`。修改 C 练习时，分别编译并运行对应文件中的 `assert` 测试；修改网页时，打开 `index.html` 检查交互。保持 `assets/data.js` 与讲义、C 源码下载内容一致，并更新 `文件校验_SHA256.csv` 中受影响的已有条目。提交信息用简短动词描述实际改动，例如 `fix: correct progress import`。

## 分支与 Pull Request

推送工作分支后，在 GitHub 网页点击 **Compare & pull request**，目标选 `main`。PR 描述应说明改了什么、为什么改、如何验证；页面样式变化可附截图。你审核通过后在网页合并。合并后运行 `git switch main` 和 `git pull --ff-only` 更新本地。新电脑上使用 `git clone https://github.com/你的用户名/embedded-interview-guide.git` 下载仓库。

## 常用命令速查

| 命令 | 用途 |
| --- | --- |
| `git status` | 查看已修改、已暂存和未跟踪文件。 |
| `git diff` / `git diff --cached` | 查看未暂存 / 已暂存的改动。 |
| `git log --oneline -5` | 查看最近五次提交。 |
| `git remote -v` | 确认远端仓库地址。 |
| `git fetch origin` | 获取远端更新，但暂不合并。 |
| `git pull --ff-only` | 在本地没有分叉时同步当前分支。 |
| `git restore --staged 文件名` | 撤出暂存，保留工作区修改。 |

如果 `git pull --ff-only` 因本地修改或分支分叉而失败，先用 `git status` 和 `git log --oneline --graph --all` 查明情况；不要强制推送覆盖远端提交。
