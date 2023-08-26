BLOG_SOURCE_DIR = _myblog

GITEE_URL = https://gitee.com/wjundong/code.git

help:
	@echo "push                 -- 推送到远程仓库."
	@echo "cleanpath            -- 清空所有路径."
	@echo "blog                 -- 生成博客"
	@echo "blog_server          -- 生成博客并启动本地服务"
	@echo "blog_deploy          -- 生成博客并发布到远程仓库"
	@echo "blog_template        -- 生成博客文档模板"
	@echo "simple               -- 快速完成清空路径, 提交 git, 和发布博客"

# 同时也 push 到 gitee
push:
	@if ! git remote | grep -q "gitee_origin"; then \
		echo add gitee remote origin;			  \
		git remote add gitee_origin $(GITEE_URL); \
	fi
	git push -u gitee_origin master

simple:
	make cleanpath
	git add . && git commit -m "1"	|| true
	make push
	make blog_deploy

cleanpath:
	@Scripts/cleanpath.py .

$(BLOG_SOURCE_DIR):
	git clone https://github.com/wjdready/wjdready.github.io.git $(BLOG_SOURCE_DIR)

blog: $(BLOG_SOURCE_DIR)
	@Scripts/generateblog.py . $(BLOG_SOURCE_DIR)

blog_server: $(BLOG_SOURCE_DIR)
	make blog && cd $(BLOG_SOURCE_DIR) && hexo server

blog_deploy: $(BLOG_SOURCE_DIR)
	make blog && cd $(BLOG_SOURCE_DIR) && hexo deploy

blog_template:
	@Scripts/blogtemplate.sh
