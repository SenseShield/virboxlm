[TestSlmRuntimeEasy.java 说明]

该文件是测试SSRuntimeEasyJava.dll 的测试类demo，使用时，需要新建java工程，将该类加载到工程中进行使用，具体流程如下：

1、使用eclipse创建java Project

2、将TestSlmRuntimeEasy.java类加载到工程中。另外，将SDK目录中java目录下的SSRuntimeEasyJava.jar包拷贝到java工程下，并加载到java工程。

3、从“./SDK/JAVA/DLL/”目录下拷贝SSRuntimeEasyJava.dll 和 “./SDK/C/DLL/”目录下拷贝slm_runtime_easy.dll两个动态库文件到java工程下。

4、运行程序即可执行demo程序


注意：开发商在开发过程中，请使用_dev版本的动态库进行开发调试，_dev版本的动态库不存在反调试功能，调试器可以正常使用；开发商在发布产品时，请将_dev版本的动态库更换为正式动态库（动态库文件名不带_dev标识），正式版动态库附带反调试功能，不可用于调试操作。

