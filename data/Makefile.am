# automake
AUTOMAKE_OPTIONS        = color-tests subdir-objects

# task
md5:
	find . -name \*.img -print | xargs -i basename {} .img | xargs -I{} sh -c "md5sum '{}.img' > '{}.hash'"
