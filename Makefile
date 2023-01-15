
help:
	@echo "push 		-- 推送到远程仓库."
	@echo "cleanpath 	-- 清空所有路径."


push:
	git push -u origin master 
	git push -u gitee_origin master

cleanpath:
	@Scripts/cleanpath.sh ./

