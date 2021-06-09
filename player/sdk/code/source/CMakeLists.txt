# 设置源文件路径列表
aux_source_directory(. SRC_LIST)

# 生成链接
add_library(source-objects OBJECT ${SRC_LIST})

# 将链接向上传递
set(tmp
    $<TARGET_OBJECTS:source-objects>
    PARENT_SCOPE)