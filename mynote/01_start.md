

1. 参考
游戏引擎架构
代码库：
https://github.com/jj4jj/GameEngineFromScratch

2. 文章系列
   * https://zhuanlan.zhihu.com/p/28587092
   * https://zhuanlan.zhihu.com/p/28589792
   * https://zhuanlan.zhihu.com/p/28598462
     * 下载环境git,cmake,vscode,vs,python(anaconda)
     * 执行脚本编译scripts/build_xxxx 依赖库
     * build_3rd_libs.bat
       * build_llvm_clang
       * build_bullet
       * build_zlib
       * build_bjson
       * build_cef
       * build_crossguid
       * build_glslangValidator
       * build_imgui
       * build_ispc
       * build_libpng
       * build_opengex
       * build_emscipten
       * build_sdl
       * 使用CMAKE将第三方库整合,所有过程都是从源码编译,git依赖第三方库代码,然后编译将结果整合在一起,给项目可用
       * 某些命令行工具编译后需要修改环境变量, 例如clang
       * 
     * 执行脚本编译scripts/build.bat 编译
   * 



