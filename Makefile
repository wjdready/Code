BLOG_SOURCE_DIR = _myblog

help:
	@echo "push                 -- 推送到远程仓库."
	@echo "cleanpath            -- 清空所有路径."
	@echo "blog                 -- 生成博客"
	@echo "blog_server          -- 生成博客并启动本地服务"
	@echo "blog_deploy          -- 生成博客并发布到远程仓库"
	@echo "blog_template        -- 生成博客文档模板"
	@echo "simple               -- 快速完成清空路径, 提交 git, 和发布博客"

push:
	git push -u origin master 
	git push -u gitee_origin master

simple:
	make cleanpath
	git add . && git commit -m "1"
	make push
	make blog_deploy

cleanpath:
	@Scripts/cleanpath.sh .

blog:
	@Scripts/generateblog.sh . $(BLOG_SOURCE_DIR)

blog_server:
	make blog && cd $(BLOG_SOURCE_DIR) && hexo server

blog_deploy:
	make blog && cd $(BLOG_SOURCE_DIR) && hexo deploy

blog_template:
	@Scripts/blogtemplate.sh
