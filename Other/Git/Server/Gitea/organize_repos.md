# Gitea 仓库分类管理方案

## 问题
仓库数量达到几百个时，难以管理和查找

## 解决方案

### 1. 使用组织 (Organizations) ⭐ 推荐

按业务/技术领域分类：

```bash
# 创建组织
tea orgs create Flutter --description "Flutter 相关项目"
tea orgs create Embedded --description "嵌入式开发"
tea orgs create Learning --description "学习笔记"

# 转移仓库到组织
tea repos transfer --owner Flutter repo-name
```

**优点**：
- 清晰的层级结构
- 独立的权限管理
- 团队协作友好

### 2. 使用 Topics (标签)

给仓库打标签分类：

```bash
# API 方式批量添加 topic
curl -X PUT http://localhost:3000/api/v1/repos/owner/repo/topics \
  -H "Authorization: token YOUR_TOKEN" \
  -H "Content-Type: application/json" \
  -d '{"topics": ["flutter", "mobile", "active"]}'
```

**常用标签**：
- 技术栈：`flutter`, `python`, `embedded`, `web`
- 状态：`active`, `archived`, `wip`, `deprecated`
- 类型：`library`, `tool`, `learning`, `demo`

### 3. 命名规范

统一前缀：
- `flutter-app-name`
- `embedded-board-project`
- `learn-topic-name`
- `tool-utility-name`

### 4. 归档策略

**归档不活跃仓库**：
- 仓库设置 → 归档此仓库
- 归档后只读，不占用活跃列表

**批量归档脚本**：
```bash
# 归档 6 个月未更新的仓库
tea repos list --json | jq -r '.[] | select(.updated_at < "2025-11-01") | .name' | \
  while read repo; do
    echo "Archiving $repo"
    # 需要通过 API 实现
  done
```

### 5. 使用 Gitea 搜索功能

**高级搜索**：
- 按组织：`org:Flutter`
- 按 topic：`topic:embedded`
- 按语言：`language:Python`
- 按更新时间：`updated:>2025-01-01`
- 组合：`org:Flutter topic:mobile language:Dart`

### 6. 创建索引仓库

创建一个 `awesome-repos` 仓库，用 README 分类列出所有仓库：

```markdown
# 仓库索引

## Flutter 项目
- [flutter-app1](http://localhost:3000/Flutter/app1) - 描述
- [flutter-app2](http://localhost:3000/Flutter/app2) - 描述

## 嵌入式项目
- [embedded-board1](http://localhost:3000/Embedded/board1) - 描述

## 学习笔记
- [learn-python](http://localhost:3000/Learning/python) - 描述
```

### 7. 使用 Projects 看板

创建项目看板管理跨仓库的任务：
- `Active Development` - 正在开发的仓库
- `Maintenance` - 维护中的仓库
- `Archived` - 已归档的仓库

## 推荐组合方案

**小规模 (< 50 个仓库)**：
- Topics 标签 + 命名规范

**中等规模 (50-200 个)**：
- 组织分类 + Topics 标签

**大规模 (> 200 个)**：
- 组织分类 + Topics 标签 + 归档策略 + 索引仓库

## 实施步骤

1. **规划组织结构**
   ```
   个人账户 (shino)
   ├── Flutter 组织
   ├── Embedded 组织
   ├── Learning 组织
   └── Tools 组织
   ```

2. **批量转移仓库**
   ```bash
   # 列出所有仓库
   tea repos list
   
   # 转移到对应组织
   tea repos transfer --owner Flutter flutter-project-1
   ```

3. **添加 Topics**
   - 手动：仓库设置页面
   - 批量：使用 API 脚本

4. **定期清理**
   - 每季度归档不活跃仓库
   - 更新索引文档

## 工具脚本

### 批量添加 Topic
```python
import requests

GITEA_URL = "http://localhost:3000"
TOKEN = "your_token"

repos = [
    {"owner": "shino", "name": "repo1", "topics": ["flutter", "mobile"]},
    {"owner": "shino", "name": "repo2", "topics": ["python", "tool"]},
]

for repo in repos:
    url = f"{GITEA_URL}/api/v1/repos/{repo['owner']}/{repo['name']}/topics"
    headers = {"Authorization": f"token {TOKEN}"}
    data = {"topics": repo["topics"]}
    response = requests.put(url, json=data, headers=headers)
    print(f"{repo['name']}: {response.status_code}")
```

### 生成仓库索引
```bash
#!/bin/bash
echo "# 仓库索引" > index.md
echo "" >> index.md

for org in Flutter Embedded Learning Tools; do
    echo "## $org" >> index.md
    tea repos list --org $org --json | \
      jq -r '.[] | "- [\(.name)](\(.html_url)) - \(.description)"' >> index.md
    echo "" >> index.md
done
```
