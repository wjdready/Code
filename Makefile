BLOG_SOURCE_DIR = _myblog

help:
	@echo "push 			-- 推送到远程仓库."
	@echo "cleanpath 		-- 清空所有路径."
	@echo "generateblog		-- 生成博客"

push:
	git push -u origin master 
	git push -u gitee_origin master

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
